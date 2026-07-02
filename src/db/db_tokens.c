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

#define NB_OF_USAD_STABLECOIN_ALEO_FUNCTIONS (5)
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
    {.name        = "join",
     .tx_type     = TX_TOKEN_JOIN,
     .input_count = 2,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xd7badee44e6580e4, 0xf0b9d07081a7d8d7, 0x679bb213b16140cf, 0x0dd79cef46faaf4f}},
        {.big.u64
         = {0xe1c7f022dc20c990, 0x6ddfe2afe116919e, 0x31939b9688a0b82f, 0x06b25bd9c6bbbffd}}}},
};

#define NB_OF_TEST_USAD_STABLECOIN_ALEO_FUNCTIONS (5)
const function_parameters_t test_usad_stablecoin_aleo[NB_OF_TEST_USAD_STABLECOIN_ALEO_FUNCTIONS] = {
    {.name        = "transfer_public",
     .tx_type     = TX_TOKEN_TRANSFER_PUBLIC,
     .input_count = 2,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000}},
        {.big.u64
         = {0x0d4fcc2ba1f0570d, 0xe214b11632efcaf0, 0x991a3475d996d772, 0x0ba8dafc7125ba9a}}}},
    {.name        = "transfer_private",
     .tx_type     = TX_TOKEN_TRANSFER_PRIVATE,
     .input_count = 4,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000}},
        {.big.u64
         = {0x1da31b3b81650676, 0xb7a789c97f32e5ab, 0xcfdf3f9dbd271390, 0x06442a951fbe2d7d}}}},
    {.name        = "transfer_public_to_private",
     .tx_type     = TX_TOKEN_TRANSFER_PUBLIC_TO_PRIVATE,
     .input_count = 2,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000}},
        {.big.u64
         = {0xb638c34f292c4b11, 0x289c531e34e212d3, 0xa1598223d621f6fe, 0x09cd2f815a68ec85}}}},
    {.name        = "transfer_private_to_public",
     .tx_type     = TX_TOKEN_TRANSFER_PRIVATE_TO_PUBLIC,
     .input_count = 4,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000}},
        {.big.u64
         = {0x2d174e7c145b4ea0, 0xecd238cc68294850, 0x844ec82806ec2467, 0x01ce18cae8c9f595}}}},
    {.name        = "join",
     .tx_type     = TX_TOKEN_JOIN,
     .input_count = 2,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000}},
        {.big.u64
         = {0x88b8e9ebc87f27dd, 0xf1689c9af7a6477a, 0x927ba86b40b5330a, 0x052461de8c432700}}}},
};

#define NB_OF_LDG_USAD_P_28_ALEO_FUNCTIONS (7)
const function_parameters_t ldg_usad_p_28_aleo[NB_OF_LDG_USAD_P_28_ALEO_FUNCTIONS] = {
    {.name        = "transfer_private_2",
     .tx_type     = TX_TOKEN_TRANSFER_BATCH_PRIVATE,
     .input_count = 5,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x39f1fe653f7ef795, 0x40c0d8e57dcf1644, 0x71f7be01be2e9717, 0x033ef762f6bfd343}},
        {.big.u64
         = {0x65ff74772030a863, 0x5a678ecdb0181cc5, 0xe4e6b6452d35d8e5, 0x0b7ed550ab48802b}}}},
    {.name        = "transfer_private_3",
     .tx_type     = TX_TOKEN_TRANSFER_BATCH_PRIVATE,
     .input_count = 6,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xe0aa4effcacbbe54, 0x4bf2a2cb479e8186, 0x5a949b396d37a0a4, 0x127e595d5d79db01}},
        {.big.u64
         = {0x24837ef0742381f1, 0x078f0a0d27d88783, 0x98370709f184390c, 0x0d626b78c08d74d1}}}},
    {.name        = "transfer_private_4",
     .tx_type     = TX_TOKEN_TRANSFER_BATCH_PRIVATE,
     .input_count = 7,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xe32b03db722bf5e5, 0x756c44248fdb558a, 0xdda3f0190fe6b7b5, 0x080a5991c8f94783}},
        {.big.u64
         = {0x148ef87f418b7ac9, 0x110b85a7de7a63b6, 0x57b38daeff1650f7, 0x0cb1c2b43fb17531}}}},
    {.name        = "transfer_private_5",
     .tx_type     = TX_TOKEN_TRANSFER_BATCH_PRIVATE,
     .input_count = 8,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xa2757a08c07ef7ed, 0xc6b023da158ae655, 0x43010b48d07bc68f, 0x104ffc25a08de270}},
        {.big.u64
         = {0xce5bc64f0e24e38e, 0x8a8b87987e229fd4, 0xf0aeab3b8ffc596d, 0x10c36980ab68906d}}}},
    {.name        = "transfer_private_6",
     .tx_type     = TX_TOKEN_TRANSFER_BATCH_PRIVATE,
     .input_count = 9,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x19641a4eb92cc09d, 0x72c0e362623e7b52, 0x823f322b5b968eff, 0x066524bf7d279e74}},
        {.big.u64
         = {0x29ee361fba6d5555, 0x0c0185cefd0d5907, 0x93fc47c6dae8e8fe, 0x01de8db717dbc26a}}}},
    {.name        = "transfer_private_7",
     .tx_type     = TX_TOKEN_TRANSFER_BATCH_PRIVATE,
     .input_count = 10,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xc6b9cf18453203a2, 0x41e532ca1831d950, 0x04ede17a78f002ad, 0x0b5c00decea78913}},
        {.big.u64
         = {0x39d0e08e34ed5781, 0x69103f6390a3352f, 0x6dbac9538626d650, 0x023741b4e52c7571}}}},
    {.name        = "transfer_private_8",
     .tx_type     = TX_TOKEN_TRANSFER_BATCH_PRIVATE,
     .input_count = 11,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xc2a7262e1d5e61b7, 0x42585ea95eb80925, 0xb642b7f73dbaf879, 0x0cf1834d57a71ab3}},
        {.big.u64
         = {0x45f67daaae8cc2aa, 0xe8901e073442bfe8, 0x9e9ea5550b1e8ba9, 0x0bcdf6f1720b3fc3}}}},
};

