// Copyright 2019 The Beam Team
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

#include "common.h"
#include "../../common.h"

#include "bitcoin/bitcoin.hpp"

namespace beam::bitcoin_cash
{
    uint8_t getAddressVersion()
    {
        return wallet::UseMainnetSwap() ?
            libbitcoin::wallet::ec_private::mainnet_p2kh :
            libbitcoin::wallet::ec_private::testnet_p2kh;
    }

    std::vector<std::string> getGenesisBlockHashes()
    {
        if (wallet::UseMainnetSwap())
            return { bitcoin::kMainnetGenesisBlockHash };

        return { bitcoin::kTestnetGenesisBlockHash , bitcoin::kRegtestGenesisBlockHash };
    }
} // namespace beam::bitcoin_cash
