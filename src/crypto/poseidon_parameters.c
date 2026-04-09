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

#include <stdint.h>   // uint*_t
#include <stddef.h>   // size_t
#include <stdbool.h>  // bool
#include <string.h>   // memmove

#include "poseidon_parameters.h"

#define ARK_RATE_2_SIZE (117)
#define MDS_RATE_2_SIZE (9)
#define ARK_RATE_4_SIZE (195)
#define MDS_RATE_4_SIZE (25)
#define ARK_RATE_8_SIZE (351)
#define MDS_RATE_8_SIZE (81)

const field_t ark_rate_2[ARK_RATE_2_SIZE] = {
    {.big.u64 = {0x9ec464191dff626d, 0xe3afe4fc52de2c3e, 0x55098efb31c5bb8a, 0x0f51daa50d9eca73,}},
    {.big.u64 = {0x5d10c94384e955b9, 0xa0ff049c6b09597b, 0x88f1e8263b3c7219, 0x072ebc82c44a6f65,}},
    {.big.u64 = {0x49caf68d25cdb9f9, 0xfe42e5325bd12d75, 0xe3e3cb6b932d45af, 0x0f66f5ed24cd0873,}},

    {.big.u64 = {0x53ef1989e533d3ca, 0xa45703bbb56d59c1, 0xba8aedc20c662bf6, 0x05a32d3a8421d9ad,}},
    {.big.u64 = {0x6457a7e4c5678437, 0xba7a6987f4f57a6a, 0xf4815167645f1a57, 0x0a5dc656713fb8a0,}},
    {.big.u64 = {0x235b0b1555642db5, 0xcec9b78e44325abc, 0x2c9563f8da1d08d0, 0x0a57cccb809b2880,}},

    {.big.u64 = {0x1029d909e6027efe, 0xaf9468e7372d5724, 0x3bc1ed72b652b16f, 0x03ff277709c4d56f,}},
    {.big.u64 = {0x8832930f567fa582, 0xa8f83263a19791a3, 0x50d0d3408b6b04e5, 0x037574ef84e565ae,}},
    {.big.u64 = {0xb35c1903ca017011, 0xc1386268816fd161, 0x1445750501a3d4e7, 0x04be77d725b0dd8f,}},

    {.big.u64 = {0xd294950b1e1e3741, 0x3b334cc79a833de3, 0xff5871e159439902, 0x029925877e112131,}},
    {.big.u64 = {0xb3a38d95e1f97fd5, 0x545cf6fe79bb544f, 0x894e3746765042df, 0x0889051459c99d4c,}},
    {.big.u64 = {0x299bc35698f34a2c, 0x711b785113e1400b, 0xaac79823d636106f, 0x0f8d542c206a2e9f,}},

    {.big.u64 = {0x09938734953d0983, 0x700ed38f8bc8ea7b, 0x49b257f4f65d2812, 0x0d72e7d074c95c5b,}},
    {.big.u64 = {0x097737f650c322d2, 0xd0d7b9b75e137f4f, 0x1f92e2819efe9a5b, 0x10a6d950e906c671,}},
    {.big.u64 = {0x1aeb383a27e36474, 0x500e50d8c9564698, 0x870bc0e7677c4113, 0x116378547e3583a9,}},

    {.big.u64 = {0x3f6b9f1ead24181e, 0x0aa25f4a586c604a, 0xf8501dd7c7261c88, 0x0b6995183189f583,}},
    {.big.u64 = {0xeaeb5379b4aa7a8d, 0x6646f3cc489c24ed, 0x6b27d15587e9a0fc, 0x094100315653d99e,}},
    {.big.u64 = {0x991568444e180311, 0xd2737438c26ce90a, 0xdbc275ba4f952e3d, 0x0cf2901c8061a86c,}},

    {.big.u64 = {0x420fe6c269716a0b, 0x08b7dd4708ef709e, 0x329d8a2e6fe200ed, 0x1222e31cd92b62f0,}},
    {.big.u64 = {0xb652e596980e08ad, 0x2a4cc531884fedc8, 0xc46ca74eec39d49d, 0x0b48e7b1d7a509fd,}},
    {.big.u64 = {0x4fbaa3701ddeaa0d, 0x46253f48809fe521, 0xd112ddebd5e32030, 0x1097e3ec6e60e118,}},

    {.big.u64 = {0x07f6c938967b0c46, 0x66ad30d9230cadd4, 0x8e29f7672e4a0be7, 0x04c41db88b35e10a,}},
    {.big.u64 = {0x2e06c4c8fccce6bb, 0xcbcc115dfebbaec8, 0xb9bfb46a2bb6c2bd, 0x03f7022fec7006a3,}},
    {.big.u64 = {0x9ba3cd1b5a7ea410, 0x2846943e7e492f26, 0x4b5467dac4c06c22, 0x0fee1050a62457c4,}},

    {.big.u64 = {0x596d83e6afb9ad36, 0x691980fd98760a84, 0x1c0fb72236b50683, 0x0fb35167160dc2a4,}},
    {.big.u64 = {0x358fb4ef306096a6, 0xb56d4c3b786a0e9e, 0x62bb94eba70f9139, 0x069e36e501704d5f,}},
    {.big.u64 = {0x869458f0325ec1bb, 0x30284fcd2d534c3c, 0xcbef149296fc72ca, 0x03552f3324516b98,}},

    {.big.u64 = {0xb90e6db2001a76e5, 0xb0a48954a7c16964, 0x4c1708dfa8a00722, 0x0c282b1347afa86e,}},
    {.big.u64 = {0xc546217837e92b33, 0x16e80e8b0b4c9e40, 0xcbaea3f8561857e8, 0x0982959ca9f93cb6,}},
    {.big.u64 = {0x137b20572180ca19, 0x4bec88c627bc3286, 0x437845a137c09c0f, 0x0dd87b8180b6642a,}},

    {.big.u64 = {0xef8ae14f80579ec4, 0xff00af70e7f82ca8, 0x349a668ef4a24268, 0x09045a3ec71217dd,}},
    {.big.u64 = {0x0575c565182420ff, 0x81855a68599238f4, 0x4e193b16c1e2b053, 0x030c8d8f831a71b9,}},
    {.big.u64 = {0xed1d25ee13da70a5, 0xa803756488855b3e, 0x2d11bea5f5a54c36, 0x064cb52b22e2d297,}},

    {.big.u64 = {0x1f821ae0a7913832, 0x175e966e73ae8438, 0xe28721be2235efa8, 0x116cc59db8e844a2,}},
    {.big.u64 = {0xf53466063aa88798, 0x2b79ed8957bb7285, 0x97b8d49913859afe, 0x0947b6de25ca7063,}},
    {.big.u64 = {0x80b1cec4a438e5ca, 0xf9aa695bca686274, 0x65295130b59fbc7a, 0x003454a81643d399,}},

    {.big.u64 = {0x23586842f3dcbd7b, 0xadbbed7a47cd64f6, 0x0593a5c48a5ba0b1, 0x0e1760cb4dd92ff9,}},
    {.big.u64 = {0xe459ec695b72e1cb, 0xd059df313554e8b2, 0xe797a63fe1b23a52, 0x0839931866d47ce4,}},
    {.big.u64 = {0x6ea750cf756662d9, 0x79d1a39f46657aa8, 0x061746109e6df99d, 0x0f409eea34d3c04b,}},

    {.big.u64 = {0xe2b164f19764e6cb, 0xe6bbad23d87191ab, 0xc695fe80e0895bfa, 0x03a97f7c1c702554,}},
    {.big.u64 = {0xec79f5a78833fb19, 0x091c7690074f5edb, 0xa0746c7f5da5ea7f, 0x036afe094bfd4997,}},
    {.big.u64 = {0x8535b06e643956d0, 0x71e46f2cb9deda7c, 0x2b8551524c6b29c6, 0x115df1e317d6ab8b,}},

    {.big.u64 = {0x81ddb164cc2b4916, 0xd63ce6f3cbd46b92, 0x69788d568278b866, 0x05b29eb4dc7bfc1e,}},
    {.big.u64 = {0xf96b859445f2c615, 0xa97ca470c11bdb4a, 0x7ce851d297375458, 0x0a2198edf79ecc3b,}},
    {.big.u64 = {0x5e33cc209fd478ac, 0x14a3d98d7598304e, 0xb1d9c34490abbcc8, 0x09c86ed5b899a089,}},

    {.big.u64 = {0x6982c91b12222cc3, 0x94b68f470f5beb94, 0x9f8c30285f5c05dc, 0x0a05981c0a98b710,}},
    {.big.u64 = {0x2153fb9b52f9a5cc, 0x0ed6e5f7dd4033f2, 0xd07544ce5b5cd24b, 0x03be7474bc150d13,}},
    {.big.u64 = {0x9a56a64d048f50de, 0x286b69ead08aed25, 0xb41a061755e2d61a, 0x038458139d19b6fc,}},

    {.big.u64 = {0xb3f42fcd95791a2b, 0x9d79b2900c749c2a, 0x13ea88cdc661cad2, 0x051f6f4a81dac2d6,}},
    {.big.u64 = {0x1017d43acc69eb34, 0x409a0366fdfb255c, 0x6dbcb7cba12fd028, 0x06ca7b023199e459,}},
    {.big.u64 = {0xd8f156db12d243b7, 0xb4218274948f02e4, 0x2d93f5aefc12c2a9, 0x05395869efcac3fc,}},

    {.big.u64 = {0x44abcef2bb378bf6, 0x19d41c39a3ac77b2, 0x91c2afbf2fdcc886, 0x0e458d57f71ffad6,}},
    {.big.u64 = {0x0c642bde093f2dc2, 0x2156fa7445a2704f, 0xc55a1d0705344d4e, 0x0c6150424cc81af4,}},
    {.big.u64 = {0xf06d469b327ae6d0, 0x69c61dfacf3e40a2, 0xbae3ec026f6c6672, 0x00ff33bbef179f8f,}},

    {.big.u64 = {0x862aeb8da2a507c1, 0xeda0be75dda9289e, 0x1729577c9b5a9827, 0x0ea511b4272c347b,}},
    {.big.u64 = {0xff4ae063f9472246, 0xc4297d2def661cad, 0x385a933723a0e760, 0x0c65a9a8a43949cd,}},
    {.big.u64 = {0x9d03995b7b1b474e, 0x36d5a5ad0f8d6072, 0x651d898ee87b7135, 0x00b0e118897c3b45,}},

    {.big.u64 = {0xcae938200e46105a, 0x1f6b2efc7872cc03, 0xfbe6168061e31f80, 0x0f2272719835b3c5,}},
    {.big.u64 = {0xbe06e0ac481e787a, 0x4e5320ef47f4e988, 0xeb9fc868d7f38492, 0x0c73d223c95e12fd,}},
    {.big.u64 = {0xec6695b677cf0913, 0x0a7b157ea72ff71b, 0x32fd977ba15f343c, 0x031fd0756e79a9ac,}},

    {.big.u64 = {0x7426205ccae3b157, 0x856d6be2bc010f64, 0x391fb5e9a31f9f38, 0x0589644dd9371cc0,}},
    {.big.u64 = {0xc9f7135f2d78a54a, 0xeb5788f04a55ad34, 0x267622b89f984a02, 0x06ab1f5a030e99e1,}},
    {.big.u64 = {0x500c67b669c4765b, 0x4da6fa1b807a9198, 0x4cca1e31ed54966f, 0x09dc9480c4b47a1f,}},

    {.big.u64 = {0x5b9aa111f3d61034, 0x3e6b17d174b4b22d, 0xf964603ba7e1bb93, 0x128614f1a579c017,}},
    {.big.u64 = {0xbe80b8945b441e43, 0x7714c42ee8a722ea, 0x83cb858b82e71e94, 0x11bb67615d302201,}},
    {.big.u64 = {0x2cfa9008d32c72f9, 0xdb89b27958ae6d83, 0x5acd39c9f2940e49, 0x0d657c9194233125,}},

    {.big.u64 = {0x7b176b7feba59989, 0x6d8ee1a0b09e5bb3, 0xc134d0e7c43176f0, 0x006db3edfb4c260a,}},
    {.big.u64 = {0x5f2eb47cdfbff1b9, 0x9d95b48e6d2f6793, 0xd09a9ae4b6aabc84, 0x0d165c0f7e611b25,}},
    {.big.u64 = {0xed1d02c9e159c1af, 0xcb9e58ab4a9763e7, 0xdde6dddf8faa0e65, 0x0b5b1eaa9ac18b7c,}},

    {.big.u64 = {0x771e186a3d6ad630, 0x78fac17292a67062, 0x78902142b5104a6b, 0x0fd613359f241b94,}},
    {.big.u64 = {0xf4f326f9505a0e0f, 0xdccb5076a7459e0f, 0x190973660cdd8c05, 0x00242310fd779c3b,}},
    {.big.u64 = {0xb874fe8f171a17b4, 0x6df42b9a3709bd13, 0x0d38327ceb98797c, 0x10eef1a001cce1f8,}},

    {.big.u64 = {0x1c05d7265d7da8a9, 0x58b260fa34434b12, 0xe808027528326c4f, 0x101e692d3e838010,}},
    {.big.u64 = {0x1a1661b5232dc811, 0x919bd6452d7c1682, 0xa4fb971c6bf20c6f, 0x0c6d4311b009b244,}},
    {.big.u64 = {0x4d9254fcd7bb93da, 0x4dd6c35e99e37982, 0x9b0963aca9ead932, 0x0681390062fa94ea,}},

    {.big.u64 = {0x5e059d100f1b3f34, 0x23e9c67978c601f7, 0xccb310d0e7a4018d, 0x0623d4ba2971122f,}},
    {.big.u64 = {0x116a2078cac7a34b, 0xd94c0edf8476f4b6, 0x132162f3d5aeb5f2, 0x12901bbc2a27c164,}},
    {.big.u64 = {0x5c271c72349a01b8, 0x262d47c9fe072023, 0x49138c4989636720, 0x0efb0a0e8b74bcbe,}},

    {.big.u64 = {0xf6b05e568158d1d8, 0xbb8cb0ef8700f134, 0x18d6cedd3931f532, 0x0ea0b608f8c8a715,}},
    {.big.u64 = {0x114a58e627776dce, 0x0fadfac8e64f4283, 0xb40caeea84db18b4, 0x0f135db0d8e08986,}},
    {.big.u64 = {0x88345f4a15c0ec0e, 0xecaa89ec71a68724, 0x16a85d9b1f3ce58b, 0x05f144646764db35,}},

    {.big.u64 = {0xba2b234bb6a8d7ad, 0x47d996f15572ca34, 0x69f77b3a09be26c9, 0x0b1dba6322b682d1,}},
    {.big.u64 = {0xa3107d5ae4d19ba4, 0x381baa613fa56d49, 0x881af809dd72bcae, 0x04123c3627c60841,}},
    {.big.u64 = {0x2d6135db280dcbc1, 0x95330c86044ea2f9, 0x864b0b1afc615a88, 0x086117f7ab8b4ca2,}},

    {.big.u64 = {0x7696662f8aaf709e, 0x4d420c5ea6ce1644, 0x0f31105c111066ad, 0x0f2580713531d85b,}},
    {.big.u64 = {0x9e379d8ff43bc547, 0x964d11f183103fab, 0x44a53be274dbe1ad, 0x06a267ad2b2033ab,}},
    {.big.u64 = {0x7a37ea31754d349b, 0xdf63eece1b8badbf, 0xf4f8ab96617e05f3, 0x0cb906610127516b,}},

    {.big.u64 = {0x318688a65d1a53d3, 0x7feb26c13ec1630b, 0xae82f204469a8e86, 0x0dd6b64a1858126e,}},
    {.big.u64 = {0x66703c46f98c950e, 0x168e49b4ed701705, 0x26962547178879ea, 0x0de52afe5395b717,}},
    {.big.u64 = {0x8365113fed764f0c, 0x590e29c31c68e8cc, 0x8a43c53398da1a9d, 0x10ba00dedcc99067,}},

    {.big.u64 = {0x4e88bfec6902e5d7, 0xbb39701f486adf29, 0xf34de7b5a4cd494c, 0x09336475128a6cda,}},
    {.big.u64 = {0xe73bea7689bd3eb2, 0x56c62b3060b33d07, 0x18594b3864cb7166, 0x0c047cf731278da0,}},
    {.big.u64 = {0x8f19b50d5b1926e8, 0x929da129a3366b1e, 0x4234f5d772f8cd58, 0x0da188ed55431b4d,}},

    {.big.u64 = {0xfff038246a912103, 0x7b8f4bede1c99953, 0x87f1c8f0b548d761, 0x0cc0d16bb8d06f11,}},
    {.big.u64 = {0xcc5611d257410762, 0xcfb9252a3290bc01, 0x2b555f4bd9767b58, 0x06ebe3ac3f172e88,}},
    {.big.u64 = {0x2e5d5dfd8d86565f, 0xe503aeda26517138, 0xfe018fe276e0e33d, 0x09f5e383c9a32953,}},

    {.big.u64 = {0x3edab5f0f8de7ed8, 0xb028ef019a59c73e, 0x2d592d52e2da549a, 0x0c59ef1f23f0b132,}},
    {.big.u64 = {0x68f8ba6ff8693fa2, 0xa73cfb4799873150, 0xde60a3170e57994d, 0x04b26beea79585be,}},
    {.big.u64 = {0x21fae7cd88c1ea99, 0xf2ed6694e80a29c6, 0x55f427ee15533d45, 0x04091b4623cce678,}},

    {.big.u64 = {0x2168e590097d7fdc, 0x3de49bdf74a09d6a, 0x1d807d5083ac4ffb, 0x114a99fa392f1035,}},
    {.big.u64 = {0x5e763211a68f7bfd, 0x757efb09f06d1fbe, 0xd3ed4aac99b32364, 0x1179f44e98dbfa2d,}},
    {.big.u64 = {0xeb3c03e28ff48138, 0xa404f1c754096f96, 0x422f9c9b0ffbd49f, 0x002d28553ce8d773,}},

    {.big.u64 = {0xc229d9ecfe01ac19, 0xf06795be03d6ae9d, 0x4cfad8f1e6ee49f4, 0x00fa1bb36fa18432,}},
    {.big.u64 = {0xdf59b5601c3cd323, 0xdb4ad0ec6d2e9a63, 0x7afb96d36b7c8dfb, 0x023ffae15409e6c0,}},
    {.big.u64 = {0xafd9b28917086ac6, 0xed627b6585aef0bf, 0x1f1b4afdf4a133e3, 0x021b7329bc02e25b,}},

    {.big.u64 = {0x96c3de5813aec901, 0xd191b509038ccb27, 0xc6d4105f483d3884, 0x105773b2d1ff2110,}},
    {.big.u64 = {0x4b6abfe837876890, 0x2b0c259d7a9c5e18, 0xfabae1eed4089bea, 0x118181aca8f07377,}},
    {.big.u64 = {0xaa54445143377422, 0x646da8d02b8b2a43, 0xcc3088e3c70d1170, 0x054812e23a7fbdcc,}},

    {.big.u64 = {0x74f6197a0e79f261, 0xc538d0d224c893fc, 0x353ca81bc5669525, 0x0a317eecbaa836d1,}},
    {.big.u64 = {0xe128dfac01da21f8, 0x34e658f0aaa917ba, 0x20737042db392ee4, 0x12008b969d02ab26,}},
    {.big.u64 = {0x228968205643dcf5, 0x16ac507dc2b4810c, 0x37b6529b3c3b881b, 0x05d8cbac60dd80a8,}},

    {.big.u64 = {0xb17db0a3dc14521d, 0x4377c2c7a159716c, 0x881f3e12c9c4c71e, 0x0a3bbadeeaadceec,}},
    {.big.u64 = {0x4e6ed3a8acdbf36f, 0x3b35ef84bdbc6fa5, 0xafd736f19bf4ff74, 0x05e6b12e1d90a59d,}},
    {.big.u64 = {0x45b9cd311568ffce, 0x516cb8d6c7992c27, 0xdd9d0e19c9b3a0ec, 0x08677eeb6551763c,}},

    {.big.u64 = {0xfac7e21f9fbf6c01, 0xc1e2cff3ef351175, 0xa4b3ab9e619d711f, 0x1262af5fa841fed9,}},
    {.big.u64 = {0x811bce6f814483fe, 0x0bcadf20ebdfd74d, 0xe4d53e15214f5e7f, 0x0513673d5124c924,}},
    {.big.u64 = {0x6938ab32caf0e7bc, 0xf0c30598c1145300, 0x1033e8c7338e9131, 0x01e7d4225297a076,}},

};

