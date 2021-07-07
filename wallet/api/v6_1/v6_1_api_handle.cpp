// Copyright 2018 The Beam Team
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include "v6_1_api.h"
#include "version.h"

namespace beam::wallet
{
    void V61Api::onHandleEvSubUnsub(const JsonRpcId &id, const EvSubUnsub &data)
    {
        auto oldSubs = _evSubs;

        if (data.systemState.is_initialized())
        {
            _evSubs = *data.systemState ? _evSubs | SubFlags::SystemState : _evSubs & ~SubFlags::SystemState;
        }

        if (data.syncProgress.is_initialized())
        {
            _evSubs = *data.syncProgress ? _evSubs | SubFlags::SyncProgress : _evSubs & ~SubFlags::SyncProgress;
        }

        if (data.assetChanged.is_initialized())
        {
            _evSubs = *data.assetChanged ? _evSubs | SubFlags::AssetChanged : _evSubs & ~SubFlags::AssetChanged;
        }

        if (data.coinsChanged.is_initialized())
        {
            _evSubs = *data.coinsChanged ? _evSubs | SubFlags::CoinsChanged : _evSubs & ~SubFlags::CoinsChanged;
        }

        if (_evSubs && !_subscribedToListener)
        {
            getWallet()->Subscribe(this);
            _subscribedToListener = true;
        }

        EvSubUnsub::Response resp{true};
        doResponse(id, resp);

        //
        // Generate all newly subscribed events immediately
        // This is convenient for subscribers to get initial system state
        //
        if ((_evSubs & SubFlags::SystemState) != 0 && (oldSubs & SubFlags::SystemState) == 0)
        {
            Block::SystemState::ID stateID = {};
            getWalletDB()->getSystemStateID(stateID);
            onSystemStateChanged(stateID);
        }

        if ((_evSubs & SubFlags::SyncProgress) != 0 && (oldSubs & SubFlags::SyncProgress) == 0)
        {
            onSyncProgress(0, 0);
        }

        if ((_evSubs & SubFlags::AssetChanged) != 0 && (oldSubs & SubFlags::AssetChanged) == 0)
        {
            getWalletDB()->visitAssets([this](const WalletAsset& info) -> bool {
                onAssetChanged(info.m_ID);
                return true;
            });
        }

        if ((_evSubs & SubFlags::CoinsChanged) != 0 && (oldSubs & SubFlags::CoinsChanged) == 0)
        {
            // TODO: add shielded
            std::vector<Coin> coins;
            getWalletDB()->visitCoins([&coins](const Coin& c) -> bool {
                coins.push_back(c);
                return true;
            });

            if (!coins.empty())
            {
                onCoinsChanged(ChangeAction::Reset, coins);
            }
        }
    }

    void V61Api::onHandleGetVersion(const JsonRpcId& id, const GetVersion& params)
    {
        GetVersion::Response resp;

        resp.apiVersion          = _apiVersion;
        resp.apiVersionMajor     = _apiVersionMajor;
        resp.apiVersionMinor     = _apiVersionMinor;
        resp.beamVersion         = PROJECT_VERSION;
        resp.beamVersionMajor    = VERSION_MAJOR;
        resp.beamVersionMinor    = VERSION_MINOR;
        resp.beamVersionRevision = VERSION_REVISION;
        resp.beamCommitHash      = GIT_COMMIT_HASH;
        resp.beamBranchName      = BRANCH_NAME;

        doResponse(id, resp);
    }

    void V61Api::onHandleWalletStatusV61(const JsonRpcId &id, const WalletStatusV61 &data)
    {
        LOG_DEBUG() << "WalletStatusV61(id = " << id << ")";

        WalletStatusV61::Response response;
        auto walletDB = getWalletDB();

        {
            Block::SystemState::ID stateID = {};
            walletDB->getSystemStateID(stateID);
            response.currentHeight = stateID.m_Height;
            response.currentStateHash = stateID.m_Hash;

            Block::SystemState::Full state;
            walletDB->get_History().get_At(state, stateID.m_Height);

            Merkle::Hash stateHash;
            state.get_Hash(stateHash);
            assert(stateID.m_Hash == stateHash);

            response.prevStateHash = state.m_Prev;
            response.difficulty = state.m_PoW.m_Difficulty.ToFloat();
            response.currentStateTimestamp = state.m_TimeStamp;
            response.isInSync = IsValidTimeStamp(state.m_TimeStamp);
        }

        storage::Totals allTotals(*walletDB);
        const auto& totals = allTotals.GetBeamTotals();

        response.available = AmountBig::get_Lo(totals.Avail);    response.available += AmountBig::get_Lo(totals.AvailShielded);
        response.receiving = AmountBig::get_Lo(totals.Incoming); response.receiving += AmountBig::get_Lo(totals.IncomingShielded);
        response.sending   = AmountBig::get_Lo(totals.Outgoing); response.sending   += AmountBig::get_Lo(totals.OutgoingShielded);
        response.maturing  = AmountBig::get_Lo(totals.Maturing); response.maturing  += AmountBig::get_Lo(totals.MaturingShielded);

        if (data.withAssets)
        {
            response.totals = allTotals;
        }

        doResponse(id, response);
    }
}
