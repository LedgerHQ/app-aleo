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

#define NB_OF_LDG_ARC20_P_213_ALEO_FUNCTIONS (12)
const function_parameters_t ldg_arc20_p_213_aleo[NB_OF_LDG_ARC20_P_213_ALEO_FUNCTIONS] = {
    {.name        = "transfer_private_2",
     .tx_type     = TX_TOKEN_ARC20_TRANSFER_BATCH_PRIVATE,
     .input_count = 5,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x247b4473e96290f4, 0xf8754bc369a24e4a, 0x53d9cc0cf8bdb808, 0x06f5b3832356bd68}},
        {.big.u64
         = {0x63dce63980f6c551, 0xadb6665333d2e887, 0x505bdada84af5b64, 0x070a8518fed98713}}}},
    {.name        = "transfer_private_3",
     .tx_type     = TX_TOKEN_ARC20_TRANSFER_BATCH_PRIVATE,
     .input_count = 6,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xce94c0e9a26c9304, 0xf540991f4e784af3, 0xf001deea1b56b3f4, 0x0023459f813d2987}},
        {.big.u64
         = {0xab6f1dad4cde07cc, 0xf697c167333d5909, 0xf62069310a501261, 0x06dae020967f4b85}}}},
    {.name        = "transfer_private_4",
     .tx_type     = TX_TOKEN_ARC20_TRANSFER_BATCH_PRIVATE,
     .input_count = 7,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x4f07f0b8621283c9, 0x4ed8ff802dffcb57, 0x72cee4df8990e1f0, 0x050a7ca1e4d2e52e}},
        {.big.u64
         = {0x393cc9e45c678aa5, 0xe6ca9eb344ddf63a, 0x823c99d2eae822e7, 0x03795339cdfc2cc6}}}},
    {.name        = "transfer_private_5",
     .tx_type     = TX_TOKEN_ARC20_TRANSFER_BATCH_PRIVATE,
     .input_count = 8,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x18d45b0b51d78f69, 0x7c1b853ad0716caf, 0x86add96f5aa5aff1, 0x00072c33530c6209}},
        {.big.u64
         = {0xce196e7841f4b698, 0xb19bfcebc0d9d9ff, 0xa79daf0ef29c5acf, 0x03ffb02834a4908f}}}},
    {.name        = "transfer_private_6",
     .tx_type     = TX_TOKEN_ARC20_TRANSFER_BATCH_PRIVATE,
     .input_count = 9,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xdd082ffbad8dc56a, 0xa2c23d59457986ef, 0x59ba397b085f8f35, 0x0490b53f070b11b3}},
        {.big.u64
         = {0xb17af48457c01ebf, 0xb77c21fee426341d, 0xa48257fd4fccca9c, 0x065525cdd34b2e1a}}}},
    {.name        = "transfer_private_7",
     .tx_type     = TX_TOKEN_ARC20_TRANSFER_BATCH_PRIVATE,
     .input_count = 10,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x4853bfbb2c487c88, 0xc9817b0adddb3bfc, 0x25a2d9ba27ab0b7b, 0x0bb7a4188f287a84}},
        {.big.u64
         = {0x57be29e5fb004f15, 0x340aea04a6bff79f, 0x4da6faa2f0498679, 0x0fca8efecca8428f}}}},
    {.name        = "transfer_private_8",
     .tx_type     = TX_TOKEN_ARC20_TRANSFER_BATCH_PRIVATE,
     .input_count = 11,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x31c0050e8e69f242, 0xb4dfd08c5b0719a9, 0x17d83388c985ca48, 0x0a7818b16c66f731}},
        {.big.u64
         = {0x73a92bd19b517e84, 0x037ed8bfa0cf406b, 0x147e5fded5442eee, 0x12a2d31cd2bc48e8}}}},
    {.name        = "transfer_private_9",
     .tx_type     = TX_TOKEN_ARC20_TRANSFER_BATCH_PRIVATE,
     .input_count = 12,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xfd4871098045ccee, 0xa4356c66e80b1971, 0x7a811438d363fc66, 0x0c7d4857577b2b46}},
        {.big.u64
         = {0x7dc616fe64c155ea, 0x31dabe54f77fc3be, 0x9ca57ac8436f8430, 0x075bc52489f6f004}}}},
    {.name        = "transfer_private_10",
     .tx_type     = TX_TOKEN_ARC20_TRANSFER_BATCH_PRIVATE,
     .input_count = 13,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x43d5c939fd89832a, 0xd3151bfaf1988345, 0x0f3b87867a1b5221, 0x0128397c4bb2c432}},
        {.big.u64
         = {0xdf54f29963d3cfd6, 0xd56f564f863d3ed0, 0x89a1e24fdee6f606, 0x0a78155c1790b26f}}}},
    {.name        = "transfer_private_11",
     .tx_type     = TX_TOKEN_ARC20_TRANSFER_BATCH_PRIVATE,
     .input_count = 14,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x2862d96ffd7800ca, 0xb23049f321ec28f5, 0x0233b764cad33dc3, 0x081112033f964005}},
        {.big.u64
         = {0xc0d44f94a29476e6, 0x5709ec867efec54e, 0x2533f94331c88f87, 0x0c0762d8aa4fb567}}}},
    {.name        = "transfer_private_12",
     .tx_type     = TX_TOKEN_ARC20_TRANSFER_BATCH_PRIVATE,
     .input_count = 15,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xc3f1c98893489259, 0x35270e6b1b6eeeb0, 0x68fcc028dede8ded, 0x11b5be57caa74c37}},
        {.big.u64
         = {0x73eead9b3c9d6b15, 0x8601725286feb8f7, 0xcfdbc35d2db654a5, 0x02f8b35225a2bfbe}}}},
    {.name        = "transfer_private_13",
     .tx_type     = TX_TOKEN_ARC20_TRANSFER_BATCH_PRIVATE,
     .input_count = 16,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xec64de2c8f2c152d, 0xbe79f4b9581aef50, 0x49358cc9ff79e133, 0x03a7eaec501aced9}},
        {.big.u64
         = {0x6fb9e4a1fcece715, 0x4b22f21bba215e7f, 0x1cb518feebcab375, 0x0d22c7798c71c7ac}}}},
};