const field_t mds_rate_2[MDS_RATE_2_SIZE]={
    {.big.u64 = {0xd3e1b719627eef9c, 0x944031bef18c05b7, 0x05f93620ee657611, 0x092696314ebee66e,}},
    {.big.u64 = {0x54aea0c191840710, 0x7c6572388b138cc5, 0xe99b1822dc11ec04, 0x10d3c62b2749cd64,}},
    {.big.u64 = {0x5370bc9058093ead, 0x931289d50ba24947, 0x9a5ce8875869e795, 0x0d53f93182fa097b,}},

    {.big.u64 = {0x8f4dc124731197d2, 0x2a36bbcd15582c3e, 0xc96c0705c91d3bb0, 0x0ce47d4a9f5a4deb,}},
    {.big.u64 = {0x0d204dd5e85dbef9, 0x2e8997d74794711f, 0x05ab70b99149188b, 0x0bdcba2f9a789715,}},
    {.big.u64 = {0xae376ca177578394, 0xbb07742229d10c42, 0x685c9a4cddde050b, 0x07bc47e84d8e154d,}},

    {.big.u64 = {0xbca10ed4ff18a096, 0x1f8a77b246da31e1, 0x16c033b302c66787, 0x060a726dd83142a7,}},
    {.big.u64 = {0x79b1112619f1dbe1, 0xf4daee489fff6d9f, 0x34e6125090a92526, 0x0b85b6021fb7ced6,}},
    {.big.u64 = {0x20a59b5ebb6c205b, 0x24fd7ff921f78dd2, 0xe5cf0906ceef128f, 0x0a754100a042d8b9,}},

};

