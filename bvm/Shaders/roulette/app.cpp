#include "../common.h"
#include "contract.h"

#define Roulette_manager_create(macro)
#define Roulette_manager_view(macro)
#define Roulette_manager_view_params(macro) macro(ContractID, cid)
#define Roulette_manager_destroy(macro) macro(ContractID, cid)
#define Roulette_manager_view_bids(macro) macro(ContractID, cid)

#define Roulette_manager_view_bid(macro) \
    macro(ContractID, cid) \
    macro(PubKey, pubKey)

#define RouletteRole_manager(macro) \
    macro(manager, create) \
    macro(manager, destroy) \
    macro(manager, view) \
    macro(manager, view_params) \
    macro(manager, view_bids) \
    macro(manager, view_bid)

#define Roulette_dealer_spin(macro) macro(ContractID, cid)
#define Roulette_dealer_bets_off(macro) macro(ContractID, cid)

#define RouletteRole_dealer(macro) \
    macro(dealer, spin) \
    macro(dealer, bets_off)

#define Roulette_player_check(macro) macro(ContractID, cid)
#define Roulette_player_take(macro) macro(ContractID, cid)

#define Roulette_player_bet(macro) \
    macro(ContractID, cid) \
    macro(uint32_t, iSector)

#define RouletteRole_player(macro) \
    macro(player, check) \
    macro(player, bet) \
    macro(player, take)

#define RouletteRoles_All(macro) \
    macro(manager) \
    macro(dealer) \
    macro(player)

export void Method_0()
{
    // scheme
    Env::DocGroup root("");

    {   Env::DocGroup gr("roles");

#define THE_FIELD(type, name) Env::DocAddText(#name, #type);
#define THE_METHOD(role, name) { Env::DocGroup grMethod(#name);  Roulette_##role##_##name(THE_FIELD) }
#define THE_ROLE(name) { Env::DocGroup grRole(#name); RouletteRole_##name(THE_METHOD) }
        
        RouletteRoles_All(THE_ROLE)
#undef THE_ROLE
#undef THE_METHOD
#undef THE_FIELD
    }
}

#define THE_FIELD(type, name) const type& name,
#define ON_METHOD(role, name) void On_##role##_##name(Roulette_##role##_##name(THE_FIELD) int unused = 0)

void OnError(const char* sz)
{
    Env::DocAddText("error", sz);
}

#pragma pack (push, 1)

// TODO: Move to the common.h
struct KeyPrefix
{
    ContractID m_Cid;
    uint8_t m_Tag; // used to differentiate between keys used by the virtual machine and those used by the contract
};

struct KeyRaw // Key of specific player 
{
    KeyPrefix m_Prefix;
    PubKey m_Player;
};

struct KeyGlobal
{
    KeyPrefix m_Prefix;
    uint8_t m_Val = 0;
};

struct DealerKey
{
#define DEALER_SEED "roulette-dealyer-key" // used to differntiate between public keys for different contracts
    ShaderID m_SID;
    uint8_t m_pSeed[sizeof(DEALER_SEED)];

    DealerKey()
    {
        Env::Memcpy(&m_SID, &Roulette::s_SID, sizeof(Roulette::s_SID));
        Env::Memcpy(m_pSeed, DEALER_SEED, sizeof(DEALER_SEED));
    }

    void DerivePk(PubKey& pk) const
    {
        Env::DerivePk(pk, this, sizeof(*this)); // create public key for the user
    }
};

#pragma pack (pop)

struct StateInfoPlus
{
    Roulette::State m_State;

    bool m_RoundOver;
    bool m_isDealer;

    bool Init(const ContractID& cid) // Reads current state from a specific contract id
    {
        KeyGlobal k;
        k.m_Prefix.m_Cid = cid;
        k.m_Prefix.m_Tag = 0;

        // Read contract variable from the node
        // TODO Add syntactic sugar for this case?
        Env::VarsEnum(&k, sizeof(k), &k, sizeof(k)); // read a range of variables from k to k (one variable)

        const void* pK;
        const Roulette::State* pVal;

        uint32_t nKey, nVal;
        if (!Env::VarsMoveNext(&pK, &nKey, (const void**) &pVal, &nVal) || (sizeof(*pVal) != nVal))
        {
            OnError("failed to read");
            return false;
        }

        m_State = *pVal;

        DealerKey dk;
        PubKey pk;
        dk.DerivePk(pk); // create public key from dealer key seed

        m_isDealer = !Env::Memcmp(&pk, &m_State.m_Dealer, sizeof(pk));
        m_RoundOver = (m_State.m_iWinner < m_State.s_Sectors);

        return true;
    }

