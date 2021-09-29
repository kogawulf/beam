#include "../common.h"
#include "contract.h"
#include "../Math.h"

#define BEAMX_ALLOCATION_All(macro, macro_seed) \
    macro(macro_seed(0xad,0x9a,0xf9,0x28,0x79,0xd8,0xb9,0xb9,0xcf,0x9c,0xfe,0x33,0x01,0xbb,0xc4,0x36,0xc6,0x69,0xa0,0xc0,0x51,0x9a,0xe0,0xc5,0xa6,0xf7,0x15,0x26,0x29,0x69,0x35,0x48,0x01), 1450000, 0, 1)  /* Kate Veremeyuk          */ \
    macro(macro_seed(0x37,0xec,0x6f,0x86,0x24,0x07,0xb0,0x22,0x75,0x56,0xcc,0x75,0x0c,0xd3,0x4b,0x7d,0x2b,0x3b,0xcf,0x65,0xa3,0x62,0x8a,0x76,0x31,0xdf,0x8d,0xac,0x1c,0xde,0x4e,0x92,0x01), 1350000, 0, 3)  /* Robert Yehiazaran       */ \
    macro(macro_seed(0x45,0xcc,0x26,0x1f,0x97,0xa7,0x20,0xe8,0xdf,0x09,0x08,0x9c,0xfc,0x57,0x94,0x6a,0xd9,0xf3,0x58,0x2d,0x71,0x63,0x2b,0x01,0xb5,0xae,0x7d,0xe5,0xdf,0xb3,0xe2,0x3a,0x01), 1370000, 1, 3)  /* Max                     */ \
    macro(macro_seed(0xc7,0x7d,0x20,0x83,0x79,0x95,0xcc,0x45,0x31,0xbd,0xa9,0x40,0x2b,0x9f,0xf3,0xf5,0x5a,0x48,0x74,0x4e,0xd1,0x07,0x35,0x8b,0xfa,0xe3,0x5a,0xfa,0xc8,0x9c,0x75,0x96,0x00), 1210000, 0, 2)  \
    macro(macro_seed(0x9c,0xf2,0x80,0x9a,0x9d,0x3f,0xe4,0x90,0x8b,0x2a,0x83,0xef,0x3f,0xe0,0x5c,0xc2,0x28,0x46,0xcc,0xf1,0x47,0x61,0xc1,0x0c,0x25,0xea,0x58,0x3b,0x06,0x72,0x0f,0x85,0x01), 1790000, 0, 1)  /* Denis Asipenko (vadim)  */ \
    macro(macro_seed(0x3a,0xd0,0x36,0x6a,0xc9,0x32,0x8b,0xf7,0xb2,0x48,0xd4,0xf0,0x19,0xad,0xb4,0x3a,0x03,0x34,0x4a,0xd4,0x69,0x68,0xeb,0x52,0xb5,0x44,0x0b,0x03,0xf2,0x2a,0x61,0x09,0x01), 1180000, 0, 1)  /* Last Jedi               */ \
    macro(macro_seed(0xc9,0x19,0xf2,0xbb,0xeb,0x3d,0x12,0xed,0x93,0x1e,0x11,0x2d,0x0c,0x5d,0x43,0x92,0x4d,0xb8,0x1a,0x26,0xa8,0xbb,0x2d,0x40,0x97,0xc6,0xd0,0x1e,0x3a,0xc0,0x62,0x0d,0x00), 1220000, 0, 1)  /* Asia                    */ \
    macro(macro_seed(0x31,0x7d,0x15,0x46,0x22,0xbc,0x5e,0xb7,0xa2,0xc5,0x81,0xa6,0xa0,0x4a,0x20,0x86,0x5b,0xbb,0x23,0xdd,0x70,0xe7,0x86,0xac,0x35,0x83,0x97,0x18,0x8c,0xec,0x47,0x5a,0x00), 1380000, 0, 2)  /* Alexander Braginskiy    */ \
    macro(macro_seed(0x2a,0xcd,0x0f,0xbb,0x82,0x4f,0xb0,0x32,0x25,0xe8,0xa8,0xb6,0x8d,0x15,0xcd,0x23,0xb0,0x01,0xf5,0x75,0x13,0xa1,0x7b,0xf8,0xee,0x81,0x13,0xe1,0x1b,0x33,0xa7,0x6f,0x00), 1900000, 0, 2)  /* Vika                    */ \
    macro(macro_seed(0x0c,0xbb,0x1b,0x66,0xfb,0x1c,0x6a,0xf6,0xf2,0xa1,0xa4,0xc9,0x4b,0x9f,0x41,0xcc,0xf9,0xb6,0x68,0x97,0x0d,0x3c,0x0d,0x6b,0x11,0xf0,0x4f,0x02,0x8c,0x2b,0x10,0xcf,0x01), 1830000, 0, 2)  /* Sergei Zayats           */ \
    macro(macro_seed(0xe4,0xf2,0x3b,0xca,0x24,0x39,0x1c,0xfd,0x83,0x8e,0xe2,0x64,0x5a,0x77,0x16,0xd8,0xcb,0x8d,0xb1,0x67,0xd7,0x79,0xfd,0x03,0x45,0xd6,0xba,0x9a,0x51,0xcc,0x44,0x8c,0x00), 1700000, 0, 2)  /* Anatol                  */ \
    macro(macro_seed(0xfa,0x39,0x9a,0x38,0x19,0x9e,0x54,0x33,0x21,0x34,0x6d,0x79,0x5c,0x0c,0x16,0xfd,0x07,0x29,0x2e,0x3e,0x69,0xb8,0x6a,0xd8,0xab,0x8d,0x07,0x44,0x56,0x83,0xd0,0x38,0x01), 1650000, 0, 3)  /* bigromanov              */ \
    macro(macro_seed(0x36,0xcc,0xb3,0x3f,0x36,0x0d,0xfa,0xe0,0xeb,0xa4,0x2c,0x5f,0xb4,0x80,0x19,0x33,0x8c,0x53,0x1b,0xd4,0x92,0xe5,0x1c,0xa7,0x94,0x67,0xd6,0x03,0xea,0x7e,0xa2,0xff,0x00), 1050000, 0, 3)  /* vain onnellinen         */ \
    macro(macro_seed(0x10,0xb6,0x3d,0x83,0x4e,0x04,0xf8,0x6c,0xc8,0x4a,0x97,0x09,0x43,0x0a,0x88,0x4a,0xb5,0xa1,0xd9,0x00,0x96,0xe1,0x96,0x73,0x98,0xe8,0x42,0xbe,0xe9,0x20,0x4c,0xfe,0x00), 1430000, 0, 3)  /* Artem                   */ \
    macro(macro_seed(0xea,0x88,0x6a,0xa5,0x8e,0x4d,0x24,0xb9,0x6a,0xbb,0x5d,0x9a,0x9e,0x5f,0xca,0xbd,0xe9,0x43,0xc7,0xfb,0xb0,0x9b,0x4b,0x73,0xd8,0x9a,0x2c,0x6f,0xda,0x3c,0xae,0x8f,0x00), 1390000, 0, 3)  /* Roman                   */ \
    macro(macro_seed(0x35,0x3e,0x2e,0x65,0x18,0x51,0x7e,0x2e,0x7b,0xa7,0x87,0xf3,0x8c,0xb9,0x76,0x2a,0x8a,0xb7,0x64,0x8e,0x1a,0x48,0x3b,0x61,0xd3,0x29,0x10,0xa0,0xba,0x4c,0xef,0xbb,0x01), 1770000, 1, 2)  /* happy Sasha             */ \
    macro(macro_seed(0x1e,0x54,0x5b,0xce,0xf7,0xb4,0xd2,0xc8,0x1a,0x24,0xe2,0xa8,0xe1,0x26,0x46,0x9e,0x0f,0x3e,0x4d,0xec,0x5f,0x11,0x0d,0xcd,0xef,0x35,0x65,0x52,0x10,0x27,0x5e,0x01,0x01), 1290000, 1, 2)  /* Starun                  */ \
    macro(macro_seed(0x8b,0xb3,0x37,0x5b,0x45,0x5d,0x9c,0x57,0x71,0x34,0xb0,0x0e,0x8b,0x0b,0x10,0x8a,0x29,0xce,0x2b,0xc0,0xfc,0xe9,0x29,0x04,0x93,0x06,0xcf,0x4f,0xed,0x72,0x3b,0x7d,0x01), 1310000, 1, 3)  /* Vladik                  */ \
    macro(macro_seed(0xa4,0x22,0xf0,0xc6,0xd7,0x35,0x0c,0x48,0xdc,0x2c,0x3d,0x63,0xcc,0x04,0xdc,0x8b,0xb0,0xeb,0x28,0x3f,0x19,0xb3,0x0b,0xd2,0xde,0x44,0xba,0x1c,0xab,0x1d,0xad,0x02,0x01), 1480000, 0, 1)  /* Denis                   */ \
    macro(macro_seed(0xc2,0x90,0xe9,0x45,0xac,0xe6,0x6e,0xe7,0xdb,0x85,0xd9,0x8c,0x0f,0x59,0xcc,0xe1,0x7b,0x71,0x93,0xcc,0xb5,0x14,0x86,0xd5,0x45,0x11,0x31,0x3d,0xf1,0x02,0x4c,0x3b,0x01), 1780000, 0, 1) \
    macro(macro_seed(0x81,0x11,0x27,0x43,0xe2,0x98,0xb4,0x09,0x6b,0xfd,0xb3,0x4c,0xf9,0x7d,0x81,0x5e,0xb3,0x82,0xaf,0x91,0xa2,0x38,0x78,0xd7,0x83,0x49,0x8c,0x8f,0xcf,0xf6,0xa2,0xd6,0x01), 1630000, 0, 2) \
    macro(macro_seed(0x4b,0x80,0x53,0xd3,0x26,0x59,0xa9,0xd4,0xb0,0x33,0x8d,0x51,0xc2,0x91,0xd1,0xe7,0xd2,0x08,0xa8,0x64,0x5b,0x05,0xc5,0x6b,0x8d,0x77,0x23,0x6e,0x9e,0x0e,0x1c,0x8d,0x01), 1180000, 1, 2) \
    macro(macro_seed(0x60,0x03,0xcd,0x94,0x34,0xc4,0x8a,0x86,0xdc,0x43,0x5a,0x3c,0x16,0x0a,0x0b,0xfc,0x64,0xbc,0xac,0xc3,0x9e,0x2d,0x48,0x12,0xa0,0xd6,0x2d,0x2a,0xe4,0x6e,0x18,0x4b,0x00), 1950000, 1, 3) \
    macro(macro_seed(0x7f,0x2c,0x0e,0x81,0xdb,0x39,0x2e,0x25,0x2d,0xae,0x70,0x35,0x99,0x3b,0x03,0xeb,0xcf,0x4d,0x90,0x01,0xa0,0x6d,0x50,0x79,0x4d,0x4f,0x32,0x0a,0x56,0x46,0x65,0x17,0x01), 1260000, 1, 2) \
    macro(macro_seed(0x24,0x8a,0xaa,0x8d,0xfa,0x15,0xe7,0x8c,0x77,0xea,0x7d,0x36,0x82,0xb9,0x76,0x58,0x3e,0xf2,0xd8,0xcc,0x9c,0x40,0xe5,0x17,0x0f,0x94,0x1d,0x2e,0xf0,0xc8,0x05,0xc7,0x00), 1370000, 0, 1) \
    macro(macro_seed(0x3f,0xf6,0xbb,0xef,0x62,0x32,0xe6,0x3c,0xc7,0x0d,0x23,0x05,0x34,0xbe,0x2d,0x7a,0x28,0xe6,0xae,0xe1,0xd5,0x0f,0xe8,0x0a,0xe6,0x56,0x65,0xcc,0xb6,0x91,0x81,0xd1,0x01), 1710000, 0, 1) \
    macro(macro_seed(0xa6,0x77,0x67,0x5b,0xe5,0x0f,0x3e,0xb6,0xaf,0xee,0x8d,0xa2,0x10,0xbc,0x3a,0xca,0xd2,0xd8,0xa2,0xc6,0x79,0x33,0xe5,0xbb,0x97,0x96,0x51,0xd9,0x13,0x42,0x5e,0x9b,0x01), 1550000, 1, 3) \
    macro(macro_seed(0xe4,0x3f,0x26,0xcf,0xf1,0x0a,0x17,0x3c,0xf5,0x94,0x3e,0xeb,0xf9,0xb8,0x2d,0x94,0x7d,0x66,0x9f,0x45,0x8b,0xa3,0x7c,0x9b,0x3f,0xf6,0x18,0xab,0x98,0xca,0xaf,0x3c,0x00), 1420000, 2, 3) \
    macro(macro_seed(0xa1,0xaa,0x9e,0x09,0x7f,0xad,0xa9,0xf8,0x6f,0x7f,0x05,0x2f,0xc9,0x79,0x9f,0xa5,0xee,0x35,0x42,0xee,0xaf,0x54,0x00,0x7a,0x7d,0xa9,0x7a,0x64,0xe5,0xa1,0xab,0xc1,0x01), 1790000, 0, 1) \
    macro(macro_seed(0xd9,0xf9,0x27,0xb5,0x6e,0xce,0xbd,0xe6,0xdc,0x5c,0xfc,0x8f,0x27,0x79,0x1c,0x7e,0x00,0xb3,0xb6,0x09,0x18,0xea,0xfe,0xeb,0x71,0x5f,0x7c,0xcf,0x0a,0xf1,0x24,0x52,0x00),  610000, 0, 1) \