const field_t ark_rate_4[ARK_RATE_4_SIZE] = {
    {.big.u64 = {0x34c37d28c3f4c1ce, 0xa15d6ace8616e46e, 0x23cbab5413838926, 0x104c314c3151dae4,}},
    {.big.u64 = {0xaa647468e42a8bf2, 0x1fbfc06f37137b2e, 0xad99ee12341f6ede, 0x0bbfd74e0ec870e1,}},
    {.big.u64 = {0xddbbbc740d2294ca, 0xc3e128c0d7dd604d, 0x255f41bb46a79480, 0x068dbe6acd82f805,}},
    {.big.u64 = {0x90f678904cdbbb1b, 0x8cca6e86b931e33c, 0x2f5a8c702f02d1ef, 0x04f922bd39962578,}},
    {.big.u64 = {0xec1c2b7fa2cf16f3, 0x56cff368c27540df, 0xe94ce9e353905aa6, 0x08b69ab751354e78,}},

    {.big.u64 = {0xdcdb0ce947415b57, 0xa58640bf51d2d722, 0x1507d69f356b3efa, 0x09aef6be7d445a91,}},
    {.big.u64 = {0xada311e49e6344d4, 0x6b54bb8bf3f6c3bc, 0x2375d4e6dfa3f68a, 0x02a7697b221d8309,}},
    {.big.u64 = {0xfdcd4e42c21b4c57, 0x2fdf1f71db60dc35, 0x54e5fcd2cfca7e88, 0x073e18896b646529,}},
    {.big.u64 = {0xccb0aca3c31fdcbb, 0xbeebf628d0e42ff6, 0x8f7076bbfa3131f1, 0x0d30f61a86a6eec1,}},
    {.big.u64 = {0x6909306d4d5f197d, 0x4e4e2ec064a78bf7, 0x9211335926874176, 0x09f84e5a12803f24,}},

    {.big.u64 = {0x6e9ff29a57c7586c, 0x21c86b8998317719, 0x87d841931e401400, 0x105039b7cbf555e4,}},
    {.big.u64 = {0x15fec2d52ab8e037, 0x9fbe089eecf4d984, 0x0c8b77d7f9adfe05, 0x0d5987bddd8774c5,}},
    {.big.u64 = {0x1f2d4b5aaa225253, 0xc6fff2e5a39cb1f7, 0x8efe88620e2012e7, 0x120863ef9d5ebca5,}},
    {.big.u64 = {0x28a7e408f4d89e2d, 0x30b1384f83f24117, 0x3d521a1bead655a0, 0x072eb2bbab541550,}},
    {.big.u64 = {0xd601888ff79fbbd6, 0x2ca0adbf3f51ed4a, 0x7094fd5baf8a3315, 0x085818ced58ba1e9,}},

    {.big.u64 = {0x0c0f52252b1a4c64, 0x96520852c13bbeb0, 0x5b65bf0b25636cb3, 0x06595e5284467015,}},
    {.big.u64 = {0xbc96ba2af10cc5a6, 0xc298886cd6c7477d, 0x647deb76273e2621, 0x0be79cbb4d37eac2,}},
    {.big.u64 = {0x7a7af65d24a249dd, 0xbc11132a1ba3a7e1, 0x25aaec6ef8dc1644, 0x0bdccbbcded73aad,}},
    {.big.u64 = {0xd4b87b7bbe86aace, 0x458f859956c6d676, 0xf48c952d4f3f6882, 0x02098bfab39dec4f,}},
    {.big.u64 = {0x11e07957d3846c56, 0xe809fa1f0e2e43f3, 0xc2b57778c69bca98, 0x0cdb9db69b0a69ab,}},

    {.big.u64 = {0xa2ca7211220a4d79, 0x582361afcefd52be, 0x493420b52c934da5, 0x118d6b21c72879a8,}},
    {.big.u64 = {0x29ef8d5d581604da, 0xc5aeaf6bb2e4523c, 0xc96996eb002f6910, 0x1130af4d2207ec7f,}},
    {.big.u64 = {0xa338343cc03d4ef7, 0x37eca5e2df411571, 0xcfd2d5d9ba513061, 0x103ebafa7dab362e,}},
    {.big.u64 = {0x0d4e2e618c771d47, 0x5da39258c214919f, 0xb0fc9b388cb15e5e, 0x0df15627d2b464ab,}},
    {.big.u64 = {0x9f873162321aac9b, 0x475678e3050ea787, 0x48cb443511c72462, 0x0c72da20c254f6f4,}},

    {.big.u64 = {0x167adddbc391f92c, 0xf9559d306020b80d, 0xda6e554c43bf0d1b, 0x10b0b1d99a8a70dd,}},
    {.big.u64 = {0xb5b38b488c2a80ee, 0xbf28f7c488a54a7d, 0xc12e49f849ffc73d, 0x111d6fe065bc1289,}},
    {.big.u64 = {0x32b8b59ad747d991, 0x315096567c62740a, 0x429e115409dea89a, 0x0c64049e2777047d,}},
    {.big.u64 = {0x97edd0963ef1d7eb, 0x8b0ef8cb1eab153b, 0x7140101ce5243cf4, 0x11c3c3b63dfabda3,}},
    {.big.u64 = {0x502bb3fff4d7f479, 0xb187ae88b336d48e, 0xb2bcaabbefa4971a, 0x0b9968415565ad61,}},

    {.big.u64 = {0xad39f3f13be6c76d, 0x7f27700d97e5ce64, 0xc8c1c26d672c065b, 0x0e8621753f3ea92b,}},
    {.big.u64 = {0x2990443d9538a2f4, 0x45faf014d9e605c5, 0x5e4441ed0a04dbc1, 0x0ac056dbfa18445a,}},
    {.big.u64 = {0x49b6ef075fa8641b, 0x9b5bef50cb9a1c20, 0x457f7ba3f4bc5fcc, 0x038b476188015ae9,}},
    {.big.u64 = {0x0b2092352ffe771a, 0xcc66e3f302656e2b, 0xe1b8db8831ed4db1, 0x0b928bdebf20c78c,}},
    {.big.u64 = {0xcbebeb41843295fe, 0x261ef51eb9b5906a, 0x2712ad1a5ecb082f, 0x10d2608b97965e7f,}},

    {.big.u64 = {0x9f617291b171a9f7, 0x3b039dfba1472575, 0x1869215c5eb1ec7d, 0x107fc91d08fb367b,}},
    {.big.u64 = {0xb587b1b73e934464, 0x5cc72919492a496d, 0x9da440f0b532735a, 0x06a44c5c19d392d9,}},
    {.big.u64 = {0x06a93869c3c208a1, 0xb529badf8f835874, 0x2a945765cc84e74d, 0x0f0bbf3cef073008,}},
    {.big.u64 = {0x7fe548f7e0aeded1, 0x95e00b600db64ff8, 0x44928f2f7cec7269, 0x08544cadc2117384,}},
    {.big.u64 = {0x400d083772377385, 0xcb269d24b1589ecc, 0x819fbeb9d515fa7c, 0x07f28567b239cc59,}},

    {.big.u64 = {0xcd8f3be99e042143, 0xcdeb14f751d2807d, 0xc8fa3207c948296f, 0x023e97cb99a87c8c,}},
    {.big.u64 = {0xc06dfd6c5b8eee77, 0x9a9c8b2f8c3ebde7, 0xa002e85c30aa07e4, 0x0b3cf51fb1859e50,}},
    {.big.u64 = {0xe14bc6e625da6023, 0x89eea52c5f07c766, 0x3c781bffdf3730b0, 0x094a532f8a00ea35,}},
    {.big.u64 = {0xbd167487258e056b, 0xce6d56b2781ef341, 0xf40e53a38f172ac2, 0x01d527175b8c3651,}},
    {.big.u64 = {0x74417c2756e72513, 0x1cd2ca018a3a65dc, 0x7038ce070cc37cef, 0x0e9dba000ab116a4,}},

    {.big.u64 = {0xb87b2455a433f4af, 0xf871a198d59dceb7, 0x0f0c0dc8cfccae52, 0x050480245a2fd0f3,}},
    {.big.u64 = {0x65c96345906465bc, 0x4d102a632b5c3864, 0x958abd70d99a4e47, 0x0805d82c1d4d1dac,}},
    {.big.u64 = {0xc188075f55d9e35a, 0x57db16a1f99ac37f, 0x917a9fbee3e84bfe, 0x0da5044ed3ec926b,}},
    {.big.u64 = {0x2425606f78e318d2, 0xb2dc9e5f260f6817, 0xfa822b7742b677ac, 0x10ad41a2309f5020,}},
    {.big.u64 = {0xe5e4ad9988d4b04a, 0xccd765ad72f7a4d0, 0xb57b6d4348075cb4, 0x0aca762c5dbe8a3e,}},

    {.big.u64 = {0x688ea9cfe303b7c8, 0x139fa7e653420b04, 0xb4802dacdfa1fbde, 0x01b635cf3389e678,}},
    {.big.u64 = {0x6938538c3b33f939, 0x63af4ac9c2de1cc1, 0x67dc46362bc6e521, 0x07c3c9118294bb8c,}},
    {.big.u64 = {0x634bc50dcde4c554, 0xe3db0e9ca0c66d54, 0xb2dc4a501cefd871, 0x02ac3749af29db28,}},
    {.big.u64 = {0x293d7211eddc16b0, 0xa9771f50e347905d, 0x595b4d5289da3b24, 0x08ffe8fe0636cec9,}},
    {.big.u64 = {0x36e87451743d7ef2, 0x1a2e2f65d69855b0, 0x3594965eaa21e534, 0x00f6f95943bbdd62,}},

    {.big.u64 = {0x0030a090b73e0797, 0x46d84dd9c0f19133, 0x743de2d0c67906c8, 0x099cf8417ba1d665,}},
    {.big.u64 = {0xc170ef6c20f02b0a, 0xa333e35df4af0980, 0x1a5c945bb1bf3f05, 0x07e592d5bbc32539,}},
    {.big.u64 = {0x94e3c9f2c21d95b8, 0x2cfb8bfc9adeb7c0, 0x2e79f7526027f872, 0x1132d66d295ae621,}},
    {.big.u64 = {0x8eef330df70a6ebe, 0xfd9e9282fab5a003, 0x7a165a419f5c5b6a, 0x0ca44a2d89c17174,}},
    {.big.u64 = {0xd4fc2c03aeeda74a, 0xf2a4cdc1aecfd79f, 0x72f6083dca91c611, 0x0187f77329cd287f,}},

    {.big.u64 = {0x5f6040a29d869f25, 0xfb4938e01247eb26, 0xbf94d235776ccf78, 0x05b731a8869be4da,}},
    {.big.u64 = {0x59a3b2a97c6a7af9, 0x6312befaf03547f2, 0x8367ee09b57052f1, 0x0c43bd7b922a39f8,}},
    {.big.u64 = {0x2d24a503b1763e3e, 0x43d6bf9b01bb0aa3, 0x4d1e0a48f480ad87, 0x101d51b2c3e8914f,}},
    {.big.u64 = {0x3e1a75a02da22093, 0x3b9895e1e8fe2fd4, 0x6e8bc592a778a3a1, 0x0e252e15f4864588,}},
    {.big.u64 = {0x3aed004bebde3cdf, 0x07ed1ed0787f4d3f, 0xd7c0c29d4bd8473b, 0x08d985b3755235f2,}},

    {.big.u64 = {0x4dba8f919595622b, 0xd2d1539dd3ca8bb5, 0x52ab104504439985, 0x11fb398ad74f2771,}},
    {.big.u64 = {0x2f4bc537fbbf38e4, 0xfe1f3c3005a630e1, 0x61d4e4b826863247, 0x10418292cb3ef6fc,}},
    {.big.u64 = {0x90ec2a9c4d6a054a, 0x758cd3149d7ae87c, 0xec56da0a647b77a2, 0x10879f53b660692e,}},
    {.big.u64 = {0x6c24ce4858f19cb3, 0x43cc054c43e4b699, 0xe839b3d5a29e0c93, 0x12a9e711f21e3980,}},
    {.big.u64 = {0xdd8b6f349a46331f, 0xda9450c3ea3a477b, 0x9221541950bde249, 0x102f5eb54f11ffa1,}},

    {.big.u64 = {0x162cf89ab2f73ce7, 0x0a6039a81b98e415, 0x14019636f3657381, 0x0945a72c85ef09d5,}},
    {.big.u64 = {0x888e8f5ff57a1ae8, 0x88d04acb0ee7aaec, 0x0ddb5c39fbbb8109, 0x059c6317d8eacd82,}},
    {.big.u64 = {0xf228407ee9c0a9b7, 0xc24d1d7a0717ed03, 0x0aa68ed25cd3b73d, 0x0337e0ede33c09b3,}},
    {.big.u64 = {0x5da37b3a38a03bcd, 0x7f1655b0b63e5cc3, 0xe67a15745fa6e197, 0x0cba72e2efb674d5,}},
    {.big.u64 = {0xb553b9b696b58894, 0xb4bb899870242463, 0xb032073dfb0880f6, 0x0afd4cd91665c8a4,}},

    {.big.u64 = {0x6a0272f7e458c433, 0x070f6ab60f763582, 0xeab30a554f9b67dd, 0x0d74a86e83fc2723,}},
    {.big.u64 = {0xc2b9880839f16c31, 0x97987365d89ebc77, 0x71b2f9785a26812b, 0x0a19ea2659db528f,}},
    {.big.u64 = {0x5aab2040e1ead6a7, 0x289a6bd0262f0cd1, 0x8b2a8d7e78c8c19d, 0x07ad794ea33eea7c,}},
    {.big.u64 = {0x9ce68bceea681940, 0x73959a24f1f6f5e5, 0x51c8df5d0b5fa41f, 0x11b7cb680cc4b53b,}},
    {.big.u64 = {0xd688d4697190501f, 0x34babf9b854081fe, 0xcb0090a903978abb, 0x074bde68eec41146,}},

    {.big.u64 = {0x859bd6d4f879f537, 0x1b869c608b505239, 0xf2c209687462c87f, 0x0af6b270eebee755,}},
    {.big.u64 = {0xd03d737a961a9fd2, 0x929d861d357ef6f8, 0xf11eec843a9f0b02, 0x00963825edd0a4ef,}},
    {.big.u64 = {0x18f702362f7412a5, 0x71438355db963d5b, 0x092fdb40a57bb3d6, 0x0ccf0e7c4e37d427,}},
    {.big.u64 = {0x4c457b1d2ba50d35, 0x832ea8bff401da97, 0x79974a597d572ddf, 0x017ad1fb4f362db5,}},
    {.big.u64 = {0x9362b1fd5ee4dc7b, 0x0eade2fe9859cec2, 0x5f3ea79920716c1b, 0x0b56bb898efa108f,}},

    {.big.u64 = {0xa88261330b9d34b0, 0xcb17f525cdf0f687, 0x876c6541016d8705, 0x0bc8356a32bb041c,}},
    {.big.u64 = {0xd31d9b5f90096c81, 0x1fb7646ade9feb05, 0xa26c3914a33276ef, 0x0a4641b44b3db043,}},
    {.big.u64 = {0xf640a35bda189c6c, 0xeebe4718cfaf5e58, 0x4fee5508b8fb50e4, 0x041a024bc6b0bfec,}},
    {.big.u64 = {0xbc0dbe84f46679d4, 0x493bcdb078d45328, 0xc07b460aa57046f7, 0x06c24edb181dde8b,}},
    {.big.u64 = {0xeb6d6e8a48166bec, 0xc324174dfb7b988f, 0x69216d7471157a3f, 0x051b5d911d3b93ff,}},

    {.big.u64 = {0x5833ed6fe9e7e3f9, 0x7e6de07d099ea8e6, 0x8d354e6bcb146c55, 0x0325b2aa4c97e6c2,}},
    {.big.u64 = {0x051b107e9309ad8a, 0xcefcd03bcbbc7a1a, 0x2ad271e2f87f3773, 0x0342208cfcf46040,}},
    {.big.u64 = {0x48e9bfda404fffe6, 0x16bc877fdd554ed5, 0x72453babf5e77a74, 0x0dc84cd5cc8c3d14,}},
    {.big.u64 = {0x7c46170a6cb78f5f, 0x134981473844dba1, 0xac7bf308c52b9c4e, 0x03139b762b677c9c,}},
    {.big.u64 = {0xb218f9a143ff5fdc, 0x74a5c08bdbf821db, 0x7569e1e873db2380, 0x03b0bea510fa6786,}},

    {.big.u64 = {0x448ad0fa2fb3e597, 0xb076399fa84bc5a9, 0xa83659cc3ac644c6, 0x0d8c98e02afcb9fa,}},
    {.big.u64 = {0xd77d2bd8f4efee7b, 0x39ca474365a03660, 0x3a941f14efc26616, 0x0ba7978ce89e33f0,}},
    {.big.u64 = {0xc65fed745ee84154, 0xe77a5623b767d7b2, 0x2ded87d02eb52f99, 0x00cff75cf4c26c28,}},
    {.big.u64 = {0x577dbde6cafe8690, 0x8af4de9379c64309, 0x24c5890a649d1430, 0x059383df1f9b62e2,}},
    {.big.u64 = {0x9fa86d03a17e3376, 0x380f8378349f164b, 0xb54a60fe5a754f5b, 0x0c31fce13a634122,}},

    {.big.u64 = {0xab63e5d983a9044b, 0xcc6d4e642efc0c57, 0x122b55b548ebbf17, 0x02c0e698a7e0f856,}},
    {.big.u64 = {0x145f9d6f04693433, 0xe06c2e4691e831b9, 0x1c1df9c0974a2265, 0x0d32cb93030d4a39,}},
    {.big.u64 = {0x4025f9caacae07a4, 0xbdbab610db2beced, 0xcabfee44cc27e4a5, 0x0ac9f6ce85f2a0c9,}},
    {.big.u64 = {0x12d524b998a17902, 0xf04fab11ca9bd409, 0x1768162d17d595a7, 0x092cfa945f907e77,}},
    {.big.u64 = {0x5799954e456611a6, 0x80300ea45cd278a9, 0x6edf0423f7c79be7, 0x03036966d8f89ce6,}},

    {.big.u64 = {0xa8265ec8e6f10c83, 0x8adb45afe7efb4ee, 0xcbaafe2c2f4dd48b, 0x08a1a9da277a88aa,}},
    {.big.u64 = {0x956526e083a9824a, 0xa1223ca1d4a11f9d, 0xafee830a3f4becc0, 0x05bea67ba403d513,}},
    {.big.u64 = {0xc6461b492ef7a262, 0xddae2f057426772c, 0xdf7d15a5e1fca523, 0x0d82d478e0c635b0,}},
    {.big.u64 = {0x7ab00ba2096ec291, 0x09bb6953ac55b841, 0xeaff2f15d9816b35, 0x0d5f9c88a334ad17,}},
    {.big.u64 = {0x97da35ab9b73ae17, 0x494608dcf9234260, 0xa10d56bfc1cf8011, 0x0614b8cec2f5c375,}},

    {.big.u64 = {0x8d2fd21cb62d5204, 0xb5c602ba2ea3e9e9, 0x33cb889da0022348, 0x02f744fa31f6162b,}},
    {.big.u64 = {0x4d231ed6f312a63a, 0x8e57dbdd00603d53, 0x13f6c3dbb3fb2e05, 0x0d6eb536d00015cf,}},
    {.big.u64 = {0xdbf131ffa1f352e5, 0xc7110a15fa608152, 0x13a3d9ec914dee54, 0x0eba0185f6f38e89,}},
    {.big.u64 = {0xf2e8045a6c7ca686, 0x2e0b2f20d2331143, 0x6b21aad9ac6d6580, 0x00fe63082868a2fc,}},
    {.big.u64 = {0xa60789902acece97, 0x762dd75c026e4148, 0xe651eb7d47ce249c, 0x00e0e1c71346d91b,}},

    {.big.u64 = {0xd89b2f50ee965e9b, 0xab62490d2e7d9561, 0xb4faba9c8c7cab2c, 0x026294199b38da09,}},
    {.big.u64 = {0x1508d4287de2442f, 0x264ae64f320a479b, 0xd192557efe510882, 0x011268c6dd3dc572,}},
    {.big.u64 = {0xd782d0af425a2c0e, 0x307eebd12931d9b2, 0x085ff0d1bd04bad5, 0x010024c5d330a1e7,}},
    {.big.u64 = {0xbf688e6a4d8a2e07, 0xbe8cbe265dd4ebd6, 0x512c59c81d19f571, 0x02a8ea3af3745f54,}},
    {.big.u64 = {0x4d02a9478590cd86, 0x9e536ca4434db3c2, 0xebb83a1259942e9c, 0x1076f6fe42d1a9d5,}},

    {.big.u64 = {0x1299d0c32fb1bcae, 0x8c27b990f3aab453, 0x9bd9689bea61be75, 0x06b9f9335dd29fc6,}},
    {.big.u64 = {0x7f64e95cc79e5175, 0x249139f0a2ce8825, 0x570abc043ed6eae8, 0x0b109341de652715,}},
    {.big.u64 = {0x74e2eddf0312e0ad, 0x58806c84a1de42dd, 0x2a3dce3daf88f3f4, 0x1050f4085d4df22f,}},
    {.big.u64 = {0x898a4c1f25d419a4, 0x7b613cd515398fcc, 0x5d2280ffdf58cce4, 0x0e09b509b50d2e68,}},
    {.big.u64 = {0x128bc6f92cc0870e, 0x27b047fa0b6800a3, 0x6c80f4bfebb35804, 0x114b4bd63b7258b6,}},

    {.big.u64 = {0xe8e8c6430b902d48, 0xff1c0f40d1163fdf, 0xf1ad609231888476, 0x0e97d0b87a126466,}},
    {.big.u64 = {0x48f0c346ae6dfcea, 0xb6c18de6421a3319, 0x8adc7eb9b075e6d3, 0x045fd0c3329d6c3a,}},
    {.big.u64 = {0x78c140b1e389362b, 0xb4104def11d44981, 0x426653664e573024, 0x0bf60395946cbd3d,}},
    {.big.u64 = {0xdb02df52a29d9192, 0x4987bc95d2d1a447, 0xbad550d28a11278b, 0x09c231bba8190693,}},
    {.big.u64 = {0x5d7d24abda2bbac0, 0x2514d65cac29fef0, 0x2b70abfba3b94316, 0x100c7fb410bf792c,}},

    {.big.u64 = {0xed01ef1a384d386e, 0xb37eb6abf0ad43ea, 0x6a9a84774878ac3e, 0x0f610861dea68692,}},
    {.big.u64 = {0x316e107cbbbe875e, 0x88bcdc7722f5b164, 0x73c9295fcf9ad31f, 0x0f0d9387e64d3c42,}},
    {.big.u64 = {0xcff86cac5d7030a6, 0x4adb98220eff6926, 0x43a5e4fd246bbb54, 0x077bba3b66e5d01d,}},
    {.big.u64 = {0x0f363a2c549aaf4c, 0x284e6db304956aea, 0x53e3540d8af76106, 0x04daebe999da21e3,}},
    {.big.u64 = {0x1f5c654a34db2c92, 0x493a05e12dbcefdd, 0xbfb3e642bef6267a, 0x0a6a39dfc688bd91,}},

    {.big.u64 = {0x78ff280e79e30945, 0x1587115963137fe0, 0x5656f8e128f5b73f, 0x0638cb3b46cf12c0,}},
    {.big.u64 = {0x242d861623367732, 0xf6d5e94ee280555f, 0xd5c79835035b2ed3, 0x0dfaed7ad427b8ef,}},
    {.big.u64 = {0x3fe87c3f86695156, 0x82961eb972ea1466, 0x8c2c3583560ab09a, 0x00ab154dda0cd21b,}},
    {.big.u64 = {0x4caeeca3d9ba0726, 0x1b15111c4457a0b6, 0xbeb63842406c735c, 0x0408555969ee39aa,}},
    {.big.u64 = {0xc4c54ad863ce1c57, 0xbdd25fa4f1ee5894, 0xbf55788b3847f169, 0x11789570cdd04ada,}},

    {.big.u64 = {0x380d6db568f77f5f, 0x8f78b2d87eda69a5, 0xd0e882e89b731612, 0x110487262edc3cf8,}},
    {.big.u64 = {0x61f3573f07416fac, 0x2ba9fd5ff1272882, 0x46dac88926ddf356, 0x0e1231540a489e0c,}},
    {.big.u64 = {0xae86bed43d5fcdb3, 0x65e55f6fdb7f47ff, 0x3f7ecabd02040fbc, 0x11bbd2f6433688af,}},
    {.big.u64 = {0xfc39133de865a40b, 0x3669c400ecf608af, 0xbd13a8fdbfdef917, 0x0b7dfe74ece0f224,}},
    {.big.u64 = {0x656a5db9ab9103db, 0xe90f85dd6c16beae, 0xd879eb52bccbf9c0, 0x004b2b4086bc7222,}},

    {.big.u64 = {0x42c36c90b21930bb, 0x6ac6f2bb382436f5, 0xc963d8f5d8822fd6, 0x128bd86e756ce9cf,}},
    {.big.u64 = {0x4078ada5c50a442f, 0x4a422fe00fa8d3b1, 0xbcb731eaf6ceed76, 0x11aa9e44fb2b5c1c,}},
    {.big.u64 = {0x13363fc3bb98378f, 0xae9d5633cf029b0a, 0x00c114dba7435624, 0x0aeb63b41468ee90,}},
    {.big.u64 = {0xdb0561c351d6e2de, 0x5d597fa942b6c8dd, 0x08fd516123efb1b7, 0x015fe557f5c1a709,}},
    {.big.u64 = {0xff5e9f023346d59b, 0xca9befbc2edad620, 0x8a63b40f2f217052, 0x00f98b27de9920a2,}},

    {.big.u64 = {0xde9af9978c5241f7, 0xc9e0473e54244f68, 0x589bfa3ce39d556e, 0x07feafb67dbd45e6,}},
    {.big.u64 = {0xbfde5917e92826cc, 0xd430229ef05761b7, 0xb2890e99ae7a1a4c, 0x0384eaf87db9a8d3,}},
    {.big.u64 = {0xd01c5b11f8621284, 0x94b95ee8e84fc9e2, 0x317a5a5b517cda9e, 0x1280d42d1c5778f0,}},
    {.big.u64 = {0x3a6a753c29b95a0e, 0x78208c32bb1178e3, 0x044d2d24a360e6c7, 0x03819b1372e208dd,}},
    {.big.u64 = {0xabdb0892036b1e09, 0xf0abdb453318165a, 0xc04e3f5fd0ee8a3d, 0x121705838a86935a,}},

    {.big.u64 = {0x276b2c3c3dd3e19a, 0xb53234564a7defe2, 0xa1d7cccceff0b2e4, 0x0519780f267659b8,}},
    {.big.u64 = {0x21560b6827899154, 0x6c6eb1ec2caaa856, 0x30f6ee8ced76e57f, 0x12600045de17f916,}},
    {.big.u64 = {0xd63100dbaee85415, 0x90ae0cf52556a20f, 0xbe5c704c9bebb53a, 0x08b45688d081556e,}},
    {.big.u64 = {0xb85c89704e1a4699, 0x6de380c5331a2fd7, 0x23d5916e45e61d29, 0x0cb375ddab9fec4e,}},
    {.big.u64 = {0x6f7b0bc222c60206, 0x9b3786b4b42c9388, 0xae80c61ec4a38d48, 0x0a2adfeaff491ad7,}},

    {.big.u64 = {0xd1085b331108584d, 0x97d339295343d45f, 0x15892b5aef1a7703, 0x068105652ebc3849,}},
    {.big.u64 = {0x50a6304cd7c11586, 0x5d96dc42517e3a07, 0xf5a560bd0343a602, 0x04565d8ba6dea95c,}},
    {.big.u64 = {0x782a490c17303181, 0x2b6458e617518440, 0x21b62c5244ac5c09, 0x0ff234a44a39373a,}},
    {.big.u64 = {0xf13c259dbf2a0473, 0x6ce1d4888e1e0479, 0xb5e1a9094e3097b3, 0x0ad9356a333183db,}},
    {.big.u64 = {0x986ef649ab644550, 0x0d8dea2ef3d1a2b4, 0x8f5cbba58460e252, 0x03544ec0b82fae0e,}},

    {.big.u64 = {0xb9732b249337513d, 0x6c62d0a90ca1a8be, 0x2f3045e8088ed437, 0x10ad45388be4cc95,}},
    {.big.u64 = {0x2a3ecda70e7ea33d, 0xe2343860d067b2c9, 0x09d90a6ba0d940b7, 0x1027578a2a9a7ec0,}},
    {.big.u64 = {0x4b44d51cdbec611e, 0x0c02672aaf3aa0a4, 0xce2e107ff5bf45d2, 0x095e5628dd672e52,}},
    {.big.u64 = {0x5b6c65807ed07a24, 0x8cc9efb055b8a809, 0xf1a087e288ecc998, 0x04513a3aacc413e5,}},
    {.big.u64 = {0x9087f58f97355058, 0x32af21f9b119da52, 0x53bfd9300f04a136, 0x0e4d6b9eb07eafcf,}},

    {.big.u64 = {0x64809306615b7b06, 0x9e1858fa7e16da34, 0xabff74f8ec1b0a15, 0x0449513498871599,}},
    {.big.u64 = {0x6b9d654c6a690663, 0xd9aa5dee7c304339, 0xe897952b121561c9, 0x0102bb91132e9859,}},
    {.big.u64 = {0xbbc512cb89498a9a, 0x2a45576e6c256300, 0x6e89fe9b7694f804, 0x093aaaecafbccf76,}},
    {.big.u64 = {0x6dcb42f92f5c0904, 0xe455214f738202d6, 0x2a6520fac52447fe, 0x036c7411034fe04b,}},
    {.big.u64 = {0xc55f6c57b49b0fcb, 0x1db20b350069a738, 0x94b2367d412e9e66, 0x0858b5ad6de3d9a5,}},

    {.big.u64 = {0x7789e62c71985cf2, 0xfa4ddca5e9dd9a9b, 0x42829fa83fcd075a, 0x10eebe8dbb4186cc,}},
    {.big.u64 = {0x43c3211015add090, 0x25a2e447003b4c21, 0x01ef903ca0625c2a, 0x0a8b56a026290280,}},
    {.big.u64 = {0x40f0bf601aba7773, 0x1ea6d782df09119b, 0x649f1a6ce33adb29, 0x12608155f13bf144,}},
    {.big.u64 = {0xde2d8d9446602158, 0x5217e6ce01d8a258, 0xd895e6beef8ae623, 0x002da6dd9b5e88fd,}},
    {.big.u64 = {0xe3645a9003fdc222, 0xf09ffcdcea9a0eff, 0x5310ab42b9068235, 0x0d6a501b6d72ecf4,}},

    {.big.u64 = {0x3deb0ed2b28ec2d8, 0x5b89eb6e6cabce7d, 0xfbda2c1aab050a97, 0x086aea3ac2145b94,}},
    {.big.u64 = {0xa12b9b4259a738da, 0x445e3db34e7f0e08, 0xd284767dd7f59220, 0x00c2c7974e05364f,}},
    {.big.u64 = {0xfa9b191b5ab58802, 0x68571c25cf5a6587, 0xec10f287aa1d19ae, 0x07f6ea8da886ec05,}},
    {.big.u64 = {0x82d526d105227aa3, 0x14354ca7f7fe5485, 0x2cf46cece7e8a9e9, 0x079eb08b4781465e,}},
    {.big.u64 = {0xef2bd27bd63cec2a, 0xe3684cc96911dfe2, 0x5ee736b49ecf411d, 0x08b791ac931f5995,}},

    {.big.u64 = {0xdb2863eb26202d85, 0x3a5ef8834a9affb5, 0xdf480bade7f98823, 0x10d086817eb596c0,}},
    {.big.u64 = {0xf513d0cfaa2aad81, 0x6cd929a0ea6ca1f8, 0xeb9a74907f256902, 0x095b43d8fb50a430,}},
    {.big.u64 = {0xa7195caa864bf87e, 0x5fae28c5396fecc8, 0x01b96f27a2e85d0d, 0x11776ff9bbc79564,}},
    {.big.u64 = {0x9986c01d2d8e48c2, 0x601803dabaf5c486, 0xd2dca46de51c71cb, 0x11ce371da44a6068,}},
    {.big.u64 = {0x9ee257eeabe8bc1c, 0x81b56b88b54712c9, 0x13cdc095fb4a3db7, 0x1115c977a6979c8c,}},

    {.big.u64 = {0xb0a9a8cfad579055, 0x7c8835a1aa770573, 0x0637a89c5a445552, 0x006658641187ec07,}},
    {.big.u64 = {0xb220d1b3553af7ae, 0xb56d99ce11ab4c19, 0x6a96ed8f19b96e92, 0x123354b6348c89b7,}},
    {.big.u64 = {0x064abdb927f37c1c, 0xa892dd98b0c838ad, 0xa25abbc04dea8d6c, 0x092c870a499ed2fe,}},
    {.big.u64 = {0xb86e529d6b164570, 0xe81e1a580647e2cc, 0x48312bce7f7a513f, 0x093653080d45cd92,}},
    {.big.u64 = {0x8517575c6b18774a, 0x8abcd4390557e245, 0x09bb2e43571e6b69, 0x0edef9abcd4ea40e,}},

};

