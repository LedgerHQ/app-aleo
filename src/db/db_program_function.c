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
    {.name        = "transfer_public",
     .tx_type     = TX_ALEO_TRANSFER_PUBLIC,
     .input_count = 2,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x405245447621401a, 0x51c07b62a9c53d26, 0xb928e2f9112d0953, 0x0f511e31d6855446}},
        {.big.u64
         = {0x1a45c36fff8ff4a4, 0x69712402399b89f1, 0xb3e921a03d522403, 0x0b8deb3e586ce4f4}}}},
    {.name        = "transfer_private",
     .tx_type     = TX_ALEO_TRANSFER_PRIVATE,
     .input_count = 3,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x57853955ec65e959, 0xc04dacceb1a2026a, 0x0b8c74f5b07ee388, 0x0913b0e1e8289dba}},
        {.big.u64
         = {0x14050f83d8bc8ad5, 0x68c1128533fd9691, 0xb5bfd0497dcbe60d, 0x061bcb7383584d42}}}},
    {.name        = "transfer_private_to_public",
     .tx_type     = TX_ALEO_TRANSFER_PRIVATE_TO_PUBLIC,
     .input_count = 3,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x2ed6c22fde6f93b0, 0x7673e8d503d0995f, 0x6ec97fff81556086, 0x099abee7b8c03597}},
        {.big.u64
         = {0x7c27587b57c5d2d4, 0x78e193e449543290, 0xf4e07ed6a53dea1e, 0x018801c51078171d}}}},
    {.name        = "transfer_public_to_private",
     .tx_type     = TX_ALEO_TRANSFER_PUBLIC_TO_PRIVATE,
     .input_count = 2,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x6e2c39b3c306d0f5, 0x1dc66c4befdfbc77, 0x0e2106ac3953c377, 0x0d3c811de289fd10}},
        {.big.u64
         = {0x116b3d6259fc8332, 0x3b6f6a1a5b3344dd, 0x66c3bccc5ed9580b, 0x0b218600c10bd9a3}}}},
    {.name        = "fee_public",
     .tx_type     = TX_FEE_PUBLIC,
     .input_count = 3,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xf9bcb7f7a577a934, 0x98f4346a10f70cf8, 0x8f26408e9addc51b, 0x0aa82c82d4ff15a4}},
        {.big.u64
         = {0x8d31084295c605ef, 0xbfdc39996575504e, 0xae32c9cce746275e, 0x0bbfb3251558eee9}}}},
    {.name        = "fee_private",
     .tx_type     = TX_FEE_PRIVATE,
     .input_count = 4,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xccf5dc9907364acc, 0x1cf2578ad1d69649, 0x95c4227bca877a48, 0x0c218239cc930255}},
        {.big.u64
         = {0xc3c204d98b5e12b2, 0x318d93a9b12ba7c9, 0x9d42d8fb1c715281, 0x01505646987444fa}}}},
    {.name        = "split",
     .tx_type     = TX_SPLIT,
     .input_count = 2,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x842cb43ec1d6bbe9, 0xa2c33251d4bded16, 0x0fe74f79fc7c63d4, 0x022fb51a7d2acc90}},
        {.big.u64
         = {0x0bb53e032f965178, 0x6cd697d8b5df7efc, 0x12e2736c492f2495, 0x0529c62123003bca}}}},
    {.name        = "join",
     .tx_type     = TX_JOIN,
     .input_count = 2,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xb56e347d9733de05, 0x29c0febaeb2bee52, 0x6c4d0ed1285f19fd, 0x0ec48ec3bdf25eb8}},
        {.big.u64
         = {0x921936604e6eddfe, 0x429d23effcfce58b, 0x072b665308c3d12b, 0x1241fcb49a8a9b72}}}},
};

