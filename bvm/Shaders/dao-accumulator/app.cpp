#include "../common.h"
#include "../common.h"
#include "../app_common_impl.h"
#include "../upgradable3/app_common_impl.h"
#include "contract.h"

#define DaoAcc_schedule_upgrade(macro) Upgradable3_schedule_upgrade(macro)
#define DaoAcc_replace_admin(macro) Upgradable3_replace_admin(macro)
#define DaoAcc_set_min_approvers(macro) Upgradable3_set_min_approvers(macro)
#define DaoAcc_explicit_upgrade(macro) macro(ContractID, cid)

#define DaoAcc_deploy(macro) \
    Upgradable3_deploy(macro) \
    macro(Height, hPrePhaseEnd) \
    macro(AssetID, aidBeamX)

#define DaoAcc_view_deployed(macro)

#define DaoAccActions_All(macro) \
    macro(view_deployed) \
    macro(deploy) \
	macro(schedule_upgrade) \
	macro(replace_admin) \
	macro(set_min_approvers) \
	macro(explicit_upgrade) \


namespace DaoAccumulator {

using MultiPrecision::Float;

BEAM_EXPORT void Method_0()
{
    // scheme
    Env::DocGroup root("");

    {   Env::DocGroup gr("actions");

#define THE_FIELD(type, name) Env::DocAddText(#name, #type);
#define THE_ACTION(name) { Env::DocGroup gr(#name);  DaoAcc_##name(THE_FIELD) }
        
        DaoAccActions_All(THE_ACTION)
#undef THE_ACTION
#undef THE_FIELD
    }
}

#define THE_FIELD(type, name) const type& name,
#define ON_METHOD(name) void On_##name(DaoAcc_##name(THE_FIELD) int unused = 0)

void OnError(const char* sz)
{
    Env::DocAddText("error", sz);
}

const char g_szAdminSeed[] = "upgr3-dao-accumulator";

struct AdminKeyID :public Env::KeyID {
    AdminKeyID() :Env::KeyID(g_szAdminSeed, sizeof(g_szAdminSeed)) {}
};

const Upgradable3::Manager::VerInfo g_VerInfo = { s_pSID, _countof(s_pSID) };

ON_METHOD(view_deployed)
{
    AdminKeyID kid;
    g_VerInfo.DumpAll(&kid);
}

ON_METHOD(deploy)
{
    AdminKeyID kid;
    PubKey pk;
    kid.get_Pk(pk);

    Method::Create arg;
    if (!g_VerInfo.FillDeployArgs(arg.m_Upgradable, &pk))
        return;

    if (hPrePhaseEnd <= Env::get_Height())
        return OnError("pre-phase too short");
    arg.m_hPrePhaseEnd = hPrePhaseEnd;

    if (!aidBeamX)
        return OnError("BeamX aid not specified");
    arg.m_aidBeamX = aidBeamX;

    const uint32_t nCharge =
        Upgradable3::Manager::get_ChargeDeploy() +
        Env::Cost::SaveVar_For(sizeof(State)) +
        Env::Cost::Cycle * 50;

    Env::GenerateKernel(nullptr, arg.s_iMethod, &arg, sizeof(arg), nullptr, 0, nullptr, 0, "Deploy Dao-Accumulator contract", nCharge);
}

ON_METHOD(schedule_upgrade)
{
    AdminKeyID kid;
    g_VerInfo.ScheduleUpgrade(cid, kid, hTarget);
}

ON_METHOD(explicit_upgrade)
{
    Upgradable3::Manager::MultiSigRitual::Perform_ExplicitUpgrade(cid);
}

ON_METHOD(replace_admin)
{
    AdminKeyID kid;
    Upgradable3::Manager::MultiSigRitual::Perform_ReplaceAdmin(cid, kid, iAdmin, pk);
}

ON_METHOD(set_min_approvers)
{
    AdminKeyID kid;
    Upgradable3::Manager::MultiSigRitual::Perform_SetApprovers(cid, kid, newVal);
}


#undef ON_METHOD
#undef THE_FIELD

BEAM_EXPORT void Method_1() 
{
    Env::DocGroup root("");

    char szAction[0x20];

    if (!Env::DocGetText("action", szAction, sizeof(szAction)))
        return OnError("Action not specified");

#define PAR_READ(type, name) type arg_##name; Env::DocGet(#name, arg_##name);
#define PAR_PASS(type, name) arg_##name,

#define THE_METHOD(name) \
        static_assert(sizeof(szAction) >= sizeof(#name)); \
        if (!Env::Strcmp(szAction, #name)) { \
            DaoAcc_##name(PAR_READ) \
            On_##name(DaoAcc_##name(PAR_PASS) 0); \
            return; \
        }

    DaoAccActions_All(THE_METHOD)

#undef THE_METHOD
#undef PAR_PASS
#undef PAR_READ

    OnError("unknown action");
}

} // namespace DaoAccumulator