const field_t mds_rate_4[MDS_RATE_4_SIZE]={
    {.big.u64 = {0x129d6d7d1cc5fcb4, 0x1145e5509561cecb, 0x4e629f7cb462339e, 0x0dd2bd5fef796a36,}},
    {.big.u64 = {0xe36af2942c2ebc1c, 0xd362c226962eaae9, 0xac22db0fa407eaed, 0x11cc78593acf08a0,}},
    {.big.u64 = {0x88fbf0b649ef6e17, 0x56090c7aa12854bd, 0x23463285ff405e9c, 0x0c4a501067fd8396,}},
    {.big.u64 = {0xec1e908736f3fa32, 0x8973d9f0da481cfa, 0x15589e4018d83a41, 0x05e5979d71acb269,}},
    {.big.u64 = {0xf97603086c5e7f3c, 0x5332d13d00579cbd, 0xac3128d6d827532b, 0x0b620c983e995129,}},

    {.big.u64 = {0x85dfecc5bf31fba8, 0x7246021413774b15, 0x7f0f7fa389237614, 0x00d92aa1c6b54979,}},
    {.big.u64 = {0x8a64d8dd952bc3ab, 0x5fcf9ce67589f31d, 0x32404ea89150a17c, 0x08b7dbcc25ecc7a4,}},
    {.big.u64 = {0x7df7495995f15e3b, 0xe75b75be2862b48c, 0xea8225a012986253, 0x0a0cd4dcb440e416,}},
    {.big.u64 = {0xcb0cead5a20db973, 0x88f4d85880202dab, 0x5102163a6dbad1f6, 0x0c03574ecbf95ca9,}},
    {.big.u64 = {0x2e0c8052d9f9795a, 0x56d92804cefdf567, 0x9549c7653d0da000, 0x0d385ab83935bf7a,}},

    {.big.u64 = {0x45526e24c0a806a7, 0xfa80fd186caa0f4f, 0x1d243cd68f7e3255, 0x09ba56ff8a955021,}},
    {.big.u64 = {0x0db4bcc7aed25d1b, 0xeba3dfaa6fa07fd0, 0x6c54ef136f5a02be, 0x0875a92b82d4858f,}},
    {.big.u64 = {0x74a9a40bdab2bdd5, 0x721224c4b9ff6c34, 0xf78cf573836345ba, 0x0cc753802bd32522,}},
    {.big.u64 = {0x18eee7badb695e22, 0x9431cd0644342165, 0x6896249cdd8671fb, 0x02d7432d8be840d5,}},
    {.big.u64 = {0xac6309126ebb04db, 0xde7d2a8310c77358, 0xe410e6303b5648b9, 0x0652ae9f55129873,}},

    {.big.u64 = {0x4fafd74cc4c97736, 0x665a55f3a17a7761, 0x49e3bcd21f99666f, 0x082986d19f468ddd,}},
    {.big.u64 = {0x471da1e64f4a6302, 0xdf9eb918fd69eb41, 0x01d364ec2f5a9ed3, 0x1022a290908c57f0,}},
    {.big.u64 = {0x12e463771ac88c0b, 0x20ec679b6849a9ad, 0xc1a53f18d45bfcbc, 0x0a1a97836aa33944,}},
    {.big.u64 = {0x87782a091cedb75a, 0xb9e54366a990e29e, 0x21beffb0046a7408, 0x116eca7765190cdc,}},
    {.big.u64 = {0x51e753af6a13a2b9, 0xe27a1bb5e81ba686, 0x0371dc61379f6cbc, 0x00d5f588f2c4e861,}},

    {.big.u64 = {0xab8c5132a2e1751f, 0xdfdee98d985b930a, 0x93a1aa655751698b, 0x02f088261c2e5a74,}},
    {.big.u64 = {0x81c3f0233d3de76d, 0xc2186fa7e83de988, 0xf9d8d6159302f65f, 0x0f21d8d618cececa,}},
    {.big.u64 = {0xfde0238537d11331, 0x52c0f497f2d255f2, 0xf2a50629c87f5656, 0x0cd886e4975b02a2,}},
    {.big.u64 = {0xee17d462d64f3465, 0x646c6895c2c977ae, 0x76593027d7587229, 0x0725ff3232434ece,}},
    {.big.u64 = {0x067b04ecddc55d7d, 0xfe8ff0359ca9ed39, 0xce33b9d427a37b8a, 0x1186e1ed389df5b1,}},

};

