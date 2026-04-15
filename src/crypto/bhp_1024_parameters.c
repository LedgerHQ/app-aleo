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

#include "bhp_1024_parameters.h"

#define NB_OF_CREDITS_ALEO_FUNCTIONS (8)
const function_hashes_t credits_aleo[NB_OF_CREDITS_ALEO_FUNCTIONS] = {
    {.string = "transfer_public",
     .hashes
     = {{.big.u64
         = {0x405245447621401a, 0x51c07b62a9c53d26, 0xb928e2f9112d0953, 0x0f511e31d6855446}},
        {.big.u64
         = {0x1a45c36fff8ff4a4, 0x69712402399b89f1, 0xb3e921a03d522403, 0x0b8deb3e586ce4f4}}}},
    {.string = "transfer_private",
     .hashes
     = {{.big.u64
         = {0x57853955ec65e959, 0xc04dacceb1a2026a, 0x0b8c74f5b07ee388, 0x0913b0e1e8289dba}},
        {.big.u64
         = {0x14050f83d8bc8ad5, 0x68c1128533fd9691, 0xb5bfd0497dcbe60d, 0x061bcb7383584d42}}}},
    {.string = "transfer_private_to_public",
     .hashes
     = {{.big.u64
         = {0x2ed6c22fde6f93b0, 0x7673e8d503d0995f, 0x6ec97fff81556086, 0x099abee7b8c03597}},
        {.big.u64
         = {0x7c27587b57c5d2d4, 0x78e193e449543290, 0xf4e07ed6a53dea1e, 0x018801c51078171d}}}},
    {.string = "transfer_public_to_private",
     .hashes
     = {{.big.u64
         = {0x6e2c39b3c306d0f5, 0x1dc66c4befdfbc77, 0x0e2106ac3953c377, 0x0d3c811de289fd10}},
        {.big.u64
         = {0x116b3d6259fc8332, 0x3b6f6a1a5b3344dd, 0x66c3bccc5ed9580b, 0x0b218600c10bd9a3}}}},
    {.string = "fee_public",
     .hashes
     = {{.big.u64
         = {0xf9bcb7f7a577a934, 0x98f4346a10f70cf8, 0x8f26408e9addc51b, 0x0aa82c82d4ff15a4}},
        {.big.u64
         = {0x8d31084295c605ef, 0xbfdc39996575504e, 0xae32c9cce746275e, 0x0bbfb3251558eee9}}}},
    {.string = "fee_private",
     .hashes
     = {{.big.u64
         = {0xccf5dc9907364acc, 0x1cf2578ad1d69649, 0x95c4227bca877a48, 0x0c218239cc930255}},
        {.big.u64
         = {0xc3c204d98b5e12b2, 0x318d93a9b12ba7c9, 0x9d42d8fb1c715281, 0x01505646987444fa}}}},
    {.string = "split",
     .hashes
     = {{.big.u64
         = {0x842cb43ec1d6bbe9, 0xa2c33251d4bded16, 0x0fe74f79fc7c63d4, 0x022fb51a7d2acc90}},
        {.big.u64
         = {0x0bb53e032f965178, 0x6cd697d8b5df7efc, 0x12e2736c492f2495, 0x0529c62123003bca}}}},
    {.string = "join",
     .hashes
     = {{.big.u64
         = {0xb56e347d9733de05, 0x29c0febaeb2bee52, 0x6c4d0ed1285f19fd, 0x0ec48ec3bdf25eb8}},
        {.big.u64
         = {0x921936604e6eddfe, 0x429d23effcfce58b, 0x072b665308c3d12b, 0x1241fcb49a8a9b72}}}}
};

const bhp_1024_parameter_t bhp_1024_parameters[NB_OF_PROGRAMS] = {
    {.program_id         = "credits",
     .program_id_network = "aleo",
     .nb_of_functions    = NB_OF_CREDITS_ALEO_FUNCTIONS,
     .functions          = credits_aleo}
};