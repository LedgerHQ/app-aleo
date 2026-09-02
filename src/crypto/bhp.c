/*****************************************************************************
 *   Ledger App Aleo.
 *   (c) 2026 Ledger SAS.
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

#if 0
#include <stdint.h>   // uint*_t
#include <stddef.h>   // size_t
#include <stdbool.h>  // bool
#include <string.h>   // memmove

#include "os_utils.h"
#include "cx.h"

#if 0
#include "bhp_parameters.h"
#include "bhp.h"

#define BHP_1024_MAX_BITS_PER_ITERATION (1044)
#define BHP_1024_DOMAIN_SIZE_IN_BITS    (188)
static const uint8_t bhp_1024_domain[24]
    = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
       0xc0, 0xc2, 0xc4, 0xc0, 0xa8, 0x20, 0x21, 0x64, 0x6f, 0x6a, 0x23, 0x08};  // "AleoBHP1024"

#define BHP_512_MAX_BITS_PER_ITERATION (522)
#define BHP_512_DOMAIN_SIZE_IN_BITS    (188)
static const uint8_t bhp_512_domain[24]
    = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0xc0, 0xc4, 0xc8, 0xaa, 0x20, 0x21, 0x64, 0x6f, 0x6a, 0x23, 0x08};  // "AleoBHP512"

static const group_t bhp_512_domain_sum = {
    .x = {0xadc1debccef1edc2, 0x75798e0c7e3e4517, 0x28cbcb148b41e651, 0x06be6c247f339f53},
    .y = {0x9b0d612d008b4895, 0xd629bf3065662d3a, 0x88cbee7d2b1e3ce7, 0x1121fee88d131f7a}
};

static const compressed_group_t zero = {
    .compressed = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01},
    .sign       = 0
};

const field_t SERIAL_NUMBER_DOMAIN_2 = {
    .big.u64 = {0xf993d63c8bc0a4ad, 0xe22f8477532c74cc, 0x367fe5e2c9f74b96, 0x0309ea7b80fce820}
};

static uint8_t bhp_buffer[256];
static uint8_t bhp_buffer_bit_len;

static void sum_init(cx_ecpoint_t *sum)
{
    (void) cx_bn_lock(32, 0);
    (void) cx_ecpoint_alloc(sum, CX_CURVE_EdBLS12);
    (void) cx_ecpoint_decompress(sum, zero.compressed, 32, zero.sign);
}

static void sum_add_bn(cx_ecpoint_t *sum, bn_group_t *a)
{
    cx_ecpoint_t point_a;
    bigint_256_t s;
    uint8_t      bn_x[32];
    uint8_t      bn_y[32];

    (void) cx_ecpoint_alloc(&point_a, CX_CURVE_EdBLS12);
    (void) cx_ecpoint_init(&point_a, a->x, 32, a->y, 32);
    (void) cx_ecpoint_add(sum, sum, &point_a);

    group_t b;

    (void) cx_ecpoint_export(&point_a, bn_x, 32, bn_y, 32);
    bn_to_big_int(bn_x, &s);
    field_from_big_int(&b.x, &s);
    bn_to_big_int(bn_y, &s);
    field_from_big_int(&b.y, &s);
    group_println(&b);

    (void) cx_ecpoint_export(sum, bn_x, 32, bn_y, 32);
    bn_to_big_int(bn_x, &s);
    field_from_big_int(&b.x, &s);
    bn_to_big_int(bn_y, &s);
    field_from_big_int(&b.y, &s);
    group_println(&b);

    PRINTF("\n");

    (void) cx_ecpoint_destroy(&point_a);
}

static void sum_add_group(cx_ecpoint_t *sum, group_t *a)
{
    cx_ecpoint_t point_a;
    bigint_256_t s;
    uint8_t      bn_x[32];
    uint8_t      bn_y[32];

    field_to_big_int(&a->x, &s);
    big_int_to_bn(&s, bn_x);
    field_to_big_int(&a->y, &s);
    big_int_to_bn(&s, bn_y);

    (void) cx_ecpoint_alloc(&point_a, CX_CURVE_EdBLS12);
    (void) cx_ecpoint_init(&point_a, bn_x, 32, bn_y, 32);
    (void) cx_ecpoint_add(sum, sum, &point_a);

    group_t b;

    (void) cx_ecpoint_export(&point_a, bn_x, 32, bn_y, 32);
    bn_to_big_int(bn_x, &s);
    field_from_big_int(&b.x, &s);
    bn_to_big_int(bn_y, &s);
    field_from_big_int(&b.y, &s);
    group_println(&b);

    (void) cx_ecpoint_export(sum, bn_x, 32, bn_y, 32);
    bn_to_big_int(bn_x, &s);
    field_from_big_int(&b.x, &s);
    bn_to_big_int(bn_y, &s);
    field_from_big_int(&b.y, &s);
    group_println(&b);

    PRINTF("\n");

    (void) cx_ecpoint_destroy(&point_a);
}

static void sum_add_compressed_group(cx_ecpoint_t *sum, compressed_group_t *a)
{
    cx_ecpoint_t point_a;

    (void) cx_ecpoint_alloc(&point_a, CX_CURVE_EdBLS12);
    (void) cx_ecpoint_decompress(&point_a, a->compressed, 32, a->sign);

    (void) cx_ecpoint_add(sum, sum, &point_a);

    /*bigint_256_t s;
    group_t      b;
    uint8_t      bn_x[32];
    uint8_t      bn_y[32];

    (void) cx_ecpoint_export(&point_a, bn_x, 32, bn_y, 32);
    bn_to_big_int(bn_x, &s);
    field_from_big_int(&b.x, &s);
    bn_to_big_int(bn_y, &s);
    field_from_big_int(&b.y, &s);
    group_println(&b);

    (void) cx_ecpoint_export(sum, bn_x, 32, bn_y, 32);
    bn_to_big_int(bn_x, &s);
    field_from_big_int(&b.x, &s);
    bn_to_big_int(bn_y, &s);
    field_from_big_int(&b.y, &s);
    group_println(&b);

    PRINTF("\n");*/

    (void) cx_ecpoint_destroy(&point_a);
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