#define NB_OF_LDG_USAD_P_910_ALEO_FUNCTIONS (2)
const function_parameters_t ldg_usad_p_910_aleo[NB_OF_LDG_USAD_P_910_ALEO_FUNCTIONS] = {
    {.name        = "transfer_private_9",
     .tx_type     = TX_TOKEN_TRANSFER_BATCH_PRIVATE,
     .input_count = 12,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xc451192c396a0a33, 0x3386ce943a2cd78a, 0x69204faec1cf5beb, 0x1066bf548b23d33c}},
        {.big.u64
         = {0xa3d3841d2cfcd0ee, 0x1b641b61cc8d53d2, 0x2ddb77d090f1ff01, 0x061ea280c92807e7}}}},
    {.name        = "transfer_private_10",
     .tx_type     = TX_TOKEN_TRANSFER_BATCH_PRIVATE,
     .input_count = 13,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x37a72ac1c2cc7056, 0x142e460e660f5c35, 0x04665ce3b7db226b, 0x0bf595acb9c0ed78}},
        {.big.u64
         = {0x3605a2c3fcc37e93, 0x3ef0b92e9c1f580b, 0x4b18ba374b9f14ed, 0x110a6ba30469c2ae}}}},
};

#define NB_OF_LDG_USAD_P_1113_ALEO_FUNCTIONS (3)
const function_parameters_t ldg_usad_p_1113_aleo[NB_OF_LDG_USAD_P_1113_ALEO_FUNCTIONS] = {
    {.name        = "transfer_private_11",
     .tx_type     = TX_TOKEN_TRANSFER_BATCH_PRIVATE,
     .input_count = 14,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x28d957e6f4278974, 0xe1e7e89866c8d4e8, 0x4b55e8ee2286f21a, 0x096cc688708776fe}},
        {.big.u64
         = {0x0ffaf46a7682aa34, 0x62b538947d2b1b32, 0x3c6e1c92b69ffede, 0x072be4d8a7f67cfd}}}},
    {.name        = "transfer_private_12",
     .tx_type     = TX_TOKEN_TRANSFER_BATCH_PRIVATE,
     .input_count = 15,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xe3eb8e1646115603, 0x5623f3bc2fddd358, 0xb1b25ee4979295c7, 0x0749c1dd2a6abc38}},
        {.big.u64
         = {0x6f62b06d09627091, 0x0fa99c5d146e5a4d, 0xefff1c9db95fc668, 0x0fd0cb873b20a782}}}},
    {.name        = "transfer_private_13",
     .tx_type     = TX_TOKEN_TRANSFER_BATCH_PRIVATE,
     .input_count = 16,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x6f22e7c7fd65aedd, 0x112bdf5750620a94, 0x6237d994a15230d0, 0x027bbdb589e70088}},
        {.big.u64
         = {0xb48cea939f2a2cd4, 0x1426c1d9d8c17cd0, 0x8d85c519ad95564c, 0x01d8619ecd0f1e09}}}},
};