#define NB_OF_LDG_ARC20_P2P_213_ALEO_FUNCTIONS (12)
const function_parameters_t ldg_arc20_p2p_213_aleo[NB_OF_LDG_ARC20_P2P_213_ALEO_FUNCTIONS] = {
    {.name        = "transfer_private_to_public_2",
     .tx_type     = TX_TOKEN_ARC20_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 5,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xa49c082b89d3c182, 0xbdfb963c6f3cebb5, 0xe36631cef579f98a, 0x1099a729fb474157}},
        {.big.u64
         = {0xa0ebbdd30ce87057, 0x7cf80b187642ab51, 0xe8bee3b31fa0d332, 0x10e23dada94279b5}}}},
    {.name        = "transfer_private_to_public_3",
     .tx_type     = TX_TOKEN_ARC20_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 6,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x958c00f1d7f23377, 0x7f7db7bdd2179d70, 0xd1ccc59a2e759efd, 0x0a95a4aff164b829}},
        {.big.u64
         = {0x80fbf673ea0d4eab, 0x41e46e53e948968b, 0x77f4f1a526f2876c, 0x0bdd18f532bda35a}}}},
    {.name        = "transfer_private_to_public_4",
     .tx_type     = TX_TOKEN_ARC20_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 7,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x4b43e77f8ff2bdfc, 0x3a686606139e55f1, 0x8efbbf73d084889e, 0x0332d35bbe45f2ea}},
        {.big.u64
         = {0xb4ec9c8f4231c4f4, 0x1e3a227d2b4572a9, 0xc6be05d7d0f1f703, 0x03c5d4e27979eaa6}}}},
    {.name        = "transfer_private_to_public_5",
     .tx_type     = TX_TOKEN_ARC20_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 8,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x3791df1aeaf47e77, 0x5358e474e38e088d, 0x25b53b630986ffe0, 0x04b214cf4a27c07d}},
        {.big.u64
         = {0xd96a3f5ccce2044a, 0xa8aefb78369475ea, 0x89dfe72f810f731e, 0x0fa38b1d75fa2be9}}}},
    {.name        = "transfer_private_to_public_6",
     .tx_type     = TX_TOKEN_ARC20_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 9,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x916fc5a67f4163f7, 0xcff14c8745e84535, 0xb2652851251ca9ea, 0x12070e0b325bfbd4}},
        {.big.u64
         = {0x44e7faca31776a87, 0x8a86c689ec1f1cfc, 0x20368ad8e6792aa8, 0x0a3c5c7f2d800db6}}}},
    {.name        = "transfer_private_to_public_7",
     .tx_type     = TX_TOKEN_ARC20_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 10,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x9547d4563668870f, 0xf96ef08dd7a1c971, 0xf409ad22e0ff0380, 0x014f2a6402cba355}},
        {.big.u64
         = {0x41ed115ded7de160, 0x9b4ad807ab6c5184, 0xc6e77073d00c2cb7, 0x0cc42ca242eb9180}}}},
    {.name        = "transfer_private_to_public_8",
     .tx_type     = TX_TOKEN_ARC20_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 11,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x2ea7cff038d63cc6, 0xf475735fbf254f35, 0x9c3ccefef0e358f4, 0x0ea49c33deabaa0b}},
        {.big.u64
         = {0x1c0de3a0dcfa95bf, 0xb01f8d71a8452c87, 0x937a00bba2b9b678, 0x020ccfc6d79c3e66}}}},
    {.name        = "transfer_private_to_public_9",
     .tx_type     = TX_TOKEN_ARC20_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 12,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xff56eb0731b94b3e, 0xe765753ae81bb66a, 0x94a78f75f88ac111, 0x0ad3f4b30a36ae03}},
        {.big.u64
         = {0x43896b1d9e6d700e, 0xca5ecaa28484ece1, 0xc2f07604b69bf326, 0x08bd13a9e5e39846}}}},
    {.name        = "transfer_private_to_public_10",
     .tx_type     = TX_TOKEN_ARC20_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 13,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xe5946fe4f83135d3, 0xb11b8410f4740a69, 0xe12d1ba0d3969c94, 0x040f176abc37ed56}},
        {.big.u64
         = {0x6934c2b5d5e22078, 0x6300164506d7a279, 0xd9e53d9f10c635de, 0x002fe9c42e35c1b6}}}},
    {.name        = "transfer_private_to_public_11",
     .tx_type     = TX_TOKEN_ARC20_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 14,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xd51927847d99b942, 0x889fe9f1fa58465c, 0x51e7bd834e737789, 0x0684ac1468e0bb96}},
        {.big.u64
         = {0xfcae953e781e4f0f, 0x210836c7d2ada53a, 0x5d830aa654b31bcb, 0x025fcc8501a84e7d}}}},
    {.name        = "transfer_private_to_public_12",
     .tx_type     = TX_TOKEN_ARC20_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 15,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x0bd63fd1d7a11ae1, 0xb47ee71ca387838f, 0xe32ac39c29bf9d57, 0x0853718d1e8582cb}},
        {.big.u64
         = {0x188999520beb95dc, 0x9118f5f05d1e3ce5, 0x98c6555437bdba0f, 0x0d2c08905ac2c94a}}}},
    {.name        = "transfer_private_to_public_13",
     .tx_type     = TX_TOKEN_ARC20_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 16,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x461e7134a8916fe9, 0x95e997641950a73a, 0x208c6f8caacec6aa, 0x0087223c69feb640}},
        {.big.u64
         = {0x1a4939ca02db1641, 0x1f22d0b3efb04814, 0x9ae5b70748b8e0ff, 0x08b7a784691b59a2}}}},
};

