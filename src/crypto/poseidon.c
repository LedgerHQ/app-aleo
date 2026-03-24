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

#include "os.h"
#include "field.h"
#include "scalar.h"
#include "poseidon_parameters.h"
#include "poseidon.h"

#define CAPACITY          (1)
#define MAX_RATE          (8)
#define SPONGE_STATE_SIZE (CAPACITY + MAX_RATE)

typedef enum {
    SPONGE_MODE_ABSORBING,
    SPONGE_MODE_SQUEEZING,
} duplex_sponge_mode_type_e;

typedef struct {
    duplex_sponge_mode_type_e type;
    uint8_t                   next_absorb_index;
    uint8_t                   next_squeeze_index;
} duplex_sponge_mode_t;

typedef struct {
    uint8_t               rate;
    poseidon_parameters_t parameters;
    field_t               state[SPONGE_STATE_SIZE];
    uint8_t               state_length;
    duplex_sponge_mode_t  mode;
} poseidon_sponge_t;

static const field_t F_DOMAIN_ALEO_POSEIDON_2 = {
    .big.u64 = {0x844cdc5dd65a2f88, 0x4e59b8a38ac312a2, 0xac5224f02e45b97a, 0x09bc808f890957b8}
};

static const field_t F_DOMAIN_ALEO_POSEIDON_4 = {
    .big.u64 = {0x956b0ceb63cb0a8b, 0x802e892134c2f208, 0x87e48c28ef0b2e6e, 0x0fdcef28655d9dc5}
};

static const field_t F_DOMAIN_ALEO_POSEIDON_8 = {
    .big.u64 = {0xad95ee067eacc090, 0x8a2db31db8c2b0d3, 0xde550d7c145e6855, 0x097266fb83d98488}
};

static poseidon_sponge_t sponge;

static int sponge_init(uint8_t rate)
{
    // Sanity check
    if (rate > MAX_RATE) {
        PRINTF("Poseidon rate is too high (%d)\n", rate);
        return -1;
    }
    sponge.rate         = rate;
    sponge.state_length = (CAPACITY + rate);

    // Init state
    for (uint8_t i = 0; i < sponge.state_length; i++) {
        memset(&sponge.state[i], 0, sizeof(field_t));
    }

    // init mode
    sponge.mode.type              = SPONGE_MODE_ABSORBING;
    sponge.mode.next_absorb_index = 0;

    // init parameters
    poseidon_parameters_init(&sponge.parameters, rate);

    // Sanity check
    if (sponge.state_length * (sponge.parameters.full_rounds + sponge.parameters.partial_rounds)
        > sponge.parameters.ark_size) {
        PRINTF("Poseidon ark parameters size is too low\n");
        return -1;
    }
    if (sponge.state_length * sponge.state_length > sponge.parameters.mds_size) {
        PRINTF("Poseidon mds parameters size is too low\n");
        return -1;
    }

    return 0;
}

static void apply_ark(uint8_t round_number)
{
    for (uint8_t i = 0; i < sponge.state_length; i++) {
        field_add_assign(&sponge.state[i],
                         &sponge.parameters.ark[round_number * sponge.state_length + i]);
    }
}

static void apply_s_box(bool is_full_round)
{
    if (is_full_round) {
        for (uint8_t i = 0; i < sponge.state_length; i++) {
            field_pow_assign(&sponge.state[i], sponge.parameters.alpha);
        }
    }
    else {
        field_pow_assign(&sponge.state[0], sponge.parameters.alpha);
    }
}

static void apply_mds(void)
{
    field_t new_state[SPONGE_STATE_SIZE];

    memset(new_state, 0, sizeof(field_t) * sponge.state_length);
    for (uint8_t i = 0; i < sponge.state_length; i++) {
        field_sum_of_products(sponge.state,
                              &sponge.parameters.mds[i * sponge.state_length],
                              sponge.state_length,
                              &new_state[i]);
    }
    memcpy(sponge.state, new_state, sizeof(field_t) * sponge.state_length);
}

static void sponge_permute(void)
{
    uint8_t num_rounds           = sponge.parameters.full_rounds + sponge.parameters.partial_rounds;
    uint8_t start_partial_rounds = sponge.parameters.full_rounds / 2;
    uint8_t end_partial_rounds   = start_partial_rounds + sponge.parameters.partial_rounds;

    for (uint8_t i = 0; i < num_rounds; i++) {
        bool is_full_round = true;
        if ((i >= start_partial_rounds) && (i < end_partial_rounds)) {
            is_full_round = false;
        }
        apply_ark(i);
        apply_s_box(is_full_round);
        apply_mds();
    }
}

