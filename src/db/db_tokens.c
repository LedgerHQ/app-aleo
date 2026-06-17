/*****************************************************************************
 *   Ledger App Aleo.
 *   (c) 2025 Ledger SAS.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *****************************************************************************/

#include "db_tokens.h"

#define NB_OF_USAD_STABLECOIN_ALEO_FUNCTIONS (4)
const function_parameters_t usad_stablecoin_aleo[NB_OF_USAD_STABLECOIN_ALEO_FUNCTIONS] = {
    {.name        = "transfer_public",
     .tx_type     = TX_TOKEN_TRANSFER_PUBLIC,
     .input_count = 2,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x7016c26f09710167, 0x663eee036a2b84c7, 0xff969945963665f0, 0x0840f7694adf19c4}},
        {.big.u64
         = {0xaf7aeb36ab9d3daa, 0xe21fd99055ac25e5, 0xa98bc5eb3e42ffd3, 0x0db8bcd8c762fcff}}}},
    {.name        = "transfer_private",
     .tx_type     = TX_TOKEN_TRANSFER_PRIVATE,
     .input_count = 4,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xe855cada0ee78a09, 0x93a372431d2a139c, 0x0783886c6ea1e534, 0x041b23aee6485e92}},
        {.big.u64
         = {0xeaa38337c84bcbbf, 0x7b1568681328e2bd, 0xbc212f0f4d98fcf6, 0x060df5c76f296d04}}}},
    {.name        = "transfer_public_to_private",
     .tx_type     = TX_TOKEN_TRANSFER_PUBLIC_TO_PRIVATE,
     .input_count = 2,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xb1309cd5c98b79c7, 0x18b60953403f2dd1, 0x71b6c039d9064f0a, 0x035a3c084523aa55}},
        {.big.u64
         = {0x7021ac1342e8bc61, 0x7214aed9c0a0afb5, 0xbd3182099e8d2ba9, 0x0690ff6b15ae3658}}}},
    {.name        = "transfer_private_to_public",
     .tx_type     = TX_TOKEN_TRANSFER_PRIVATE_TO_PUBLIC,
     .input_count = 4,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x9bbc069ec4e31ec2, 0xbbb819962b8cd2a7, 0xa4d5ecd5078ef89d, 0x0fb27990548cfcc0}},
        {.big.u64
         = {0xcfc5b3729e5fc2f6, 0x37465905a0b5261e, 0x713ef38dfbcd3f4d, 0x0857ea6a5c492ccf}}}},
};

#define NB_OF_USDCX_STABLECOIN_ALEO_FUNCTIONS (4)
const function_parameters_t usdcx_stablecoin_aleo[NB_OF_USDCX_STABLECOIN_ALEO_FUNCTIONS] = {
    {.name        = "transfer_public",
     .tx_type     = TX_TOKEN_TRANSFER_PUBLIC,
     .input_count = 2,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x2a242068285ecf20, 0xc803693b3e8df42e, 0x011fcdff780d1ece, 0x11cddd13e1401ff0}},
        {.big.u64
         = {0xf361e9e8e9fa1bee, 0x1c7dc49ebde6ee1d, 0x5f22e0981647766d, 0x06898a1eb5965ab9}}}},
    {.name        = "transfer_private",
     .tx_type     = TX_TOKEN_TRANSFER_PRIVATE,
     .input_count = 4,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x07bb94b895d82618, 0xf6ac1a6477071357, 0x13df2a7a88126fa1, 0x11fa2fcead8a976a}},
        {.big.u64
         = {0x238e0600a378e1a8, 0x4117b05533e7778a, 0x76e11388424c4751, 0x097327232628cf27}}}},
    {.name        = "transfer_public_to_private",
     .tx_type     = TX_TOKEN_TRANSFER_PUBLIC_TO_PRIVATE,
     .input_count = 2,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x1e670aa0d6347688, 0x8d3a5be48ba5f147, 0x8e7978060750d391, 0x0e70e7a7290eec8c}},
        {.big.u64
         = {0x9261645621d8f156, 0xdcd0c6b0e1ad7c2e, 0x9960881d283ce0a9, 0x0210db289dc0204a}}}},
    {.name        = "transfer_private_to_public",
     .tx_type     = TX_TOKEN_TRANSFER_PRIVATE_TO_PUBLIC,
     .input_count = 4,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x1888586e77bdb05e, 0x688553ba3390fd91, 0x7c371e44396fb01a, 0x115cc9d19e7890f7}},
        {.big.u64
         = {0x37c242eee1297068, 0xc24ff51694fda7de, 0xfdfce6135844cd74, 0x05ba645b7227ed1a}}}},
};

const token_parameter_t token_parameters[NB_OF_TOKENS] = {
    {.program_id   = "usad_stablecoin.aleo",
     .display_info = {.type = TOKEN_TYPE_ARC22, .ticker = "USAD", .decimals = 6},
     .token_id
     = {.big.u64
        = {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000}},
     .nb_of_functions = NB_OF_USAD_STABLECOIN_ALEO_FUNCTIONS,
     .functions       = usad_stablecoin_aleo },
    {.program_id   = "usdcx_stablecoin.aleo",
     .display_info = {.type = TOKEN_TYPE_ARC22, .ticker = "USDCx", .decimals = 6},
     .token_id
     = {.big.u64
        = {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000}},
     .nb_of_functions = NB_OF_USDCX_STABLECOIN_ALEO_FUNCTIONS,
     .functions       = usdcx_stablecoin_aleo},
};