#define NB_OF_MM2_LDG_ARC20_P_213_ALEO_FUNCTIONS (12)
const function_parameters_t mm2_ldg_arc20_p_213_aleo[NB_OF_MM2_LDG_ARC20_P_213_ALEO_FUNCTIONS] = {
    {.name        = "transfer_private_2",
     .tx_type     = TX_TOKEN_ARC20_TRANSFER_BATCH_PRIVATE,
     .input_count = 5,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x71d05dfd545df9b9, 0xec936758bebf4a5b, 0xe8200b8b277d49d1, 0x1022dca37d40b814}},
        {.big.u64
         = {0x1940ad51676ebe8f, 0x15c3433143841b16, 0xc6df6714347e80ad, 0x04d8474c078a369f}}}},
    {.name        = "transfer_private_3",
     .tx_type     = TX_TOKEN_ARC20_TRANSFER_BATCH_PRIVATE,
     .input_count = 6,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xf0c606d93d5d66a4, 0x5641da4ec2aa6251, 0xfcbd1111896695c0, 0x0949884d613105f9}},
        {.big.u64
         = {0x22e413abae6f2d5c, 0x4e0a143176b80728, 0x222d98a513c61ad7, 0x07967ead4b53b4b9}}}},
    {.name        = "transfer_private_4",
     .tx_type     = TX_TOKEN_ARC20_TRANSFER_BATCH_PRIVATE,
     .input_count = 7,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x21748922f27dc733, 0xfcd40d926a7c7278, 0x4077ed96706c5125, 0x0f559d42e0c2bb5a}},
        {.big.u64
         = {0xeb73f548fcbab10b, 0xdf5a2e331ce43129, 0x87eb8526ef582f7c, 0x0fdedbb0ced9a58b}}}},
    {.name        = "transfer_private_5",
     .tx_type     = TX_TOKEN_ARC20_TRANSFER_BATCH_PRIVATE,
     .input_count = 8,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x5064a6d676edade7, 0x5d3a2ed8fb994807, 0xacfb976f719d7534, 0x12857d17f9f77228}},
        {.big.u64
         = {0xd9a45b319a6dfd22, 0x6ba9e50424c34d7c, 0xdf4ff4de94adc24c, 0x08214d5716726872}}}},
    {.name        = "transfer_private_6",
     .tx_type     = TX_TOKEN_ARC20_TRANSFER_BATCH_PRIVATE,
     .input_count = 9,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x1a4560631215de7c, 0xc511b07f85002c60, 0xedd5493a369cd51e, 0x027551427fa0b1d2}},
        {.big.u64
         = {0x4a80c8d9698df9a7, 0x5a48d9d5c723eb39, 0x819a6740fe2002a1, 0x1114e4c2f3741eea}}}},
    {.name        = "transfer_private_7",
     .tx_type     = TX_TOKEN_ARC20_TRANSFER_BATCH_PRIVATE,
     .input_count = 10,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xd398fb701f342df3, 0xc09270ef4f7b8f00, 0x44c4e02542492c5a, 0x0489ccaa6b9405e8}},
        {.big.u64
         = {0x81d881db695cc7e9, 0x931dbb5c3d64c079, 0x889e1a2d07a80261, 0x0dc01f7aee1f77d2}}}},
    {.name        = "transfer_private_8",
     .tx_type     = TX_TOKEN_ARC20_TRANSFER_BATCH_PRIVATE,
     .input_count = 11,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xb50c095e96a73c87, 0x3c95ad3b963831e1, 0xa9b1548364f98630, 0x03f23c346b03b9c3}},
        {.big.u64
         = {0x21dc74211e00e655, 0xa9ff43a87836fc50, 0xe630790c02609e0d, 0x09a313e59f6654fa}}}},
    {.name        = "transfer_private_9",
     .tx_type     = TX_TOKEN_ARC20_TRANSFER_BATCH_PRIVATE,
     .input_count = 12,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x7ad84c6c7b75ef59, 0xed121c845ee552a9, 0x35f1c63622d668b6, 0x0226722ae96a9be4}},
        {.big.u64
         = {0x58f84c14f56fd54b, 0xf92250c9111caeba, 0x7319ef7f3e273242, 0x0729acffd82d000e}}}},
    {.name        = "transfer_private_10",
     .tx_type     = TX_TOKEN_ARC20_TRANSFER_BATCH_PRIVATE,
     .input_count = 13,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xb385d3df072eb4ee, 0x5448fa93872dcbab, 0xc7acb2b94af464bb, 0x07956f8d227151b6}},
        {.big.u64
         = {0xbd60c8db28daca20, 0x2b8e16667b28548a, 0x6974f5ef51179707, 0x061b3ff5cce853bc}}}},
    {.name        = "transfer_private_11",
     .tx_type     = TX_TOKEN_ARC20_TRANSFER_BATCH_PRIVATE,
     .input_count = 14,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x6e4e42fa789a4aef, 0xa0e3b95a9e9c8d92, 0xafb9c5570f408497, 0x0aa1d0851befdc79}},
        {.big.u64
         = {0xc300e9a08ef52790, 0x01708f3b7a843fce, 0x910c57dddb1ae70a, 0x12055159e9407336}}}},
    {.name        = "transfer_private_12",
     .tx_type     = TX_TOKEN_ARC20_TRANSFER_BATCH_PRIVATE,
     .input_count = 15,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xed4620dc165498b9, 0x6c2cf3d658ae3c21, 0xf43948ebf977f160, 0x0f5f8fae21fc4835}},
        {.big.u64
         = {0xf067299657d35e12, 0x1c12bd17e5128b6d, 0x8aaffc686caa9e17, 0x0c656cdb4e34152e}}}},
    {.name        = "transfer_private_13",
     .tx_type     = TX_TOKEN_ARC20_TRANSFER_BATCH_PRIVATE,
     .input_count = 16,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xa602f4e16b8d2dec, 0xe7bf4ae7b683d053, 0x503ef05d30e33e30, 0x09200f569c1d4ebc}},
        {.big.u64
         = {0x630e957d5137a5ea, 0xf28fd476412e0996, 0x539b303a7e6088ae, 0x10f93c5ee9cfa0dc}}}},
};

