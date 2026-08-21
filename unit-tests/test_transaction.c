#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <cmocka.h>

#include "os.h"
#include "tx.h"

global_ctx_t G_context;

static void test_tx_extract(void** state)
{
    (void)state;
    memset(&G_context, 0, sizeof(G_context));

    // tx_extract_intent
    uint8_t payload[161]
        = "\x01\x01\x28\x02\x01\x01\x81\xB0\x04\x00\x00\x01\xF4\x81\xB1\x04\x00\x00\x00\x64\x81\xB2"
        "\x0A\x66\x65\x65\x5F\x70\x75\x62\x6C\x69\x63\x81\xB3\x0C\x63\x72\x65\x64\x69\x74\x73\x2E"
        "\x61\x6C\x65\x6F\x81\xB4\x6E\x01\x01\x29\x02\x01\x01\x81\xC3\x02\x00\x00\x81\xB5\x0C\x63"
        "\x72\x65\x64\x69\x74\x73\x2E\x61\x6C\x65\x6F\x81\xB6\x0F\x74\x72\x61\x6E\x73\x66\x65\x72"
        "\x5F\x70\x75\x62\x6C\x69\x63\x81\xB7\x01\x02\x81\xB9\x03\x01\x00\x00\x81\xB8\x20\x82\x48"
        "\xD5\xE8\x5A\xC4\xC1\x23\x46\xF8\x45\x8B\xD9\x39\xF1\xCE\x25\xAE\x03\xE9\xC6\xCB\xC8\x86"
        "\x28\x6D\xF1\x61\x63\x0A\x75\x0C\x81\xB9\x03\x01\x00\x0C\x81\xB8\x08\xE8\x03\x00\x00\x00"
        "\x00\x00\x00\x81\xBA\x01\x00";
    buffer_t buffer;
    buffer.ptr = payload;
    buffer.size = sizeof(payload);
    buffer.offset = 0;
    assert_int_equal(tx_extract_intent(&buffer), 0);

    uint8_t fee_function_name[73]
        = "\x81\xb2\x46\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69"
        "\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74"
        "\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73"
        "\x63\x72\x65\x64\x69\x74\x73";
    buffer.ptr = fee_function_name;
    buffer.size = sizeof(fee_function_name);
    buffer.offset = 0;
    assert_int_equal(tx_extract_intent(&buffer), -1);

    uint8_t fee_program_id[73]
        = "\x81\xb3\x46\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69"
        "\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74"
        "\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73"
        "\x63\x72\x65\x64\x69\x74\x73";
    buffer.ptr = fee_program_id;
    buffer.size = sizeof(fee_program_id);
    buffer.offset = 0;
    assert_int_equal(tx_extract_intent(&buffer), -1);

    // tx_extract_prepared_request
    uint8_t program_id[73]
        = "\x81\xb5\x46\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69"
        "\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74"
        "\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73"
        "\x63\x72\x65\x64\x69\x74\x73";
    buffer.ptr = program_id;
    buffer.size = sizeof(program_id);
    buffer.offset = 0;
    assert_int_equal(
        tx_extract_prepared_request(&buffer, &G_context.sign_transaction_datas.prepared_request),
        -1);

    uint8_t function_name[73]
        = "\x81\xb6\x46\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69"
        "\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74"
        "\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73"
        "\x63\x72\x65\x64\x69\x74\x73";
    buffer.ptr = function_name;
    buffer.size = sizeof(function_name);
    buffer.offset = 0;
    assert_int_equal(
        tx_extract_prepared_request(&buffer, &G_context.sign_transaction_datas.prepared_request),
        -1);

    uint8_t input_value[144]
        = "\x81\xb8\x81\x8c\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64"
        "\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69"
        "\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74"
        "\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73"
        "\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63"
        "\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72"
        "\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73";
    buffer.ptr = input_value;
    buffer.size = sizeof(input_value);
    buffer.offset = 0;
    assert_int_equal(
        tx_extract_prepared_request(&buffer, &G_context.sign_transaction_datas.prepared_request),
        -1);

    uint8_t input_values[78]
        = "\x81\xb8\x01\x01\x81\xb8\x01\x02\x81\xb8\x01\x03\x81\xb8\x01\x04\x81\xb8\x01\x05\x81\xb8"
        "\x01\x06\x81\xb8\x01\x07\x81\xb8\x01\x08\x81\xb8\x01\x09\x81\xb8\x01\x0a\x81\xb8\x01\x0b"
        "\x81\xb8\x01\x0c\x81\xb8\x01\x0d\x81\xb8\x01\x0e\x81\xb8\x01\x0f\x81\xb8\x01\x10\x81\xb8"
        "\x01\x11";
    buffer.ptr = input_values;
    buffer.size = sizeof(input_values);
    buffer.offset = 0;
    assert_int_equal(
        tx_extract_prepared_request(&buffer, &G_context.sign_transaction_datas.prepared_request),
        -1);

    uint8_t input_type[144]
        = "\x81\xb9\x81\x8c\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64"
        "\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69"
        "\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74"
        "\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73"
        "\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63"
        "\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72"
        "\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73";
    buffer.ptr = input_type;
    buffer.size = sizeof(input_type);
    buffer.offset = 0;
    assert_int_equal(
        tx_extract_prepared_request(&buffer, &G_context.sign_transaction_datas.prepared_request),
        -1);

    uint8_t input_types[78]
        = "\x81\xb9\x01\x01\x81\xb9\x01\x02\x81\xb9\x01\x03\x81\xb9\x01\x04\x81\xb9\x01\x05\x81\xb9"
        "\x01\x06\x81\xb9\x01\x07\x81\xb9\x01\x08\x81\xb9\x01\x09\x81\xb9\x01\x0a\x81\xb9\x01\x0b"
        "\x81\xb9\x01\x0c\x81\xb9\x01\x0d\x81\xb9\x01\x0e\x81\xb9\x01\x0f\x81\xb9\x01\x10\x81\xb9"
        "\x01\x11";
    buffer.ptr = input_types;
    buffer.size = sizeof(input_types);
    buffer.offset = 0;
    assert_int_equal(
        tx_extract_prepared_request(&buffer, &G_context.sign_transaction_datas.prepared_request),
        -1);

    uint8_t program_checksum[35]
        = "\x81\xc4\x20\x01\x81\xb9\x01\x02\x81\xb9\x01\x03\x81\xb9\x01\x04\x81\xb9\x01\x05\x81\xb9"
        "\x01\x06\x81\xb9\x01\x07\x81\xb9\x01\x08\x81\xb9\x01";
    buffer.ptr = program_checksum;
    buffer.size = sizeof(program_checksum);
    buffer.offset = 0;
    assert_int_equal(
        tx_extract_prepared_request(&buffer, &G_context.sign_transaction_datas.prepared_request),
        0);

    uint8_t program_checksum_2[34]
        = "\x81\xc4\x1f\x01\x81\xb9\x01\x02\x81\xb9\x01\x03\x81\xb9\x01\x04\x81\xb9\x01\x05\x81\xb9"
        "\x01\x06\x81\xb9\x01\x07\x81\xb9\x01\x08\x81\xb9";
    buffer.ptr = program_checksum_2;
    buffer.size = sizeof(program_checksum_2);
    buffer.offset = 0;
    assert_int_equal(
        tx_extract_prepared_request(&buffer, &G_context.sign_transaction_datas.prepared_request),
        -1);
}

