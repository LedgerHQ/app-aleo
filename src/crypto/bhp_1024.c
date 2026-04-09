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

#include "ledger_assert.h"
#include "os_utils.h"
#include "cx.h"

#include "bhp_1024.h"

static uint8_t bhp_buffer[100];

int bhp_1024_hash_function_id(const function_id_datas_t *data, field_t *hash)
{
    uint8_t offset = 0;
    uint8_t length = 0;

    LEDGER_ASSERT(data != NULL, "NULL data");
    LEDGER_ASSERT(hash != NULL, "NULL hash");

    U2LE_ENCODE(&bhp_buffer[offset], 0, data->network_id);
    offset += 2;

    length             = strlen(data->program_id_name);
    bhp_buffer[offset] = length * 8;
    offset += 1;
    memcpy(&bhp_buffer[offset], data->program_id_name, length);
    offset += length;

    length             = strlen(data->program_id_network);
    bhp_buffer[offset] = length * 8;
    offset += 1;
    memcpy(&bhp_buffer[offset], data->program_id_network, length);
    offset += length;

    length             = strlen(data->function_name);
    bhp_buffer[offset] = length * 8;
    offset += 1;
    memcpy(&bhp_buffer[offset], data->function_name, length);
    offset += length;

    if ((offset == 31)
        && (!memcmp(bhp_buffer,
                    "\x00\x00\x38\x63\x72\x65\x64\x69\x74\x73\x20\x61\x6c\x65\x6f\x78\x74\x72"
                    "\x61\x6e\x73\x66\x65\x72\x5f\x70\x75\x62\x6c\x69\x63",
                    offset))) {
        // 0/credits.aleo/transfer_public
        hash->big.u64[0] = 0x405245447621401a;
        hash->big.u64[1] = 0x51c07b62a9c53d26;
        hash->big.u64[2] = 0xb928e2f9112d0953;
        hash->big.u64[3] = 0x0f511e31d6855446;
        return 0;
    }
    else if ((offset == 31)
             && (!memcmp(bhp_buffer,
                         "\x01\x00\x38\x63\x72\x65\x64\x69\x74\x73\x20\x61\x6c\x65\x6f\x78\x74\x72"
                         "\x61\x6e\x73\x66\x65\x72\x5f\x70\x75\x62\x6c\x69\x63",
                         offset))) {
        // 1/credits.aleo/transfer_public
        hash->big.u64[0] = 0x1a45c36fff8ff4a4;
        hash->big.u64[1] = 0x69712402399b89f1;
        hash->big.u64[2] = 0xb3e921a03d522403;
        hash->big.u64[3] = 0x0b8deb3e586ce4f4;
        return 0;
    }
    else if ((offset == 32)
             && (!memcmp(bhp_buffer,
                         "\x00\x00\x38\x63\x72\x65\x64\x69\x74\x73\x20\x61\x6c\x65\x6f\x80\x74\x72"
                         "\x61\x6e\x73\x66\x65\x72\x5f\x70\x72\x69\x76\x61\x74\x65",
                         offset))) {
        // 0/credits.aleo/transfer_private
        hash->big.u64[0] = 0x57853955ec65e959;
        hash->big.u64[1] = 0xc04dacceb1a2026a;
        hash->big.u64[2] = 0x0b8c74f5b07ee388;
        hash->big.u64[3] = 0x0913b0e1e8289dba;
        return 0;
    }
    else if ((offset == 32)
             && (!memcmp(bhp_buffer,
                         "\x01\x00\x38\x63\x72\x65\x64\x69\x74\x73\x20\x61\x6c\x65\x6f\x80\x74\x72"
                         "\x61\x6e\x73\x66\x65\x72\x5f\x70\x72\x69\x76\x61\x74\x65",
                         offset))) {
        // 1/credits.aleo/transfer_private
        hash->big.u64[0] = 0x14050f83d8bc8ad5;
        hash->big.u64[1] = 0x68c1128533fd9691;
        hash->big.u64[2] = 0xb5bfd0497dcbe60d;
        hash->big.u64[3] = 0x061bcb7383584d42;
        return 0;
    }
    else if ((offset == 42)
             && (!memcmp(bhp_buffer,
                         "\x00\x00\x38\x63\x72\x65\x64\x69\x74\x73\x20\x61\x6c\x65\x6f\xd0\x74\x72"
                         "\x61\x6e\x73\x66\x65\x72\x5f\x70\x72\x69\x76\x61\x74\x65\x5f\x74\x6f\x5f"
                         "\x70\x75\x62\x6c\x69\x63",
                         offset))) {
        // 0/credits.aleo/transfer_private_to_public
        hash->big.u64[0] = 0x2ed6c22fde6f93b0;
        hash->big.u64[1] = 0x7673e8d503d0995f;
        hash->big.u64[2] = 0x6ec97fff81556086;
        hash->big.u64[3] = 0x099abee7b8c03597;
        return 0;
    }
    else if ((offset == 42)
             && (!memcmp(bhp_buffer,
                         "\x01\x00\x38\x63\x72\x65\x64\x69\x74\x73\x20\x61\x6c\x65\x6f\xd0\x74\x72"
                         "\x61\x6e\x73\x66\x65\x72\x5f\x70\x72\x69\x76\x61\x74\x65\x5f\x74\x6f\x5f"
                         "\x70\x75\x62\x6c\x69\x63",
                         offset))) {
        // 1/credits.aleo/transfer_private_to_public
        hash->big.u64[0] = 0x7c27587b57c5d2d4;
        hash->big.u64[1] = 0x78e193e449543290;
        hash->big.u64[2] = 0xf4e07ed6a53dea1e;
        hash->big.u64[3] = 0x018801c51078171d;
        return 0;
    }
    else if ((offset == 42)
             && (!memcmp(bhp_buffer,
                         "\x00\x00\x38\x63\x72\x65\x64\x69\x74\x73\x20\x61\x6c\x65\x6f\xd0\x74\x72"
                         "\x61\x6e\x73\x66\x65\x72\x5f\x70\x75\x62\x6c\x69\x63\x5f\x74\x6f\x5f\x70"
                         "\x72\x69\x76\x61\x74\x65",
                         offset))) {
        // 0/credits.aleo/transfer_public_to_private
        hash->big.u64[0] = 0x6e2c39b3c306d0f5;
        hash->big.u64[1] = 0x1dc66c4befdfbc77;
        hash->big.u64[2] = 0x0e2106ac3953c377;
        hash->big.u64[3] = 0x0d3c811de289fd10;
        return 0;
    }
    else if ((offset == 42)
             && (!memcmp(bhp_buffer,
                         "\x01\x00\x38\x63\x72\x65\x64\x69\x74\x73\x20\x61\x6c\x65\x6f\xd0\x74\x72"
                         "\x61\x6e\x73\x66\x65\x72\x5f\x70\x75\x62\x6c\x69\x63\x5f\x74\x6f\x5f\x70"
                         "\x72\x69\x76\x61\x74\x65",
                         offset))) {
        // 1/credits.aleo/transfer_public_to_private
        hash->big.u64[0] = 0x116b3d6259fc8332;
        hash->big.u64[1] = 0x3b6f6a1a5b3344dd;
        hash->big.u64[2] = 0x66c3bccc5ed9580b;
        hash->big.u64[3] = 0x0b218600c10bd9a3;
        return 0;
    }
    else if ((offset == 26)
             && (!memcmp(bhp_buffer,
                         "\x00\x00\x38\x63\x72\x65\x64\x69\x74\x73\x20\x61\x6c\x65\x6f\x50\x66\x65"
                         "\x65\x5f\x70\x75\x62\x6c\x69\x63",
                         offset))) {
        // 0/credits.aleo/fee_public
        hash->big.u64[0] = 0xf9bcb7f7a577a934;
        hash->big.u64[1] = 0x98f4346a10f70cf8;
        hash->big.u64[2] = 0x8f26408e9addc51b;
        hash->big.u64[3] = 0x0aa82c82d4ff15a4;
        return 0;
    }
    else if ((offset == 26)
             && (!memcmp(bhp_buffer,
                         "\x01\x00\x38\x63\x72\x65\x64\x69\x74\x73\x20\x61\x6c\x65\x6f\x50\x66\x65"
                         "\x65\x5f\x70\x75\x62\x6c\x69\x63",
                         offset))) {
        // 1/credits.aleo/fee_public
        hash->big.u64[0] = 0x8d31084295c605ef;
        hash->big.u64[1] = 0xbfdc39996575504e;
        hash->big.u64[2] = 0xae32c9cce746275e;
        hash->big.u64[3] = 0x0bbfb3251558eee9;
        return 0;
    }
    else if ((offset == 27)
             && (!memcmp(bhp_buffer,
                         "\x00\x00\x38\x63\x72\x65\x64\x69\x74\x73\x20\x61\x6c\x65\x6f\x58\x66\x65"
                         "\x65\x5f\x70\x72\x69\x76\x61\x74\x65",
                         offset))) {
        // 0/credits.aleo/fee_private
        hash->big.u64[0] = 0xccf5dc9907364acc;
        hash->big.u64[1] = 0x1cf2578ad1d69649;
        hash->big.u64[2] = 0x95c4227bca877a48;
        hash->big.u64[3] = 0x0c218239cc930255;
        return 0;
    }
    else if ((offset == 27)
             && (!memcmp(bhp_buffer,
                         "\x01\x00\x38\x63\x72\x65\x64\x69\x74\x73\x20\x61\x6c\x65\x6f\x58\x66\x65"
                         "\x65\x5f\x70\x72\x69\x76\x61\x74\x65",
                         offset))) {
        // 1/credits.aleo/fee_private
        hash->big.u64[0] = 0xc3c204d98b5e12b2;
        hash->big.u64[1] = 0x318d93a9b12ba7c9;
        hash->big.u64[2] = 0x9d42d8fb1c715281;
        hash->big.u64[3] = 0x01505646987444fa;
        return 0;
    }
    else if ((offset == 21)
             && (!memcmp(bhp_buffer,
                         "\x00\x00\x38\x63\x72\x65\x64\x69\x74\x73\x20\x61\x6c\x65\x6f\x28\x73\x70"
                         "\x6c\x69\x74",
                         offset))) {
        // 0/credits.aleo/split
        hash->big.u64[0] = 0x842cb43ec1d6bbe9;
        hash->big.u64[1] = 0xa2c33251d4bded16;
        hash->big.u64[2] = 0x0fe74f79fc7c63d4;
        hash->big.u64[3] = 0x022fb51a7d2acc90;
        return 0;
    }
    else if ((offset == 21)
             && (!memcmp(bhp_buffer,
                         "\x01\x00\x38\x63\x72\x65\x64\x69\x74\x73\x20\x61\x6c\x65\x6f\x28\x73\x70"
                         "\x6c\x69\x74",
                         offset))) {
        // 1/credits.aleo/split
        hash->big.u64[0] = 0x0bb53e032f965178;
        hash->big.u64[1] = 0x6cd697d8b5df7efc;
        hash->big.u64[2] = 0x12e2736c492f2495;
        hash->big.u64[3] = 0x0529c62123003bca;
        return 0;
    }

    return -1;
}