void AllocateAll(const DaoCore::State& s)
{
    // Ensure totals agree.
#define MACRO_NOP(...)
#define THE_MACRO(seed, valBeams, start, end) valBeams+
    // should be inlined
    constexpr Amount nPreallocatedAssigned = g_Beam2Groth * (BEAMX_ALLOCATION_All(THE_MACRO, MACRO_NOP) 0);
#undef THE_MACRO

    constexpr Amount nPreallocatedUnassigned = g_Beam2Groth * (
        35'000'000 + // liquidity mining
        20'000'000 // Dao treasury 
    );

    static_assert(nPreallocatedAssigned + nPreallocatedUnassigned == DaoCore::Preallocated::s_Emission);

    // Ensure vesting periods are well-defined (order, won't overflow)
    constexpr uint32_t nBlockPerMonth = 1440 * 365 / 12;

#define THE_MACRO(seed, valBeams, start, end) \
    static_assert(start < end); \
    static_assert(((uint32_t) (start * nBlockPerMonth)) / nBlockPerMonth == start); \
    static_assert(((uint32_t) (end * nBlockPerMonth)) / nBlockPerMonth == end);

    BEAMX_ALLOCATION_All(THE_MACRO, MACRO_NOP)
#undef THE_MACRO
#undef MACRO_NOP


    // Allocate everything
    Env::Halt_if(!Env::AssetEmit(s.m_Aid, DaoCore::Preallocated::s_Emission + DaoCore::Farming::State::s_Emission, 1));

    // Assign preallocated
#pragma pack (push, 1)
    struct Entry {
        Opaque<33> m_Pk;
        uint32_t m_ValueBeams;
        uint8_t m_Month_0;
        uint8_t m_Month_Delta;
    };
#pragma pack (pop)

#define THE_MACRO(seed, valBeams, month_start, month_end) { { seed }, valBeams, month_start, month_end - month_start },
#define THE_MACRO_SEED(...) { __VA_ARGS__ }

    static const Entry s_pE[] = {
        BEAMX_ALLOCATION_All(THE_MACRO, THE_MACRO_SEED)
    };
#undef THE_MACRO
#undef THE_MACRO_SEED


    Height h = Env::get_Height();

    DaoCore::Preallocated::User::Key puk;
    DaoCore::Preallocated::User pu;
    pu.m_Received = 0;

    for (uint32_t i = 0; i < _countof(s_pE); i++)
    {
        const auto& e = s_pE[i];
        _POD_(puk.m_Pk) = e.m_Pk;

        pu.m_Total = g_Beam2Groth * (Amount) e.m_ValueBeams;

        pu.m_Vesting_h0 = h + nBlockPerMonth * e.m_Month_0;
        pu.m_Vesting_dh = nBlockPerMonth * e.m_Month_Delta;

        Env::Halt_if(Env::SaveVar_T(puk, pu)); // would fail if key is duplicated
    }
}