#define NB_OF_LDG_USAD_P2P_28_ALEO_FUNCTIONS (7)
const function_parameters_t ldg_usad_p2p_28_aleo[NB_OF_LDG_USAD_P2P_28_ALEO_FUNCTIONS] = {
    {.name        = "transfer_private_to_public_2",
     .tx_type     = TX_TOKEN_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 5,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x30d5194ea95caa50, 0xff2cdd668562a34f, 0x34af544ea920b5e2, 0x0fe12ba118a40854}},
        {.big.u64
         = {0x17c4dd306017edff, 0x1598562a2520a08c, 0x8b165c641b8dd8a7, 0x00440aea9eeda87e}}}},
    {.name        = "transfer_private_to_public_3",
     .tx_type     = TX_TOKEN_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 6,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x0f0f56692a28b472, 0x7dbd7e7d4a9f2945, 0x558e215c125e00b6, 0x0192244ac2e5a704}},
        {.big.u64
         = {0x8ee669e7f09a53f6, 0x1e6edea126b057d2, 0xbfda9a66d994d5e6, 0x088eb84269c541d7}}}},
    {.name        = "transfer_private_to_public_4",
     .tx_type     = TX_TOKEN_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 7,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x09e0f1326dbfc9a4, 0x3c9b0a3c8ca7b306, 0xbd43ce25c1f07348, 0x0084324da0b4c63d}},
        {.big.u64
         = {0xba02be424952fa7b, 0x041c6cc1c300668e, 0xfd4e6b93a693a7b6, 0x0541b8ff4465df0c}}}},
    {.name        = "transfer_private_to_public_5",
     .tx_type     = TX_TOKEN_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 8,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x17a540017204a3d9, 0x3e92d69235de28f8, 0x52f6a4ca20f0d9be, 0x09b3c9a16346b1ea}},
        {.big.u64
         = {0xde0ad62e3c9b440d, 0x379e4dadc82fcbd3, 0xb04f7dafc16b542e, 0x0363015bbded8efe}}}},
    {.name        = "transfer_private_to_public_6",
     .tx_type     = TX_TOKEN_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 9,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x0f4523fbc687cff5, 0xf53775c63d4271ff, 0x8daac0c48cb1c82f, 0x00af9bbed9cb89bb}},
        {.big.u64
         = {0x9ae08e3babb0ebf4, 0x92415f041ba377df, 0x2e8ab78c3b3c725a, 0x07e0677a0ac7b352}}}},
    {.name        = "transfer_private_to_public_7",
     .tx_type     = TX_TOKEN_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 10,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x5346464340859ac7, 0x0051c2d5672d688c, 0x88b996d8c9cd93cb, 0x0e6c9604da4ec059}},
        {.big.u64
         = {0xd48e7a0ad6473f4e, 0x573b8aef3cc18511, 0x66fa4bd662fbcf65, 0x07c7cee0c038d389}}}},
    {.name        = "transfer_private_to_public_8",
     .tx_type     = TX_TOKEN_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 11,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xa81a4f9cc9fcaf22, 0x2abaf9d6aa6efcd5, 0xc54de8f8d74afc5a, 0x0bbdfc83b28addc4}},
        {.big.u64
         = {0x5de4f8bd63939e21, 0x3c3321bbf5b76e91, 0x58feb29109d6975e, 0x0f1c3be966db3d99}}}},
};

#define NB_OF_LDG_USAD_P2P_910_ALEO_FUNCTIONS (2)
const function_parameters_t ldg_usad_p2p_910_aleo[NB_OF_LDG_USAD_P2P_910_ALEO_FUNCTIONS] = {
    {.name        = "transfer_private_to_public_9",
     .tx_type     = TX_TOKEN_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 12,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xbb7f311be52790ca, 0x4b7676ca0d67074d, 0x100df5ce6865cdae, 0x08e49906146d7129}},
        {.big.u64
         = {0xf514b034a3ca6ef3, 0xee43d8ed2ad27a43, 0xa8408a7d7dc8cce1, 0x0808a86530ee5e4c}}}},
    {.name        = "transfer_private_to_public_10",
     .tx_type     = TX_TOKEN_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 13,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x81d0a05f5ba51313, 0xf75bab2ab8092e42, 0x007fd1d5c29afa13, 0x09ae69244c15bf3b}},
        {.big.u64
         = {0xfac5ed39c2b2b1a9, 0x7e0b4aef11f16a44, 0xfb47468db2cedd00, 0x005453bad48f1755}}}},
};

