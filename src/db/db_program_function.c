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

#include "db_program_function.h"

#define NB_OF_CREDITS_ALEO_FUNCTIONS (8)
const function_parameters_t credits_aleo[NB_OF_CREDITS_ALEO_FUNCTIONS] = {
    {.string      = "transfer_public",
     .tx_type     = TX_ALEO_TRANSFER_PUBLIC,
     .input_count = 2,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x405245447621401a, 0x51c07b62a9c53d26, 0xb928e2f9112d0953, 0x0f511e31d6855446}},
        {.big.u64
         = {0x1a45c36fff8ff4a4, 0x69712402399b89f1, 0xb3e921a03d522403, 0x0b8deb3e586ce4f4}}}},
    {.string      = "transfer_private",
     .tx_type     = TX_ALEO_TRANSFER_PRIVATE,
     .input_count = 3,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x57853955ec65e959, 0xc04dacceb1a2026a, 0x0b8c74f5b07ee388, 0x0913b0e1e8289dba}},
        {.big.u64
         = {0x14050f83d8bc8ad5, 0x68c1128533fd9691, 0xb5bfd0497dcbe60d, 0x061bcb7383584d42}}}},
    {.string      = "transfer_private_to_public",
     .tx_type     = TX_ALEO_TRANSFER_PRIVATE_TO_PUBLIC,
     .input_count = 3,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x2ed6c22fde6f93b0, 0x7673e8d503d0995f, 0x6ec97fff81556086, 0x099abee7b8c03597}},
        {.big.u64
         = {0x7c27587b57c5d2d4, 0x78e193e449543290, 0xf4e07ed6a53dea1e, 0x018801c51078171d}}}},
    {.string      = "transfer_public_to_private",
     .tx_type     = TX_ALEO_TRANSFER_PUBLIC_TO_PRIVATE,
     .input_count = 2,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x6e2c39b3c306d0f5, 0x1dc66c4befdfbc77, 0x0e2106ac3953c377, 0x0d3c811de289fd10}},
        {.big.u64
         = {0x116b3d6259fc8332, 0x3b6f6a1a5b3344dd, 0x66c3bccc5ed9580b, 0x0b218600c10bd9a3}}}},
    {.string      = "fee_public",
     .tx_type     = TX_FEE_PUBLIC,
     .input_count = 3,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xf9bcb7f7a577a934, 0x98f4346a10f70cf8, 0x8f26408e9addc51b, 0x0aa82c82d4ff15a4}},
        {.big.u64
         = {0x8d31084295c605ef, 0xbfdc39996575504e, 0xae32c9cce746275e, 0x0bbfb3251558eee9}}}},
    {.string      = "fee_private",
     .tx_type     = TX_FEE_PRIVATE,
     .input_count = 4,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xccf5dc9907364acc, 0x1cf2578ad1d69649, 0x95c4227bca877a48, 0x0c218239cc930255}},
        {.big.u64
         = {0xc3c204d98b5e12b2, 0x318d93a9b12ba7c9, 0x9d42d8fb1c715281, 0x01505646987444fa}}}},
    {.string      = "split",
     .tx_type     = TX_UNKNOWN,
     .input_count = 2,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x842cb43ec1d6bbe9, 0xa2c33251d4bded16, 0x0fe74f79fc7c63d4, 0x022fb51a7d2acc90}},
        {.big.u64
         = {0x0bb53e032f965178, 0x6cd697d8b5df7efc, 0x12e2736c492f2495, 0x0529c62123003bca}}}},
    {.string      = "join",
     .tx_type     = TX_UNKNOWN,
     .input_count = 2,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xb56e347d9733de05, 0x29c0febaeb2bee52, 0x6c4d0ed1285f19fd, 0x0ec48ec3bdf25eb8}},
        {.big.u64
         = {0x921936604e6eddfe, 0x429d23effcfce58b, 0x072b665308c3d12b, 0x1241fcb49a8a9b72}}}},
};

#define NB_OF_LDGBATCHER_ALEO_FUNCTIONS (7)
const function_parameters_t ldgbatcher_aleo[NB_OF_LDGBATCHER_ALEO_FUNCTIONS] = {
    {.string      = "transfer_private_2",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE,
     .input_count = 4,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xfbac5db2cf40ee19, 0x35f84ea37404640a, 0x17c4e3abc57fd1af, 0x10fd9156abb4b25d}},
        {.big.u64
         = {0x0adab560276134ff, 0x26f8fcc13392b029, 0xebe7b1410049b4fa, 0x0f76243a00adcb04}}}},
    {.string      = "transfer_private_3",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE,
     .input_count = 5,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x51d1819b35bd5afe, 0x7accaa4a95486cda, 0xfc5e6bb51a18e791, 0x06d9fa4e36adf23f}},
        {.big.u64
         = {0x2b280960f9104bb0, 0x89c946f7320e64ec, 0xb7b44e27df42f1bf, 0x09973bace4c635bb}}}},
    {.string      = "transfer_private_4",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE,
     .input_count = 6,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xcbe4ecaf43a86fbd, 0x436a1c6d5d7ee950, 0x8ebc87b6b4f39c34, 0x08a9be65eae4015a}},
        {.big.u64
         = {0x6262160bc158aa69, 0x9f358ea890f25727, 0x47afbee2159b7169, 0x049678e2c92437f1}}}},
    {.string      = "transfer_private_5",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE,
     .input_count = 7,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xf02deda9358effd9, 0x7af6cc22336a456f, 0x49707780bac09b39, 0x032593eb33a6126b}},
        {.big.u64
         = {0x4381976d822ee991, 0x722647a911af9a02, 0x227764d2e098df27, 0x04d1650192aaae55}}}},
    {.string      = "transfer_private_6",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE,
     .input_count = 8,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x515ac7ada0c40928, 0x10049221ede7929f, 0x1b4ad180928a06f1, 0x05d48d51f6859276}},
        {.big.u64
         = {0x3c3e9cf604ee9167, 0x74752c0f54d3d4ab, 0xeec47b38bbc7aafb, 0x0082a015a8e3000f}}}},
    {.string      = "transfer_private_7",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE,
     .input_count = 9,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xd38d1c5eb82bb4e8, 0x861ca80304fcf75c, 0xa221533214f43459, 0x0834c1485357a6d0}},
        {.big.u64
         = {0xb7a238c350488600, 0x200d32f3bddc5d2f, 0xce9f3b41e042d62c, 0x108c157aa7eff088}}}},
    {.string      = "transfer_private_8",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE,
     .input_count = 10,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x1b43a65addfd26f2, 0xd555e2822f91a008, 0x20659e4d709f0eff, 0x0ebb5b25c8e7b7cd}},
        {.big.u64
         = {0x555767c5a872f04e, 0xdda7b950e1bed997, 0xc76b9bc219ba999c, 0x0521b68860091346}}}},
};

const program_parameter_t program_parameters[NB_OF_PROGRAMS] = {
    {.program_id      = "credits.aleo",
     .nb_of_functions = NB_OF_CREDITS_ALEO_FUNCTIONS,
     .functions       = credits_aleo   },
    {.program_id      = "ldgbatcher.aleo",
     .nb_of_functions = NB_OF_LDGBATCHER_ALEO_FUNCTIONS,
     .functions       = ldgbatcher_aleo},
};