BEAM_EXPORT void Ctor(const void*)
{
    if (Env::get_CallDepth() > 1)
    {
        DaoCore::State s;

        static const char szMeta[] = "STD:SCH_VER=1;N=BeamX Token;SN=BeamX;UN=BEAMX;NTHUN=GROTHX";
        s.m_Aid = Env::AssetCreate(szMeta, sizeof(szMeta) - 1);
        Env::Halt_if(!s.m_Aid);

        AllocateAll(s);

        Env::SaveVar_T((uint8_t) s.s_Key, s);
    }
}

BEAM_EXPORT void Dtor(void*)
{
}

BEAM_EXPORT void Method_2(void*)
{
    // called on upgrade. N/A
    Env::Halt();
}

BEAM_EXPORT void Method_3(const DaoCore::GetPreallocated& r)
{
    DaoCore::Preallocated::User::Key puk;
    _POD_(puk.m_Pk) = r.m_Pk;

    DaoCore::Preallocated::User pu;
    Env::Halt_if(!Env::LoadVar_T(puk, pu));

    Strict::Add(pu.m_Received, r.m_Amount);

    Height dh = Env::get_Height();
    Strict::Sub(dh, pu.m_Vesting_h0);
    dh = std::min(dh, pu.m_Vesting_dh);

    Env::Halt_if(
        MultiPrecision::From(pu.m_Received) * MultiPrecision::From(pu.m_Vesting_dh) >
        MultiPrecision::From(pu.m_Total) * MultiPrecision::From(dh));

    // ok
    if (pu.m_Received < pu.m_Total)
        Env::SaveVar_T(puk, pu);
    else
        Env::DelVar_T(puk);

    DaoCore::State s;
    Env::LoadVar_T((uint8_t) s.s_Key, s);
    Env::FundsUnlock(s.m_Aid, r.m_Amount);

    Env::AddSig(r.m_Pk);
}