#define NB_OF_LDG_USAD_P2P_1113_ALEO_FUNCTIONS (3)
const function_parameters_t ldg_usad_p2p_1113_aleo[NB_OF_LDG_USAD_P2P_1113_ALEO_FUNCTIONS] = {
    {.name        = "transfer_private_to_public_11",
     .tx_type     = TX_TOKEN_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 14,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xecc3fd6e47f5cc79, 0x75410b65f0084ce5, 0x333b0ece5da7c3a5, 0x088a6f4de2edffac}},
        {.big.u64
         = {0x5b8080ec8dafbfae, 0xdc11d4ca55271b01, 0xdb50d924f571c4b4, 0x11c235b7bede6658}}}},
    {.name        = "transfer_private_to_public_12",
     .tx_type     = TX_TOKEN_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 15,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x9a5b6a69240211d6, 0x58a7b0bfed2a345d, 0x95b0bb4c132ebdc2, 0x06d4cef242c22260}},
        {.big.u64
         = {0x1f3ad370c6f62d8b, 0x857675e7cec02f80, 0x4f78e4fa42a0227f, 0x0877bf008b87cba2}}}},
    {.name        = "transfer_private_to_public_13",
     .tx_type     = TX_TOKEN_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 16,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xd17fdce89383709f, 0x5621cfc7b5c5d208, 0x8b9124ec42202552, 0x0fc6d0b3aeaace77}},
        {.big.u64
         = {0x2433019b010172f5, 0x8cc1d7f5855580a6, 0x6c057d3a831da358, 0x110eaaa7ee98560e}}}},
};

#define NB_OF_USDCX_STABLECOIN_ALEO_FUNCTIONS (5)
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
    {.name        = "join",
     .tx_type     = TX_TOKEN_JOIN,
     .input_count = 2,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xb1eb3056def63cf2, 0xe78f41178abbd49e, 0xc03407366d403c37, 0x00f143956cddf939}},
        {.big.u64
         = {0x1657b029b48ffc85, 0x9e6abe5afd4a732e, 0xad74be604f005b76, 0x0c72c1e5385b30d9}}}},
};

#define NB_OF_TEST_USDCX_STABLECOIN_ALEO_FUNCTIONS (5)
const function_parameters_t test_usdcx_stablecoin_aleo[NB_OF_TEST_USDCX_STABLECOIN_ALEO_FUNCTIONS]
    = {
        {.name        = "transfer_public",
         .tx_type     = TX_TOKEN_TRANSFER_PUBLIC,
         .input_count = 2,
         .bhp_1024_hashes
         = {{.big.u64
             = {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000}},
            {.big.u64
             = {0x38119084c4c9ab1e, 0xf156fa7f2c43c68b, 0x011f6e8d4c941fc5, 0x06960c5ab972f66b}}}},
        {.name        = "transfer_private",
         .tx_type     = TX_TOKEN_TRANSFER_PRIVATE,
         .input_count = 4,
         .bhp_1024_hashes
         = {{.big.u64
             = {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000}},
            {.big.u64
             = {0xecad8f222ca5fe36, 0x0e1d4d69632f69e3, 0x42adc5f202f9f0e0, 0x0bef4a7e56088ff2}}}},
        {.name        = "transfer_public_to_private",
         .tx_type     = TX_TOKEN_TRANSFER_PUBLIC_TO_PRIVATE,
         .input_count = 2,
         .bhp_1024_hashes
         = {{.big.u64
             = {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000}},
            {.big.u64
             = {0x4022dbec73adedbf, 0xf2bf8d3bf8b5cb7a, 0xce749a92e6c16848, 0x09260e3a528557d6}}}},
        {.name        = "transfer_private_to_public",
         .tx_type     = TX_TOKEN_TRANSFER_PRIVATE_TO_PUBLIC,
         .input_count = 4,
         .bhp_1024_hashes
         = {{.big.u64
             = {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000}},
            {.big.u64
             = {0x2015651c56999978, 0x332d51f8251b7943, 0xbbff73638e9b5998, 0x04343bf1976cca46}}}},
        {.name        = "join",
         .tx_type     = TX_TOKEN_JOIN,
         .input_count = 2,
         .bhp_1024_hashes
         = {{.big.u64
             = {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000}},
            {.big.u64
             = {0x9565c687dc97050f, 0x8309e78b8104785a, 0xe76e79cddf8d5261, 0x024d664989a99a20}}}},
};