static void hash_uncompressed(uint8_t *input, size_t bit_len, group_t *g_sum)
{
    size_t       index       = 0;
    size_t       base_offset = 0;
    cx_ecpoint_t sum;

    PRINTF("bit_len %d\n", bit_len);
    sum_init(&sum);
    for (index = 0; index < bit_len; index += BHP_CHUNK_SIZE) {
        base_offset = (index / BHP_CHUNK_SIZE) * 8;
        for (size_t i = 0; i < 3; i++) {
            if ((bhp_buffer[(index + i) / 8]) & (1 << ((index + i) % 8))) {
                base_offset += 1 << i;
            }
        }
        sum_add_bn(&sum, &bhp_512_base_bn[base_offset]);
        // sum_add(&sum, &bhp_512_base_compressed[base_offset]);
        // sum_add_2(&sum, &bhp_1024_base_bn[base_offset]);
        // sum_add_3(&sum, &bhp_512_base[base_offset]);
    }

    sum_end(&sum, g_sum);
}

void bhp_512_hash_optim(uint8_t *input, size_t input_total_bit_len, field_t *hash)
{
    cx_ecpoint_t sum;
    cx_ecpoint_t point_a;
    bn_group_t  *a;
    size_t       index  = 0;
    size_t       offset = 0;

    size_t input_bit_index  = 0;
    size_t input_bit_length = 0;

    size_t buffer_bit_index = 0;

    group_t digest;

    bigint_256_t s;
    uint8_t      bn_x[32];
    uint8_t      bn_y[32];

    memset(bhp_buffer, 0, sizeof(bhp_buffer));

    (void) cx_bn_lock(32, 0);
    (void) cx_ecpoint_alloc(&point_a, CX_CURVE_EdBLS12);
    (void) cx_ecpoint_alloc(&sum, CX_CURVE_EdBLS12);

    field_to_big_int(&bhp_512_domain_sum.x, &s);
    big_int_to_bn(&s, bn_x);
    field_to_big_int(&bhp_512_domain_sum.y, &s);
    big_int_to_bn(&s, bn_y);

    (void) cx_ecpoint_init(&sum, bn_x, 32, bn_y, 32);

    buffer_bit_index = 0;
    input_bit_index  = 0;
    input_bit_length = 64;  // add length information size (64 bits)
    input_bit_length += input_total_bit_len;

    for (size_t i = 0; i < input_total_bit_len; i++) {
        if ((input[input_bit_index / 8]) & (1 << (input_bit_index % 8))) {
            bhp_buffer[buffer_bit_index / 8] |= (1 << (buffer_bit_index % 8));
            PRINTF("true, ");
        }
        else {
            PRINTF("false, ");
        }
        input_bit_index++;
        buffer_bit_index++;
    }
    PRINTF("\n");

    // Adjust to chunk
    if (buffer_bit_index % BHP_CHUNK_SIZE) {
        buffer_bit_index += BHP_CHUNK_SIZE - (buffer_bit_index % BHP_CHUNK_SIZE);
    }

    // debug
    group_t b;

    // Process additions
    for (index = 0; index < buffer_bit_index; index += BHP_CHUNK_SIZE) {
        offset = (index / BHP_CHUNK_SIZE) * 8;
        for (size_t i = 0; i < 3; i++) {
            if ((bhp_buffer[(index + i) / 8]) & (1 << ((index + i) % 8))) {
                offset += 1 << i;
            }
        }
        // debug
        (void) cx_ecpoint_export(&sum, bn_x, 32, bn_y, 32);
        bn_to_big_int(bn_x, &s);
        field_from_big_int(&b.x, &s);
        bn_to_big_int(bn_y, &s);
        field_from_big_int(&b.y, &s);
        group_println(&b);
        PRINTF("\n");

        a = &bhp_512_base_bn[offset];
        (void) cx_ecpoint_init(&point_a, a->x, 32, a->y, 32);
        (void) cx_ecpoint_add(&sum, &sum, &point_a);

        bn_to_big_int(a->x, &s);
        field_from_big_int(&b.x, &s);
        bn_to_big_int(a->y, &s);
        field_from_big_int(&b.y, &s);
        group_println(&b);
    }

    (void) cx_ecpoint_destroy(&point_a);
    sum_end(&sum, &digest);
    memcpy(hash, &digest.x, sizeof(field_t));
    group_println(&digest);
}