static void test_tx_parse(void** state)
{
    (void)state;
    memset(&G_context, 0, sizeof(G_context));
    tx_t tx;

    sign_transaction_datas_t datas_public = {
        .max_base_fee = 100,
        .max_priority_fee = 500,
        .fee_function_name_length = 10,
        .fee_function_name = "fee_public",
        .fee_program_id_length = 12,
        .fee_program_id = "credits.aleo",
        .prepared_request = {
            .program_id_length = 12,
            .program_id = "credits.aleo",
            .function_name_length = 15,
            .function_name = "transfer_public",
            .inputs_count = 2,
            .inputs
            = {
                {
                    .value_length = 32,
                    .value
                    = (uint8_t*)"\x82\x48\xd5\xe8\x5a\xc4\xc1\x23\x46\xf8\x45\x8b\xd9\x39\xf1\xce"
                    "\x25\xae\x03\xe9\xc6\xcb\xc8\x86\x28\x6d\xf1\x61\x63\x0a\x75\x0c",
                    .type_length = 3,
                    .type = (uint8_t*)"\x01\x00\x00"
                },
                {
                    .value_length = 8,
                    .value = (uint8_t*)"\xe8\x03\x00\x00\x00\x00\x00\x00",
                    .type_length = 3,
                    .type = (uint8_t*)"\x01\x00\x0c"
                }
            },
        }
    };

    datas_public.prepared_request.inputs_count = 1;
    assert_int_equal(tx_parse(&datas_public, &tx), -1);
    datas_public.prepared_request.inputs_count = 2;

    // get_u64
    datas_public.prepared_request.inputs[1].type_length = 2;
    assert_int_equal(tx_parse(&datas_public, &tx), -1);
    datas_public.prepared_request.inputs[1].type_length = 3;

    uint8_t type_1[3] = "\x01\x00\x0c";
    uint8_t type_2[3] = "\x02\x00\x0c";
    uint8_t type_3[3] = "\x01\x01\x0c";
    datas_public.prepared_request.inputs[1].type = type_2;
    assert_int_equal(tx_parse(&datas_public, &tx), -1);
    datas_public.prepared_request.inputs[1].type = type_3;
    assert_int_equal(tx_parse(&datas_public, &tx), -1);
    datas_public.prepared_request.inputs[1].type = type_1;

    datas_public.prepared_request.inputs[0].type_length = 2;
    assert_int_equal(tx_parse(&datas_public, &tx), -1);
    datas_public.prepared_request.inputs[0].type_length = 3;
    uint8_t type_11[3] = "\x01\x00\x00";
    uint8_t type_12[3] = "\x02\x00\x00";
    uint8_t type_13[3] = "\x01\x01\x00";
    datas_public.prepared_request.inputs[0].type = type_12;
    assert_int_equal(tx_parse(&datas_public, &tx), -1);
    datas_public.prepared_request.inputs[0].type = type_13;
    assert_int_equal(tx_parse(&datas_public, &tx), -1);
    datas_public.prepared_request.inputs[0].type = type_11;

    char program_id[12] = "credits.aleo";
    datas_public.prepared_request.program_id = NULL;
    assert_int_equal(tx_parse(&datas_public, &tx), -1);
    datas_public.prepared_request.program_id = program_id;

    char function_name[15] = "transfer_public";
    datas_public.prepared_request.function_name = NULL;
    assert_int_equal(tx_parse(&datas_public, &tx), -1);
    datas_public.prepared_request.function_name = function_name;

    char program_id_2[12] = "credots.aleo";
    datas_public.prepared_request.program_id = program_id_2;
    assert_int_equal(tx_parse(&datas_public, &tx), -1);
    datas_public.prepared_request.program_id = program_id;

    char function_name_2[15] = "tronsfer_public";
    datas_public.prepared_request.function_name = function_name_2;
    assert_int_equal(tx_parse(&datas_public, &tx), -1);
    datas_public.prepared_request.function_name = function_name;

    assert_int_equal(tx_parse(&datas_public, &tx), 0);

    char function_name_3[26] = "transfer_public_to_private";
    datas_public.prepared_request.function_name_length = sizeof(function_name_3);
    datas_public.prepared_request.function_name = function_name_3;
    datas_public.prepared_request.inputs[0].type = type_12;
    assert_int_equal(tx_parse(&datas_public, &tx), 0);
    datas_public.prepared_request.inputs[0].type = type_11;
    assert_int_equal(tx_parse(&datas_public, &tx), -1);

    const uint8_t hash_record_c[96]
        = "\xf4\x69\x19\x61\x50\x7b\x8f\x32\x92\xaf\x47\xac\x64\xdf\x59\xf7"
        "\xc4\x39\xf6\xb2\x48\xa9\x55\x10\xfa\x95\xcc\x96\x25\xe7\xfd\x07"
        "\x2a\xe1\x18\xb5\x2d\x46\xd6\x0b\x96\x63\x06\x72\x73\x3d\x25\x2c"
        "\xa9\x3f\xbb\x8d\x56\xd5\x26\x1c\x0c\x4c\xbb\x8c\xf2\x92\x5d\x05"
        "\xd9\x75\x64\x3e\x43\x24\x5c\x7a\xf2\x8d\xb2\xa8\x5b\x54\x59\xb4"
        "\xb7\x7b\x38\x2a\x0c\x74\x33\x0c\x1d\x53\x7a\xa9\x22\xe1\x7c\x00";
    uint8_t hash_record[96];
    sign_transaction_datas_t datas_private = {
        .max_base_fee = 100,
        .max_priority_fee = 500,
        .fee_function_name_length = 11,
        .fee_function_name = "fee_private",
        .fee_program_id_length = 12,
        .fee_program_id = "credits.aleo",
        .prepared_request
        = {
            .program_id_length = 12,
            .program_id = "credits.aleo",
            .function_name_length = 16,
            .function_name = "transfer_private",
            .inputs_count = 3,
            .inputs
            = {
                {
                    .value_length = 96,
                    .value = hash_record,
                    .type_length = 9,
                    .type = (uint8_t*)"\x03\x07\x63\x72\x65\x64\x69\x74\x73"
                },
                {
                    .value_length = 32,
                    .value
                    = (uint8_t*)"\x82\x48\xd5\xe8\x5a\xc4\xc1\x23\x46\xf8\x45\x8b\xd9\x39\xf1\xce"
                    "\x25\xae\x03\xe9\xc6\xcb\xc8\x86\x28\x6d\xf1\x61\x63\x0a\x75\x0c",
                    .type_length = 3,
                    .type = (uint8_t*)"\x02\x00\x00"
                },
                {
                    .value_length = 8,
                    .value = (uint8_t*)"\xe8\x03\x00\x00\x00\x00\x00\x00",
                    .type_length = 3,
                    .type = (uint8_t*)"\x02\x00\x0c"
                }
            }
        }
    };

    memcpy(hash_record, hash_record_c, 96);
    assert_int_equal(tx_parse(&datas_private, &tx), 0);

    datas_private.prepared_request.inputs[2].type = type_1;
    assert_int_equal(tx_parse(&datas_private, &tx), -1);
    datas_private.prepared_request.inputs[2].type = type_2;

    char function_name_4[26] = "transfer_private_to_public";
    datas_private.prepared_request.function_name_length = sizeof(function_name_4);
    datas_private.prepared_request.function_name = function_name_4;
    datas_private.prepared_request.inputs[1].type = type_11;
    datas_private.prepared_request.inputs[2].type = type_1;
    assert_int_equal(tx_parse(&datas_private, &tx), 0);

    char function_name_5[11] = "fee_private";
    datas_private.prepared_request.function_name_length = sizeof(function_name_5);
    datas_private.prepared_request.function_name = function_name_5;

    sign_transaction_datas_t datas_private_2 = {
        .max_base_fee = 100,
        .max_priority_fee = 500,
        .fee_function_name_length = 11,
        .fee_function_name = "fee_private",
        .fee_program_id_length = 12,
        .fee_program_id = "credits.aleo",
        .prepared_request
        = {
            .program_id_length = 12,
            .program_id = "credits.aleo",
            .function_name_length = 11,
            .function_name = "fee_private",
            .inputs_count = 4,
            .inputs
            = {
                {
                    .value_length = 96,
                    .value = hash_record,
                    .type_length = 9,
                    .type = (uint8_t*)"\x03\x07\x63\x72\x65\x64\x69\x74\x73"
                },
                {
                    .value_length = 8,
                    .value = (uint8_t*)"\xe8\x03\x00\x00\x00\x00\x00\x00",
                    .type_length = 3,
                    .type = (uint8_t*)"\x01\x00\x0c"
                },
                {
                    .value_length = 8,
                    .value = (uint8_t*)"\x64\x00\x00\x00\x00\x00\x00\x00",
                    .type_length = 3,
                    .type = (uint8_t*)"\x01\x00\x0c"
                },
                {
                    .value_length = 32,
                    .value
                    = (uint8_t*)"\x82\x48\xd5\xe8\x5a\xc4\xc1\x23\x46\xf8\x45\x8b\xd9\x39\xf1\xce"
                    "\x25\xae\x03\xe9\xc6\xcb\xc8\x86\x28\x6d\xf1\x61\x63\x0a\x75\x0c",
                    .type_length = 3,
                    .type = (uint8_t*)"\x01\x00\x02"
                }
            }
        }
    };

    assert_int_equal(tx_parse(&datas_private_2, &tx), 0);

    sign_transaction_datas_t datas_private_3 = {
        .max_base_fee = 100,
        .max_priority_fee = 500,
        .fee_function_name_length = 10,
        .fee_function_name = "fee_public",
        .fee_program_id_length = 12,
        .fee_program_id = "credits.aleo",
        .prepared_request
        = {
            .program_id_length = 12,
            .program_id = "credits.aleo",
            .function_name_length = 10,
            .function_name = "fee_public",
            .inputs_count = 3,
            .inputs
            = {
                {
                    .value_length = 8,
                    .value = (uint8_t*)"\xe8\x03\x00\x00\x00\x00\x00\x00",
                    .type_length = 3,
                    .type = (uint8_t*)"\x01\x00\x0c"
                },
                {
                    .value_length = 8,
                    .value = (uint8_t*)"\x64\x00\x00\x00\x00\x00\x00\x00",
                    .type_length = 3,
                    .type = (uint8_t*)"\x01\x00\x0c"
                },
                {
                    .value_length = 32,
                    .value
                    = (uint8_t*)"\x82\x48\xd5\xe8\x5a\xc4\xc1\x23\x46\xf8\x45\x8b\xd9\x39\xf1\xce"
                    "\x25\xae\x03\xe9\xc6\xcb\xc8\x86\x28\x6d\xf1\x61\x63\x0a\x75\x0c",
                    .type_length = 3,
                    .type = (uint8_t*)"\x01\x00\x02"
                }
            }
        }
    };

    assert_int_equal(tx_parse(&datas_private_3, &tx), 0);

    sign_transaction_datas_t datas_batch_private = {
        .max_base_fee = 100,
        .max_priority_fee = 500,
        .fee_function_name_length = 11,
        .fee_function_name = "fee_private",
        .fee_program_id_length = 12,
        .fee_program_id = "credits.aleo",
        .prepared_request
        = {
            .program_id_length = 13,
            .program_id = "ldg_p_28.aleo",
            .function_name_length = 18,
            .function_name = "transfer_private_2",
            .inputs_count = 4,
            .inputs
            = {
                {
                    .value_length = 96,
                    .value = hash_record,
                    .type_length = 9,
                    .type = (uint8_t*)"\x03\x07\x63\x72\x65\x64\x69\x74\x73"
                },
                {
                    .value_length = 96,
                    .value = hash_record,
                    .type_length = 9,
                    .type = (uint8_t*)"\x03\x07\x63\x72\x65\x64\x69\x74\x73"
                },
                {
                    .value_length = 32,
                    .value
                    = (uint8_t*)"\x82\x48\xd5\xe8\x5a\xc4\xc1\x23\x46\xf8\x45\x8b\xd9\x39\xf1\xce"
                    "\x25\xae\x03\xe9\xc6\xcb\xc8\x86\x28\x6d\xf1\x61\x63\x0a\x75\x0c",
                    .type_length = 3,
                    .type = (uint8_t*)"\x02\x00\x00"
                },
                {
                    .value_length = 8,
                    .value = (uint8_t*)"\xe8\x03\x00\x00\x00\x00\x00\x00",
                    .type_length = 3,
                    .type = (uint8_t*)"\x02\x00\x0c"
                }
            }
        }
    };

    assert_int_equal(tx_parse(&datas_batch_private, &tx), 0);

    datas_batch_private.prepared_request.inputs[2].type = type_1;
    assert_int_equal(tx_parse(&datas_batch_private, &tx), -1);

    sign_transaction_datas_t datas_batch_private_to_public = {
        .max_base_fee = 100,
        .max_priority_fee = 500,
        .fee_function_name_length = 11,
        .fee_function_name = "fee_private",
        .fee_program_id_length = 12,
        .fee_program_id = "credits.aleo",
        .prepared_request = {
            .program_id_length = 15,
            .program_id = "ldg_p2p_28.aleo",
            .function_name_length = 28,
            .function_name = "transfer_private_to_public_2",
            .inputs_count = 3,
            .inputs = {
                {
                    .value_length = 96,
                    .value = hash_record,
                    .type_length = 1,
                    .type = (uint8_t*)"\x04"
                },
                {
                    .value_length = 96,
                    .value = hash_record,
                    .type_length = 1,
                    .type = (uint8_t*)"\x04"
                },
                {
                    .value_length = 8,
                    .value = (uint8_t*)"\xe8\x03\x00\x00\x00\x00\x00\x00",
                    .type_length = 3,
                    .type = (uint8_t*)"\x01\x00\x0c"
                }
            }
        }
    };

    assert_int_equal(tx_parse(&datas_batch_private_to_public, &tx), 0);

    datas_batch_private_to_public.prepared_request.inputs[2].type = type_2;
    assert_int_equal(tx_parse(&datas_batch_private_to_public, &tx), -1);
}