#define NB_OF_LDG_USDCX_P_28_ALEO_FUNCTIONS (7)
const function_parameters_t ldg_usdcx_p_28_aleo[NB_OF_LDG_USDCX_P_28_ALEO_FUNCTIONS] = {
    {.name        = "transfer_private_2",
     .tx_type     = TX_TOKEN_TRANSFER_BATCH_PRIVATE,
     .input_count = 5,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xdbf855243e2a5fdc, 0xf9db69fbebc05cf9, 0x8d55e8088c9583da, 0x10a0c41816e41a18}},
        {.big.u64
         = {0x28ab6f530cdcef18, 0xf654b0d255f2f944, 0x8086b67044b0f33f, 0x0d236277b703e080}}}},
    {.name        = "transfer_private_3",
     .tx_type     = TX_TOKEN_TRANSFER_BATCH_PRIVATE,
     .input_count = 6,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x70a53665dc35fd93, 0x21cf58e64dc93b9c, 0x62e1f9906c96172e, 0x08b57c67afea25c7}},
        {.big.u64
         = {0xb9d0984fd635cb57, 0xf03681cb7b472ef3, 0xb554e581a87b5a1d, 0x011f7b3009c40ba7}}}},
    {.name        = "transfer_private_4",
     .tx_type     = TX_TOKEN_TRANSFER_BATCH_PRIVATE,
     .input_count = 7,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x691c6eb91c20732b, 0x932a4cd4608330cf, 0x3a201c69a4692276, 0x03e4ce7200bca26d}},
        {.big.u64
         = {0xaa2d4272407acf97, 0xf6668bcc7655da51, 0x0af70d8bc6d1881a, 0x12701de5161b47ac}}}},
    {.name        = "transfer_private_5",
     .tx_type     = TX_TOKEN_TRANSFER_BATCH_PRIVATE,
     .input_count = 8,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xc467b88d2e66fcbb, 0xb5968570bb3fad02, 0xf8f187d01b3a6ccc, 0x1010ed0fe7db0bbe}},
        {.big.u64
         = {0xd877d6cc67b7e732, 0x79862af5245cb9b8, 0x0e5f0eb6187e8068, 0x0d059bed1570eb76}}}},
    {.name        = "transfer_private_6",
     .tx_type     = TX_TOKEN_TRANSFER_BATCH_PRIVATE,
     .input_count = 9,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x3c9f223891f24b56, 0x1269b2234bf5d0e0, 0xc2db44c08ac68c1b, 0x031c19ad7d0d520a}},
        {.big.u64
         = {0x945331f41bbef9e6, 0xe739c26d5d86d67b, 0x0af25b3657d105c4, 0x05c3ff3b0d3ac4aa}}}},
    {.name        = "transfer_private_7",
     .tx_type     = TX_TOKEN_TRANSFER_BATCH_PRIVATE,
     .input_count = 10,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x73f7d565b574743f, 0xe47544e6387541bf, 0xf67a65ba7960d3e7, 0x0dbc2d310ea0743b}},
        {.big.u64
         = {0xe4237fbcf71bcaca, 0x4e0e138b246a30bb, 0x6ac8908788e91ea7, 0x0e93be92d7163c7d}}}},
    {.name        = "transfer_private_8",
     .tx_type     = TX_TOKEN_TRANSFER_BATCH_PRIVATE,
     .input_count = 11,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x6a77cb6ebcf072e3, 0x8eaf0387731e430f, 0xbd62d75cd639d3d7, 0x06315253ace0226f}},
        {.big.u64
         = {0x11c0fd499fee34d9, 0x1f088ab731bfc122, 0x8bfe7b56bb47ebe7, 0x0916ee42a3e53434}}}},
};

#define NB_OF_LDG_USDCX_P_910_ALEO_FUNCTIONS (2)
const function_parameters_t ldg_usdcx_p_910_aleo[NB_OF_LDG_USDCX_P_910_ALEO_FUNCTIONS] = {
    {.name        = "transfer_private_9",
     .tx_type     = TX_TOKEN_TRANSFER_BATCH_PRIVATE,
     .input_count = 12,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x19f972a86ea6dee2, 0x394f4241ceacfb90, 0x855fce56f6949406, 0x0cdf1e3d04654795}},
        {.big.u64
         = {0xf19e6cbeb4c7738b, 0xcd22891025565093, 0xbc150fb16461d333, 0x0f7195dbe678a70e}}}},
    {.name        = "transfer_private_10",
     .tx_type     = TX_TOKEN_TRANSFER_BATCH_PRIVATE,
     .input_count = 13,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x5280958d2ab08989, 0x5bd72b59413cca47, 0xa7e83edb629da465, 0x0b5d670006e465fb}},
        {.big.u64
         = {0x5a20fa01823b939d, 0x53da512d1e2cf280, 0x6e5a39468a90f4e6, 0x01943e58595d7d38}}}},
};