#define NB_OF_LDGBATCHER_P28_ALEO_FUNCTIONS (7)
const function_parameters_t ldgbatcher_p28_aleo[NB_OF_LDGBATCHER_P28_ALEO_FUNCTIONS] = {
    {.name        = "transfer_private_2",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE,
     .input_count = 4,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x711bd2f0fd3b8860, 0x2140898228c34911, 0x0aae5605d3b999fe, 0x0487b38e575e54a4}},
        {.big.u64
         = {0x94fe7832f23f69f8, 0x79ed7c0a94a507b8, 0x19375744cd4d66cf, 0x0a0d8242d888337b}}}},
    {.name        = "transfer_private_3",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE,
     .input_count = 5,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xc52d556215ca8d42, 0xdd6855255a8999fc, 0x96a0067debdae976, 0x05457b2b07ff2258}},
        {.big.u64
         = {0xaa089e65aad01997, 0x3c449656a3f1cfb6, 0x034a6d7ada794cc6, 0x0e0823456dc1865e}}}},
    {.name        = "transfer_private_4",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE,
     .input_count = 6,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x5a549bc57d7ed08e, 0x57081afbde550716, 0xfd460bb66b8552f7, 0x0d98770ff6216767}},
        {.big.u64
         = {0x6bcc11ae56749122, 0x260e2b57d5d5fbef, 0xce5371258b939c59, 0x0884f29ecd7a4f1f}}}},
    {.name        = "transfer_private_5",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE,
     .input_count = 7,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x224995cd589d1640, 0x6373aaa70a095470, 0xdf9bc58530309041, 0x01f320b522e45a10}},
        {.big.u64
         = {0xfec2e5eb27e81ed1, 0x30342feb79e7b799, 0x895988e43a223100, 0x0d77f3954dc2bd3a}}}},
    {.name        = "transfer_private_6",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE,
     .input_count = 8,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xbc81a96cea70096d, 0x8b056fb89161bad6, 0x63cacb85d316cadb, 0x0e3562712c147531}},
        {.big.u64
         = {0xdd5ec7c602f5a9db, 0xc846d46174e69814, 0xcb0457c70b198957, 0x1178e2cfd2c4f6de}}}},
    {.name        = "transfer_private_7",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE,
     .input_count = 9,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x433f8bef1e650481, 0x10d600f6d41e3197, 0xe862611390273369, 0x0158dab93d6cbea6}},
        {.big.u64
         = {0x080a8386e49889ef, 0xb896be0d0173b849, 0x74fe7461b27002f1, 0x0002580b37e0e04f}}}},
    {.name        = "transfer_private_8",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE,
     .input_count = 10,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x738c7d8d9546e4c7, 0xb3a42c93014c13b5, 0xa7bfbaa1fdbfaedf, 0x0cdd3604cfb57700}},
        {.big.u64
         = {0x963d210f3ca0a9c6, 0xdef5b86772cb88ae, 0xc937f1616d1cabbb, 0x0050e835094c6a46}}}},
};

#define NB_OF_LDGBATCHER_P910_ALEO_FUNCTIONS (2)
const function_parameters_t ldgbatcher_p910_aleo[NB_OF_LDGBATCHER_P910_ALEO_FUNCTIONS] = {
    {.name        = "transfer_private_9",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE,
     .input_count = 11,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x98aa9cc1c8ee2642, 0xbe62182fbb92ffca, 0x35a23ab833271ffa, 0x03310b27a0d00370}},
        {.big.u64
         = {0x32ee6dde1ebe65de, 0xb8c26f628d1e6f21, 0xe1c2b67275d1b93f, 0x0e9e1d8fc177dd94}}}},
    {.name        = "transfer_private_10",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE,
     .input_count = 12,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x186d4555fde9475c, 0x2e8cef1de078b120, 0x5fdccdbe70ef5a5d, 0x02ffa736d1f5dee5}},
        {.big.u64
         = {0x348fed2e2ebff55a, 0xaaa512a36c5161d1, 0xc15f74c614285ada, 0x011281933285eae8}}}},
};