void bhp_512_hash(uint8_t *input, size_t input_total_bit_len, field_t *hash)
{
    size_t index  = 0;
    size_t offset = 0;

    size_t input_bit_index   = 0;
    size_t input_bit_length  = 0;
    size_t input_block_index = 0;
    size_t input_nb_of_block = 0;

    size_t buffer_bit_index = 0;

    group_t digest;

    input_nb_of_block = (input_total_bit_len) + (BHP_512_MAX_BITS_PER_ITERATION - 1);
    input_nb_of_block /= BHP_512_MAX_BITS_PER_ITERATION;
    if (!input_total_bit_len) {
        input_nb_of_block = 1;
    }

    for (input_block_index = 0; input_block_index < input_nb_of_block; input_block_index++) {
        PRINTF("input_block_index %d\n", input_block_index);
        memset(bhp_buffer, 0, sizeof(bhp_buffer));
        if (input_block_index == 0) {
            memcpy(bhp_buffer, bhp_512_domain, sizeof(bhp_512_domain));
            buffer_bit_index = BHP_512_DOMAIN_SIZE_IN_BITS;
            input_bit_index  = 0;
            input_bit_length = 0;
            if (input_total_bit_len) {
                input_bit_length = 64;
            }
        }
        else {
            // TODO
        }

        if ((input_bit_index + BHP_512_MAX_BITS_PER_ITERATION) <= (input_total_bit_len)) {
            input_bit_length += BHP_512_MAX_BITS_PER_ITERATION;
        }
        else {
            input_bit_length += (input_total_bit_len) % BHP_512_MAX_BITS_PER_ITERATION;
        }

        PRINTF("toto1  %d\n", buffer_bit_index);
        for (size_t i = 0; i < input_bit_length; i++) {
            if ((input[input_bit_index / 8]) & (1 << (input_bit_index % 8))) {
                bhp_buffer[buffer_bit_index / 8] |= (1 << (buffer_bit_index % 8));
                PRINTF("true, ");
            }
            else {
                PRINTF("false, ");
            }
            input_bit_index++;
            buffer_bit_index++;
        }
        PRINTF("\n");

        PRINTF("toto2  %d\n", buffer_bit_index);
        if (buffer_bit_index % BHP_CHUNK_SIZE) {
            buffer_bit_index += BHP_CHUNK_SIZE - (buffer_bit_index % BHP_CHUNK_SIZE);
            PRINTF("toto3  %d\n", buffer_bit_index);
        }

        hash_uncompressed(bhp_buffer, buffer_bit_index, &digest);
    }
    memcpy(hash, &digest.x, sizeof(field_t));
}

