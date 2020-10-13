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

#pragma once

#define BVMOp_Memcpy(macro, sep) \
	macro(void*, pDst) sep \
	macro(const void*, pSrc) sep \
	macro(uint32_t, size)

#define BVMOp_Memcmp(macro, sep) \
	macro(const void*, p1) sep \
	macro(const void*, p2) sep \
	macro(uint32_t, size)

#define BVMOp_Memis0(macro, sep) \
	macro(const void*, p) sep \
	macro(uint32_t, size)

#define BVMOp_Memset(macro, sep) \
	macro(const void*, pDst) sep \
	macro(uint8_t, val) sep \
	macro(uint32_t, size)

#define BVMOp_LoadVar(macro, sep) \
	macro(const void*, pKey) sep \
	macro(uint32_t, nKey) sep \
	macro(void*, pVal) sep \
	macro(uint32_t, nVal)

#define BVMOp_SaveVar(macro, sep) \
	macro(const void*, pKey) sep \
	macro(uint32_t, nKey) sep \
	macro(const void*, pVal) sep \
	macro(uint32_t, nVal)

#define BVMOp_CallFar(macro, sep) \
	macro(const ContractID&, cid) sep \
	macro(uint32_t, iMethod) sep \
	macro(void*, pArgs)

#define BVMOp_Halt(macro, sep)

#define BVMOp_AddSig(macro, sep) \
	macro(const PubKey&, pubKey)

#define BVMOp_FundsLock(macro, sep) \
	macro(AssetID, aid) sep \
	macro(Amount, amount)

#define BVMOp_FundsUnlock(macro, sep) \
	macro(AssetID, aid) sep \
	macro(Amount, amount)

#define BVMOp_RefAdd(macro, sep) \
	macro(const ContractID&, cid)

#define BVMOp_RefRelease(macro, sep) \
	macro(const ContractID&, cid)

#define BVMOp_AssetCreate(macro, sep) \
	macro(const void*, pMeta) sep \
	macro(uint32_t, nMeta)

#define BVMOp_AssetEmit(macro, sep) \
	macro(AssetID, aid) sep \
	macro(Amount, amount) sep \
	macro(uint8_t, bEmit)

#define BVMOp_AssetDestroy(macro, sep) \
	macro(AssetID, aid)

#define BVMOpsAll(macro) \
	macro(0x10, void*    , Memcpy) \
	macro(0x11, void*    , Memset) \
	macro(0x12, int32_t  , Memcmp) \
	macro(0x13, uint8_t  , Memis0) \
	macro(0x20, uint32_t , LoadVar) \
	macro(0x21, void     , SaveVar) \
	macro(0x23, void     , CallFar) \
	macro(0x28, void     , Halt) \
	macro(0x29, void     , AddSig) \
	macro(0x30, void     , FundsLock) \
	macro(0x31, void     , FundsUnlock) \
	macro(0x32, uint8_t  , RefAdd) \
	macro(0x33, uint8_t  , RefRelease) \
	macro(0x38, AssetID  , AssetCreate) \
	macro(0x39, uint8_t  , AssetEmit) \
	macro(0x3A, uint8_t  , AssetDestroy) \