const field_t ark_rate_8[ARK_RATE_8_SIZE] = {
    {.big.u64 = {0x971775a2bdf40803, 0x0d38e7d60de22dd4, 0x8a18a0e2806bed3c, 0x116bcacaccf56784,}},
    {.big.u64 = {0xcf7ee7d6ff9c266c, 0x5e0fade0afe0d233, 0xf9171a2d04f8da1d, 0x12655f2559cb4378,}},
    {.big.u64 = {0xa33477761ee894e8, 0x8472ac18eda3e34a, 0x9efc357cd1687ac4, 0x09e42e07c1ea762e,}},
    {.big.u64 = {0x38ab2034796fd0fb, 0xb3c0d138762c3221, 0x7659e068ee779c8e, 0x0935ce2c29881ada,}},
    {.big.u64 = {0x81b84197e0301f09, 0x48b27d4d20304353, 0x5ec2c672fbe75ac8, 0x070227d9eececc34,}},
    {.big.u64 = {0x40cdd495c5bd3ba9, 0x04cfebce1f78de96, 0x7611a496275f6966, 0x0791611be69aa1a3,}},
    {.big.u64 = {0x1669c35a1b2af770, 0x53224f5e9b585be7, 0xdb69f15dbfaa6868, 0x03b7259cc4195dce,}},
    {.big.u64 = {0xbefbe16974aa538e, 0xdc1c3d950b492c47, 0x2945c801df7264ab, 0x06880da2e28efd73,}},
    {.big.u64 = {0xf7e40ba6a8d77e57, 0x7327958e2a8795ca, 0x712289ba59184337, 0x06c68475207c59cf,}},

    {.big.u64 = {0x55c0de02de4acbd3, 0xcb509de8001f15cb, 0xb5a0818df06d4016, 0x07eb1804280e9e3d,}},
    {.big.u64 = {0x418e9707769157fa, 0xef765a9a0f60534b, 0xa2daec6819214ac7, 0x02899a4f5ba14fce,}},
    {.big.u64 = {0x0b2f3bca0c754556, 0x1d794b80b25ef730, 0xe03b64a8507f8fda, 0x0042520038c16fe3,}},
    {.big.u64 = {0x2c3d714b9cc9e2fa, 0x63153d1f5e35831d, 0x538746a9cdabfbfb, 0x073cdebaff1f32f2,}},
    {.big.u64 = {0xf2dd17e260fbe67e, 0x5a4c693504a86880, 0x0683b5e4d2650b1e, 0x0221c99b0f7a0923,}},
    {.big.u64 = {0x7fb1835ba1f0e288, 0xfe64d398f00a3404, 0x4f1488c829a09a03, 0x01f99afd96c75cad,}},
    {.big.u64 = {0xaebbf0c5e6394805, 0x47a6a21f60a059cc, 0xbc552572f76d37ad, 0x06e8c85b86cf19dd,}},
    {.big.u64 = {0x7aada7078f688382, 0xfcc7941f6d0518ba, 0x698ac551a0c38c07, 0x123e58383b4a9640,}},
    {.big.u64 = {0x7d3d1eddc6586b65, 0x2a659615143e304f, 0xbe9bb7e3510459ad, 0x04f1d50f3766697d,}},

    {.big.u64 = {0x8fff28993e9187ea, 0xb54273d0ec887ed7, 0xa117606763349902, 0x02c0a78081b10ec1,}},
    {.big.u64 = {0x1bd97226fc1c4fb3, 0xe094ababa2bed992, 0x64d9a482f2f08f5c, 0x0bf8c834b2aa2870,}},
    {.big.u64 = {0xf65a9500bcad60bd, 0x1a71974c467bfe80, 0x06138b119bcef1e2, 0x126ed30c79ba132a,}},
    {.big.u64 = {0x7de6e5b9c50d6eb0, 0x6370da3f5c78eca4, 0xce6046a4b1caf3c6, 0x097f3367a16a78f4,}},
    {.big.u64 = {0xfe87d882bb1db1e0, 0x5f97f9d4b493b403, 0xbfe3f45ae42c973e, 0x083568dcf9788b01,}},
    {.big.u64 = {0x34f81d6c5728e861, 0xf1d741c296daa473, 0x95418662f0600fc0, 0x017588d591b4ff7f,}},
    {.big.u64 = {0x56983e673f5cec9d, 0xb32caeac07b8ace1, 0x79eac56b1cbd44c2, 0x12290d8ade2ac2c3,}},
    {.big.u64 = {0x2437620e878b3d73, 0x12aed77570b73cda, 0x5c3571c74d7dbc25, 0x0a05b2dc69217fda,}},
    {.big.u64 = {0xe2b2dc9ee1e3d4dc, 0xeb663976b89105d1, 0x518b788e96ffbb42, 0x00eaa7e231594940,}},

    {.big.u64 = {0x014193b470a6fd0d, 0x8a2227c291ba8730, 0x81f894d6c3ea94d6, 0x0a3d0f070dc426b8,}},
    {.big.u64 = {0x0bc8a1dde9786fd5, 0xc92ca01acf5bcaf9, 0xcbb22d6f9ea413b1, 0x0fce976471c2276c,}},
    {.big.u64 = {0x067be44d84e4bae4, 0x5add88e03e30f1ce, 0x74444de9220a9678, 0x087081966d94d0a5,}},
    {.big.u64 = {0xc98e72c130065ac5, 0x3fe7696dfb13d887, 0xd88653ebcef91af3, 0x0ff8f0812a2eaa67,}},
    {.big.u64 = {0xf60914c5d75304ab, 0xb357c6114e172abb, 0x804c561dc8deb468, 0x00a08da852595994,}},
    {.big.u64 = {0xb516bed1ed3439a2, 0x643d529b72524b58, 0x0270a8281155df1a, 0x0a55fb4475920b37,}},
    {.big.u64 = {0x93ff0b2b8dc974fd, 0xcd45ca313af0d5b3, 0xb1f9283c017748b9, 0x04cd5c335acac90a,}},
    {.big.u64 = {0x02da00a53acbcea1, 0x46e8b9bbf28a8ee2, 0x3c21cc6d30604bb0, 0x0bb56ce74ce4334c,}},
    {.big.u64 = {0x30ab252db00e42a7, 0x9e5544a39cc0d4c1, 0x88f8cebf2047dc98, 0x0957f8da33d063a8,}},

    {.big.u64 = {0xb37c86eb2614550f, 0xbe84d1daca2a388d, 0x437fb3712dbf3ace, 0x05af61753dbe9c3d,}},
    {.big.u64 = {0xb396c64c3f72b5ad, 0x0c118b5f501cc07a, 0x34b306219fa04e27, 0x04b3223778aa137a,}},
    {.big.u64 = {0xeaa05779474cf616, 0xb40100c2b18a31c8, 0x55443a0648c36f5c, 0x001531c839a51dcb,}},
    {.big.u64 = {0x85c1f3c3248a6476, 0xa1a568e082676664, 0x12003f59417da971, 0x0673cac395f1136b,}},
    {.big.u64 = {0x008d30e9e2cfcaea, 0x336a26e5bfd87665, 0x4827c945b0927a25, 0x11afd11596889561,}},
    {.big.u64 = {0x76d28a6b4bea929d, 0xda7842f150d3044f, 0xcd8570cf1c3d0f0b, 0x07e2de8f06b27e86,}},
    {.big.u64 = {0x1ac2586bc1d4935e, 0x8f8d50dfb3b8cd48, 0xe3d6fed97ae1b928, 0x01c453a98af9282b,}},
    {.big.u64 = {0xb780d785d6bd8313, 0x8f3bc6438bb90a65, 0x137d805b813d7e18, 0x09f16bbc9804ce5d,}},
    {.big.u64 = {0xbd1c702650569cac, 0xf6e63d6130d00270, 0x106e736465cea669, 0x0c0fbfa6f24e0983,}},

    {.big.u64 = {0x43c4e574389aacbb, 0xb69f739b1855fb3d, 0x5e757c5156d6b24d, 0x003ef7f9c0663db2,}},
    {.big.u64 = {0xfc26a6e1f3be5ed3, 0x6f913690680a213c, 0xd4da068c6bb08250, 0x0790e25690a87059,}},
    {.big.u64 = {0x5b12e8222f8ac422, 0x4aba8d346923b0db, 0x81ed36992bc97ceb, 0x11c08b54076766ea,}},
    {.big.u64 = {0x92cf0303463106dc, 0xce5dc2af8c3c44e5, 0x191354a3399569cf, 0x0f9f10b348dd0dda,}},
    {.big.u64 = {0x0f69bf2c7b25b4ce, 0x29e125a07f657774, 0x50bc84a2abf3723d, 0x1112c5d278a73484,}},
    {.big.u64 = {0xd1f054d1c9e1117e, 0x1ba1737c73d67bf0, 0x4ee56bb59e1b703c, 0x050c1e2464eccd11,}},
    {.big.u64 = {0x2fc7886e09167717, 0x05f8d3d04a10471f, 0xbd4c03654255aa31, 0x10e33b5248f0ec7e,}},
    {.big.u64 = {0xab726648ac2b3d92, 0x3395b862e6e66ca6, 0xe8b1134a64bd1fd6, 0x0b38119358e995de,}},
    {.big.u64 = {0xf19f1a58703818f6, 0x3602ffda7cc93206, 0x406fbff9e2021feb, 0x01948eef17707d75,}},

    {.big.u64 = {0xfdaf38b53e116f05, 0x3238cf05ccac9da8, 0xbb51a14e6fd1ef4a, 0x0d9da5bd3c569ac5,}},
    {.big.u64 = {0x2fed9bca13746539, 0xf130a65698beb4f2, 0xe8101e33f73ae9c3, 0x04dbbd65cf07f29c,}},
    {.big.u64 = {0x437c8d62e1034971, 0x18872167960f3350, 0x35d9ffbc0dc9310a, 0x0d2526ede84f0dc0,}},
    {.big.u64 = {0x887c4b3ddbcf6eb3, 0x905d78d865854d27, 0x5898801ce6dc2682, 0x11bfa8f417f727b8,}},
    {.big.u64 = {0x0620deaf2e977a29, 0x3d302701261a3836, 0x610c118c63377f35, 0x0317150a68bd512e,}},
    {.big.u64 = {0xac9820df46db318e, 0xb179311bff7ef6f8, 0x6d59115df389bcb4, 0x091a32b631bc55d5,}},
    {.big.u64 = {0xf22f08844abb45d0, 0x374583b74187fea1, 0x5438bcbe3aa4b30c, 0x06fc1106f07c92f8,}},
    {.big.u64 = {0xbc3f23b87c48c019, 0xf22a4ec476a02bf8, 0x6891f93edb1922c1, 0x06d78cf0307fdd1f,}},
    {.big.u64 = {0x943a70e922bb73d3, 0xf2ef655be129e00f, 0xcff0461fd59c3638, 0x0f3e478906d5a969,}},

    {.big.u64 = {0x2ec4d6faf10204eb, 0x3880109d04cca8e2, 0x3fd282032b4b2ff6, 0x032fa7ac754c35c1,}},
    {.big.u64 = {0x4b33d7d6c99b6115, 0x7fcf877b648f1737, 0xf1e9b89b803f1c6b, 0x0e408252314de0b7,}},
    {.big.u64 = {0x928eb7a9705870ef, 0x6b688e4b51d62020, 0xf48f7d1ee6b3f46b, 0x096dae400926bd3d,}},
    {.big.u64 = {0xbd86f59381e8d269, 0xff4215dcd9dcdd5c, 0xb8586aac3dc5db3c, 0x0dbb592c1a269ee1,}},
    {.big.u64 = {0xb107327e21e4ef79, 0xf3ee74d809017954, 0x16ee97cdb54aa99b, 0x087444fdef6dc449,}},
    {.big.u64 = {0x89d74857c38d5757, 0x41344bc1cc4d8976, 0x653903e1c9060e84, 0x07058ae4b531245d,}},
    {.big.u64 = {0x0f1726fa3627862f, 0x849dcecebbf7417b, 0x032a0c98ceb497a0, 0x0788fa27d2777c5b,}},
    {.big.u64 = {0xbc6200813bf25325, 0x099ac941366bb6db, 0x5728a417ab828ac0, 0x0e9831558102821b,}},
    {.big.u64 = {0x331c90fb5a4c692b, 0x6a34e93603f49ebc, 0x9d2ee1ae4f118d2b, 0x098842de8d7171c2,}},

    {.big.u64 = {0xd1b7fc5b9e32c186, 0x66311d2bb7aa27ae, 0x7426d107d42fded6, 0x0f0b314a085cc03d,}},
    {.big.u64 = {0xf1610563a1614149, 0x6d0cc64dd65b50e8, 0x49d96a31f0444c1d, 0x0d52c1518efff214,}},
    {.big.u64 = {0x4e513a0136f2ff29, 0xd918491e91f27bf5, 0xbafa1d3009c10a86, 0x03842cdcdc747db1,}},
    {.big.u64 = {0x082074e629cd0d33, 0x266cba3a6fe9726c, 0x66b95d8bec469d35, 0x0062c10c729fd2cf,}},
    {.big.u64 = {0xa09b3865da08b911, 0xa3e84d9ed651a554, 0x2c606eb6ed7e70e1, 0x04479462f0f2630d,}},
    {.big.u64 = {0x04538016d68a8bef, 0x757a08c19595fa1c, 0x89110e8993699ccb, 0x06f74b53988147f5,}},
    {.big.u64 = {0x2b44ca8336b73a8c, 0x588442f41fca2149, 0xb338f3d71e549b07, 0x117af216fe26ba25,}},
    {.big.u64 = {0x9ef845262d7393a4, 0xba918d028a4f629d, 0xbf9847b5a941cec7, 0x025a13fffd06ad4a,}},
    {.big.u64 = {0xf938a1f00d729fe7, 0xe040b54cd596a9d6, 0x8843c41d76d261ca, 0x06ffe5654ac0c34a,}},

    {.big.u64 = {0x8bae36f2d9d55c3a, 0x2857eeeedabc6284, 0xf7062155f61ac0c5, 0x10bc7308b0ba47eb,}},
    {.big.u64 = {0x73cc42f8a2ff873b, 0xd1298b07cbdf93da, 0x0b5e6d80b91209cd, 0x06b0206f3f7852e0,}},
    {.big.u64 = {0xb57632f812a77119, 0xf15571a4497d784a, 0x09e0a1c45a6c203b, 0x080ab8a795415f3a,}},
    {.big.u64 = {0x9944a368e3cc066d, 0xa7000423fcdfc125, 0x238d70eeb34fc69b, 0x0953506273b3eb5b,}},
    {.big.u64 = {0x6e05879f322db350, 0x57e31f00e6bf560f, 0x67f75edd050aa530, 0x0accb7992979f716,}},
    {.big.u64 = {0xd1e0f70e9005ffe5, 0x1fd544a87b6af9a3, 0xde2e7ed6eabecc90, 0x120f5becf1e43cac,}},
    {.big.u64 = {0x371999b365d23746, 0x996e1fc287b2dbdf, 0xd7240f7e09d45315, 0x0f67ac824674dccf,}},
    {.big.u64 = {0x6889847178fa7957, 0x4b00856b9c075780, 0x9d45ec9b4169c008, 0x04d3dcbc8e158160,}},
    {.big.u64 = {0x9cfd3eabeefc9864, 0x049008654a540e55, 0x380861228f05e334, 0x0f92b3e3f395e41d,}},

    {.big.u64 = {0xfd1204f009d14ec1, 0x38bec5fb9e484c35, 0xf41fd88bf9c668a5, 0x076633629a7f8280,}},
    {.big.u64 = {0xaa6a33a234be3c1a, 0x60cf884908df95bd, 0xa859112c619273d3, 0x113c7b7318982d0f,}},
    {.big.u64 = {0xfc58a5dc3e1124ed, 0x881256b24d0529fd, 0x060f02f8ef220a65, 0x0326d14b9ff707ac,}},
    {.big.u64 = {0x8cc164b925f192d9, 0x952fd3d47971df84, 0x7db4698142abc5ac, 0x11756a58085d775d,}},
    {.big.u64 = {0x7372eac28216ebfc, 0xb956a1e1d0af0819, 0x87e3d2b50db72fbc, 0x0ae53b9ac2b8f8c6,}},
    {.big.u64 = {0xa585a21f295c49cf, 0xf544efdea2be1fd3, 0xd8a6baa80c9a9b32, 0x021120e71cebdd09,}},
    {.big.u64 = {0xb23466faef2964ca, 0x120af47dacda4aad, 0x9f51bb40f3345ac7, 0x021d8ead48935f49,}},
    {.big.u64 = {0x5c3c651f6b6b2b5f, 0x3bacade06531d6bd, 0xbef8ab8df26b9d4c, 0x06ebae213a284341,}},
    {.big.u64 = {0x2df5e9e1e78c2b97, 0xbb54a95b82c9094f, 0x761c3e8e94b2612b, 0x01ccbd098a13d5bb,}},

    {.big.u64 = {0x66f865a3c535d656, 0x7c152f0a92673645, 0x229c1bc299924524, 0x0dd14bfb905ae7e6,}},
    {.big.u64 = {0x42316068322324a7, 0xfa7f32aaa1179d84, 0x248f1a9e63e255cf, 0x0effad93a25dd9e2,}},
    {.big.u64 = {0x9a0892040c07c99c, 0xc50b1f7f031645e8, 0xe49bf65e526df6ee, 0x0b870e0a4f9fcfa1,}},
    {.big.u64 = {0x03735b6aa4da43fd, 0xf53c480bae3e074f, 0x482ad9da532b0e11, 0x015637e8e26b099f,}},
    {.big.u64 = {0xba01d3a07b9e749a, 0xf3133ab1594714a0, 0x35770984263d1a27, 0x08d9d2b98321101d,}},
    {.big.u64 = {0x1fbbbffe0f443429, 0x8a7cd99c977fb746, 0x787a188c7c471328, 0x0fc55b0fb097e35e,}},
    {.big.u64 = {0x1a39c2f6548661b1, 0x146c9e095bb0cc10, 0x62d2d5f83d8ad2b5, 0x071ebf1fce0c974e,}},
    {.big.u64 = {0xb6a6bc17b0cf6e6c, 0x57b39a470dbdec58, 0x1395ca9de7438d6f, 0x0edcf2baabc1dfbe,}},
    {.big.u64 = {0x438fef257ebeecfb, 0x8b8f30cd7b19f006, 0x014f45165920b228, 0x1294dc5a1390fbe3,}},

    {.big.u64 = {0xf403245503121cc3, 0x3423e6478694646f, 0x270385b63b541ea9, 0x0182d1a259c6de3e,}},
    {.big.u64 = {0x976b74f600ff5739, 0x99dd927e81550483, 0x03f530e0a99158d2, 0x0356462efab6e5ee,}},
    {.big.u64 = {0xfe3f500ed54ac121, 0x8335386d4f428de5, 0x2e019d7e67173522, 0x013d5825740b2b4c,}},
    {.big.u64 = {0x263fd5f8ecfe5e11, 0xecec606b45a86093, 0x92a23dd18f584d34, 0x071d6dedb8316177,}},
    {.big.u64 = {0xae68c9e54c586b37, 0x1ac7b6f40d785732, 0x2a389643189687f9, 0x0aaa5c2f3f9d5744,}},
    {.big.u64 = {0xe55bb55c3a5204ca, 0x68b6bfcf805ab7d4, 0xde6526718d565c98, 0x0a234209536506f7,}},
    {.big.u64 = {0x08cd42e2a923cb4f, 0xd8b22a3c9173069a, 0xcb1aa800370d0a5b, 0x0a41a5dc175925b3,}},
    {.big.u64 = {0x9c53bf6b1ccd1d32, 0x59782f3a78736eac, 0xa10c8039e344bf23, 0x07bee0acc461cef0,}},
    {.big.u64 = {0xb2af3d6c40e5f26c, 0xc955b3b664359677, 0x5949d08360b9e2c0, 0x0071ee24c46a96a3,}},

    {.big.u64 = {0x7b7daecaa19c4b64, 0x34f95b9044ba87e6, 0x0bc0e9802edaaed5, 0x03873c3f1bc5ed89,}},
    {.big.u64 = {0x8977f4b25920152a, 0xc1471ba44ac454ca, 0xf944d5c2df0709a7, 0x12311a2ae47c7af4,}},
    {.big.u64 = {0xaa90cffed5a0257c, 0xa4f3c6592201faeb, 0x4172cd935b1b3337, 0x0dceaf6c8dcdf7af,}},
    {.big.u64 = {0xb88409f94aed7f06, 0x01a8086ef4b9f4b3, 0xf45a42eaa56703d7, 0x04b9bc6ac4f239dd,}},
    {.big.u64 = {0x20c8542da91da97c, 0x64f9fd3a77063605, 0x334b6e698c1f17ca, 0x09644b481fb32ba6,}},
    {.big.u64 = {0x13dc8b3b4c6eba4e, 0x5122a1533f7895f1, 0xe69572e5b5d1b54f, 0x00c04450d11b8972,}},
    {.big.u64 = {0x3f4f141f310e6565, 0x6a7bdd1f903b8c78, 0x542ac6d446f9e11c, 0x01dbda62de31c7bc,}},
    {.big.u64 = {0x9d93dbc514ec92a2, 0xa2c1aca02dbe03d3, 0xd315db77dd29a962, 0x094edf6f74c2d9d2,}},
    {.big.u64 = {0x63e4433a92984aae, 0x0e0efd4f54394656, 0x88769744d566fe9d, 0x00dc91e137e83b03,}},

    {.big.u64 = {0xaf7e9cacb2759b11, 0x745d8e253114d781, 0xad6166ce9f438d52, 0x041035afe8249033,}},
    {.big.u64 = {0xa441ee0f9344ce99, 0xfb3ff657b713f912, 0x6bad46833d5e23bb, 0x0b8736e007394bb9,}},
    {.big.u64 = {0x27a1adaf9cf57540, 0x8dd416d1562e6605, 0xd49c815f0619998c, 0x0f5d7499e26f1778,}},
    {.big.u64 = {0x0566282bbda72d99, 0xdc1d15e4ce5de4e3, 0x8ba96a9dc7f0dc06, 0x0836082adf39d9f7,}},
    {.big.u64 = {0x112fd4ee26ba5ca1, 0xa94e3262e5d83547, 0xd8a0602090d538cf, 0x061b2e0818cd27b8,}},
    {.big.u64 = {0xeb7cec314cf4133f, 0x4c0d807b788f890e, 0x203906e049a01cf7, 0x04ef4b52ea8557a9,}},
    {.big.u64 = {0x34882915b51f831d, 0x60fe70f7d8690874, 0xaee2c6d002fc11ab, 0x0397463927199246,}},
    {.big.u64 = {0x03557f611be3c62a, 0x0e03c7bf987dfdea, 0xe1cc2dc2f1e1186a, 0x036756c0a6187ca2,}},
    {.big.u64 = {0xa574416568f917d2, 0x4c78c268a6923099, 0x6b5fd2f0032a64c7, 0x0098ea78bcee1ba6,}},

    {.big.u64 = {0x1074155c12eb2aeb, 0x67d397cceefbd34f, 0x9847a9b47dd9b332, 0x0e8b869869ce15a5,}},
    {.big.u64 = {0xbeae3ade0759d764, 0x74d6d6d234d36dfd, 0x5817fcf639e21f25, 0x0ce8fc51d58bcbae,}},
    {.big.u64 = {0x129eb3f2dc5ef23d, 0x2752d1afca65caeb, 0xd8c1669b10cb17fa, 0x11ef24705fd90c80,}},
    {.big.u64 = {0x37966173cc2c9dbf, 0x663661a280b6662c, 0xf6db6686a8c2d9ab, 0x0a1e1d357731b139,}},
    {.big.u64 = {0x9abdb76c89918142, 0x3df82dd3b24b40e1, 0xd7e963871d2d4ac0, 0x0d8b329956485532,}},
    {.big.u64 = {0x222f4ac5462b8ab7, 0xfd12c65608d8af5c, 0x2690b5230c3b64f8, 0x04106d5674f7d5c2,}},
    {.big.u64 = {0xe075a8950500608e, 0xcd711815a542dc51, 0x1d937353eb9d0d91, 0x08a57768a63e6e0b,}},
    {.big.u64 = {0xe8db46f4eac93337, 0x38ebfb0029f81684, 0xd73009af1f026371, 0x10aa3fd9e3d5b2ec,}},
    {.big.u64 = {0x8620d237621403b5, 0x76f47a1f2a810e40, 0x883d1d1ca2bf178b, 0x089a622ebd3a3f11,}},

    {.big.u64 = {0xeac724b50c772ae4, 0x1d516aa2e96e3fe4, 0x36e66bb3ea053d8a, 0x08caf9b3b6b88082,}},
    {.big.u64 = {0xda99114944de695a, 0xece6764ec14909e9, 0xa0719bb4f5e7259d, 0x0adc38c1f793c060,}},
    {.big.u64 = {0xf8d9281f8177ad49, 0xbc1d26e4f6199e80, 0xb1e9b3ae8fe8462a, 0x007d9b221eb980e4,}},
    {.big.u64 = {0x963abed135c5b03c, 0x451058a24df0de76, 0xf7117f3ff07dcc87, 0x0c9e82a336753143,}},
    {.big.u64 = {0xc8b5f2896b755a62, 0xccae828a2b84cfe5, 0x21de0bc3dda0ebe5, 0x0839fac7bc68ae76,}},
    {.big.u64 = {0x559d840b8f552c39, 0x76eae238f5db3a72, 0x2f5d7c2fbef16aea, 0x008380c6fad255d7,}},
    {.big.u64 = {0x5800c5f1b59242ce, 0x7c8f3fb93b0532f8, 0x3ab305436c600b6a, 0x06f4d1c67982bd34,}},
    {.big.u64 = {0x8d4aa63e9d984320, 0xb6af7792f6bb2132, 0x2b1a378c7d2c556e, 0x06772cb1bfc0592d,}},
    {.big.u64 = {0x753943eb0b601768, 0xd9be98b147bc4508, 0x616bc30c5b1306a8, 0x0e1fed7e619591a6,}},

    {.big.u64 = {0xe027d701bd1b4eed, 0x2628a1b36503350a, 0x4e2dbf126a23a873, 0x034fd7e7e2146393,}},
    {.big.u64 = {0xbe49770f51aa400d, 0xb657ce98ff694ccd, 0xdb0a953c9f938d4c, 0x10e0e950f42a654a,}},
    {.big.u64 = {0x085658ec652b432a, 0x69c6a49685dcac9e, 0x534488d1493d122d, 0x0c6b0cfb936c721a,}},
    {.big.u64 = {0xa79d709615734ee5, 0x07d489f14393f457, 0xc12647b024522354, 0x08832d7e27d04864,}},
    {.big.u64 = {0x37bd19dd488992cd, 0xef0d841afa0e513b, 0x3d65a7e5a682fafa, 0x01dee962462470ad,}},
    {.big.u64 = {0x860e1da0927aadf4, 0xef8ffe6d8710fa92, 0x05ee991044408b99, 0x08d00822146a29f6,}},
    {.big.u64 = {0xea27dd2b7fe8b561, 0x26bbe9b622d09b3a, 0x0e8b1ad714e69a0c, 0x0262ad687c36c1fb,}},
    {.big.u64 = {0x3dad5a1477f5649a, 0xd4a092911db6d09b, 0x8bab97f4beb4936d, 0x061be6a4fa0ee26e,}},
    {.big.u64 = {0x9546a9db91e83e09, 0x61cea8644b7d9eb9, 0x382c2d13f720be8e, 0x00fdba7a54aa10b3,}},

    {.big.u64 = {0x0e3c3fe33154ea90, 0x607e0b4f319df57b, 0xa199d84bf59e6f79, 0x0483c23e8dd9fa07,}},
    {.big.u64 = {0xcbe04145381c0abf, 0xbcb231ff45e2ccd0, 0x0f65bf8204ebcdd9, 0x0a7e8655cbd3f548,}},
    {.big.u64 = {0x0cbabb0b3b08dba0, 0x5c98142ba4fbb917, 0xbebadb2cf8ffc595, 0x00f59a1de9565c07,}},
    {.big.u64 = {0x896891b817abeec2, 0x544dbc78bbd2122a, 0x226799af201bcad8, 0x0ba52bd31220bd77,}},
    {.big.u64 = {0x7bb70e6527c593f7, 0xac20ae18e3467bcf, 0xf2797b5644570b2f, 0x0814461270db3c1a,}},
    {.big.u64 = {0x7e6378730694d380, 0x5fe1fa13dcc787a8, 0x48ea8952fe6df4a2, 0x02ec1564cac0225e,}},
    {.big.u64 = {0xfb8d0b8ba84e159d, 0x5d7ca8495e11bf90, 0x5c2e2398bb996331, 0x02eaa190c7e85ed4,}},
    {.big.u64 = {0x542a045ec031fe36, 0x5709d069e400ce2c, 0xcf65cda0de3cc78c, 0x0b44cd9f4e10aaf9,}},
    {.big.u64 = {0xf415e280d99e9323, 0x64070af4f45969ca, 0x3bd5cff30d0ddc6f, 0x10f6229f7f374b86,}},

    {.big.u64 = {0x4239db4a61e6d40e, 0x62f707234336af54, 0x6245dec45d953cf6, 0x0cb0a2adca887e87,}},
    {.big.u64 = {0xd846675ccb640dc5, 0x79249483dac22d86, 0x062d5b73ddb764df, 0x037793673b6b30ca,}},
    {.big.u64 = {0x39f6383f9f25a8c9, 0xd62c104d94a4d97e, 0x17eb7ee52ef3dd82, 0x051b52600bab0176,}},
    {.big.u64 = {0x10a7e8372712fc16, 0x18dd8d3043f74b43, 0xd76ba7f8c536b76b, 0x0eb9dc854e071ae0,}},
    {.big.u64 = {0xa0cd6f2c394a3e99, 0x4bdc0a731c737d4d, 0x9f1526f7dde84130, 0x0bac668f19fa181e,}},
    {.big.u64 = {0xcea660b10c524f0f, 0x47a93e424b5d84d5, 0x7753611af5d41941, 0x00d8adeb236fe68b,}},
    {.big.u64 = {0x07a3151729b23112, 0x4db76e83259ecb27, 0x9620626859c727f9, 0x126b59ea34dc19ae,}},
    {.big.u64 = {0xf663a977cf8d26d5, 0x3f51a6e341df51e6, 0x8bc86596aa11db60, 0x09a75b62b29a3012,}},
    {.big.u64 = {0x0adb10e20fd67ec4, 0x6500dcafc8e3ce88, 0xe8ca15f49020331a, 0x04e65ebf24b74427,}},

    {.big.u64 = {0x944bffdfc7d5d47d, 0xdab0455265083161, 0xa83b2d0d983e03f3, 0x0bc23068cddded61,}},
    {.big.u64 = {0x23e10fffbf7eea70, 0x0d780704eafd8018, 0xa0da9ad2fccf9665, 0x08d32722d94670fd,}},
    {.big.u64 = {0x332f1d533bc8cb31, 0x31b33b957fb7cd9c, 0xd74eed82ab63cd73, 0x0f342b61a9cbc46a,}},
    {.big.u64 = {0xe55c5b49c3b38d43, 0xf50e27eb93ea2524, 0x2d71e5403308f84a, 0x1244e89946a7bca7,}},
    {.big.u64 = {0xa5b73b5052c3b02c, 0x1caad2f3750abf7f, 0x7c36cfddbf4a87fa, 0x0ed9b971c0e5cdf1,}},
    {.big.u64 = {0xa754c965bc9df147, 0x2a037b84a052f1c2, 0x467a61d5d244546f, 0x079d01c2d35f7b9d,}},
    {.big.u64 = {0x3dd61a4a7f79df05, 0x0f0b3c1e7c16752d, 0xbd7683eb1a76eb8f, 0x0272c88d2e40a475,}},
    {.big.u64 = {0x4c368602273554c8, 0x33eb71d354ae330a, 0x2d578bd7d49eee66, 0x0813365b1c14b45e,}},
    {.big.u64 = {0x61aaea08809337a9, 0x68348f49df35d973, 0x8e2f0057256601cf, 0x0435f9ce89925ffb,}},

    {.big.u64 = {0x82e2372295fe2d8e, 0xf323f0324cb25911, 0x8061a21bb9edbde8, 0x0f091aac537bfce7,}},
    {.big.u64 = {0x7c7dc3fc3e18c648, 0x4c107a0421a1428f, 0xa82c29378c66c734, 0x115c944a6356768a,}},
    {.big.u64 = {0xa4579e205ecdcb5d, 0x4ceb21a6303aadd5, 0x16661320ab1425e5, 0x0a0e2c0e70b0b33e,}},
    {.big.u64 = {0xbcc3290f9bab75e0, 0x977e7815378f7352, 0x2fe00b6856e9a6b2, 0x126ce35e80539181,}},
    {.big.u64 = {0xe5d13574e18ef60b, 0x748e9ef43e48c6b2, 0x39f4bed46eac4c93, 0x07672054adf7cec3,}},
    {.big.u64 = {0x509364c4913d3065, 0x9e4512279691ac10, 0x6cfaccfcf62351ad, 0x100cbf8c134386c0,}},
    {.big.u64 = {0x1a3f2b4fd55e86dd, 0xd4ca0fe21c1e373d, 0x84f78cff45839b93, 0x00396798420bc276,}},
    {.big.u64 = {0xb0b744bed016683f, 0x31f6e6e7fc868f53, 0xfb8fc6f191058275, 0x00609a2ff328cf30,}},
    {.big.u64 = {0x44f4cf175829381e, 0xe90bd3e64a5e8dc5, 0x7b8396dfd699267f, 0x067f7b07bb894a69,}},

    {.big.u64 = {0xe9066793042bbf49, 0x96e590d534398369, 0xdbceee986e343974, 0x042c293c5116ba29,}},
    {.big.u64 = {0xfd62f252b45fde07, 0xa62788461517e6b6, 0x1ced59ca1c9e8c6d, 0x01824c91710272dd,}},
    {.big.u64 = {0xd03a62b25e305a42, 0x0123fe97c1759b58, 0xa3f031e3c5556696, 0x04837949b8cc3351,}},
    {.big.u64 = {0x119f48d8d155d77d, 0xfd4cffeb4413f5b7, 0x08b72b743a2bcff7, 0x123596b0bdd9d383,}},
    {.big.u64 = {0x6bd6ff54f13b8523, 0x93a596d6a1376e23, 0xb94ce11138a69e74, 0x0f6dd3bb1ad93794,}},
    {.big.u64 = {0x47e8ec1e878cb12d, 0x287c9f24c4fc42ab, 0x09936b6ebab7d39f, 0x121e58c54ac7c1f9,}},
    {.big.u64 = {0xe78b07922bd7c0e5, 0x566ad0a96b14b85a, 0x99c99f1438dfdcdc, 0x0c87a75f16217140,}},
    {.big.u64 = {0x58e4cef7cfcaca63, 0xfd69e79d516ae623, 0x9efaebc50606bc4d, 0x011a1d6746b95eb1,}},
    {.big.u64 = {0x8fc108e28e61b1d6, 0xb65e60ecb475cb92, 0xac66cd6dc263709f, 0x0e53fc4f86e16834,}},

    {.big.u64 = {0xfcdab7ed7e5cbe86, 0xdad48050df36afe9, 0x55b4cd0f91846349, 0x0fe488d34b4f6bb3,}},
    {.big.u64 = {0x1288e1f19258634f, 0xfbf1824ff5c894b0, 0x41fc613ef210d02d, 0x02d8898bb037ae4c,}},
    {.big.u64 = {0x059af60817ab77ed, 0xfa69cb73b655d82d, 0x9263bec12f3f8de4, 0x0541328b4a135452,}},
    {.big.u64 = {0xcb13b28912a54ad3, 0xf61fee9f00ffe5f8, 0xc5bf8a7c65e41e5d, 0x0b3b28935e00d195,}},
    {.big.u64 = {0xaf78333fb1d2cfdc, 0x1f65220a410854cd, 0xea031a6016759cf0, 0x046b6f67fbc60b8e,}},
    {.big.u64 = {0x718e188eff5f43b7, 0x5914bfcf1bb39472, 0x853e391de1d54279, 0x0eebc6c8729d8380,}},
    {.big.u64 = {0x6214d1f658e73342, 0x263a906ffdf271f2, 0x62847bea7552b9c7, 0x0a659f9a281ecb3c,}},
    {.big.u64 = {0xb64e2198d649ffcf, 0x3b99b3551d57bc7e, 0xb875cd584c8487a6, 0x00b7ea300209b74b,}},
    {.big.u64 = {0x92df2bfa508624bb, 0xc8418ba6bfb13195, 0xf003780a2199323d, 0x035b2caa6ee74a60,}},

    {.big.u64 = {0x10e134d0ba056348, 0x64e1cc6ec0c7e80d, 0x5b1d7f86e795ae48, 0x007f2e0cb303f2a5,}},
    {.big.u64 = {0x45d12f0a14bc8371, 0x2323796685e9c9ba, 0xc93a6477056eee25, 0x0d2b99892713e6f3,}},
    {.big.u64 = {0xbae898149040afad, 0x8e1ff44c39180054, 0x6d3f13fc21d6b1b6, 0x0161e2def847b61d,}},
    {.big.u64 = {0x1b526231e158a87c, 0xb5a76b6526be8ddf, 0x225b03e40f65d833, 0x129d388fa122e611,}},
    {.big.u64 = {0x0d901626741f1c67, 0xa06c5be6a7787292, 0x320ec3acfc2e17e7, 0x0cbb5b3ad4298511,}},
    {.big.u64 = {0x942be3f4a3441402, 0x8f708203fcbb8ef6, 0xc7f83eee5bfb679d, 0x09819e465f500697,}},
    {.big.u64 = {0x3064757a81dd5477, 0x4cc33fc237fc6a50, 0x9344d720073af95b, 0x11066a1855ea99d0,}},
    {.big.u64 = {0x7366f0eb59eb41af, 0x35596469593ec304, 0xac921c711bc8538d, 0x0ab46ae1df308f92,}},
    {.big.u64 = {0xfca93ac5d82b3eea, 0xff371e5af00dfded, 0x29019ed7e9ce2a58, 0x0aebd23712c82baa,}},

    {.big.u64 = {0x973090607ddb50d5, 0x7129c4158887f793, 0x22413fab598fcfdd, 0x01757675423126e5,}},
    {.big.u64 = {0xc3961c964ff23c3b, 0x4fb6fc40f7cbb9bc, 0xc71f5e79fed72cba, 0x0de429c7c2173065,}},
    {.big.u64 = {0x3aeba67afbd584ef, 0x682419bc8aef8e81, 0xb46f591d30412e35, 0x050c524c46c1c236,}},
    {.big.u64 = {0xbcc99aa03159741e, 0x38af467403a6cde0, 0x6ae17f30fd7342ae, 0x0002dc87666bb986,}},
    {.big.u64 = {0x864b10927324a1b9, 0xc79b355411bac318, 0x208606b96832d435, 0x0189bef22770e7a3,}},
    {.big.u64 = {0x7128da2908b519d7, 0x31be79d746749b27, 0xd6790a78ad4a590a, 0x018ba45890f567fb,}},
    {.big.u64 = {0x39009ee42e338aa0, 0xd0df35027d248fe5, 0xf7ea4bd210c97630, 0x07b172a2c1cc97dc,}},
    {.big.u64 = {0x4ae01f9b33f2796f, 0x877444afbb967bcb, 0xe789296ab31b6cfb, 0x0f9f8726211f1b55,}},
    {.big.u64 = {0xf72748f71ea11a1a, 0x8ddda8d19ed80c8e, 0x2f4cc51cf620457c, 0x0fa8523a352c423d,}},

    {.big.u64 = {0x1b4d77a8fdcf1067, 0xd0f884e2c05a52df, 0x1885a5014fa77e0b, 0x08f62b1a71449842,}},
    {.big.u64 = {0x0482564ff20d12c1, 0x1e0539e6bb96c907, 0x0755b9794e0a3253, 0x0ff1061f95e250ed,}},
    {.big.u64 = {0x34fe2a54834334b7, 0x2db0032bcfe630d8, 0x1f5f5d8e05dfa8c5, 0x074524e5c32af148,}},
    {.big.u64 = {0x94f1ac44ff61dfcd, 0x069fda4fad38fdd5, 0xd77a6f439b7aba2f, 0x0cffa391ebaf2c1a,}},
    {.big.u64 = {0xc650a34ee7d6c199, 0x87f07f4d6375895e, 0x4619e96980848bb3, 0x01fa2735287c7927,}},
    {.big.u64 = {0x4b4e303ebaaaee8d, 0x63f1ffbf75e04038, 0x057aeee013ad6d40, 0x0f9c7f7669f49789,}},
    {.big.u64 = {0x8b21ed614afe08d5, 0x2000d4b1215ac6de, 0x8a8c93a357a297b1, 0x0fc5b04bb7055d03,}},
    {.big.u64 = {0x4916bb59a506fc96, 0xef98639914613488, 0xdde13be0f91e8b32, 0x0b020681fdd7ec97,}},
    {.big.u64 = {0x433fce53e7f46963, 0x6d24ff0bb780e529, 0x6a940dfbfdd29e14, 0x0560acde24b1a5b9,}},

    {.big.u64 = {0x2db85599b3be3155, 0x5ff88cb8674d2397, 0xe1b12eb779e62bbb, 0x071aabb29c31987e,}},
    {.big.u64 = {0xc01b7c0aabb80065, 0x1b33c3004cde1fe1, 0xae1c45c4c5fdea04, 0x068655984b155b79,}},
    {.big.u64 = {0xf52c3201752c62ac, 0xa2ae837c4c76906d, 0x178d2e80c4813bca, 0x11e7b48455056e09,}},
    {.big.u64 = {0xb746eaf296a3155a, 0x9acb497b3b52f0b8, 0x992f55413177442f, 0x008f492129563096,}},
    {.big.u64 = {0x2841e5b64de79f55, 0x534c3ecb896b84d0, 0x26aff952d18e7a9e, 0x02e876cc2caf0e1c,}},
    {.big.u64 = {0x84b665dbcd0f26a9, 0x569c556c5d5b7f74, 0x5daf2cb0dd3b4392, 0x0d5d75599baed1ee,}},
    {.big.u64 = {0x4bcf9003a1fc4030, 0x887005a6f9671857, 0x09cf2bb13820d7d6, 0x030a9fa8ecd135c0,}},
    {.big.u64 = {0x949fe8333ff500b0, 0x7205b32c712cc1c9, 0xfb45014b9535259e, 0x0954eab95cb4e161,}},
    {.big.u64 = {0x11d6bd09704f2dbf, 0xae7ab01440bcaac1, 0xe8dc23323606670a, 0x018d1168d1d63c93,}},

    {.big.u64 = {0x5e9723265cf29041, 0x82a61fcf6d7e55e5, 0x2c5bc942938a0bd3, 0x0cb3a476b606fbec,}},
    {.big.u64 = {0x27ddb2b4996d0d75, 0x51b5e23fb804dde9, 0x960fe27b558549c3, 0x0befa561998a8543,}},
    {.big.u64 = {0xa2ff1767b47f287f, 0xa23cc571645fd41f, 0xe60f36cbf05201c1, 0x0152491e33484fac,}},
    {.big.u64 = {0x9fc25117276743c3, 0x182916329c84c384, 0xf3788e7d1fa8dc5c, 0x032dd7d8a946911f,}},
    {.big.u64 = {0x6f7719f93bf3bef2, 0xf64f29fe6f378ed0, 0x4c7081d825b07e6e, 0x035116e60a042eb7,}},
    {.big.u64 = {0xc4fb0162866f92b6, 0x3ea3b15c0ac39df4, 0xf328a6f8dd1cf51d, 0x0141deb3b9f4890e,}},
    {.big.u64 = {0x64af31acf6ea177c, 0x457886d9f9ff089b, 0xf7856e43110db9b9, 0x036de62769b61aaf,}},
    {.big.u64 = {0x75529b299cd9e6d5, 0x04beea6938727e8e, 0xe74d3f8d71399c9f, 0x0e165cbafcc84848,}},
    {.big.u64 = {0x7d7db51f784b82a0, 0xea89fe5cd4a1d780, 0xe9922ff0e62b78b3, 0x0548d1b0a7faf99f,}},

    {.big.u64 = {0x4cb71ef58a91e571, 0xbcc8c69b44b09a47, 0xf15a48710ed5b479, 0x073a7a756751e3f7,}},
    {.big.u64 = {0x9f567fdaa9d8d41e, 0x8c9f4fd358074ee2, 0x63d0e2f5082711ac, 0x0159228ae7f09d87,}},
    {.big.u64 = {0x87ef3ab5ad737bb2, 0xd023c5be1ceae7e3, 0xd5962861980d2020, 0x04ffbb21068bb4a8,}},
    {.big.u64 = {0xa3277a6da85287ac, 0xa36196c09faee5c6, 0xb5fe24ba70f9355a, 0x02a73e19405126ce,}},
    {.big.u64 = {0x3c36b0b12f776bd0, 0x72c74861f95a900d, 0xcc3406b672c4863e, 0x06eaf9c952a34918,}},
    {.big.u64 = {0x814817514f42d246, 0x39fac9d080d8afe5, 0xc3f9eb9ff1ce5e4d, 0x0212063909d64760,}},
    {.big.u64 = {0x9d9c578420d42937, 0x949496329712f1b7, 0x2124ea95b0adc2e9, 0x0673a08e116e3fc1,}},
    {.big.u64 = {0x8ea20deb843a74fd, 0x8924991ad586c072, 0xa36ccf26ad1a5581, 0x02e10bebbc75982c,}},
    {.big.u64 = {0xfd70073200e019d1, 0x68acf4fd10ca669a, 0xefcc4d212789f7c2, 0x032b487e38f82f0b,}},

    {.big.u64 = {0xa850ed3a7281f7eb, 0xa5e9fdccffc16720, 0xebc52596f4420909, 0x10db0533ecb75a08,}},
    {.big.u64 = {0x1df685a9181e5f68, 0xb19bd95e5968b1a7, 0x8b4fb7ce667bd257, 0x00cec4ff333edfc5,}},
    {.big.u64 = {0x6371e4d8616e348b, 0xe4f816735a0ecd93, 0x75135742cbe298d0, 0x0c73a5618e45fd56,}},
    {.big.u64 = {0x59df42e5ac9a8779, 0x6e0ff6ff36920a43, 0xc3669648c1a48301, 0x0a3613fd9459a92f,}},
    {.big.u64 = {0x37800420960e0e4a, 0xe6b1853c197186c0, 0xdd3ea3fff5cf58f7, 0x07b4fcccf5f986ec,}},
    {.big.u64 = {0x493ee17286684422, 0x02b4495718089863, 0xf25bd4aff4409b7e, 0x11bfd2d787024ce7,}},
    {.big.u64 = {0x1af38795e967eac9, 0xd0b0d3d7caf59180, 0x5dc8b6a1db1ab659, 0x0d11ca7d7196d74d,}},
    {.big.u64 = {0xe74e6a7978308fac, 0x1d379449271562f1, 0x56f4d9e107db8d62, 0x0797c81aa7ea5629,}},
    {.big.u64 = {0x468e74ed62b36c76, 0x2e6d8fe582efa72e, 0x16968cbf28f40822, 0x0368c18942fbc330,}},

    {.big.u64 = {0x46392412b6d99dc9, 0x13740511f1395876, 0x73a457997aa2d524, 0x11dfc19be07fdd2b,}},
    {.big.u64 = {0x70095c6c25aa0298, 0xd8dd397ba6461cbf, 0x462a710ea325e967, 0x0193dc23b37ccb81,}},
    {.big.u64 = {0x261bb19571e42fcf, 0xc6b44a8ba6a088ff, 0xefa7713586e2a136, 0x0b75cbf34c8bdade,}},
    {.big.u64 = {0x9eb743eb7c70f7cf, 0x858f2f824a3e3b6f, 0x9d06da005bba8a28, 0x0dd58625f7d8ba2c,}},
    {.big.u64 = {0x00906b620488be69, 0x638857aec2972a14, 0x2966c965faaec841, 0x0b3e19069be42c7b,}},
    {.big.u64 = {0x096a6d94a21fd818, 0x8cc42fbc6f7557c4, 0x2cde67f06b5e232a, 0x03273eb0ff5d2332,}},
    {.big.u64 = {0xeeaa7fdec69aa67f, 0xc29e85ab3361284c, 0xa51311a2747c9bbb, 0x118dbb357c8fb563,}},
    {.big.u64 = {0x0395210d7f327dfe, 0x31e6348d45d84624, 0xda2d495eb2c65074, 0x051efaf1f2bb7c6c,}},
    {.big.u64 = {0x2655a85f45b25bbe, 0xadc383f2d66eab6e, 0xbdad5f86ce461af0, 0x0abd6731f02feffa,}},

    {.big.u64 = {0xc352353321f1b09c, 0x844fdb431e3cf237, 0xe50ea2433ab02150, 0x0aee8c2a1114782f,}},
    {.big.u64 = {0x54261a9e46ec09a9, 0x6ad85e84e1011a9c, 0x45ceee985d0509f9, 0x123c4f150683fad1,}},
    {.big.u64 = {0x21660b988b87fb13, 0xc24a1aa44168dc33, 0x97d578ab9ba381a5, 0x0111684ac6516201,}},
    {.big.u64 = {0xc141ebf84d389c9b, 0x7ddb464e2a372439, 0xa1b051c19071f4d7, 0x10b2339aaaf4ea8e,}},
    {.big.u64 = {0x0f513760c8d49570, 0x44319602f19e30c1, 0xa62718c34b2e8d8c, 0x0eeb7b4df30d118b,}},
    {.big.u64 = {0xc76b8732f1434ef6, 0x4252eac5df70aaaf, 0x1d2f69e0f5756fa3, 0x0593c877be5982a0,}},
    {.big.u64 = {0x8586de80bd2eddf5, 0x00082ea7e113dbc6, 0x759f2972b2e621a9, 0x0adca9e616bc7d38,}},
    {.big.u64 = {0xa4741758e3d14a28, 0xc1ff6470649171fb, 0xc76c255b17dee034, 0x015cf931662e188d,}},
    {.big.u64 = {0xff6636e342421a43, 0xb4062de61a3827b9, 0xab8da5385cdc2902, 0x08fd091b1903f580,}},

    {.big.u64 = {0xea81d5bd14119478, 0xc4ce3b9e386dae73, 0x88db745603b2b39c, 0x0cf5477c5a5ae408,}},
    {.big.u64 = {0x544f32976ca8821c, 0x2f783f7f7ffb4d3d, 0xb97239db13d02046, 0x0cdb4765d5e4bc05,}},
    {.big.u64 = {0x8b0e64446d9edc8f, 0x1c1b7c0a3c181650, 0xe28907564337b0b7, 0x0d310858a7d45d2e,}},
    {.big.u64 = {0x125b9c57ac836fcb, 0x044a34432291c71e, 0xdbabf4cb8a7527b7, 0x06e2f96b7a87e223,}},
    {.big.u64 = {0x1796c1c3f4dda140, 0x4463fc9c22818fca, 0x4d1a30c7f1fac564, 0x0e31829073a41502,}},
    {.big.u64 = {0x5ed4517ccdf2887b, 0x173f0e2711db2c5a, 0xb71a361502124e46, 0x01b3016533140ab3,}},
    {.big.u64 = {0x88cf1b33b4b76dff, 0xca2c288a9085245b, 0x7444d4f899aa8804, 0x042846b37d8998c7,}},
    {.big.u64 = {0x4a9d431c538ad64d, 0x581ee5e3b3563ad5, 0x981ee2036c1fe92a, 0x019425743358a3ed,}},
    {.big.u64 = {0xb93bc3b8ed2015c4, 0xea2941156d2dc760, 0x8771867ce54d572c, 0x0af1e48b26df066c,}},

    {.big.u64 = {0x8c3eb2a125df085c, 0x20ef2eda8ad0b334, 0x278edf12407de2d6, 0x00d05e88bf1f36ec,}},
    {.big.u64 = {0x58a2027d92d91246, 0x362d4eb84fdf9b48, 0x9ec5186206191973, 0x02b794aab8bec9dd,}},
    {.big.u64 = {0xf0111c4ec888eb2a, 0x69c59420e1b73f4c, 0xf96351d7244da57d, 0x091ffc26975767de,}},
    {.big.u64 = {0x5c4ba95f793be1c6, 0x7611fd0a5dbf7e34, 0xf66fe9ac1ab9ecd8, 0x09081a7e60c704c1,}},
    {.big.u64 = {0x74748863e50cb037, 0xf34a15f43b6f77f1, 0x15926e5da6afa910, 0x0de8dbd17ebb2d4b,}},
    {.big.u64 = {0xe71d5d549850265d, 0x6c8352655a5cf9c8, 0x09420149bcc47d99, 0x0768971b37fb5041,}},
    {.big.u64 = {0xab0eb5845bb96527, 0x7cc79c72b167dfb9, 0x753af1281a89d506, 0x0a3eb3ef5663ca17,}},
    {.big.u64 = {0xad16cc2a5c2cca3d, 0x01754f922162c673, 0xec1f7726c62fa72a, 0x0748326dae712ab7,}},
    {.big.u64 = {0xb843d3a42535fe89, 0xa704434f61e7c70c, 0x91f11060cf3bfc7c, 0x0d28f205aa929b00,}},

    {.big.u64 = {0xa6ec73a0ac8cf323, 0x37ee3c717d18eb47, 0x3702427c0acb76ad, 0x0c5c6f1d76ed5b0f,}},
    {.big.u64 = {0xc9ecd5898ad9b2c5, 0xc350024d33ef34b5, 0xe939e8674cdd6728, 0x11e32a4cb3896d7a,}},
    {.big.u64 = {0xbd707dee4a35cbed, 0xe3f628ded539640e, 0x5d28dc88b3dc85ea, 0x022c801ae2c66e9f,}},
    {.big.u64 = {0x5d9810b0929205e6, 0x755774883130d7d4, 0x4bd58a907909754a, 0x08e455f81c865932,}},
    {.big.u64 = {0x2f4d72991ac8eb86, 0xa13af222d995d8b5, 0xba159423101ed99a, 0x00161065d7a60ece,}},
    {.big.u64 = {0xcff3c8d2bb9b55ce, 0x4ff02fb4510b43f0, 0x868b16e6af7b4fec, 0x0d7c1c571a4df5cb,}},
    {.big.u64 = {0xa539ca81ab88aa82, 0x7689e5d163c68f89, 0xd23d2e0dd670554c, 0x0e3381a6752ee31e,}},
    {.big.u64 = {0x813f88d8e68e33b0, 0x06d304137f0c5e53, 0x1371450512b0d67f, 0x03c7d2db2166622a,}},
    {.big.u64 = {0x197eec626b115b34, 0xb518865bc4b12d74, 0xe15099b81fa8e7cf, 0x0b5716a33519ce88,}},

    {.big.u64 = {0xfc0956aee277c06c, 0x70023ec59b2ad2bd, 0x323717820bf40d17, 0x0f925c078d97485d,}},
    {.big.u64 = {0xfdffcb311cfa3502, 0xea384798fe619564, 0x842972c4d671ba2d, 0x02f1155ce9b16d87,}},
    {.big.u64 = {0x209d8c14419e1721, 0x5e43e3f248d8d48e, 0x817866a9a9b0962c, 0x09d685fce3de5c8d,}},
    {.big.u64 = {0x912fa40436e77e4d, 0xb0185cf0c4d1a4f2, 0x91a3f11434fe9fc0, 0x05ea892cc78e3abc,}},
    {.big.u64 = {0x25549e88c00f2b37, 0xd017f5032f949575, 0x85f7ee9871a66fe7, 0x09befce9b6f63ce9,}},
    {.big.u64 = {0x8a44cb7c170d03bd, 0xf94efb48148ef6e3, 0x0c73b8289eefc832, 0x04a13333325fca6a,}},
    {.big.u64 = {0x7dc886922008f623, 0x242706905402933e, 0x5781ea53a5acf7ec, 0x029a7504290ea16b,}},
    {.big.u64 = {0x143e63d4ed159d06, 0x6cddda020237d723, 0x607885e7635e1fc4, 0x0b46a872fa83f563,}},
    {.big.u64 = {0x6c552c0fb1a64ef5, 0xdcab07069bdafc31, 0x3696e38286704369, 0x046db68d7dab3c64,}},

    {.big.u64 = {0x535f26ced9266dc8, 0xe335b8031fdfee06, 0xa055cf1989e213ed, 0x094ace0184827304,}},
    {.big.u64 = {0xee429c234ddc65ce, 0x362c66ff2461e94f, 0x21658e2a9ad7828a, 0x045018081675ecd5,}},
    {.big.u64 = {0xbf27d2fd16ef3eb7, 0x004fe3897d9e3738, 0xae68da69fa574cda, 0x0e0f5b30d0c2c9ca,}},
    {.big.u64 = {0xed2b322e56fb2bf7, 0x49cdff4a924c1fa2, 0xd319fb5dcd08b8d7, 0x074add2aee302ced,}},
    {.big.u64 = {0xe3916dae3c3b3c53, 0xbcd84ab7c1ee14ac, 0x1e076b0927297f63, 0x111e170c11195b04,}},
    {.big.u64 = {0x9f341689c4579678, 0xbb6b34528fc4adb2, 0x3abeb28fc8f21615, 0x059f1c038a606f1c,}},
    {.big.u64 = {0x336414ee8e2e3487, 0xba97774641c92aa7, 0xe713297c203e1f89, 0x04eb19fa929a4a64,}},
    {.big.u64 = {0x5b71612363d2642b, 0xad64c1ad3112646e, 0x6159f53aa7c46f77, 0x0de5da14c6748027,}},
    {.big.u64 = {0x1a1452bd843fb0c6, 0xd5627031f682ddad, 0xec89ca134b955b55, 0x12a91e59d3752283,}},

    {.big.u64 = {0x0d50d7483d725b64, 0x5c7aea0655de62f1, 0xe16887abcd76eb23, 0x11289af3812c8514,}},
    {.big.u64 = {0xb4cf4334841ec075, 0x9a3eed5751750b6a, 0x4f22d39aaf7a5d62, 0x088aef5df79a00c4,}},
    {.big.u64 = {0x280cca1105dbe74f, 0x44919e50160e55bc, 0xb2b4a1c44a3c56a5, 0x079677b7b714c60c,}},
    {.big.u64 = {0xe09f761cd13c5c54, 0xc656d6793a8626a0, 0xa5b21265a32e956b, 0x04652600cc886d34,}},
    {.big.u64 = {0x47e281b57c89a911, 0xa8285fe3b5b72afc, 0x39e6fe0b69539616, 0x107577d4ccc82540,}},
    {.big.u64 = {0x5c6e6d4cf6516923, 0x61bf28ef8226cf74, 0xe552ec0e737376c8, 0x03ef32e11dd351ac,}},
    {.big.u64 = {0x61802190a74b0a04, 0xf3e26ccd31988b87, 0xd224e8103a70f285, 0x07754e7b0c54be3e,}},
    {.big.u64 = {0xcf9019e6661311b8, 0xcf67d8701340ea01, 0x720acfe40a28bb23, 0x119a6b161de12f83,}},
    {.big.u64 = {0x0e64bfc1f39e6509, 0x4c1fc0b4acba1983, 0x2ad62c8b0ed27228, 0x0d5faa6e92d82fc2,}},

};