#define NB_OF_LDG_USDCX_P_1113_ALEO_FUNCTIONS (3)
const function_parameters_t ldg_usdcx_p_1113_aleo[NB_OF_LDG_USDCX_P_1113_ALEO_FUNCTIONS] = {
    {.name        = "transfer_private_11",
     .tx_type     = TX_TOKEN_TRANSFER_BATCH_PRIVATE,
     .input_count = 14,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x37ee5bf3ce5c7397, 0x4775e82f8b53a4e3, 0x75c15c69b13b3b7b, 0x1077aa3874edf5c7}},
        {.big.u64
         = {0x0753e778041a5e12, 0xae2f3a2bd32e38c4, 0xbda6db3afbb0a51e, 0x0d87c6d512153638}}}},
    {.name        = "transfer_private_12",
     .tx_type     = TX_TOKEN_TRANSFER_BATCH_PRIVATE,
     .input_count = 15,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xa318355e5c2b14ac, 0x4d2b90ff6f266c00, 0x58e40c9cbd60ba7c, 0x02625b427c4337b8}},
        {.big.u64
         = {0x879522ec20dac968, 0x6373092d1f131730, 0xddd46b0752983f6b, 0x07bf6eda94cdf8a1}}}},
    {.name        = "transfer_private_13",
     .tx_type     = TX_TOKEN_TRANSFER_BATCH_PRIVATE,
     .input_count = 16,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xad09cf0667e496eb, 0x391b591cfd49d6f7, 0x5f7592bb57716a57, 0x0f8663ee0b310b1c}},
        {.big.u64
         = {0x7574653aea38c29d, 0xf70a9cc3c900c067, 0x6e99ce0d58da63e0, 0x0104ff51181fc471}}}},
};

#define NB_OF_LDG_USDCX_P2P_28_ALEO_FUNCTIONS (7)
const function_parameters_t ldg_usdcx_p2p_28_aleo[NB_OF_LDG_USDCX_P2P_28_ALEO_FUNCTIONS] = {
    {.name        = "transfer_private_to_public_2",
     .tx_type     = TX_TOKEN_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 5,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x68eb5067d2059a3c, 0x9e38ce392823b2b6, 0xc08cb69b3ad0f222, 0x06575ca70c140a80}},
        {.big.u64
         = {0x1b60f1ffa829e2fb, 0x7c5e147b1b38b08f, 0x765579c1df904984, 0x0adb0ec5231dfd06}}}},
    {.name        = "transfer_private_to_public_3",
     .tx_type     = TX_TOKEN_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 6,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xebe5ba180f3dba0f, 0x64fb90003507228f, 0xf29dac49196e6c05, 0x0a6ea948f84af572}},
        {.big.u64
         = {0x6540003d7811d740, 0x25a17b8fec0aff56, 0xab367c39e40f2b78, 0x0afab1d9e9c3367e}}}},
    {.name        = "transfer_private_to_public_4",
     .tx_type     = TX_TOKEN_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 7,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x37e810c1eee3b125, 0xd66ac5a49087bc1b, 0xd88320ad138be62e, 0x110a9c14c93fd9ea}},
        {.big.u64
         = {0x4e7376a4cfa2d2ae, 0x558ae43ec09c059b, 0xd950bdbdf6b133e0, 0x06e6f252a0f1075a}}}},
    {.name        = "transfer_private_to_public_5",
     .tx_type     = TX_TOKEN_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 8,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x0efd0c280b832fed, 0xbecd9d40310a53c2, 0xefd1a63f27af968b, 0x0d89a9c288e6758d}},
        {.big.u64
         = {0x148cc59f38bdb7af, 0x188f09f9f2621615, 0x53d4ac98184ad590, 0x0a251be16d86f5ad}}}},
    {.name        = "transfer_private_to_public_6",
     .tx_type     = TX_TOKEN_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 9,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x353a6f57d7682aeb, 0x001127f8e3e81eda, 0x3471c4abdee4d117, 0x04dd7589d56bb4c8}},
        {.big.u64
         = {0x88e8249a7dbbe231, 0x6db9ee6a60cfaf7f, 0x16d7b0bc61621d86, 0x1277350f0624bdf2}}}},
    {.name        = "transfer_private_to_public_7",
     .tx_type     = TX_TOKEN_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 10,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x8090bb309b82465a, 0x3d3def7288d6fffb, 0xb1c4fddcbfd81ce2, 0x0471d453e69a2a89}},
        {.big.u64
         = {0x827d867a06daa231, 0x496d03b977f42dfb, 0x1a922e774d60ceed, 0x01477261d78d6e55}}}},
    {.name        = "transfer_private_to_public_8",
     .tx_type     = TX_TOKEN_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 11,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xc12f3d70c266a958, 0x9f52672ec2af2780, 0x2472b843d00d99e0, 0x0c7ac3baf7a4bdda}},
        {.big.u64
         = {0x2f1d327cceb2519b, 0x9dbde43dbc7ef1ed, 0x56de4e410d7dd97c, 0x0fd18e7225ee4940}}}},
};

