#pragma once
namespace Upgradable2
{
    static const ShaderID s_SID = {0xa9,0x06,0x69,0xa8,0x49,0x94,0x6c,0xe1,0x33,0x56,0x2b,0x2f,0x91,0x09,0x14,0xd3,0x8d,0x9f,0x45,0xfa,0x44,0x7d,0x15,0x1c,0xb9,0x2f,0xeb,0xab,0x1a,0xda,0xef,0x00};

#pragma pack (push, 1) // the following structures will be stored in the node in binary form

    struct Active {
        ContractID m_Cid;
    };

    struct Next {
        ContractID m_Cid;
        Height m_hTarget;
    };

    struct State
    {
        static const uint16_t s_Key = 0x7700; // whatever, should not be used by the callee impl

        // part of the state (slice) that is accessed on each invocation.
        Active m_Active;
        Next m_Next;
    };

    struct Settings
    {
        static const uint16_t s_Key = 0x7701; // accessed only on admin actions

        Height m_hMinUpgadeDelay;
        uint32_t m_MinApprovers;

        static const uint32_t s_AdminsMax = 32;
        PubKey m_pAdmin[s_AdminsMax];
    };

    struct Create
    {
        static const uint32_t s_iMethod = 0;

        Active m_Active;
        Settings m_Settings;
    };

    struct Control
    {
        static const uint32_t s_iMethod = 2;

        struct Base
        {
            uint8_t m_Type;
            Base(uint8_t nType) :m_Type(nType) {}
        };

        struct ExplicitUpgrade
            :public Base
        {
            static const uint8_t s_Type = 1;
            ExplicitUpgrade() :Base(s_Type) {}

            // induce the scheduled upgrade. Without the need to 'hijack' another method.
            // doesn't need to be signed (i.e. any user can invoke)
        };


        struct Signed
            :public Base
        {
            Signed(uint8_t nType) :Base(nType) {}
            uint32_t m_ApproveMask; // bitmask of approvers. Each must sign this call
        };


        struct ScheduleUpgrade
            :public Signed
        {
            static const uint8_t s_Type = 2;
            ScheduleUpgrade() :Signed(s_Type) {}

            Next m_Next;
        };

        struct ReplaceAdmin
            :public Signed
        {
            static const uint8_t s_Type = 3;
            ReplaceAdmin() :Signed(s_Type) {}

            uint32_t m_iAdmin;
            PubKey m_Pk;
        };

        struct SetApprovers
            :public Signed
        {
            static const uint8_t s_Type = 4;
            SetApprovers() :Signed(s_Type) {}

            uint32_t m_NewVal; // must be within [1, s_AdminsMax]
        };
    };

#pragma pack (pop)
}