const field_t mds_rate_8[MDS_RATE_8_SIZE]={
    {.big.u64 = {0xd6c7b140f4848e16, 0x53991981a73f41c1, 0x4be72fee32ebe848, 0x0df5c5e7bd0c338e,}},
    {.big.u64 = {0xc6eadaff3c128b04, 0x37c430ae1daec691, 0xe12922da820dba75, 0x1121dd73267d6d57,}},
    {.big.u64 = {0xe99118064cfd0999, 0x25955e3ea47bfb3d, 0x7d57a10eb3c32639, 0x0db4791b8ba2a465,}},
    {.big.u64 = {0x17c5f21695404f89, 0x0446a4404843074c, 0xef9f24fd37e56874, 0x02717c7ef0ad3b18,}},
    {.big.u64 = {0x66181a54c8882d3e, 0x2721041f61e7cb30, 0x18e05457b053cbc9, 0x0716b0aab526190a,}},
    {.big.u64 = {0x4ab198049ad2a762, 0xfcd5ee7185e65901, 0x12afac9a1a98e79e, 0x112412f37ecf6d71,}},
    {.big.u64 = {0x05977bc95c8ffc5b, 0xae9c3b72131b58f2, 0x58086f110a950eec, 0x06a9b761c5ef90a6,}},
    {.big.u64 = {0xbe34f5b012e7279d, 0x5e535e33d50a512c, 0x94cc75d43719fe44, 0x0d291df320971baa,}},
    {.big.u64 = {0xb562b38623afd21b, 0x9de78a6bc3032948, 0x3c7c29ee858937fa, 0x0672bad7e17ec57c,}},

    {.big.u64 = {0x3be64ae4645633cc, 0x89aaa8aaf6976edd, 0x138954a0d44f5a31, 0x1218f3bb36c54c98,}},
    {.big.u64 = {0x5b5ae7361c904728, 0xe99fc43e281e5105, 0xa60245cf68b79fea, 0x0a3f6e14c7a3fa62,}},
    {.big.u64 = {0x1971fe796f41f9f1, 0x70edc283d9712c9c, 0xbd00d1f094e6b9b1, 0x0e053670380f7f5e,}},
    {.big.u64 = {0x6f3c7c07e946ecd5, 0x08c4adcb4e3f113c, 0x892b66f480649acf, 0x0617db174136360f,}},
    {.big.u64 = {0x3ccb9e54223efb60, 0x1e3593ad7dce36d0, 0x61039926ad4067e8, 0x0eb059337edb5c52,}},
    {.big.u64 = {0xc85ebcb0d62f8259, 0x3d21b03cf53ec969, 0x0825bfcc74aa0c8c, 0x101f3dfbd8bc9868,}},
    {.big.u64 = {0x4fadac6cc0a2e5ac, 0x8686c2b4fd4fcfee, 0xb5d8a17dcdefa73a, 0x009e120ff7251365,}},
    {.big.u64 = {0xcf755ed96e25e886, 0x64739f53d2d1ac52, 0x4d85bea3a720554c, 0x04d03c412f043f11,}},
    {.big.u64 = {0x000cb2452f85f3e3, 0x1439f848097fb034, 0x401a582753c50180, 0x02d1852a70c1c114,}},

    {.big.u64 = {0x005159871fad3a56, 0x841a57a2c079b51f, 0x7deed4d9c6edba94, 0x1196db92d6652a19,}},
    {.big.u64 = {0x42cdc48afc2ee7f8, 0x2dc73d19384daa35, 0x2aa45d7f504c69aa, 0x0291cfdbfbfed694,}},
    {.big.u64 = {0x701c0e1fcb62117c, 0xc2ab5b08494a14a4, 0xb7e021e0fb15f190, 0x0b5b9791448032cb,}},
    {.big.u64 = {0x1e09d6182974cc8e, 0x305811ee1b828f9e, 0xd1b9a7854f4eb591, 0x11485b9ac6177422,}},
    {.big.u64 = {0x76e9870bacf4ee7e, 0x9c8cd77812f7a4bc, 0x855b5555d2b6aa90, 0x0990e8a94f4f260a,}},
    {.big.u64 = {0xa7a0dac2a459e557, 0xf3fde73d34e8467b, 0x660f3b7842ae9886, 0x0f9220009d0334bc,}},
    {.big.u64 = {0x6c53f1d8df3493da, 0x3b40527c91c0f897, 0x8df4da3d7161bb8c, 0x122f545b1a1b5765,}},
    {.big.u64 = {0xce286d9addcb1684, 0x7a04bae657778c15, 0x005cc2f6cdf71655, 0x10b36ec6d163ffc0,}},
    {.big.u64 = {0xf63deeb026c71150, 0xfef46767cb8ed94a, 0xf77f3d0afe50cda6, 0x0be361318ffff681,}},

    {.big.u64 = {0x7961b94fc089f2cc, 0xca05503ad4c57698, 0x43b917d31c80e19b, 0x03a5d1861a6cfd9b,}},
    {.big.u64 = {0x1d7e38da506f2512, 0x299adda851e53e03, 0x3a27b7c519a6a606, 0x110d8a6cbc5179c7,}},
    {.big.u64 = {0x13038cfd6abdac6a, 0xb693c1a089e49a5f, 0x4b6601a60629b013, 0x019d5b435931f4a2,}},
    {.big.u64 = {0x89e0c6761c4e83b0, 0x45162a6556330b97, 0x830e6d66a3bb2160, 0x01b46b4aaf021663,}},
    {.big.u64 = {0xb88537cb15610593, 0x447e8f4aa11aa82a, 0xd19d09b0cd95c465, 0x0bab0c047c8ae6e3,}},
    {.big.u64 = {0x5f4f1f33b5579e5c, 0x2902dc166796c925, 0xb13ad84b2284c6cc, 0x11e435a052d02943,}},
    {.big.u64 = {0x1c6d16103ab53bb3, 0x41ba87d79ae0deb1, 0x3901f1645c1945c2, 0x09af3962cda8291f,}},
    {.big.u64 = {0x413287542efc5125, 0xdebc1216851ff94f, 0xf68e07787c41051e, 0x11e0230a58480d1f,}},
    {.big.u64 = {0x07d32028208ffe23, 0x183b34c8da294d09, 0xa6619623ff66c777, 0x0bb6d1f977106a18,}},

    {.big.u64 = {0xcbdaf50ec8221514, 0xfa84a3f3cf69fca4, 0x3b503c6330232209, 0x0aa94843362fd81a,}},
    {.big.u64 = {0x11fb8ae333b3c8d6, 0x51cc5eab6393f1e1, 0xd8a77e16a6083912, 0x06fd1e137e9037c2,}},
    {.big.u64 = {0xc8180661363881cd, 0xeae28ebce6d1aa39, 0x09d51d382ec55a78, 0x086f371fe2b983b3,}},
    {.big.u64 = {0x9ac8018c37a16f81, 0x8467172003919a58, 0xd4b2b213614640f5, 0x0551e13c9280cb1f,}},
    {.big.u64 = {0x0fc512426ed338cf, 0x0d84ce3b161c13d0, 0x239a11e00c7b9038, 0x0ed93020baea75f1,}},
    {.big.u64 = {0x2fcd6d083cd1fb03, 0x3f46cb889110b890, 0x787f4e4464492f2f, 0x0571c7e4a228670d,}},
    {.big.u64 = {0x614e95908cf0f431, 0xf7c20c40ed1c719f, 0xd661f25b5ad3a366, 0x01bc60b4f9f54eff,}},
    {.big.u64 = {0x0496928e388d0ae5, 0xd7fdc7c33c4b5e60, 0xeff635ddc0918f66, 0x0be6f51001cb846e,}},
    {.big.u64 = {0xebca1867be11acb8, 0xb4c37924aae11821, 0x74a93b175910c7ce, 0x05506b4fbd4174ad,}},

    {.big.u64 = {0x2c163fecda61edf5, 0xfbd6eaa2d5c0a7ba, 0xe863873822c8fb89, 0x04c4773474592ba5,}},
    {.big.u64 = {0x5a5eced02b635348, 0xd448ba78a6792648, 0x108fc760c99a629d, 0x0bfad0fc3fe9bd7d,}},
    {.big.u64 = {0x82766842803ed53e, 0xad21674701a23a35, 0x77cd8c8f33a45825, 0x0da8e145c7a439ce,}},
    {.big.u64 = {0x42fee821643c63ea, 0xff7a534620c21334, 0xb3487c2a38063e47, 0x0ac98489e5415830,}},
    {.big.u64 = {0x1a2ccda84eba33d5, 0xb8d5f2cde8332b04, 0x5941f25839bd5f64, 0x0f75184579fb8218,}},
    {.big.u64 = {0x274ad1e55044a35f, 0x0f01d138c917bbeb, 0x99ae7f4b054289e1, 0x0248c3a904f85ab0,}},
    {.big.u64 = {0x84d45336e5056131, 0xd7c645cca5eec525, 0xe4a077aa336ffe58, 0x06850c88ed459c3f,}},
    {.big.u64 = {0x7333c0eb60d6a3ae, 0x689f6ee0520cf65d, 0x0d21d89bf247f4f2, 0x1122f383b6dccb4a,}},
    {.big.u64 = {0x386c0b1a8f9c0b91, 0xd210caa3f7ebcc31, 0x9a8700d05c03ad5a, 0x0d69b4c3e4a07529,}},

    {.big.u64 = {0xe8ebbd91e7916b86, 0x6eba640928a624fd, 0xeb6ccca933dcff90, 0x0a0474d047ed57b5,}},
    {.big.u64 = {0x9598fe4b1e1df65f, 0x9422e239f13db9c5, 0xded575b34cec2e4f, 0x0606926fd8f4d4a3,}},
    {.big.u64 = {0x3e42c2c4ea29fc56, 0x9502b864a5fb5ea3, 0xf79997751611c577, 0x05b1b3238393f8dc,}},
    {.big.u64 = {0xd1fd2e5b312048aa, 0x4b4d22cfddbc51aa, 0xb474c19f750a4242, 0x1077704fae154ff3,}},
    {.big.u64 = {0x05afe5ccd59415f8, 0x2ff5a0968962e3f5, 0x08c169304d73e68d, 0x0dca639174b3af32,}},
    {.big.u64 = {0xe7d3f87e8393dce4, 0x3f987757400b6215, 0x997b67dfed426623, 0x03137caee4896e78,}},
    {.big.u64 = {0x3e688e08968d4283, 0x48bc81de2c6425bd, 0x619b1e7fba77a844, 0x128d4c95f43a3dac,}},
    {.big.u64 = {0xa05c4a099d4d00c6, 0xbaac96b15d1c64d0, 0xa14731981ca9351d, 0x116d0ff09e2bc145,}},
    {.big.u64 = {0x24bab86bf1856e4b, 0x80c919f5409f03a8, 0xb8f4f3a964e4fa5d, 0x05da3f4df70b5588,}},

    {.big.u64 = {0x2143eca386a47fea, 0x60035a065dd7d465, 0xb6b19c3d360bf779, 0x00e55d27c1bf93d4,}},
    {.big.u64 = {0xc42d3a2e2319279c, 0x7a5674de1955e8e3, 0xc0fe0714b5940a92, 0x0bc634c5d4a51c9b,}},
    {.big.u64 = {0xb2b563ef4067c1e9, 0xa4ce24b0d9e909ac, 0x0c5d46e581c39b14, 0x0b155c3dfaf2482c,}},
    {.big.u64 = {0x68b2ad1e6c7f3e4f, 0xd06a7a996e7a3088, 0x28a7841d6f9d825b, 0x0b617bc8e8e21eb9,}},
    {.big.u64 = {0x7dcde26815a9c170, 0xf605f8942fce0090, 0xf7174cc4691800e3, 0x0aa0a9fbd79c6d78,}},
    {.big.u64 = {0x59ee41cf1183a876, 0x1266fd5aaa5c2652, 0xcd843009dfcee440, 0x1206776e732d103d,}},
    {.big.u64 = {0x8da226986f9a2025, 0x3afb94bd0d1fc7ba, 0xcc1e46cb90523398, 0x06f5afccedf57f24,}},
    {.big.u64 = {0x59455c98111b2204, 0xecda27b45cc11490, 0x617ca6b831a21969, 0x009dcba35a6e7f8d,}},
    {.big.u64 = {0x61c27fbf75a53a7d, 0x87c7616297676839, 0x6485191282fdb0e0, 0x0676899c5fc0bffb,}},

    {.big.u64 = {0x3e45baa8bce40a99, 0x3bdef6aa8ddb77bc, 0x77d87e6d4b6513cc, 0x0a219e08aba92fb1,}},
    {.big.u64 = {0xdcccc450c235de00, 0x6255359cd8c28a13, 0xc6a1c4a82e1b3d9f, 0x0737a86e564e560a,}},
    {.big.u64 = {0x08be4ffb77300375, 0xd23f509675063653, 0xf941dc0984e01402, 0x01e4cd3889768523,}},
    {.big.u64 = {0x80c0a5fde6234890, 0x9de98e36ec24ddd6, 0x10d4a176860bb2d7, 0x07ace181182c64eb,}},
    {.big.u64 = {0x02ac52d2d32fa6d8, 0x2e4703f0d601b0a5, 0x95f55025e7635b58, 0x0fe0a43b7387bfa6,}},
    {.big.u64 = {0x28e67ede3ce0814e, 0x9265b16763f15d24, 0x1d9c03725a6e1788, 0x09b53b6fe4d3ce7e,}},
    {.big.u64 = {0xa05e1ee6679abc74, 0x1412ff016d3f1383, 0x3ba2a4e16c804c14, 0x018c747269b5a657,}},
    {.big.u64 = {0x6d9ac1c7b3d1b509, 0x7899fa3954f4c3a2, 0x95ff8280b919cc5f, 0x048e07e6b31fe7df,}},
    {.big.u64 = {0xf9d3b02831ef50d5, 0x9e097f0f6688b750, 0x0263eea26f7448e7, 0x04cd4ca449daf411,}},

};