static int absorb_internal(uint8_t rate_start, field_t *input, uint16_t input_length)
{
    uint8_t first_chunk_size = input_length;

    if ((sponge.rate - rate_start) < first_chunk_size) {
        first_chunk_size = sponge.rate - rate_start;
    }
    uint8_t num_elements_remaining = input_length - first_chunk_size;
    uint8_t total_num_chunks       = 1 + (num_elements_remaining / sponge.rate);
    if (num_elements_remaining % sponge.rate) {
        total_num_chunks += 1;
    }

    // FIRST CHUNK
    uint8_t chunk_length = first_chunk_size;

    // Sanity check
    if ((CAPACITY + rate_start + chunk_length) > SPONGE_STATE_SIZE) {
        return -1;
    }
    if (chunk_length > input_length) {
        return -1;
    }

    for (uint8_t i = 0; i < chunk_length; i++) {
        field_add_assign(&sponge.state[CAPACITY + rate_start + i], &input[i]);
    }

    if (total_num_chunks == 1) {
        sponge.mode.type              = SPONGE_MODE_ABSORBING;
        sponge.mode.next_absorb_index = rate_start + chunk_length;
        return 0;
    }
    else {
        sponge_permute();
    }
    rate_start = 0;

    // REST CHUNK
    uint8_t chunk_index = 0;
    for (chunk_index = 0; chunk_index < total_num_chunks - 1; chunk_index++) {
        uint8_t chunk_start = first_chunk_size + chunk_index * sponge.rate;
        chunk_length        = sponge.rate;
        if ((chunk_start + chunk_length) > input_length) {
            chunk_length = input_length - chunk_start;
        }

        // Sanity check
        if ((CAPACITY + rate_start + chunk_length) > SPONGE_STATE_SIZE) {
            return -1;
        }
        if ((chunk_start + chunk_length) > input_length) {
            return -1;
        }

        for (uint8_t i = 0; i < chunk_length; i++) {
            field_add_assign(&sponge.state[CAPACITY + rate_start + i], &input[chunk_start + i]);
        }

        if (chunk_index == total_num_chunks - 2) {
            sponge.mode.type              = SPONGE_MODE_ABSORBING;
            sponge.mode.next_absorb_index = rate_start + chunk_length;
            return 0;
        }
        else {
            sponge_permute();
        }
        rate_start = 0;
    }

    return 0;
}

static int squeeze_internal(uint8_t rate_start, field_t *output, uint16_t output_length)
{
    uint8_t first_chunk_size = output_length;

    if ((sponge.rate - rate_start) < first_chunk_size) {
        first_chunk_size = sponge.rate - rate_start;
    }
    uint8_t num_output_remaining = output_length - first_chunk_size;
    uint8_t total_num_chunks     = 1 + (num_output_remaining / sponge.rate);
    if (num_output_remaining % sponge.rate) {
        total_num_chunks += 1;
    }

    // FIRST CHUNK
    uint8_t chunk_length = first_chunk_size;

    // Sanity check
    if ((CAPACITY + rate_start + chunk_length) > SPONGE_STATE_SIZE) {
        return -1;
    }
    if (chunk_length > output_length) {
        return -1;
    }

    for (uint8_t i = 0; i < chunk_length; i++) {
        memcpy(&output[i], &sponge.state[CAPACITY + rate_start + i], sizeof(field_t));
    }

    if (total_num_chunks == 1) {
        sponge.mode.type               = SPONGE_MODE_SQUEEZING;
        sponge.mode.next_squeeze_index = rate_start + chunk_length;
        return 0;
    }
    else {
        sponge_permute();
    }
    rate_start = 0;

    // REST CHUNK
    chunk_length = output_length - first_chunk_size;

    // Sanity check
    if ((CAPACITY + rate_start + chunk_length) > SPONGE_STATE_SIZE) {
        return -1;
    }
    if (first_chunk_size + chunk_length > output_length) {
        return -1;
    }

    for (uint8_t i = 0; i < chunk_length; i++) {
        memcpy(&output[first_chunk_size + i],
               &sponge.state[CAPACITY + rate_start + i],
               sizeof(field_t));
    }

    if (total_num_chunks == 2) {
        sponge.mode.type               = SPONGE_MODE_SQUEEZING;
        sponge.mode.next_squeeze_index = rate_start + chunk_length;
        return 0;
    }
    else {
        sponge_permute();
    }

    return 0;
}

static int sponge_absorb(field_t *input, uint16_t input_length)
{
    if (sponge.mode.type == SPONGE_MODE_ABSORBING) {
        if (sponge.mode.next_absorb_index == sponge.rate) {
            sponge_permute();
            sponge.mode.next_absorb_index = 0;
        }
        return absorb_internal(sponge.mode.next_absorb_index, input, input_length);
    }
    else if (sponge.mode.type == SPONGE_MODE_SQUEEZING) {
        sponge_permute();
        return absorb_internal(0, input, input_length);
    }
    else {
        PRINTF("Bad poseidon sponge mode (%d)\n", sponge.mode.type);
    }

    return -1;
}