    // Check status of the bet
    const char* get_BidStatus(const Roulette::BidInfo& bi, Amount& nWin) const
    {
        nWin = 0;

        if (m_State.m_iRound != bi.m_iRound) // there is a bet for an old round
            return "not-actual";

        if (!m_RoundOver)
            return "in-progress";

        if (bi.m_iSector == m_State.m_iWinner) // exact match, big win!
        {
            nWin = Roulette::State::s_PrizeSector;
            return "jack-pot";
        }

        if (!(1 & (bi.m_iSector ^ m_State.m_iWinner)))
        {
            nWin = Roulette::State::s_PrizeParity;
            return "win";
        }

        return "lose";
    }
};


// This function assumes that ANOTHER function that does vars enum was called before
// Students, never ever write code like this ))
void EnumAndDump(const StateInfoPlus& sip)
{
    if (sip.m_RoundOver)
        Env::DocAddNum("WinSector", sip.m_State.m_iWinner);

    Env::DocArray gr("bids");

    while (true)
    {
        const KeyRaw* pRawKey;
        const Roulette::BidInfo* pVal;

        uint32_t nKey, nVal;
        if (!Env::VarsMoveNext((const void**) &pRawKey, &nKey, (const void**) &pVal, &nVal))
            break;

        if ((sizeof(*pRawKey) == nKey) && (sizeof(*pVal) == nVal))
        {
            Env::DocGroup gr("");

            Env::DocAddBlob_T("Player", pRawKey->m_Player);
            Env::DocAddNum("Sector", pVal->m_iSector);
            Env::DocAddNum("iRound", pVal->m_iRound);

            Amount nWin;
            Env::DocAddText("Status", sip.get_BidStatus(*pVal, nWin));

            if (nWin)
                Env::DocAddNum("Prize", nWin);
        }
    }
}

// 
void EnumBid(const PubKey& pubKey, const ContractID& cid)
{
    KeyRaw k;
    k.m_Prefix.m_Cid = cid;
    k.m_Prefix.m_Tag = 0;
    k.m_Player = pubKey;

    Env::VarsEnum(&k, sizeof(k), &k, sizeof(k));
}

void DumpBid(const PubKey& pubKey, const ContractID& cid)
{
    StateInfoPlus sip;
    if (!sip.Init(cid))
        return;

    EnumBid(pubKey, cid);
    EnumAndDump(sip);
}

ON_METHOD(manager, view)
{

#pragma pack (push, 1)
    struct Key {
        KeyPrefix m_Prefix;
        ContractID m_Cid;
    };
#pragma pack (pop)

    Key k0, k1;
    k0.m_Prefix.m_Cid = Roulette::s_SID;
    k0.m_Prefix.m_Tag = 0x10; // sid-cid tag
    k1.m_Prefix = k0.m_Prefix;

    Env::Memset(&k0.m_Cid, 0, sizeof(k0.m_Cid));
    Env::Memset(&k1.m_Cid, 0xff, sizeof(k1.m_Cid));

    Env::VarsEnum(&k0, sizeof(k0), &k1, sizeof(k1));

    Env::DocArray gr("Cids");

    while (true)
    {
        const Key* pKey;
        const void* pVal;
        uint32_t nKey, nVal;

        if (!Env::VarsMoveNext((const void**) &pKey, &nKey, &pVal, &nVal))
            break;

        if ((sizeof(Key) != nKey) || (1 != nVal))
            continue;

        Env::DocAddBlob_T("", pKey->m_Cid);
    }
}

static const Amount g_DepositCA = 100000000000ULL; // 1K beams

ON_METHOD(manager, create)
{
    Roulette::Params pars;

    DealerKey dk;
    dk.DerivePk(pars.m_Dealer);

    // The following structure describes the input or output of the transaction
    // Basically whether the caller loses or gains funds as a result of this transaction
    FundsChange fc;
    fc.m_Aid = 0; // asset id
    fc.m_Amount = g_DepositCA; // amount of the input or output
    fc.m_Consume = 1; // contract consumes funds (i.e input, in this case)

    // Create kernel with all the required parameters
    // 
    Env::GenerateKernel(nullptr, pars.s_iMethod, &pars, sizeof(pars), &fc, 1, nullptr, 0, 2000000U);
}

ON_METHOD(manager, destroy)
{
    DealerKey dk;

    SigRequest sig;
    sig.m_pID = &dk;
    sig.m_nID = sizeof(dk);

    FundsChange fc;
    fc.m_Aid = 0;
    fc.m_Amount = g_DepositCA;
    fc.m_Consume = 0;

    // contract id
    // number of called method (1 is the destructor)
    // arguments buffer
    // argument pointer size
    // funds change
    // number of funds changes
    // signatures
    // number of signatures
    // transaction fees
    Env::GenerateKernel(&cid, 1, nullptr, 0, &fc, 1, &sig, 1, 2000000U);
}

ON_METHOD(dealer, spin)
{
    DealerKey dk;

    SigRequest sig;
    sig.m_pID = &dk;
    sig.m_nID = sizeof(dk);

    Roulette::Spin arg;
    arg.m_PlayingSectors = 0; // max

    Env::GenerateKernel(&cid, arg.s_iMethod, &arg, sizeof(arg), nullptr, 0, &sig, 1, 2000000U);
}