#define NB_OF_MM1_LDG_ARC20_P2P_213_ALEO_FUNCTIONS (12)
const function_parameters_t mm1_ldg_arc20_p2p_213_aleo[NB_OF_MM1_LDG_ARC20_P2P_213_ALEO_FUNCTIONS]
    = {
        {.name        = "transfer_private_to_public_2",
         .tx_type     = TX_TOKEN_ARC20_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
         .input_count = 5,
         .bhp_1024_hashes
         = {{.big.u64
             = {0x3a7edc37ddd71a0d, 0x5757ce9695d56342, 0x4e2906594c3a2b15, 0x0d82755a7672fd60}},
            {.big.u64
             = {0xed521d918da4b912, 0xeee25e6710e970ee, 0x2717b6cf79fd32c6, 0x01c099402e82840e}}}},
        {.name        = "transfer_private_to_public_3",
         .tx_type     = TX_TOKEN_ARC20_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
         .input_count = 6,
         .bhp_1024_hashes
         = {{.big.u64
             = {0xa31a05b2ef5af393, 0x4fb2c0070c779050, 0x256a3a80c2cc2fff, 0x0b682db02d3c8da8}},
            {.big.u64
             = {0xb603e9ec78b8c607, 0xf13397cb4fb6288c, 0x87a5cf553161d9d6, 0x08cb279b035ea502}}}},
        {.name        = "transfer_private_to_public_4",
         .tx_type     = TX_TOKEN_ARC20_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
         .input_count = 7,
         .bhp_1024_hashes
         = {{.big.u64
             = {0x6fe918ca9c1ebf4c, 0x3048019895d389d1, 0x7a9712ceea082236, 0x0053f06c1e9e4634}},
            {.big.u64
             = {0xb9924e81df069ec1, 0xf6424c5665931838, 0xadadb2b000d87100, 0x06ff773f7397a7b2}}}},
        {.name        = "transfer_private_to_public_5",
         .tx_type     = TX_TOKEN_ARC20_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
         .input_count = 8,
         .bhp_1024_hashes
         = {{.big.u64
             = {0x3f7d8f96600f4413, 0x6e928a84dbb8f3e2, 0x7e22ea8b7d122a88, 0x0f2b0d7b570b29dc}},
            {.big.u64
             = {0x78aebb0904305965, 0xfe9a0725ac40eee8, 0x43f138916a6d6b34, 0x0bc485f8b97a14c0}}}},
        {.name        = "transfer_private_to_public_6",
         .tx_type     = TX_TOKEN_ARC20_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
         .input_count = 9,
         .bhp_1024_hashes
         = {{.big.u64
             = {0x812670b3f18d3839, 0x508dbfa2ae8f210d, 0x72454477db0f349f, 0x069303db87f8ecf9}},
            {.big.u64
             = {0xe4a33e547e93c3c8, 0x280ecf9709f52afc, 0x9ce893bec1d8498e, 0x041ae6dbe5e84555}}}},
        {.name        = "transfer_private_to_public_7",
         .tx_type     = TX_TOKEN_ARC20_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
         .input_count = 10,
         .bhp_1024_hashes
         = {{.big.u64
             = {0x38356baf5405c66e, 0xb182e87347a6a7ee, 0xc21fa69c584f3e54, 0x109e38c34df7ed40}},
            {.big.u64
             = {0xcb96d14b605574d8, 0xe50afea56fee9c95, 0x5d1953e3f1f1f8bb, 0x0650d7f13db41145}}}},
        {.name        = "transfer_private_to_public_8",
         .tx_type     = TX_TOKEN_ARC20_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
         .input_count = 11,
         .bhp_1024_hashes
         = {{.big.u64
             = {0x82086f9ba5b3678e, 0xd56a926c92b27371, 0x4b8145a3fc7f9cc7, 0x086f045aeaa10e0f}},
            {.big.u64
             = {0x6926abe0bfb98ad1, 0xe6e52c8bc2e266b0, 0x48ffb59481932d8a, 0x00a15cb849ff1cce}}}},
        {.name        = "transfer_private_to_public_9",
         .tx_type     = TX_TOKEN_ARC20_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
         .input_count = 12,
         .bhp_1024_hashes
         = {{.big.u64
             = {0xd29085ba179f3183, 0x24ed8eae1759150c, 0xafd8499b4110df9e, 0x029ae3d5001e0121}},
            {.big.u64
             = {0x90419aac5deb21a6, 0x126a66db2937bbe5, 0x513311f2f484b243, 0x10d6eacebf6d419e}}}},
        {.name        = "transfer_private_to_public_10",
         .tx_type     = TX_TOKEN_ARC20_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
         .input_count = 13,
         .bhp_1024_hashes
         = {{.big.u64
             = {0xe5bbcfb8c14541e0, 0x743da43f863dec62, 0x07a10a765412dba6, 0x0e8d3cfeb88cd4ba}},
            {.big.u64
             = {0x161e7b4310c85c21, 0x4eb8ce0e12784745, 0x8896c3ed1552d753, 0x0cf549936a300e5e}}}},
        {.name        = "transfer_private_to_public_11",
         .tx_type     = TX_TOKEN_ARC20_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
         .input_count = 14,
         .bhp_1024_hashes
         = {{.big.u64
             = {0x3191c2f4d9d93fa3, 0xf252e411b8d5bbb6, 0xa580c2597b3af334, 0x031fb49e9b1bfe68}},
            {.big.u64
             = {0x9ff3b86c37a413ea, 0x807119366c96b7c7, 0xfdb01bf8fbb1f9d9, 0x11f8d2f0854925ce}}}},
        {.name        = "transfer_private_to_public_12",
         .tx_type     = TX_TOKEN_ARC20_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
         .input_count = 15,
         .bhp_1024_hashes
         = {{.big.u64
             = {0xb95d3fa38c097d53, 0xfdbdeb9f0a34fc47, 0x67c10d8addca3315, 0x02aeff9fa837536b}},
            {.big.u64
             = {0x08e050992dc54a68, 0xc6b73689a8270eab, 0x97aa7a22865ff1ee, 0x050c6f504e35c7f2}}}},
        {.name        = "transfer_private_to_public_13",
         .tx_type     = TX_TOKEN_ARC20_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
         .input_count = 16,
         .bhp_1024_hashes
         = {{.big.u64
             = {0xe9bb41e743a49806, 0xf923154656787186, 0xcdeac1eed89e30c1, 0x11e6901bff925463}},
            {.big.u64
             = {0x1ca2e8e3ba609607, 0x9ebab2e8511e2a06, 0x3c98d64e92223f28, 0x1196ed993a521601}}}},
};