void poseidon_parameters_init(poseidon_parameters_t *parameters, uint8_t rate)
{
    switch (rate) {
        case 2:
            parameters->full_rounds    = 8;
            parameters->partial_rounds = 31;
            parameters->alpha          = 17;
            parameters->ark            = (field_t *) ark_rate_2;
            parameters->ark_size       = ARK_RATE_2_SIZE;
            parameters->mds            = (field_t *) mds_rate_2;
            parameters->mds_size       = MDS_RATE_2_SIZE;
            break;

        case 4:
            parameters->full_rounds    = 8;
            parameters->partial_rounds = 31;
            parameters->alpha          = 17;
            parameters->ark            = (field_t *) ark_rate_4;
            parameters->ark_size       = ARK_RATE_4_SIZE;
            parameters->mds            = (field_t *) mds_rate_4;
            parameters->mds_size       = MDS_RATE_4_SIZE;
            break;

        default:
            parameters->full_rounds    = 8;
            parameters->partial_rounds = 31;
            parameters->alpha          = 17;
            parameters->ark            = (field_t *) ark_rate_8;
            parameters->ark_size       = ARK_RATE_8_SIZE;
            parameters->mds            = (field_t *) mds_rate_8;
            parameters->mds_size       = MDS_RATE_8_SIZE;
            break;
    }
}