static void test_tx_token_parse(void** state)
{
    (void)state;
    memset(&G_context, 0, sizeof(G_context));
    tx_t tx;

    sign_transaction_datas_t datas_token_public = {
        .max_base_fee = 100,
        .max_priority_fee = 500,
        .fee_function_name_length = 10,
        .fee_function_name = "fee_public",
        .fee_program_id_length = 12,
        .fee_program_id = "credits.aleo",
        .prepared_request = {
            .program_id_length = 20,
            .program_id = "usad_stablecoin.aleo",
            .function_name_length = 15,
            .function_name = "transfer_public",
            .inputs_count = 2,
            .inputs
            = {
                {
                    .value_length = 32,
                    .value
                    = (uint8_t*)"\x82\x48\xd5\xe8\x5a\xc4\xc1\x23\x46\xf8\x45\x8b\xd9\x39\xf1\xce"
                    "\x25\xae\x03\xe9\xc6\xcb\xc8\x86\x28\x6d\xf1\x61\x63\x0a\x75\x0c",
                    .type_length = 3,
                    .type = (uint8_t*)"\x01\x00\x00"
                },
                {
                    .value_length = 16,
                    .value
                    = (uint8_t*)"\xe8\x03\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
                    .type_length = 3,
                    .type = (uint8_t*)"\x01\x00\x0d"
                }
            },
        }
    };

    assert_int_equal(tx_parse(&datas_token_public, &tx), 0);

    datas_token_public.prepared_request.inputs[1].type_length = 2;
    assert_int_equal(tx_parse(&datas_token_public, &tx), -1);
    datas_token_public.prepared_request.inputs[1].type_length = 3;

    uint8_t type_1[3] = "\x01\x00\x0d";
    uint8_t type_2[3] = "\x02\x00\x0d";
    uint8_t type_3[3] = "\x01\x01\x0d";
    datas_token_public.prepared_request.inputs[1].type = type_2;
    assert_int_equal(tx_parse(&datas_token_public, &tx), -1);
    datas_token_public.prepared_request.inputs[1].type = type_3;
    assert_int_equal(tx_parse(&datas_token_public, &tx), -1);
    datas_token_public.prepared_request.inputs[1].type = type_1;

    uint8_t type_11[3] = "\x01\x00\x00";
    uint8_t type_12[3] = "\x02\x00\x00";
    char function_name_3[26] = "transfer_public_to_private";
    datas_token_public.prepared_request.function_name_length = sizeof(function_name_3);
    datas_token_public.prepared_request.function_name = function_name_3;
    datas_token_public.prepared_request.inputs[0].type = type_12;
    assert_int_equal(tx_parse(&datas_token_public, &tx), 0);
    datas_token_public.prepared_request.inputs[0].type = type_11;
    assert_int_equal(tx_parse(&datas_token_public, &tx), -1);

    const uint8_t hash_record_c[96]
        = "\xf4\x69\x19\x61\x50\x7b\x8f\x32\x92\xaf\x47\xac\x64\xdf\x59\xf7"
        "\xc4\x39\xf6\xb2\x48\xa9\x55\x10\xfa\x95\xcc\x96\x25\xe7\xfd\x07"
        "\x2a\xe1\x18\xb5\x2d\x46\xd6\x0b\x96\x63\x06\x72\x73\x3d\x25\x2c"
        "\xa9\x3f\xbb\x8d\x56\xd5\x26\x1c\x0c\x4c\xbb\x8c\xf2\x92\x5d\x05"
        "\xd9\x75\x64\x3e\x43\x24\x5c\x7a\xf2\x8d\xb2\xa8\x5b\x54\x59\xb4"
        "\xb7\x7b\x38\x2a\x0c\x74\x33\x0c\x1d\x53\x7a\xa9\x22\xe1\x7c\x00";
    const uint8_t merkle_proof_c[32 * 32] = {0};
    uint8_t hash_record[96];
    uint8_t merkle_proof[32 * 32];
    sign_transaction_datas_t datas_token_private = {
        .max_base_fee = 100,
        .max_priority_fee = 500,
        .fee_function_name_length = 11,
        .fee_function_name = "fee_private",
        .fee_program_id_length = 12,
        .fee_program_id = "credits.aleo",
        .prepared_request
        = {
            .program_id_length = 20,
            .program_id = "usad_stablecoin.aleo",
            .function_name_length = 16,
            .function_name = "transfer_private",
            .inputs_count = 4,
            .inputs = {
                {
                    .value_length = 32,
                    .value
                    = (uint8_t*)"\x82\x48\xd5\xe8\x5a\xc4\xc1\x23\x46\xf8\x45\x8b\xd9\x39\xf1\xce"
                    "\x25\xae\x03\xe9\xc6\xcb\xc8\x86\x28\x6d\xf1\x61\x63\x0a\x75\x0c",
                    .type_length = 3,
                    .type = (uint8_t*)"\x02\x00\x00"
                },
                {
                    .value_length = 16,
                    .value
                    = (uint8_t*)"\xe8\x03\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
                    .type_length = 3,
                    .type = (uint8_t*)"\x02\x00\x0d"
                },
                {
                    .value_length = 96,
                    .value = hash_record,
                    .type_length = 9,
                    .type = (uint8_t*)"\x03\x07\x63\x72\x65\x64\x69\x74\x73"
                },
                {
                    .value_length = 32 * 32,
                    .value = merkle_proof,
                    .type_length = 3,
                    .type = (uint8_t*)"\x02\x01\x00"
                },
            }
        }
    };

    memcpy(hash_record, hash_record_c, sizeof(hash_record));
    memcpy(merkle_proof, merkle_proof_c, sizeof(merkle_proof));
    assert_int_equal(tx_parse(&datas_token_private, &tx), 0);

    char function_name_4[26] = "transfer_private_to_public";
    datas_token_private.prepared_request.function_name_length = sizeof(function_name_4);
    datas_token_private.prepared_request.function_name = function_name_4;
    datas_token_private.prepared_request.inputs[0].type = type_11;
    datas_token_private.prepared_request.inputs[1].type = type_1;
    assert_int_equal(tx_parse(&datas_token_private, &tx), 0);
    datas_token_private.prepared_request.inputs[0].type = type_12;
    assert_int_equal(tx_parse(&datas_token_private, &tx), -1);

    sign_transaction_datas_t datas_token_batch_private = {
        .max_base_fee = 100,
        .max_priority_fee = 500,
        .fee_function_name_length = 11,
        .fee_function_name = "fee_private",
        .fee_program_id_length = 12,
        .fee_program_id = "credits.aleo",
        .prepared_request
        = {
            .program_id_length = 18,
            .program_id = "ldg_usad_p_28.aleo",
            .function_name_length = 18,
            .function_name = "transfer_private_2",
            .inputs_count = 5,
            .inputs = {
                {
                    .value_length = 96,
                    .value = hash_record,
                    .type_length = 9,
                    .type = (uint8_t*)"\x04"
                },
                {
                    .value_length = 96,
                    .value = hash_record,
                    .type_length = 9,
                    .type = (uint8_t*)"\x04"
                },
                {
                    .value_length = 32,
                    .value
                    = (uint8_t*)"\x82\x48\xd5\xe8\x5a\xc4\xc1\x23\x46\xf8\x45\x8b\xd9\x39\xf1\xce"
                    "\x25\xae\x03\xe9\xc6\xcb\xc8\x86\x28\x6d\xf1\x61\x63\x0a\x75\x0c",
                    .type_length = 3,
                    .type = (uint8_t*)"\x02\x00\x00"
                },
                {
                    .value_length = 16,
                    .value
                    = (uint8_t*)"\xe8\x03\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
                    .type_length = 3,
                    .type = (uint8_t*)"\x02\x00\x0d"
                },
                {
                    .value_length = 32 * 32,
                    .value = merkle_proof,
                    .type_length = 3,
                    .type = (uint8_t*)"\x02\x01\x00"
                },
            }
        }
    };
    memcpy(hash_record, hash_record_c, sizeof(hash_record));
    memcpy(merkle_proof, merkle_proof_c, sizeof(merkle_proof));
    assert_int_equal(tx_parse(&datas_token_batch_private, &tx), 0);

    char program_id_5[20] = "ldg_usad_p2p_28.aleo";
    datas_token_batch_private.prepared_request.program_id_length = sizeof(program_id_5);
    datas_token_batch_private.prepared_request.program_id = program_id_5;
    char function_name_5[28] = "transfer_private_to_public_2";
    datas_token_batch_private.prepared_request.function_name_length = sizeof(function_name_5);
    datas_token_batch_private.prepared_request.function_name = function_name_5;
    datas_token_batch_private.prepared_request.inputs[2].type = type_11;
    datas_token_batch_private.prepared_request.inputs[3].type = type_1;
    assert_int_equal(tx_parse(&datas_token_batch_private, &tx), 0);
}