#define NB_OF_LDGBATCHER_P1114_ALEO_FUNCTIONS (4)
const function_parameters_t ldgbatcher_p1114_aleo[NB_OF_LDGBATCHER_P1114_ALEO_FUNCTIONS] = {
    {.name        = "transfer_private_11",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE,
     .input_count = 13,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x3ff43124394dbd33, 0x531338625ef6c666, 0x65e5e9895329685f, 0x07f68d6b21f8e833}},
        {.big.u64
         = {0x328851886470667b, 0x64e21d37f7925b67, 0x35ac4567af808ffe, 0x0026ad7b8ee7aff7}}}},
    {.name        = "transfer_private_12",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE,
     .input_count = 14,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x6d1d3896bd2839bb, 0xbd8958b0462624bd, 0xe860528af9002653, 0x112a53bacbcbe7ce}},
        {.big.u64
         = {0xedcad1eb8b147d4f, 0x02fde54d519738c2, 0x9341ee3381358a1c, 0x044020e3a68d76a0}}}},
    {.name        = "transfer_private_13",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE,
     .input_count = 15,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x09c5f6f27cbbfec9, 0xbab4c48888eaf220, 0x9881a77c0de86ec0, 0x02755a441a2744cc}},
        {.big.u64
         = {0xb1e64caf9634b655, 0xb3ac2e6fbabf1940, 0x172b5caf1d663d14, 0x11ae9762c3f6d96a}}}},
    {.name        = "transfer_private_14",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE,
     .input_count = 16,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xe3c90261c06a4b1c, 0x4fc657a9764c2ac9, 0x9447c78f2fd4ad7e, 0x0d89d16a0d9cede8}},
        {.big.u64
         = {0x8dbb40d608335e21, 0x7dafcd5c7a42044e, 0x7ed046b72a0ba325, 0x0e6ae1d15f10345d}}}},
};

#define NB_OF_LDGBATCHER_PPUB_28_ALEO_FUNCTIONS (7)
const function_parameters_t ldgbatcher_ppub_28_aleo[NB_OF_LDGBATCHER_PPUB_28_ALEO_FUNCTIONS] = {
    {.name        = "transfer_private_to_public_2",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 3,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x2e202cb29f237ff0, 0x95ac470726eec1b3, 0xf926efce4965f631, 0x06994f0de2afbbea}},
        {.big.u64
         = {0x8431ac482d71262b, 0x9dacedbb722faa48, 0x3e1242530da85257, 0x0245170992fe443b}}}},
    {.name        = "transfer_private_to_public_3",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 4,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xaefa4d43e173e47b, 0xf892da4a203a50f6, 0xc17a7f8fbcd997ff, 0x08e12aff2b496e8a}},
        {.big.u64
         = {0x7e9e6d6b849d78bf, 0xbfd7d207a7377c9c, 0x5ba17e7a2e1be0a6, 0x08d41f5f3403286b}}}},
    {.name        = "transfer_private_to_public_4",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 5,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x93a2213762b0c65a, 0x37c2eca9b7cdb3bc, 0xd56c8e5821e80bb8, 0x0e0bbd6c7cda39dd}},
        {.big.u64
         = {0x9faad67addfda98e, 0x6474f6f56eeaf3f4, 0x4afc955e45b7ed88, 0x0798b94074ccbfe9}}}},
    {.name        = "transfer_private_to_public_5",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 6,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x46c9f29a42fd6235, 0x0dafa85eae9bc356, 0x61270bbf636f934d, 0x0abaa6f52a6752f4}},
        {.big.u64
         = {0xde629954b703d153, 0xfa19518c7613adef, 0x8b30ab7453281da2, 0x07a0b6a68395b6d3}}}},
    {.name        = "transfer_private_to_public_6",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 7,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xc60fc62388df8ebf, 0xd3c8011e59977462, 0xdb14b83ca2c2aa2f, 0x0fd917a652fccd6e}},
        {.big.u64
         = {0xb273d3ef3dab4efe, 0x6b29647fd309a74e, 0x7dd0861bfb88d6f9, 0x019ce7551644284e}}}},
    {.name        = "transfer_private_to_public_7",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 8,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x81fa4f7e550ba2ed, 0x289971c5a59e4103, 0xf4a759009ea50b6b, 0x0e7841f6e4f0c7db}},
        {.big.u64
         = {0x18c759f9c7d74e44, 0x461615c31b180ffd, 0x249204dc159e6e3a, 0x06c67c9e4647b153}}}},
    {.name        = "transfer_private_to_public_8",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 9,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x795cd03fd2f21bdf, 0x549cc5aea39d33a7, 0xfc64e46494fa936f, 0x066701f2776b7405}},
        {.big.u64
         = {0xf1565b606c1c9499, 0xf2a03bcadef84933, 0x9fc2c83fd4bcdb24, 0x1138c6e52a18ec91}}}},
};

