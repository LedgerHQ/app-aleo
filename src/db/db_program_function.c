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

#define NB_OF_LDG_P_28_ALEO_FUNCTIONS (7)
const function_parameters_t ldg_p_28_aleo[NB_OF_LDG_P_28_ALEO_FUNCTIONS] = {
    {.name        = "transfer_private_2",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE,
     .input_count = 4,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xaa53141c28f75d2c, 0x65b13fba609a6035, 0x7040a11cb3a3f62b, 0x05201e528f3480c4}},
        {.big.u64
         = {0x122b908e513a4ee9, 0xb1eaf2a82d86ea62, 0x213f99d902e331a6, 0x0578c3617e2651c8}}}},
    {.name        = "transfer_private_3",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE,
     .input_count = 5,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xc5efbffe7658eef4, 0xdc84d624b875bdbd, 0xa37465e2a36bec63, 0x04b51a50403ecfea}},
        {.big.u64
         = {0xf29d99f2c2c53871, 0x719b0165a61d8b76, 0x01849030f9becec1, 0x04bb3a79745f7a02}}}},
    {.name        = "transfer_private_4",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE,
     .input_count = 6,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x9cc05109092305ba, 0x7b615a532555f011, 0x8b094b3f1c0edc45, 0x0e9a68dc1b103c7a}},
        {.big.u64
         = {0xbb293f8f1b48a386, 0xb86465e5ec6ad220, 0x3bc07a6b8afbb685, 0x124550060e45f07b}}}},
    {.name        = "transfer_private_5",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE,
     .input_count = 7,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xe8b3ef130b97897b, 0x8e4fd93480f74e02, 0x764544d67a317dc9, 0x0017b6bd21303713}},
        {.big.u64
         = {0xc647789c16496645, 0xa7a50dfc5400cfce, 0xd006732d30108dad, 0x0a28719827978fb4}}}},
    {.name        = "transfer_private_6",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE,
     .input_count = 8,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x1d28cca31b2f3b52, 0x0090094678ede7c5, 0x59302b1fa066d6cb, 0x055921d2d3c08d88}},
        {.big.u64
         = {0x72c488a09596a7cb, 0xb129fed5ebb2d725, 0x4e463da58d947f1f, 0x00a20449067cf7b6}}}},
    {.name        = "transfer_private_7",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE,
     .input_count = 9,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x69347a906aefae3e, 0xf0aef8bff72b73aa, 0x4a20df41143a795e, 0x032729808ae8392e}},
        {.big.u64
         = {0xbe6517af6fdc663b, 0xe38a0322ed28df31, 0x29fc25306d983d43, 0x03ea993c3a2bbe2b}}}},
    {.name        = "transfer_private_8",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE,
     .input_count = 10,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x72d1f721ee2455e2, 0x6a2eca0622bf617a, 0x8adfa61d037e3b19, 0x119d53a85545f29d}},
        {.big.u64
         = {0x3ad6cd9ea53fbfce, 0xdf3d3dbe8a6c7f3b, 0xb077de61a52f2ee8, 0x0a2983140a27e484}}}},
};

#define NB_OF_LDG_P_910_ALEO_FUNCTIONS (2)
const function_parameters_t ldg_p_910_aleo[NB_OF_LDG_P_910_ALEO_FUNCTIONS] = {
    {.name        = "transfer_private_9",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE,
     .input_count = 11,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x6d501b891b40d91b, 0x80b03c0c6bc37b6e, 0xd63cc12e0fbf0826, 0x0a2cd6810fa8a2dd}},
        {.big.u64
         = {0x875481377667049b, 0xf2993332afe329c4, 0xa87697c9279c895a, 0x1181f5735de8a303}}}},
    {.name        = "transfer_private_10",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE,
     .input_count = 12,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x6b21034d6e07b76e, 0x1fa946a7fb11b54d, 0x6668c085e27ba62d, 0x04c999766c155c8c}},
        {.big.u64
         = {0x80f3a8adc3d167cb, 0x89eb2b7aa33c59ef, 0x7b29dbdef43bb4bb, 0x0b6fe9d8f9515706}}}},
};

