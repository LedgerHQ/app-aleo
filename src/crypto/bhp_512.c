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

#include "bhp_512_parameters.h"
#include "bhp_512.h"

#define BHP_CHUNK_SIZE (3)

static const group_t commitment_sum = {
    .x = {0x5bb24a5e91a9ca8c, 0x9b715027001c0763, 0xc82f98226ac8e338, 0x013c39372f71a98c},
    .y = {0x2f62910dd470b44a, 0x2e7bf3d229f8ead5, 0x87aa43d956908927, 0x08e58f9fe96b972c}
};

static const compressed_group_t zero = {
    .compressed = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01},
    .sign       = 0
};

static uint8_t bhp_buffer[256];

static void sum_init(cx_ecpoint_t *sum, const group_t *init)
{
    bigint_256_t s;
    uint8_t      bn_x[32];
    uint8_t      bn_y[32];

    (void) cx_bn_lock(32, 0);
    (void) cx_ecpoint_alloc(sum, CX_CURVE_EdBLS12);
    if (init == NULL) {
        (void) cx_ecpoint_decompress(sum, zero.compressed, 32, zero.sign);
    }
    else {
        field_to_big_int(&init->x, &s);
        big_int_to_bn(&s, bn_x);
        field_to_big_int(&init->y, &s);
        big_int_to_bn(&s, bn_y);
        (void) cx_ecpoint_init(sum, bn_x, 32, bn_y, 32);
    }
}

static void sum_end(cx_ecpoint_t *sum, group_t *a)
{
    bigint_256_t s;
    uint8_t      bn_x[32];
    uint8_t      bn_y[32];

    (void) cx_ecpoint_export(sum, bn_x, 32, bn_y, 32);

    bn_to_big_int(bn_x, &s);
    field_from_big_int(&a->x, &s);
    bn_to_big_int(bn_y, &s);
    field_from_big_int(&a->y, &s);

    (void) cx_ecpoint_destroy(sum);
    (void) cx_bn_unlock();
}

void bhp_512_hash_commitment(const field_t *commitment, field_t *hash)
{
    size_t       index  = 0;
    size_t       offset = 0;
    bigint_256_t s;
    uint8_t      bn_x[32];
    cx_ecpoint_t sum;
    cx_ecpoint_t point_a;
    bn_group_t  *a;
    group_t      digest;

    memset(bhp_buffer, 0, sizeof(bhp_buffer));

    (void) cx_bn_lock(32, 0);
    (void) cx_ecpoint_alloc(&point_a, CX_CURVE_EdBLS12);
    (void) cx_ecpoint_alloc(&sum, CX_CURVE_EdBLS12);

    // Init sum
    sum_init(&sum, &commitment_sum);

    // Transform field to bn
    field_to_big_int(commitment, &s);
    big_int_to_bn(&s, bn_x);
    for (size_t i = 0; i < (sizeof(bn_x) + 1) / 2; i++) {
        uint8_t inter              = bn_x[i];
        bn_x[i]                    = bn_x[sizeof(bn_x) - 1 - i];
        bn_x[sizeof(bn_x) - 1 - i] = inter;
    }

    size_t input_bit_index  = 0;
    size_t buffer_bit_index = 1;  // 1 bit to zero remains

    for (size_t i = 0; i < FIELD_MODULUS_BITS; i++) {
        if ((bn_x[input_bit_index / 8]) & (1 << (input_bit_index % 8))) {
            bhp_buffer[buffer_bit_index / 8] |= (1 << (buffer_bit_index % 8));
        }
        input_bit_index++;
        buffer_bit_index++;
    }

    // Adjust to chunk
    if (buffer_bit_index % BHP_CHUNK_SIZE) {
        buffer_bit_index += BHP_CHUNK_SIZE - (buffer_bit_index % BHP_CHUNK_SIZE);
    }

    // Process additions
    for (index = 0; index < buffer_bit_index; index += BHP_CHUNK_SIZE) {
        offset = 0;
        for (size_t i = 0; i < 3; i++) {
            if ((bhp_buffer[(index + i) / 8]) & (1 << ((index + i) % 8))) {
                offset += 1 << i;
            }
        }
        offset += (index / BHP_CHUNK_SIZE) * 8;

        a = (bn_group_t *) &bhp_512_lookup_base_light_bn[offset];
        (void) cx_ecpoint_init(&point_a, a->x, 32, a->y, 32);
        (void) cx_ecpoint_add(&sum, &sum, &point_a);
    }

    (void) cx_ecpoint_destroy(&point_a);
    sum_end(&sum, &digest);
    memcpy(hash, &digest.x, sizeof(field_t));
    group_println(&digest);
}

static const field_t test_commitment = {
    .big.u64 = {0x05db163f2e6e0219, 0x3881beb93bfb82c2, 0xe969a7d6fde4a1fe, 0x113b1b03f8c16c1d}
};

void bhp_512_test(void)
{
    field_t hash;
    bhp_512_hash_commitment(&test_commitment, &hash);
    field_println(&hash);
}
