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

#include "os_utils.h"
#include "cx.h"

#include "bhp_parameters.h"
#include "bhp_1024.h"

static uint8_t bhp_buffer[100];

int bhp_1024_hash_function_id(const function_id_datas_t *datas, field_t *hash)
{
    uint8_t offset = 0;
    uint8_t length = 0;

    U2LE_ENCODE(&bhp_buffer[offset], 0, datas->network_id);
    offset += 2;

    length             = strlen(datas->program_id_name);
    bhp_buffer[offset] = length * 8;
    offset += 1;
    memcpy(&bhp_buffer[offset], datas->program_id_name, length);
    offset += length;

    length             = strlen(datas->program_id_network);
    bhp_buffer[offset] = length * 8;
    offset += 1;
    memcpy(&bhp_buffer[offset], datas->program_id_network, length);
    offset += length;

    length             = strlen(datas->function_name);
    bhp_buffer[offset] = length * 8;
    offset += 1;
    memcpy(&bhp_buffer[offset], datas->function_name, length);
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

    return -1;
}