#define NB_OF_LDG_P_1114_ALEO_FUNCTIONS (4)
const function_parameters_t ldg_p_1114_aleo[NB_OF_LDG_P_1114_ALEO_FUNCTIONS] = {
    {.name        = "transfer_private_11",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE,
     .input_count = 13,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x1c83112f8fbe9744, 0x1404d211a60c5f27, 0x09c6a3e952985fd2, 0x1038a06be15e2326}},
        {.big.u64
         = {0xdf8fb60e5a6a46d7, 0x579e4967135b0a85, 0x71120c0c2c011361, 0x027099e0cf2b091f}}}},
    {.name        = "transfer_private_12",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE,
     .input_count = 14,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x36fdefdcff969019, 0xc56ecec797347235, 0x51b2c0f27a342161, 0x01d6cd45f97fd86b}},
        {.big.u64
         = {0x62037507249cc333, 0xc5394356e0def99b, 0x7d17fa7873a5e437, 0x0e5b6361ba6791cd}}}},
    {.name        = "transfer_private_13",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE,
     .input_count = 15,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xb84db2f460bdfd74, 0x93a0ac1f127d263e, 0x78e94ba5b897aba5, 0x0d0304a39071bc51}},
        {.big.u64
         = {0x5f7e3f4f171c8c12, 0x719d0cf625b0db37, 0xe76bef010d869341, 0x071b63d188b80eb5}}}},
    {.name        = "transfer_private_14",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE,
     .input_count = 16,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x2709908463ddc772, 0x12c6d41deb48a7b1, 0x93fac827fc509c5e, 0x047f8c7bcbfcdca9}},
        {.big.u64
         = {0xe73262598eb64dbb, 0x3a5a5afdde2fbb6f, 0x8e9230d0722378e7, 0x06ac00bc0ffbb2df}}}},
};

#define NB_OF_LDG_P2P_28_ALEO_FUNCTIONS (7)
const function_parameters_t ldg_p2p_28_aleo[NB_OF_LDG_P2P_28_ALEO_FUNCTIONS] = {
    {.name        = "transfer_private_to_public_2",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 3,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x024afc7a35abeb5f, 0xa4b9cd021fd349c0, 0x00e8632107ac50a9, 0x072d243cb8013779}},
        {.big.u64
         = {0x668753065d63f5da, 0x448750dde6834911, 0x596669429cc94d08, 0x03f910b128421b87}}}},
    {.name        = "transfer_private_to_public_3",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 4,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xe743c48cc551f715, 0x1f2f3d1a6196248a, 0xc5a2b01c9fb7614a, 0x074ae85ad3b05169}},
        {.big.u64
         = {0x82925136d6963b81, 0xc3b15b2dbe7a4e1f, 0xc91a65db8e82c8df, 0x023ea48e3de07b07}}}},
    {.name        = "transfer_private_to_public_4",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 5,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xa66eaab99ce296af, 0x90b3a6d6c6a764f7, 0x16e54165755271f7, 0x03dcfdb5ec2dc480}},
        {.big.u64
         = {0x62abb8da6bc250d0, 0xacf3b3a520579393, 0x946b425878d27344, 0x0db7a3d9e1d588d1}}}},
    {.name        = "transfer_private_to_public_5",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 6,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x96d046eb9f2653e3, 0xfb368287e24bd7f7, 0x3e7ff1c0a91b6613, 0x02d29fc9d842d26d}},
        {.big.u64
         = {0xe6f971f96c01b808, 0x92b2ad7e67737d3e, 0x2c19562e7e828402, 0x113923d0d91c6fb4}}}},
    {.name        = "transfer_private_to_public_6",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 7,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x2bfa344283c05f46, 0xc9c5513a163c8568, 0x567c261de13825ff, 0x0b8122fcdde0336a}},
        {.big.u64
         = {0xc4b2655f5a4e98bc, 0x922ff4829ee2b7b0, 0x8190fd5716cc1e9a, 0x07614c66ce4a4ab1}}}},
    {.name        = "transfer_private_to_public_7",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 8,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x1b8fbdd16584eeb4, 0xfde3e1b1080fba3d, 0x60d0b90f021cad05, 0x07abcc3d5d7b7483}},
        {.big.u64
         = {0x220c503806b19cb3, 0x66437416ec094bc0, 0x7cb9679f6b70ba2e, 0x11b133d830ae34f7}}}},
    {.name        = "transfer_private_to_public_8",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 9,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xbca7c89e835c5c28, 0x508084a5f36646f1, 0x1e6d9b5eceb27506, 0x0cb539ef202e2101}},
        {.big.u64
         = {0x92e9ddadb8d49f56, 0xc7d341a3639c31a7, 0x69bb85ef44061f37, 0x0ebd58d4fe93f8fd}}}},
};