void bhp_1024_hash(uint8_t *input, size_t input_len, field_t *hash)
{
    size_t index  = 0;
    size_t offset = 0;

    size_t input_bit_index   = 0;
    size_t input_bit_length  = 0;
    size_t input_block_index = 0;
    size_t input_nb_of_block = 0;

    size_t buffer_bit_index = 0;

    group_t digest;

    input_nb_of_block = (input_len * 8) + (BHP_1024_MAX_BITS_PER_ITERATION - 1);
    input_nb_of_block /= BHP_1024_MAX_BITS_PER_ITERATION;

    for (input_block_index = 0; input_block_index < input_nb_of_block; input_block_index++) {
        memset(bhp_buffer, 0, sizeof(bhp_buffer));
        if (input_block_index == 0) {
            memcpy(bhp_buffer, bhp_1024_domain, sizeof(bhp_1024_domain));
            buffer_bit_index = BHP_1024_DOMAIN_SIZE_IN_BITS;
            input_bit_index  = 0;
        }
        else {
            // TODO
        }

        if ((input_bit_index + BHP_1024_MAX_BITS_PER_ITERATION) <= (input_len * 8)) {
            input_bit_length = BHP_1024_MAX_BITS_PER_ITERATION;
        }
        else {
            input_bit_length = (input_len * 8) % BHP_1024_MAX_BITS_PER_ITERATION;
        }

        for (size_t i = 0; i < input_bit_length; i++) {
            if ((input[input_bit_index / 8]) & (1 << (input_bit_index % 8))) {
                bhp_buffer[buffer_bit_index / 8] |= (1 << (buffer_bit_index % 8));
            }
            input_bit_index++;
            buffer_bit_index++;
        }

        if (buffer_bit_index % BHP_CHUNK_SIZE) {
            buffer_bit_index += BHP_CHUNK_SIZE - (buffer_bit_index % BHP_CHUNK_SIZE);
        }

        hash_uncompressed(bhp_buffer, buffer_bit_index, &digest);
    }
    memcpy(hash, &digest.x, sizeof(field_t));
}

static const uint8_t buf_test[72]
    = {0xfa, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x6c, 0x65, 0x6f, 0x53, 0x65, 0x72,
       0x69, 0x61, 0x6c, 0x4e, 0x75, 0x6d, 0x62, 0x65, 0x72, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x6c, 0x0c, 0xb2, 0x0d, 0x5c,
       0x13, 0xf2, 0x60, 0x81, 0x60, 0xd0, 0xda, 0x76, 0xfd, 0x60, 0x6f, 0xfc, 0x7c, 0xa0, 0x5c,
       0x2e, 0x8e, 0x7d, 0x72, 0x55, 0x28, 0xb2, 0xd6, 0xa7, 0x9e, 0x67, 0x01};