#define NB_OF_LDG_USDCX_P2P_910_ALEO_FUNCTIONS (2)
const function_parameters_t ldg_usdcx_p2p_910_aleo[NB_OF_LDG_USDCX_P2P_910_ALEO_FUNCTIONS] = {
    {.name        = "transfer_private_to_public_9",
     .tx_type     = TX_TOKEN_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 12,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x6b82137537d6a0dd, 0x61317df98dd4a8bc, 0xe2a11ba39f650efc, 0x124be5901a68d269}},
        {.big.u64
         = {0x2b799a8e84880723, 0xc9629165056c980a, 0xdff272619bc2e422, 0x0f4e2d2430b2265f}}}},
    {.name        = "transfer_private_to_public_10",
     .tx_type     = TX_TOKEN_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 13,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x334c2d4985443f41, 0xbba4c00d20110a8b, 0x793ee42e89cfc981, 0x0c70386a282ac2e7}},
        {.big.u64
         = {0x2f507dd57ba4389a, 0x8537771cafc6f4de, 0xb05c3d3e2dadf5cb, 0x038034f46cb0b129}}}},
};

#define NB_OF_LDG_USDCX_P2P_1113_ALEO_FUNCTIONS (3)
const function_parameters_t ldg_usdcx_p2p_1113_aleo[NB_OF_LDG_USDCX_P2P_1113_ALEO_FUNCTIONS] = {
    {.name        = "transfer_private_to_public_11",
     .tx_type     = TX_TOKEN_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 14,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x83c607ef315e7a17, 0xe2be22de288ab47b, 0x592ab7a928930422, 0x03c0b3412b2cfb8f}},
        {.big.u64
         = {0x26597a1d8ce62001, 0xadde9c0a9d7158a8, 0x281c94333b684792, 0x0ea320da7addfc5a}}}},
    {.name        = "transfer_private_to_public_12",
     .tx_type     = TX_TOKEN_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 15,
     .bhp_1024_hashes
     = {{.big.u64
         = {0xa0cd3379d39cce39, 0x7bd4913493398c11, 0x2dd57c6a8927c85f, 0x0c379f592c543496}},
        {.big.u64
         = {0x3ef6f385e738b9d3, 0xb0ab15290e0fc8de, 0x02085dcc3fca3b2f, 0x0d6094d4e30c4660}}}},
    {.name        = "transfer_private_to_public_13",
     .tx_type     = TX_TOKEN_TRANSFER_BATCH_PRIVATE_TO_PUBLIC,
     .input_count = 16,
     .bhp_1024_hashes
     = {{.big.u64
         = {0x26005dd39496a416, 0x6116dd16affe7e60, 0x3070881a0d8f93bc, 0x0d1d1b13ec6aadd2}},
        {.big.u64
         = {0x1b7237c1a9b16e65, 0x6e553acd1a2d8e98, 0x58c16f8729c90add, 0x0543239db02e00e0}}}},
};