static void test_tx_parse_staking(void** state)
{
    (void)state;
    tx_t tx;

    uint8_t addr_a[32] = "\x82\x48\xd5\xe8\x5a\xc4\xc1\x23\x46\xf8\x45\x8b\xd9\x39\xf1\xce"
        "\x25\xae\x03\xe9\xc6\xcb\xc8\x86\x28\x6d\xf1\x61\x63\x0a\x75\x0c";
    uint8_t addr_b[32] = "\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\x10"
        "\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f\x20";
    uint8_t amount_val[8] = "\xe8\x03\x00\x00\x00\x00\x00\x00"; // 1000
    uint8_t addr_type[3] = "\x01\x00\x00"; // public / plaintext / address
    uint8_t u64_type[3] = "\x01\x00\x0c"; // public / plaintext / u64

    // ---- bond_public: validator=addr_a (idx0), withdrawal=addr_b (idx1), amount (idx2) ----
    memset(&tx, 0, sizeof(tx));
    sign_transaction_datas_t datas_bond = {
        .prepared_request = {
            .program_id_length = 12,
            .program_id = "credits.aleo",
            .function_name_length = 11,
            .function_name = "bond_public",
            .inputs_count = 3,
            .inputs = {
                {.value_length = 32, .value = addr_a, .type_length = 3, .type = addr_type},
                {.value_length = 32, .value = addr_b, .type_length = 3, .type = addr_type},
                {.value_length = 8, .value = amount_val, .type_length = 3, .type = u64_type}
            },
        }
    };
    assert_int_equal(tx_parse(&datas_bond, &tx), 0);
    assert_int_equal(tx.type, TX_STAKING);
    assert_int_equal(tx.staking.type, TX_STAKING_BOND);
    assert_int_equal(tx.staking.amount, 1000);
    // validator and withdrawal decode as distinct, full-length addresses
    assert_int_equal(strlen(tx.staking.validator), ADDRESS_LEN);
    assert_int_equal(strlen(tx.staking.withdrawal), ADDRESS_LEN);
    assert_string_not_equal(tx.staking.validator, tx.staking.withdrawal);
    char bond_validator[ADDRESS_LEN + 1];
    char bond_withdrawal[ADDRESS_LEN + 1];
    strncpy(bond_validator, tx.staking.validator, sizeof(bond_validator));
    strncpy(bond_withdrawal, tx.staking.withdrawal, sizeof(bond_withdrawal));

    // Swap the two address inputs. validator must now hold what withdrawal held,
    // proving inputs[0]->validator and inputs[1]->withdrawal (the security-critical
    // mapping is not transposed by the parser).
    memset(&tx, 0, sizeof(tx));
    datas_bond.prepared_request.inputs[0].value = addr_b;
    datas_bond.prepared_request.inputs[1].value = addr_a;
    assert_int_equal(tx_parse(&datas_bond, &tx), 0);
    assert_string_equal(tx.staking.validator, bond_withdrawal);
    assert_string_equal(tx.staking.withdrawal, bond_validator);
    datas_bond.prepared_request.inputs[0].value = addr_a;
    datas_bond.prepared_request.inputs[1].value = addr_b;

    // arity mismatch rejected by the generic dispatch loop
    datas_bond.prepared_request.inputs_count = 2;
    assert_int_equal(tx_parse(&datas_bond, &tx), -1);
    datas_bond.prepared_request.inputs_count = 3;

    // ---- unbond_public: staker (idx0), amount (idx1) ----
    memset(&tx, 0, sizeof(tx));
    sign_transaction_datas_t datas_unbond = {
        .prepared_request = {
            .program_id_length = 12,
            .program_id = "credits.aleo",
            .function_name_length = 13,
            .function_name = "unbond_public",
            .inputs_count = 2,
            .inputs = {
                {.value_length = 32, .value = addr_a, .type_length = 3, .type = addr_type},
                {.value_length = 8, .value = amount_val, .type_length = 3, .type = u64_type}
            },
        }
    };
    assert_int_equal(tx_parse(&datas_unbond, &tx), 0);
    assert_int_equal(tx.type, TX_STAKING);
    assert_int_equal(tx.staking.type, TX_STAKING_UNBOND);
    assert_int_equal(tx.staking.amount, 1000);
    assert_int_equal(strlen(tx.staking.staker), ADDRESS_LEN);
    assert_string_equal(tx.staking.staker, bond_validator); // addr_a decodes consistently

    // ---- claim_unbond_public: staker (idx0), NO amount ----
    memset(&tx, 0, sizeof(tx));
    sign_transaction_datas_t datas_claim = {
        .prepared_request = {
            .program_id_length = 12,
            .program_id = "credits.aleo",
            .function_name_length = 19,
            .function_name = "claim_unbond_public",
            .inputs_count = 1,
            .inputs = {{.value_length = 32, .value = addr_a, .type_length = 3, .type = addr_type}},
        }
    };
    assert_int_equal(tx_parse(&datas_claim, &tx), 0);
    assert_int_equal(tx.type, TX_STAKING);
    assert_int_equal(tx.staking.type, TX_STAKING_CLAIM);
    assert_string_equal(tx.staking.staker, bond_validator);
    // claim must NOT populate amount (types.h: "claim leaves unset")
    assert_int_equal(tx.staking.amount, 0);
    // arity: claim with an extra input must be rejected
    datas_claim.prepared_request.inputs_count = 2;
    assert_int_equal(tx_parse(&datas_claim, &tx), -1);
}

int main()
{
    const struct CMUnitTest tests[]
        = {
            cmocka_unit_test(test_tx_extract),
            cmocka_unit_test(test_tx_parse),
            cmocka_unit_test(test_tx_parse_staking),cmocka_unit_test(test_tx_extract),
            cmocka_unit_test(test_tx_parse),
            cmocka_unit_test(test_tx_token_parse)
        };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