BEAM_EXPORT void Method_4(const DaoCore::UpdPosFarming& r)
{
    Height h = Env::get_Height();

    DaoCore::Farming::State fs;
    if (Env::LoadVar_T((uint8_t) DaoCore::Farming::s_Key, fs))
        fs.Update(h);
    else
        _POD_(fs).SetZero();
    fs.m_hLast = h;

    DaoCore::Farming::UserPos up;
    DaoCore::Farming::UserPos::Key uk;
    _POD_(uk.m_Pk) = r.m_Pk;

    if (Env::LoadVar_T(uk, up))
    {
        Amount newBeamX = fs.RemoveFraction(up);
        Strict::Add(up.m_BeamX, newBeamX);
    }
    else
        _POD_(up).SetZero();

    up.m_SigmaLast = fs.m_Sigma;

    if (r.m_WithdrawBeamX)
    {
        Strict::Sub(up.m_BeamX, r.m_WithdrawBeamX);

        DaoCore::State s;
        Env::LoadVar_T((uint8_t) s.s_Key, s);

        Env::FundsUnlock(s.m_Aid, r.m_WithdrawBeamX);
    }

    if (r.m_Beam)
    {
        if (r.m_BeamLock)
        {
            Strict::Add(up.m_Beam, r.m_Beam);
            Env::FundsLock(0, r.m_Beam);
        }
        else
        {
            Strict::Sub(up.m_Beam, r.m_Beam);
            Env::FundsUnlock(0, r.m_Beam);
        }
    }

    fs.m_WeightTotal += DaoCore::Farming::Weight::Calculate(up.m_Beam);

    // fin
    Env::SaveVar_T((uint8_t) DaoCore::Farming::s_Key, fs);

    if (up.m_Beam || up.m_BeamX)
        Env::SaveVar_T(uk, up);
    else
        Env::DelVar_T(uk);

    Env::AddSig(r.m_Pk);

}