const token_parameter_t token_parameters[NB_OF_TOKENS] = {
    {.program_id   = "usad_stablecoin.aleo",
     .display_info = {.type = TOKEN_TYPE_ARC22, .ticker = "USAD", .decimals = 6},
     .token_id
     = {.big.u64
        = {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000}},
     .nb_of_functions = NB_OF_USAD_STABLECOIN_ALEO_FUNCTIONS,
     .functions       = usad_stablecoin_aleo      },
    {.program_id   = "test_usad_stablecoin.aleo",
     .display_info = {.type = TOKEN_TYPE_ARC22, .ticker = "USAD", .decimals = 6},
     .token_id
     = {.big.u64
        = {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000}},
     .nb_of_functions = NB_OF_TEST_USAD_STABLECOIN_ALEO_FUNCTIONS,
     .functions       = test_usad_stablecoin_aleo },
    {.program_id   = "ldg_usad_p_28.aleo",
     .display_info = {.type = TOKEN_TYPE_ARC22, .ticker = "USAD", .decimals = 6},
     .token_id
     = {.big.u64
        = {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000}},
     .nb_of_functions = NB_OF_LDG_USAD_P_28_ALEO_FUNCTIONS,
     .functions       = ldg_usad_p_28_aleo        },
    {.program_id   = "ldg_usad_p_910.aleo",
     .display_info = {.type = TOKEN_TYPE_ARC22, .ticker = "USAD", .decimals = 6},
     .token_id
     = {.big.u64
        = {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000}},
     .nb_of_functions = NB_OF_LDG_USAD_P_910_ALEO_FUNCTIONS,
     .functions       = ldg_usad_p_910_aleo       },
    {.program_id   = "ldg_usad_p_1113.aleo",
     .display_info = {.type = TOKEN_TYPE_ARC22, .ticker = "USAD", .decimals = 6},
     .token_id
     = {.big.u64
        = {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000}},
     .nb_of_functions = NB_OF_LDG_USAD_P_1113_ALEO_FUNCTIONS,
     .functions       = ldg_usad_p_1113_aleo      },
    {.program_id   = "ldg_usad_p2p_28.aleo",
     .display_info = {.type = TOKEN_TYPE_ARC22, .ticker = "USAD", .decimals = 6},
     .token_id
     = {.big.u64
        = {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000}},
     .nb_of_functions = NB_OF_LDG_USAD_P2P_28_ALEO_FUNCTIONS,
     .functions       = ldg_usad_p2p_28_aleo      },
    {.program_id   = "ldg_usad_p2p_910.aleo",
     .display_info = {.type = TOKEN_TYPE_ARC22, .ticker = "USAD", .decimals = 6},
     .token_id
     = {.big.u64
        = {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000}},
     .nb_of_functions = NB_OF_LDG_USAD_P2P_910_ALEO_FUNCTIONS,
     .functions       = ldg_usad_p2p_910_aleo     },
    {.program_id   = "ldg_usad_p2p_1113.aleo",
     .display_info = {.type = TOKEN_TYPE_ARC22, .ticker = "USAD", .decimals = 6},
     .token_id
     = {.big.u64
        = {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000}},
     .nb_of_functions = NB_OF_LDG_USAD_P2P_1113_ALEO_FUNCTIONS,
     .functions       = ldg_usad_p2p_1113_aleo    },
    {.program_id   = "usdcx_stablecoin.aleo",
     .display_info = {.type = TOKEN_TYPE_ARC22, .ticker = "USDCx", .decimals = 6},
     .token_id
     = {.big.u64
        = {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000}},
     .nb_of_functions = NB_OF_USDCX_STABLECOIN_ALEO_FUNCTIONS,
     .functions       = usdcx_stablecoin_aleo     },
    {.program_id   = "test_usdcx_stablecoin.aleo",
     .display_info = {.type = TOKEN_TYPE_ARC22, .ticker = "USDCx", .decimals = 6},
     .token_id
     = {.big.u64
        = {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000}},
     .nb_of_functions = NB_OF_TEST_USDCX_STABLECOIN_ALEO_FUNCTIONS,
     .functions       = test_usdcx_stablecoin_aleo},
    {.program_id   = "ldg_usdcx_p_28.aleo",
     .display_info = {.type = TOKEN_TYPE_ARC22, .ticker = "USDCx", .decimals = 6},
     .token_id
     = {.big.u64
        = {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000}},
     .nb_of_functions = NB_OF_LDG_USDCX_P_28_ALEO_FUNCTIONS,
     .functions       = ldg_usdcx_p_28_aleo       },
    {.program_id   = "ldg_usdcx_p_910.aleo",
     .display_info = {.type = TOKEN_TYPE_ARC22, .ticker = "USDCx", .decimals = 6},
     .token_id
     = {.big.u64
        = {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000}},
     .nb_of_functions = NB_OF_LDG_USDCX_P_910_ALEO_FUNCTIONS,
     .functions       = ldg_usdcx_p_910_aleo      },
    {.program_id   = "ldg_usdcx_p_1113.aleo",
     .display_info = {.type = TOKEN_TYPE_ARC22, .ticker = "USDCx", .decimals = 6},
     .token_id
     = {.big.u64
        = {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000}},
     .nb_of_functions = NB_OF_LDG_USDCX_P_1113_ALEO_FUNCTIONS,
     .functions       = ldg_usdcx_p_1113_aleo     },
    {.program_id   = "ldg_usdcx_p2p_28.aleo",
     .display_info = {.type = TOKEN_TYPE_ARC22, .ticker = "USDCx", .decimals = 6},
     .token_id
     = {.big.u64
        = {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000}},
     .nb_of_functions = NB_OF_LDG_USDCX_P2P_28_ALEO_FUNCTIONS,
     .functions       = ldg_usdcx_p2p_28_aleo     },
    {.program_id   = "ldg_usdcx_p2p_910.aleo",
     .display_info = {.type = TOKEN_TYPE_ARC22, .ticker = "USDCx", .decimals = 6},
     .token_id
     = {.big.u64
        = {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000}},
     .nb_of_functions = NB_OF_LDG_USDCX_P2P_910_ALEO_FUNCTIONS,
     .functions       = ldg_usdcx_p2p_910_aleo    },
    {.program_id   = "ldg_usdcx_p2p_1113.aleo",
     .display_info = {.type = TOKEN_TYPE_ARC22, .ticker = "USDCx", .decimals = 6},
     .token_id
     = {.big.u64
        = {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000}},
     .nb_of_functions = NB_OF_LDG_USDCX_P2P_1113_ALEO_FUNCTIONS,
     .functions       = ldg_usdcx_p2p_1113_aleo   },
};