void bhp_test(void)
{
    uint16_t network_id            = 0;
    char     program_id_name[5]    = "token";
    char     program_id_network[4] = "aleo";
    char     function_name[8]      = "transfer";
    field_t  function_id;

    uint8_t offset = 8;
    uint8_t buffer[128];

    memset(buffer, 0, sizeof(buffer));

    // Network ID
    U2LE_ENCODE(&buffer[offset], 0, network_id);
    offset += 2;

    // Program ID name
    buffer[offset] = sizeof(program_id_name) * 8;
    offset += 1;
    memcpy(&buffer[offset], program_id_name, sizeof(program_id_name));
    offset += sizeof(program_id_name);

    // Program ID network
    buffer[offset] = sizeof(program_id_network) * 8;
    offset += 1;
    memcpy(&buffer[offset], program_id_network, sizeof(program_id_network));
    offset += sizeof(program_id_network);

    // Function name
    buffer[offset] = sizeof(function_name) * 8;
    offset += 1;
    memcpy(&buffer[offset], function_name, sizeof(function_name));
    offset += sizeof(function_name);

    PRINTF("offset = %d\n", offset - 8);
    U4LE_ENCODE(buffer, 0, (offset - 8) * 8);

    /*bhp_1024_hash(buffer, offset, &function_id);
    PRINTF("function_id : ");
    field_println(&function_id);*/

    field_t hash;
    // bhp_512_hash_optim(buf_test, 506, &hash);
    bhp_512_hash(buf_test, 0, &hash);

    bigint_256_t s;
    uint8_t      bn[32];

    field_to_big_int(&SERIAL_NUMBER_DOMAIN_2, &s);
    big_int_to_bn(&s, bn);
    bn_print(&bn);

#if 0
    uint8_t      bn_x[32];
    uint8_t      bn_y[32];
    bigint_256_t s;
    cx_ecpoint_t point;
    uint8_t      compressed[32];
    uint32_t     sign = 0;

    (void) cx_bn_lock(32, 0);
    (void) cx_ecpoint_alloc(&point, CX_CURVE_EdBLS12);

    for (int i = 0; i < BHP_512_NUM_WINDOWS * BHP_512_WINDOW_SIZE * 8; i++) {
        if ((i % (BHP_512_WINDOW_SIZE * 8)) == 0) {
            PRINTF("\n");
        }
        if (i % 8 == 0) {
            PRINTF("    // %02d %02d\n", i / (BHP_512_WINDOW_SIZE * 8), (i / (8)) % BHP_512_WINDOW_SIZE);
        }
        group_t *g = &bhp_512_base[i];
        field_to_big_int(&g->x, &s);
        big_int_to_bn(&s, bn_x);
        field_to_big_int(&g->y, &s);
        big_int_to_bn(&s, bn_y);
#if 0
        PRINTF("    {.x = {");
        for (int j = 0; j < 32; j++) {
            if (j == 16) {
                PRINTF("\n           ");
            }
            PRINTF("0x%02x", bn_x[j]);
            if (j < 31) {
                PRINTF(", ");
            }
        }
        PRINTF("},\n");
        PRINTF("     .y = {");
        for (int j = 0; j < 32; j++) {
            if (j == 16) {
                PRINTF("\n           ");
            }
            PRINTF("0x%02x", bn_y[j]);
            if (j < 31) {
                PRINTF(", ");
            }
        }
        PRINTF("},},\n");
#endif
#if 1
        (void) cx_ecpoint_init(&point, bn_x, 32, bn_y, 32);
        (void) cx_ecpoint_compress(&point, compressed, 32, &sign);
        PRINTF("    {.compressed = {");
        for (int j = 0; j < 32; j++) {
                        if (j == 16) {
                                PRINTF("\n                    ");
                        }
            PRINTF("0x%02x", compressed[j]);
            if (j < 31) {
                PRINTF(", ");
            }
        }
        PRINTF("},\n");
        PRINTF("     .sign = %d},\n", sign);
#endif
	}
#endif

#if 0
    uint8_t      bn_x[32];
    uint8_t      bn_y[32];
    bigint_256_t s;
    cx_ecpoint_t point;
    uint8_t      compressed[32];
    uint32_t     sign = 0;

    (void) cx_bn_lock(32, 0);
    (void) cx_ecpoint_alloc(&point, CX_CURVE_EdBLS12);
    for (int i = 0; i < 8 * 54 * 8; i++) {
        if ((i % (54 * 8)) == 0) {
            PRINTF("\n");
        }
        if (i % 8 == 0) {
            PRINTF("    // %02d %02d\n", i / (54 * 8), (i / (8)) % 54);
        }
        group_t *g = &bhp_1024_base[i];
        field_to_big_int(&g->x, &s);
        big_int_to_bn(&s, bn_x);
        field_to_big_int(&g->y, &s);
        big_int_to_bn(&s, bn_y);

        PRINTF("    {.x = {");
        for (int j = 0; j < 32; j++) {
            if (j == 16) {
                PRINTF("\n           ");
            }
            PRINTF("0x%02x", bn_x[j]);
            if (j < 31) {
                PRINTF(", ");
            }
        }
        PRINTF("},\n");
        PRINTF("     .y = {");
        for (int j = 0; j < 32; j++) {
            if (j == 16) {
                PRINTF("\n           ");
            }
            PRINTF("0x%02x", bn_y[j]);
            if (j < 31) {
                PRINTF(", ");
            }
        }
        PRINTF("},},\n");

        /*(void) cx_ecpoint_init(&point, bn_x, 32, bn_y, 32);
        (void) cx_ecpoint_compress(&point, compressed, 32, &sign);
        PRINTF("    {.compressed = {");
        for (int j = 0; j < 32; j++) {
                        if (j == 16) {
                                PRINTF("\n                    ");
                        }
            PRINTF("0x%02x", compressed[j]);
            if (j < 31) {
                PRINTF(", ");
            }
        }
        PRINTF("},\n");
        PRINTF("     .sign = %d},\n", sign);*/
    }

    (void) cx_ecpoint_destroy(&point);
    (void) cx_bn_unlock();
#endif
}

#endif
#endif