ON_METHOD(dealer, bets_off)
{
    DealerKey dk;

    SigRequest sig;
    sig.m_pID = &dk;
    sig.m_nID = sizeof(dk);

    Env::GenerateKernel(&cid, Roulette::BetsOff::s_iMethod, nullptr, 0, nullptr, 0, &sig, 1, 2000000U);
}

ON_METHOD(manager, view_params)
{
    StateInfoPlus sip;
    if (!sip.Init(cid))
        return;

    Env::DocGroup gr("params");

    Env::DocAddBlob_T("Dealer", sip.m_State.m_Dealer);
    Env::DocAddText("IsDealer", sip.m_isDealer ? "yes" : "no");
    Env::DocAddNum("iRound", sip.m_State.m_iRound);

    if (sip.m_RoundOver)
        Env::DocAddNum("WinSector", sip.m_State.m_iWinner);
}


ON_METHOD(manager, view_bids)
{
    StateInfoPlus sip;
    if (!sip.Init(cid))
        return;

    KeyPrefix k0, k1;
    k0.m_Cid = cid;
    k0.m_Tag = 0;
    k1.m_Cid = cid;
    k1.m_Tag = 1;

    Env::VarsEnum(&k0, sizeof(k0), &k1, sizeof(k1)); // enum all internal contract vars
    EnumAndDump(sip);
}

ON_METHOD(manager, view_bid)
{
    DumpBid(pubKey, cid);
}

void DerivePlayerPk(PubKey& pubKey, const ContractID& cid)
{
    Env::DerivePk(pubKey, &cid, sizeof(cid));
}

ON_METHOD(player, check)
{
    PubKey pubKey;
    DerivePlayerPk(pubKey, cid);
    DumpBid(pubKey, cid);
}

ON_METHOD(player, bet)
{
    Roulette::Bid arg;
    DerivePlayerPk(arg.m_Player, cid);
    arg.m_iSector = iSector;

    if (arg.m_iSector >= Roulette::State::s_Sectors)
        return OnError("sector is out of range");

    SigRequest sig;
    sig.m_pID = &cid;
    sig.m_nID = sizeof(cid);

    Env::GenerateKernel(&cid, arg.s_iMethod, &arg, sizeof(arg), nullptr, 0, &sig, 1, 1000000U);
}

ON_METHOD(player, take)
{
    StateInfoPlus sip;
    if (!sip.Init(cid))
        return;

    if (!sip.m_RoundOver)
        return OnError("round not finished");

    Roulette::Take arg;
    DerivePlayerPk(arg.m_Player, cid);

    EnumBid(arg.m_Player, cid);

    const KeyRaw* pRawKey;
    const Roulette::BidInfo* pVal;

    uint32_t nKey, nVal;
    if (!Env::VarsMoveNext((const void**) &pRawKey, &nKey, (const void**) &pVal, &nVal) ||
        (sizeof(*pRawKey) != nKey) ||
        (sizeof(*pVal) != nVal))
        return OnError("no bid");

    FundsChange fc;
    fc.m_Aid = sip.m_State.m_Aid; // receive funds of a specific asset type

    sip.get_BidStatus(*pVal, fc.m_Amount);

    if (!fc.m_Amount)
        return OnError("you lost");

    SigRequest sig;
    sig.m_pID = &cid;
    sig.m_nID = sizeof(cid);

    fc.m_Consume = 0;

    Env::GenerateKernel(&cid, arg.s_iMethod, &arg, sizeof(arg), &fc, 1, &sig, 1, 1000000U);
}

#undef ON_METHOD
#undef THE_FIELD

export void Method_1() 
{
    Env::DocGroup root("");

    char szRole[0x10], szAction[0x10];

    if (!Env::DocGetText("role", szRole, sizeof(szRole)))
        return OnError("Role not specified");

    if (!Env::DocGetText("action", szAction, sizeof(szAction)))
        return OnError("Action not specified");

    const char* szErr = nullptr;

#define PAR_READ(type, name) type arg_##name; Env::DocGet(#name, arg_##name);
#define PAR_PASS(type, name) arg_##name,

#define THE_METHOD(role, name) \
        if (!Env::Strcmp(szAction, #name)) { \
            Roulette_##role##_##name(PAR_READ) \
            On_##role##_##name(Roulette_##role##_##name(PAR_PASS) 0); \
            return; \
        }

#define THE_ROLE(name) \
    if (!Env::Strcmp(szRole, #name)) { \
        RouletteRole_##name(THE_METHOD) \
        return OnError("invalid Action"); \
    }

    RouletteRoles_All(THE_ROLE)

#undef THE_ROLE
#undef THE_METHOD
#undef PAR_PASS
#undef PAR_READ

    OnError("unknown Role");
}