#define NB_OF_LDGBATCHER_PPUB_910_ALEO_FUNCTIONS (2)
const function_parameters_t ldgbatcher_ppub_910_aleo[NB_OF_LDGBATCHER_PPUB_910_ALEO_FUNCTIONS] = {
    {.name        = "transfer_private_to_public_9",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 10,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x7e850aeff72bb532, 0x15c669e3b28d43d5, 0xa4288c550b1594c6, 0x0e54f0a78144077e}},
        {.big.u64
         = {0xef17e6595ac6125b, 0x7466d5a59c926aac, 0xb45d4c306ed5e79c, 0x03e930b3cc66d8b0}}}},
    {.name        = "transfer_private_to_public_10",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 11,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x19ecd93e5ef4470f, 0x79ef784335bdb4f1, 0xd40b7d452fb2e12c, 0x0c8b7f636f512f7e}},
        {.big.u64
         = {0x09c61c1cc4a9d941, 0x2bb6a709e1206773, 0xd5d9f01ae2a054c5, 0x0298a0f6469bf062}}}},
};

#define NB_OF_LDGBATCHER_PPUB_1114_ALEO_FUNCTIONS (4)
const function_parameters_t ldgbatcher_ppub_1114_aleo[NB_OF_LDGBATCHER_PPUB_1114_ALEO_FUNCTIONS] = {
    {.name        = "transfer_private_to_public_11",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 12,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x09afac9c39dbe1a4, 0x332e976f09cfb1f9, 0x9e00dbc824e9c873, 0x0517c8fa9f8e7c73}},
        {.big.u64
         = {0x84c054b961ad3545, 0x90012849112b8372, 0x02bcc43090085e6e, 0x10c431e4797574f9}}}},
    {.name        = "transfer_private_to_public_12",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 13,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xc34709ce79a3316b, 0xadcc93b1033a32cf, 0x3a7e7b678fcf44bc, 0x06ed307f460eed06}},
        {.big.u64
         = {0xa0c9fd2a537c2527, 0x0e7e6bfde27a2b39, 0xe2ab55373b8bb9ba, 0x116c58d8c843ace2}}}},
    {.name        = "transfer_private_to_public_13",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 14,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x75b716b410306dfb, 0x949674d9d47fad9b, 0xb5d06a456c8e0d66, 0x0a7b39e70b31ebf1}},
        {.big.u64
         = {0x83b3d743b20a8705, 0x2aa74341421735a1, 0x11e5432f70a9aeef, 0x05eb29ec75591e75}}}},
    {.name        = "transfer_private_to_public_14",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 15,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x98b984950f7992b2, 0x73c090056cf218e4, 0x828e5cd005ee2c11, 0x008ad35fcadfa529}},
        {.big.u64
         = {0x31c3c3359850db6f, 0x58b6708d903b569a, 0xc494d0b8138e6b30, 0x0ff69533a0201287}}}},
};

const program_parameter_t program_parameters[NB_OF_PROGRAMS] = {
    {.program_id      = "credits.aleo",
     .nb_of_functions = NB_OF_CREDITS_ALEO_FUNCTIONS,
     .functions       = credits_aleo             },
    {.program_id      = "ldgbatcher_p28.aleo",
     .nb_of_functions = NB_OF_LDGBATCHER_P28_ALEO_FUNCTIONS,
     .functions       = ldgbatcher_p28_aleo      },
    {.program_id      = "ldgbatcher_p910.aleo",
     .nb_of_functions = NB_OF_LDGBATCHER_P910_ALEO_FUNCTIONS,
     .functions       = ldgbatcher_p910_aleo     },
    {.program_id      = "ldgbatcher_p1114.aleo",
     .nb_of_functions = NB_OF_LDGBATCHER_P1114_ALEO_FUNCTIONS,
     .functions       = ldgbatcher_p1114_aleo    },
    {.program_id      = "ldgbatcher_ppub_28.aleo",
     .nb_of_functions = NB_OF_LDGBATCHER_PPUB_28_ALEO_FUNCTIONS,
     .functions       = ldgbatcher_ppub_28_aleo  },
    {.program_id      = "ldgbatcher_ppub_910.aleo",
     .nb_of_functions = NB_OF_LDGBATCHER_PPUB_910_ALEO_FUNCTIONS,
     .functions       = ldgbatcher_ppub_910_aleo },
    {.program_id      = "ldgbatcher_ppub_1114.aleo",
     .nb_of_functions = NB_OF_LDGBATCHER_PPUB_1114_ALEO_FUNCTIONS,
     .functions       = ldgbatcher_ppub_1114_aleo},
};