static int sponge_squeeze(field_t *output, uint16_t num_elements)
{
    if (!num_elements) {
        return -1;
    }

    memset(output, 0, sizeof(field_t) * num_elements);
    if (sponge.mode.type == SPONGE_MODE_ABSORBING) {
        sponge_permute();
        return squeeze_internal(0, output, num_elements);
    }
    else if (sponge.mode.type == SPONGE_MODE_SQUEEZING) {
        if (sponge.mode.next_squeeze_index == sponge.rate) {
            sponge_permute();
            sponge.mode.next_squeeze_index = 0;
        }
        return squeeze_internal(sponge.mode.next_squeeze_index, output, num_elements);
    }
    else {
        PRINTF("Bad poseidon sponge mode (%d)\n", sponge.mode.type);
    }

    return -1;
}

static int poseidon_hash_many(uint8_t  rate,
                              field_t *input,
                              size_t   input_length,
                              field_t *output,
                              size_t   num_output)
{
    field_t tmp[SPONGE_STATE_SIZE];

    // Sanity check
    if ((rate != 2) && (rate != 4) && (rate != 8)) {
        PRINTF("Bad poseidon rate (%d)\n", rate);
        return -1;
    }
    if (input_length < rate) {
        PRINTF("Bad poseidon input length vs rate (%d < %d)\n", input_length, rate);
        return -1;
    }
    if (num_output > SPONGE_STATE_SIZE) {
        PRINTF("Bad poseidon num output (%d)\n", num_output);
        return -1;
    }
    // init sponge
    if (sponge_init(rate) < 0) {
        return -1;
    }

    // Build preimage
    if (rate == 2) {
        memcpy(&input[0], &F_DOMAIN_ALEO_POSEIDON_2, sizeof(F_DOMAIN_ALEO_POSEIDON_2));
    }
    else if (rate == 4) {
        memcpy(&input[0], &F_DOMAIN_ALEO_POSEIDON_4, sizeof(F_DOMAIN_ALEO_POSEIDON_4));
    }
    else {
        memcpy(&input[0], &F_DOMAIN_ALEO_POSEIDON_8, sizeof(F_DOMAIN_ALEO_POSEIDON_8));
    }

    field_from_int(&input[1], input_length - rate);

    // Pad to zero if necessary
    memset(&input[2], 0, sizeof(field_t) * (rate - 2));

    if (sponge_absorb(input, input_length) < 0) {
        return -1;
    }
    if (sponge_squeeze(tmp, num_output) < 0) {
        return -1;
    }

    memcpy(output, tmp, sizeof(field_t) * num_output);

    return 0;
}

int hash_to_scalar_psd2(field_t *input, size_t input_length, scalar_t *r)
{
    int     status = 0;
    field_t output[1];

    status = poseidon_hash_many(2, input, input_length, output, 1);
    if (status == 0) {
        scalar_from_field_lossy(r, &output[0]);
    }

    return status;
}

int hash_to_scalar_psd4(field_t *input, size_t input_length, scalar_t *r)
{
    int     status = 0;
    field_t output[1];

    status = poseidon_hash_many(4, input, input_length, output, 1);
    if (status == 0) {
        scalar_from_field_lossy(r, &output[0]);
    }

    return status;
}

int hash_to_scalar_psd8(field_t *input, size_t input_length, scalar_t *r)
{
    int     status = 0;
    field_t output[1];

    status = poseidon_hash_many(8, input, input_length, output, 1);
    if (status == 0) {
        scalar_from_field_lossy(r, &output[0]);
    }

    return status;
}

int hash_psd2(field_t *input, size_t input_length, field_t *r)
{
    int     status = 0;
    field_t output[1];

    status = poseidon_hash_many(2, input, input_length, output, 1);
    if (status == 0) {
        memcpy(r, &output[0], sizeof(field_t));
    }

    return status;
}

int hash_psd4(field_t *input, size_t input_length, field_t *r)
{
    int     status = 0;
    field_t output[1];

    status = poseidon_hash_many(4, input, input_length, output, 1);
    if (status == 0) {
        memcpy(r, &output[0], sizeof(field_t));
    }

    return status;
}

int hash_psd8(field_t *input, size_t input_length, field_t *r)
{
    int     status = 0;
    field_t output[1];

    status = poseidon_hash_many(8, input, input_length, output, 1);
    if (status == 0) {
        memcpy(r, &output[0], sizeof(field_t));
    }

    return status;
}

int hash_many_psd8(field_t *input, size_t input_length, field_t *output, size_t output_length)
{
    return poseidon_hash_many(8, input, input_length, output, output_length);
}