#define NB_OF_LDG_P2P_910_ALEO_FUNCTIONS (2)
const function_parameters_t ldg_p2p_910_aleo[NB_OF_LDG_P2P_910_ALEO_FUNCTIONS] = {
    {.name        = "transfer_private_to_public_9",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 10,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xe74498bb957f07bc, 0xa619047a33b0b50e, 0xd00093dabb2c4adf, 0x1086d4168b8937d2}},
        {.big.u64
         = {0x4daa04ba4c62c680, 0x2e57f703be7c2b80, 0x6f36bde8067a4675, 0x09f518c4e84aa0de}}}},
    {.name        = "transfer_private_to_public_10",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 11,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xd53efb94b180ff2e, 0xed94525e8db0890d, 0x8595222bcf7eaac9, 0x026fd794621c156d}},
        {.big.u64
         = {0xefac2507bdd2655b, 0xc1e1dcde197a64c4, 0x495ca91149c6955c, 0x095d072369a645d3}}}},
};

#define NB_OF_LDG_P2P_1114_ALEO_FUNCTIONS (4)
const function_parameters_t ldg_p2p_1114_aleo[NB_OF_LDG_P2P_1114_ALEO_FUNCTIONS] = {
    {.name        = "transfer_private_to_public_11",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 12,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x298e19f02ae80020, 0x55351e3d2c151c1f, 0x47bc537c27a537b2, 0x0bf507aa5c98536d}},
        {.big.u64
         = {0xbf0cdae9d128a7cc, 0x05fea4578678f80a, 0x71e76a808e416f9b, 0x06e494f700c23d1a}}}},
    {.name        = "transfer_private_to_public_12",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 13,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x6ef2e6349b63fd5d, 0x6bcf946ea244d7f7, 0x9798f6aad41dad6e, 0x10199fd795a53a55}},
        {.big.u64
         = {0x718f996a6f8dc3e8, 0x5d1d6a84f83df08c, 0x7ac4ba08bf005393, 0x11c9dbc7beaa4595}}}},
    {.name        = "transfer_private_to_public_13",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 14,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x149b3b821a48b6d0, 0x1c0c19937b4e8ed0, 0x702c82cb56d61487, 0x03dbd6799e48de64}},
        {.big.u64
         = {0xe3482f7760f28b93, 0xc15b78244865e3d6, 0xa40e9da8a1855f5c, 0x12101e7ba7fe395b}}}},
    {.name        = "transfer_private_to_public_14",
     .tx_type     = TX_ALEO_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 15,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xda8189e5c2639603, 0x93a1980cc034befc, 0x08284787b9dfc918, 0x0d9751043fe14a3e}},
        {.big.u64
         = {0x6df5a23c6941d2c8, 0x2bba7da140a71fc8, 0x8b60f455805ab5d9, 0x0305bd8e7d3e4633}}}},
};

const program_parameter_t program_parameters[NB_OF_PROGRAMS] = {
    {.program_id      = "credits.aleo",
     .nb_of_functions = NB_OF_CREDITS_ALEO_FUNCTIONS,
     .functions       = credits_aleo     },
    {.program_id      = "ldg_p_28.aleo",
     .nb_of_functions = NB_OF_LDG_P_28_ALEO_FUNCTIONS,
     .functions       = ldg_p_28_aleo    },
    {.program_id      = "ldg_p_910.aleo",
     .nb_of_functions = NB_OF_LDG_P_910_ALEO_FUNCTIONS,
     .functions       = ldg_p_910_aleo   },
    {.program_id      = "ldg_p_1114.aleo",
     .nb_of_functions = NB_OF_LDG_P_1114_ALEO_FUNCTIONS,
     .functions       = ldg_p_1114_aleo  },
    {.program_id      = "ldg_p2p_28.aleo",
     .nb_of_functions = NB_OF_LDG_P2P_28_ALEO_FUNCTIONS,
     .functions       = ldg_p2p_28_aleo  },
    {.program_id      = "ldg_p2p_910.aleo",
     .nb_of_functions = NB_OF_LDG_P2P_910_ALEO_FUNCTIONS,
     .functions       = ldg_p2p_910_aleo },
    {.program_id      = "ldg_p2p_1114.aleo",
     .nb_of_functions = NB_OF_LDG_P2P_1114_ALEO_FUNCTIONS,
     .functions       = ldg_p2p_1114_aleo},
};