const program_parameter_t program_parameters[NB_OF_PROGRAMS] = {
    {.program_id      = "credits.aleo",
     .nb_of_functions = NB_OF_CREDITS_ALEO_FUNCTIONS,
     .functions       = credits_aleo              },
    {.program_id      = "ldg_p_28.aleo",
     .nb_of_functions = NB_OF_LDG_P_28_ALEO_FUNCTIONS,
     .functions       = ldg_p_28_aleo             },
    {.program_id      = "ldg_p_910.aleo",
     .nb_of_functions = NB_OF_LDG_P_910_ALEO_FUNCTIONS,
     .functions       = ldg_p_910_aleo            },
    {.program_id      = "ldg_p_1114.aleo",
     .nb_of_functions = NB_OF_LDG_P_1114_ALEO_FUNCTIONS,
     .functions       = ldg_p_1114_aleo           },
    {.program_id      = "ldg_p2p_28.aleo",
     .nb_of_functions = NB_OF_LDG_P2P_28_ALEO_FUNCTIONS,
     .functions       = ldg_p2p_28_aleo           },
    {.program_id      = "ldg_p2p_910.aleo",
     .nb_of_functions = NB_OF_LDG_P2P_910_ALEO_FUNCTIONS,
     .functions       = ldg_p2p_910_aleo          },
    {.program_id      = "ldg_p2p_1114.aleo",
     .nb_of_functions = NB_OF_LDG_P2P_1114_ALEO_FUNCTIONS,
     .functions       = ldg_p2p_1114_aleo         },
    {.program_id      = "ldg_arc20_p_213.aleo",
     .nb_of_functions = NB_OF_LDG_ARC20_P_213_ALEO_FUNCTIONS,
     .functions       = ldg_arc20_p_213_aleo      },
    {.program_id      = "ldg_arc20_p2p_213.aleo",
     .nb_of_functions = NB_OF_LDG_ARC20_P2P_213_ALEO_FUNCTIONS,
     .functions       = ldg_arc20_p2p_213_aleo    },
    {.program_id      = "mm2_ldg_arc20_p_213.aleo",
     .nb_of_functions = NB_OF_MM2_LDG_ARC20_P_213_ALEO_FUNCTIONS,
     .functions       = mm2_ldg_arc20_p_213_aleo  },
    {.program_id      = "mm1_ldg_arc20_p2p_213.aleo",
     .nb_of_functions = NB_OF_MM1_LDG_ARC20_P2P_213_ALEO_FUNCTIONS,
     .functions       = mm1_ldg_arc20_p2p_213_aleo},
};
