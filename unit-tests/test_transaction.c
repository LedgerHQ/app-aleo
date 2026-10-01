#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <cmocka.h>

#include "os.h"
#include "db.h"
#include "tx.h"

global_ctx_t G_context;

static void test_tx_extract(void **state)
{
    (void) state;
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
    buffer.ptr    = payload;
    buffer.size   = sizeof(payload);
    buffer.offset = 0;
    assert_int_equal(tx_extract_intent(&buffer), 0);

    // Structure type
    uint8_t payload_01[6] = "\x01\x01\x28\x02\x01\x01";
    buffer.ptr            = payload_01;
    buffer.size           = sizeof(payload_01);
    buffer.offset         = 0;
    assert_int_equal(tx_extract_intent(&buffer), 0);

    uint8_t payload_02[6] = "\x01\x01\x29\x02\x01\x01";
    buffer.ptr            = payload_02;
    buffer.size           = sizeof(payload_02);
    buffer.offset         = 0;
    assert_int_equal(tx_extract_intent(&buffer), -1);

    uint8_t payload_03[7] = "\x01\x02\x28\x00\x02\x01\x01";
    buffer.ptr            = payload_03;
    buffer.size           = sizeof(payload_03);
    buffer.offset         = 0;
    assert_int_equal(tx_extract_intent(&buffer), -1);

    // Version
    uint8_t payload_11[6] = "\x01\x01\x28\x02\x01\x00";
    buffer.ptr            = payload_11;
    buffer.size           = sizeof(payload_11);
    buffer.offset         = 0;
    assert_int_equal(tx_extract_intent(&buffer), -1);

    uint8_t payload_12[7] = "\x01\x01\x28\x02\x02\x01\x00";
    buffer.ptr            = payload_12;
    buffer.size           = sizeof(payload_12);
    buffer.offset         = 0;
    assert_int_equal(tx_extract_intent(&buffer), -1);

    // Fee
    uint8_t fee_function_name[79]
        = "\x01\x01\x28\x02\x01\x01"
          "\x81\xb2\x46\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69"
          "\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74"
          "\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73"
          "\x63\x72\x65\x64\x69\x74\x73";
    buffer.ptr    = fee_function_name;
    buffer.size   = sizeof(fee_function_name);
    buffer.offset = 0;
    assert_int_equal(tx_extract_intent(&buffer), -1);

    uint8_t fee_program_id[79]
        = "\x01\x01\x28\x02\x01\x01"
          "\x81\xb3\x46\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69"
          "\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74"
          "\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73"
          "\x63\x72\x65\x64\x69\x74\x73";
    buffer.ptr    = fee_program_id;
    buffer.size   = sizeof(fee_program_id);
    buffer.offset = 0;
    assert_int_equal(tx_extract_intent(&buffer), -1);

    // Malformed prepared request
    uint8_t payload_2[51]
        = "\x01\x01\x28\x02\x01\x01\x81\xB0\x04\x00\x00\x01\xF4\x81\xB1\x04\x00\x00\x00\x64\x81\xB2"
          "\x0A\x66\x65\x65\x5F\x70\x75\x62\x6C\x69\x63\x81\xB3\x0C\x63\x72\x65\x64\x69\x74\x73\x2E"
          "\x61\x6C\x65\x6F\x81\xB4\x00";
    buffer.ptr    = payload_2;
    buffer.size   = sizeof(payload_2);
    buffer.offset = 0;
    assert_int_equal(tx_extract_intent(&buffer), -1);

    // tx_extract_prepared_request

    // Structure type
    uint8_t prepared_request_01[14]
        = "\x01\x01\x2a\x02\x01\x01"
          "\x81\xb5\x01\x64\x81\xb6\x01\x64";
    buffer.ptr    = prepared_request_01;
    buffer.size   = sizeof(prepared_request_01);
    buffer.offset = 0;
    assert_int_equal(
        tx_extract_prepared_request(&buffer, &G_context.sign_transaction_datas.prepared_request),
        -1);

    uint8_t prepared_request_02[15]
        = "\x01\x02\x29\x00\x02\x01\x01"
          "\x81\xb5\x01\x64\x81\xb6\x01\x64";
    buffer.ptr    = prepared_request_02;
    buffer.size   = sizeof(prepared_request_01);
    buffer.offset = 0;
    assert_int_equal(
        tx_extract_prepared_request(&buffer, &G_context.sign_transaction_datas.prepared_request),
        -1);

    // Version
    uint8_t prepared_request_03[14]
        = "\x01\x01\x29\x02\x01\x00"
          "\x81\xb5\x01\x64\x81\xb6\x01\x64";
    buffer.ptr    = prepared_request_03;
    buffer.size   = sizeof(prepared_request_03);
    buffer.offset = 0;
    assert_int_equal(
        tx_extract_prepared_request(&buffer, &G_context.sign_transaction_datas.prepared_request),
        -1);

    uint8_t prepared_request_04[15]
        = "\x01\x01\x29\x02\x02\x01\x00"
          "\x81\xb5\x01\x64\x81\xb6\x01\x64";
    buffer.ptr    = prepared_request_04;
    buffer.size   = sizeof(prepared_request_04);
    buffer.offset = 0;
    assert_int_equal(
        tx_extract_prepared_request(&buffer, &G_context.sign_transaction_datas.prepared_request),
        -1);

    // Program ID & function name
    uint8_t prepared_request_10[14]
        = "\x01\x01\x29\x02\x01\x01"
          "\x81\xb5\x01\x64\x81\xb6\x01\x64";
    buffer.ptr    = prepared_request_10;
    buffer.size   = sizeof(prepared_request_10);
    buffer.offset = 0;
    assert_int_equal(
        tx_extract_prepared_request(&buffer, &G_context.sign_transaction_datas.prepared_request),
        0);

    uint8_t prepared_request_11[140]
        = "\x01\x01\x29\x02\x01\x01"
          "\x81\xb5\x40\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69"
          "\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74"
          "\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73"
          "\x63\x81\xb6\x40\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73\x72\x65\x64\x69"
          "\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74"
          "\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74"
          "\x63\x72";
    buffer.ptr    = prepared_request_11;
    buffer.size   = sizeof(prepared_request_11);
    buffer.offset = 0;
    assert_int_equal(
        tx_extract_prepared_request(&buffer, &G_context.sign_transaction_datas.prepared_request),
        0);

    // Wrong program id
    uint8_t prepared_request_12[78]
        = "\x01\x01\x29\x02\x01\x01"
          "\x81\xb5\x41\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69"
          "\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74"
          "\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73"
          "\x63\x72\x81\xb6\x01\x01";
    buffer.ptr    = prepared_request_12;
    buffer.size   = sizeof(prepared_request_12);
    buffer.offset = 0;
    assert_int_equal(
        tx_extract_prepared_request(&buffer, &G_context.sign_transaction_datas.prepared_request),
        -1);

    // Wrong function name
    uint8_t prepared_request_13[78]
        = "\x01\x01\x29\x02\x01\x01"
          "\x81\xb6\x41\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69"
          "\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74"
          "\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73\x63\x72\x65\x64\x69\x74\x73"
          "\x63\x72\x81\xb5\x01\x01";
    buffer.ptr    = prepared_request_13;
    buffer.size   = sizeof(prepared_request_13);
    buffer.offset = 0;
    assert_int_equal(
        tx_extract_prepared_request(&buffer, &G_context.sign_transaction_datas.prepared_request),
        -1);

    // Missing program id
    uint8_t prepared_request_14[10]
        = "\x01\x01\x29\x02\x01\x01"
          "\x81\xb6\x01\x64";
    buffer.ptr    = prepared_request_14;
    buffer.size   = sizeof(prepared_request_14);
    buffer.offset = 0;
    assert_int_equal(
        tx_extract_prepared_request(&buffer, &G_context.sign_transaction_datas.prepared_request),
        -1);

    // Empty program id
    uint8_t prepared_request_15[13]
        = "\x01\x01\x29\x02\x01\x01"
          "\x81\xb5\x00\x81\xb6\x01\x64";
    buffer.ptr    = prepared_request_15;
    buffer.size   = sizeof(prepared_request_15);
    buffer.offset = 0;
    assert_int_equal(
        tx_extract_prepared_request(&buffer, &G_context.sign_transaction_datas.prepared_request),
        -1);

    // Missing function name
    uint8_t prepared_request_16[10]
        = "\x01\x01\x29\x02\x01\x01"
          "\x81\xb5\x01\x64";
    buffer.ptr    = prepared_request_16;
    buffer.size   = sizeof(prepared_request_16);
    buffer.offset = 0;
    assert_int_equal(
        tx_extract_prepared_request(&buffer, &G_context.sign_transaction_datas.prepared_request),
        -1);

    // Empty function name
    uint8_t prepared_request_17[13]
        = "\x01\x01\x29\x02\x01\x01"
          "\x81\xb5\x01\x64\x81\xb6\x00";
    buffer.ptr    = prepared_request_17;
    buffer.size   = sizeof(prepared_request_17);
    buffer.offset = 0;
    assert_int_equal(
        tx_extract_prepared_request(&buffer, &G_context.sign_transaction_datas.prepared_request),
        -1);

    // Inputs
    // input count
    uint8_t prepared_request_20[18]
        = "\x01\x01\x29\x02\x01\x01"
          "\x81\xb5\x01\x64\x81\xb6\x01\x64\x81\xb7\x01\x00";
    buffer.ptr    = prepared_request_20;
    buffer.size   = sizeof(prepared_request_20);
    buffer.offset = 0;
    assert_int_equal(
        tx_extract_prepared_request(&buffer, &G_context.sign_transaction_datas.prepared_request),
        0);
    uint8_t prepared_request_21[18]
        = "\x01\x01\x29\x02\x01\x01"
          "\x81\xb5\x01\x01\x81\xb6\x01\x01\x81\xb7\x01\x01";
    buffer.ptr    = prepared_request_21;
    buffer.size   = sizeof(prepared_request_21);
    buffer.offset = 0;
    assert_int_equal(
        tx_extract_prepared_request(&buffer, &G_context.sign_transaction_datas.prepared_request),
        -1);
    uint8_t prepared_request_22[17]
        = "\x01\x01\x29\x02\x01\x01"
          "\x81\xb5\x01\x01\x81\xb6\x01\x01\x81\xb7\x00";
    buffer.ptr    = prepared_request_22;
    buffer.size   = sizeof(prepared_request_22);
    buffer.offset = 0;
    assert_int_equal(
        tx_extract_prepared_request(&buffer, &G_context.sign_transaction_datas.prepared_request),
        -1);
    uint8_t prepared_request_23[200];
    memset(prepared_request_23, 0, sizeof(prepared_request_23));
    memcpy(prepared_request_23,
           "\x01\x01\x29\x02\x01\x01\x81\xb5\x01\x64\x81\xb6\x01\x64\x81\xb7\x01\x10",
           18);  // struct/version/program/function/input count
    memcpy(
        &prepared_request_23[18],
        "\x81\xb9\x01\x01\x81\xb8\x01\x01\x81\xb9\x01\x01\x81\xb8\x01\x01\x81\xb9\x01\x01\x81\xb8"
        "\x01\x01\x81\xb9\x01\x01\x81\xb8\x01\x01\x81\xb9\x01\x01\x81\xb8\x01\x01\x81\xb9\x01\x01"
        "\x81\xb8\x01\x01\x81\xb9\x01\x01\x81\xb8\x01\x01\x81\xb9\x01\x01\x81\xb8\x01\x01\x81\xb9"
        "\x01\x01\x81\xb8\x01\x01\x81\xb9\x01\x01\x81\xb8\x01\x01\x81\xb9\x01\x01\x81\xb8\x01\x01"
        "\x81\xb9\x01\x01\x81\xb8\x01\x01\x81\xb9\x01\x01\x81\xb8\x01\x01\x81\xb9\x01\x01\x81\xb8"
        "\x01\x01\x81\xb9\x01\x01\x81\xb8\x01\x01\x81\xb9\x01\x01\x81\xb8\x01\x01",
        128);  // input type/value
    buffer.ptr    = prepared_request_23;
    buffer.size   = 146;
    buffer.offset = 0;
    assert_int_equal(
        tx_extract_prepared_request(&buffer, &G_context.sign_transaction_datas.prepared_request),
        0);

    memset(prepared_request_23, 0, sizeof(prepared_request_23));
    memcpy(prepared_request_23,
           "\x01\x01\x29\x02\x01\x01\x81\xb5\x01\x64\x81\xb6\x01\x64\x81\xb7\x01\x11",
           18);  // struct/version/program/function/input count
    memcpy(
        &prepared_request_23[18],
        "\x81\xb9\x01\x01\x81\xb8\x01\x01\x81\xb9\x01\x01\x81\xb8\x01\x01\x81\xb9\x01\x01\x81\xb8"
        "\x01\x01\x81\xb9\x01\x01\x81\xb8\x01\x01\x81\xb9\x01\x01\x81\xb8\x01\x01\x81\xb9\x01\x01"
        "\x81\xb8\x01\x01\x81\xb9\x01\x01\x81\xb8\x01\x01\x81\xb9\x01\x01\x81\xb8\x01\x01\x81\xb9"
        "\x01\x01\x81\xb8\x01\x01\x81\xb9\x01\x01\x81\xb8\x01\x01\x81\xb9\x01\x01\x81\xb8\x01\x01"
        "\x81\xb9\x01\x01\x81\xb8\x01\x01\x81\xb9\x01\x01\x81\xb8\x01\x01\x81\xb9\x01\x01\x81\xb8"
        "\x01\x01\x81\xb9\x01\x01\x81\xb8\x01\x01\x81\xb9\x01\x01\x81\xb8\x01\x01\x81\xb9\x01\x01"
        "\x81\xb8\x01\x01",
        136);  // input type/value
    buffer.ptr    = prepared_request_23;
    buffer.size   = 154;
    buffer.offset = 0;
    assert_int_equal(
        tx_extract_prepared_request(&buffer, &G_context.sign_transaction_datas.prepared_request),
        -1);

    memset(prepared_request_23, 0, sizeof(prepared_request_23));
    memcpy(prepared_request_23,
           "\x01\x01\x29\x02\x01\x01\x81\xb5\x01\x64\x81\xb6\x01\x64\x81\xb7\x01\x10",
           18);  // struct/version/program/function/input count
    memcpy(
        &prepared_request_23[18],
        "\x81\xb9\x01\x01\x81\xb8\x01\x01\x81\xb9\x01\x01\x81\xb8\x01\x01\x81\xb9\x01\x01\x81\xb8"
        "\x01\x01\x81\xb9\x01\x01\x81\xb8\x01\x01\x81\xb9\x01\x01\x81\xb8\x01\x01\x81\xb9\x01\x01"
        "\x81\xb8\x01\x01\x81\xb9\x01\x01\x81\xb8\x01\x01\x81\xb9\x01\x01\x81\xb8\x01\x01\x81\xb9"
        "\x01\x01\x81\xb8\x01\x01\x81\xb9\x01\x01\x81\xb8\x01\x01\x81\xb9\x01\x01\x81\xb8\x01\x01"
        "\x81\xb9\x01\x01\x81\xb8\x01\x01\x81\xb9\x01\x01\x81\xb8\x01\x01\x81\xb9\x01\x01\x81\xb8"
        "\x01\x01\x81\xb9\x01\x01\x81\xb8\x01\x01\x81\xb9\x01\x01\x81\xb8\x01\x01\x81\xb9\x01\x01"
        "\x81\xb8\x01\x01",
        136);  // input type/value
    buffer.ptr    = prepared_request_23;
    buffer.size   = 154;
    buffer.offset = 0;
    assert_int_equal(
        tx_extract_prepared_request(&buffer, &G_context.sign_transaction_datas.prepared_request),
        -1);

    memset(prepared_request_23, 0, sizeof(prepared_request_23));
    memcpy(prepared_request_23,
           "\x01\x01\x29\x02\x01\x01\x81\xb5\x01\x64\x81\xb6\x01\x64\x81\xb7\x01\x10",
           18);  // struct/version/program/function/input count
    memcpy(
        &prepared_request_23[18],
        "\x81\xb9\x01\x01\x81\xb8\x01\x01\x81\xb9\x01\x01\x81\xb8\x01\x01\x81\xb9\x01\x01\x81\xb8"
        "\x01\x01\x81\xb9\x01\x01\x81\xb8\x01\x01\x81\xb9\x01\x01\x81\xb8\x01\x01\x81\xb9\x01\x01"
        "\x81\xb8\x01\x01\x81\xb9\x01\x01\x81\xb8\x01\x01\x81\xb9\x01\x01\x81\xb8\x01\x01\x81\xb9"
        "\x01\x01\x81\xb8\x01\x01\x81\xb9\x01\x01\x81\xb8\x01\x01\x81\xb9\x01\x01\x81\xb8\x01\x01"
        "\x81\xb9\x01\x01\x81\xb8\x01\x01\x81\xb9\x01\x01\x81\xb8\x01\x01\x81\xb9\x01\x01\x81\xb8"
        "\x01\x01\x81\xb9\x01\x01\x81\xb8\x01\x01\x81\xb9\x01\x01\x81\xb8\x01\x01\x81\xb8\x01\x01"
        "\x81\xb9\x01\x01",
        136);  // input type/value
    buffer.ptr    = prepared_request_23;
    buffer.size   = 154;
    buffer.offset = 0;
    assert_int_equal(
        tx_extract_prepared_request(&buffer, &G_context.sign_transaction_datas.prepared_request),
        -1);

    // input value
    uint8_t prepared_request_28[1600];
    memset(prepared_request_28, 0, sizeof(prepared_request_22));
    memcpy(prepared_request_28,
           "\x01\x01\x29\x02\x01\x01\x81\xb5\x01\x64\x81\xb6\x01\x64\x81\xb7\x01\x01",
           18);                                               // struct/version/pprogram/function
    memcpy(&prepared_request_28[18], "\x81\xb9\x01\x01", 4);  // input type
    memcpy(&prepared_request_28[22], "\x81\xb8\x01\x01", 4);  // input value
    buffer.ptr    = prepared_request_28;
    buffer.size   = 26;
    buffer.offset = 0;
    assert_int_equal(
        tx_extract_prepared_request(&buffer, &G_context.sign_transaction_datas.prepared_request),
        0);

    memset(prepared_request_28, 0, sizeof(prepared_request_28));
    memcpy(prepared_request_28,
           "\x01\x01\x29\x02\x01\x01\x81\xb5\x01\x64\x81\xb6\x01\x64\x81\xb7\x01\x01",
           18);                                               // struct/version/pprogram/function
    memcpy(&prepared_request_28[18], "\x81\xb9\x01\x01", 4);  // input type
    memcpy(&prepared_request_28[22], "\x81\xb8\x82\x06\x00", 5);  // input value
    buffer.ptr    = prepared_request_28;
    buffer.size   = 1563;
    buffer.offset = 0;
    assert_int_equal(
        tx_extract_prepared_request(&buffer, &G_context.sign_transaction_datas.prepared_request),
        0);

    memset(prepared_request_28, 0, sizeof(prepared_request_28));
    memcpy(prepared_request_28,
           "\x01\x01\x29\x02\x01\x01\x81\xb5\x01\x64\x81\xb6\x01\x64\x81\xb7\x01\x01",
           18);                                               // struct/version/pprogram/function
    memcpy(&prepared_request_28[18], "\x81\xb9\x01\x01", 4);  // input type
    memcpy(&prepared_request_28[22], "\x81\xb8\x82\x06\x01", 5);  // input value
    buffer.ptr    = prepared_request_28;
    buffer.size   = 1564;
    buffer.offset = 0;
    assert_int_equal(
        tx_extract_prepared_request(&buffer, &G_context.sign_transaction_datas.prepared_request),
        -1);

    memset(prepared_request_28, 0, sizeof(prepared_request_28));
    memcpy(prepared_request_28,
           "\x01\x01\x29\x02\x01\x01\x81\xb5\x01\x64\x81\xb6\x01\x64\x81\xb7\x01\x01",
           18);                                               // struct/version/pprogram/function
    memcpy(&prepared_request_28[18], "\x81\xb9\x01\x01", 4);  // input type
    memcpy(&prepared_request_28[22], "\x81\xb8\x01\x01", 4);  // input value
    memcpy(&prepared_request_28[26], "\x81\xb8\x01\x01", 4);  // input value
    buffer.ptr    = prepared_request_28;
    buffer.size   = 30;
    buffer.offset = 0;
    assert_int_equal(
        tx_extract_prepared_request(&buffer, &G_context.sign_transaction_datas.prepared_request),
        -1);

    // input type
    memset(prepared_request_28, 0, sizeof(prepared_request_28));
    memcpy(prepared_request_28,
           "\x01\x01\x29\x02\x01\x01\x81\xb5\x01\x64\x81\xb6\x01\x64\x81\xb7\x01\x01",
           18);                                               // struct/version/pprogram/function
    memcpy(&prepared_request_28[18], "\x81\xb8\x01\x01", 4);  // input value
    memcpy(&prepared_request_28[22], "\x81\xb9\x81\x80", 4);  // input type
    buffer.ptr    = prepared_request_28;
    buffer.size   = 154;
    buffer.offset = 0;
    assert_int_equal(
        tx_extract_prepared_request(&buffer, &G_context.sign_transaction_datas.prepared_request),
        0);

    memset(prepared_request_28, 0, sizeof(prepared_request_28));
    memcpy(prepared_request_28,
           "\x01\x01\x29\x02\x01\x01\x81\xb5\x01\x64\x81\xb6\x01\x64\x81\xb7\x01\x01",
           18);                                               // struct/version/pprogram/function
    memcpy(&prepared_request_28[18], "\x81\xb8\x01\x01", 4);  // input value
    memcpy(&prepared_request_28[22], "\x81\xb9\x81\x81", 4);  // input type
    buffer.ptr    = prepared_request_28;
    buffer.size   = 155;
    buffer.offset = 0;
    assert_int_equal(
        tx_extract_prepared_request(&buffer, &G_context.sign_transaction_datas.prepared_request),
        -1);

    memset(prepared_request_28, 0, sizeof(prepared_request_28));
    memcpy(prepared_request_28,
           "\x01\x01\x29\x02\x01\x01\x81\xb5\x01\x64\x81\xb6\x01\x64\x81\xb7\x01\x01",
           18);                                               // struct/version/pprogram/function
    memcpy(&prepared_request_28[18], "\x81\xb9\x01\x01", 4);  // input type
    memcpy(&prepared_request_28[22], "\x81\xb8\x01\x01", 4);  // input value
    memcpy(&prepared_request_28[26], "\x81\xb9\x01\x01", 4);  // input type
    buffer.ptr    = prepared_request_28;
    buffer.size   = 30;
    buffer.offset = 0;
    assert_int_equal(
        tx_extract_prepared_request(&buffer, &G_context.sign_transaction_datas.prepared_request),
        -1);

    // Program checksum
    uint8_t prepared_request_30[53]
        = "\x01\x01\x29\x02\x01\x01"
          "\x81\xb5\x01\x64\x81\xb6\x01\x64\x81\xb7\x01\x00"
          "\x81\xc4\x20\x01\x81\xb9\x01\x02\x81\xb9\x01\x03\x81\xb9\x01\x04\x81\xb9\x01\x05\x81\xb9"
          "\x01\x06\x81\xb9\x01\x07\x81\xb9\x01\x08\x81\xb9\x01";
    buffer.ptr    = prepared_request_30;
    buffer.size   = sizeof(prepared_request_30);
    buffer.offset = 0;
    assert_int_equal(
        tx_extract_prepared_request(&buffer, &G_context.sign_transaction_datas.prepared_request),
        0);

    // Wrong program checksum
    uint8_t prepared_request_31[52]
        = "\x01\x01\x29\x02\x01\x01"
          "\x81\xb5\x01\x64\x81\xb6\x01\x64\x81\xb7\x01\x00"
          "\x81\xc4\x1f\x01\x81\xb9\x01\x02\x81\xb9\x01\x03\x81\xb9\x01\x04\x81\xb9\x01\x05\x81\xb9"
          "\x01\x06\x81\xb9\x01\x07\x81\xb9\x01\x08\x81\xb9";
    buffer.ptr    = prepared_request_31;
    buffer.size   = sizeof(prepared_request_31);
    buffer.offset = 0;
    assert_int_equal(
        tx_extract_prepared_request(&buffer, &G_context.sign_transaction_datas.prepared_request),
        -1);

    uint8_t prepared_request_32[54]
        = "\x01\x01\x29\x02\x01\x01"
          "\x81\xb5\x01\x64\x81\xb6\x01\x64\x81\xb7\x01\x00"
          "\x81\xc4\x1f\x01\x81\xb9\x01\x02\x81\xb9\x01\x03\x81\xb9\x01\x04\x81\xb9\x01\x05\x81\xb9"
          "\x01\x06\x81\xb9\x01\x07\x81\xb9\x01\x08\x81\xb9\x01\x05";
    buffer.ptr    = prepared_request_32;
    buffer.size   = sizeof(prepared_request_32);
    buffer.offset = 0;
    assert_int_equal(
        tx_extract_prepared_request(&buffer, &G_context.sign_transaction_datas.prepared_request),
        -1);

    // Nested call count
    uint8_t prepared_request_40[18]
        = "\x01\x01\x29\x02\x01\x01"
          "\x81\xb5\x01\x64\x81\xb6\x01\x64\x81\xba\x01\x01";
    buffer.ptr    = prepared_request_40;
    buffer.size   = sizeof(prepared_request_40);
    buffer.offset = 0;
    assert_int_equal(
        tx_extract_prepared_request(&buffer, &G_context.sign_transaction_datas.prepared_request),
        0);
    uint8_t prepared_request_42[18]
        = "\x01\x01\x29\x02\x01\x01"
          "\x81\xb5\x01\x64\x81\xb6\x01\x64\x81\xba\x01\x1f";
    buffer.ptr    = prepared_request_42;
    buffer.size   = sizeof(prepared_request_42);
    buffer.offset = 0;
    assert_int_equal(
        tx_extract_prepared_request(&buffer, &G_context.sign_transaction_datas.prepared_request),
        0);
    uint8_t prepared_request_41[17]
        = "\x01\x01\x29\x02\x01\x01"
          "\x81\xb5\x01\x64\x81\xb6\x01\x64\x81\xba\x00";
    buffer.ptr    = prepared_request_41;
    buffer.size   = sizeof(prepared_request_41);
    buffer.offset = 0;
    assert_int_equal(
        tx_extract_prepared_request(&buffer, &G_context.sign_transaction_datas.prepared_request),
        -1);
    uint8_t prepared_request_43[18]
        = "\x01\x01\x29\x02\x01\x01"
          "\x81\xb5\x01\x64\x81\xb6\x01\x64\x81\xba\x01\x20";
    buffer.ptr    = prepared_request_43;
    buffer.size   = sizeof(prepared_request_43);
    buffer.offset = 0;
    assert_int_equal(
        tx_extract_prepared_request(&buffer, &G_context.sign_transaction_datas.prepared_request),
        -1);
}

static void test_tx_parse(void **state)
{
    (void) state;
    memset(&G_context, 0, sizeof(G_context));
    tx_t tx;

    sign_transaction_datas_t datas_public = {
        .max_base_fee             = 100,
        .max_priority_fee         = 500,
        .fee_function_name_length = 10,
        .fee_function_name        = "fee_public",
        .fee_program_id_length    = 12,
        .fee_program_id           = "credits.aleo",
        .prepared_request         = {
                                     .program_id_length    = 12,
                                     .program_id           = "credits.aleo",
                                     .function_name_length = 15,
                                     .function_name        = "transfer_public",
                                     .inputs_count         = 2,
                                     .inputs
            = {{.value_length = 32,
                .value
                = (uint8_t *) "\x82\x48\xd5\xe8\x5a\xc4\xc1\x23\x46\xf8\x45\x8b\xd9\x39\xf1\xce"
                              "\x25\xae\x03\xe9\xc6\xcb\xc8\x86\x28\x6d\xf1\x61\x63\x0a\x75\x0c",
                .type_length = 3,
                .type        = (uint8_t *) "\x01\x00\x00"},
               {.value_length = 8,
                .value        = (uint8_t *) "\xe8\x03\x00\x00\x00\x00\x00\x00",
                .type_length  = 3,
                .type         = (uint8_t *) "\x01\x00\x0c"}},
                                     }
    };

    datas_public.prepared_request.inputs_count = 1;
    assert_int_equal(tx_parse(&datas_public, &tx), -1);
    datas_public.prepared_request.inputs_count = 2;

    // network_id must be rejected as soon as it reaches or exceeds NETWORK_ID_COUNT,
    // even though bhp_1024_hashes[] is a wider byte array: indexing must be bounded
    // by the element count, not sizeof(array).
    datas_public.prepared_request.network_id = NETWORK_ID_COUNT;
    assert_int_equal(tx_parse(&datas_public, &tx), -1);
    datas_public.prepared_request.network_id = NETWORK_ID_COUNT - 1;
    assert_int_equal(tx_parse(&datas_public, &tx), 0);
    datas_public.prepared_request.network_id = 0;

    // get_u64
    datas_public.prepared_request.inputs[1].type_length = 2;
    assert_int_equal(tx_parse(&datas_public, &tx), -1);
    datas_public.prepared_request.inputs[1].type_length = 3;

    uint8_t type_1[3]                            = "\x01\x00\x0c";
    uint8_t type_2[3]                            = "\x02\x00\x0c";
    uint8_t type_3[3]                            = "\x01\x01\x0c";
    datas_public.prepared_request.inputs[1].type = type_2;
    assert_int_equal(tx_parse(&datas_public, &tx), -1);
    datas_public.prepared_request.inputs[1].type = type_3;
    assert_int_equal(tx_parse(&datas_public, &tx), -1);
    datas_public.prepared_request.inputs[1].type = type_1;

    datas_public.prepared_request.inputs[0].type_length = 2;
    assert_int_equal(tx_parse(&datas_public, &tx), -1);
    datas_public.prepared_request.inputs[0].type_length = 3;
    uint8_t type_11[3]                                  = "\x01\x00\x00";
    uint8_t type_12[3]                                  = "\x02\x00\x00";
    uint8_t type_13[3]                                  = "\x01\x01\x00";
    datas_public.prepared_request.inputs[0].type        = type_12;
    assert_int_equal(tx_parse(&datas_public, &tx), -1);
    datas_public.prepared_request.inputs[0].type = type_13;
    assert_int_equal(tx_parse(&datas_public, &tx), -1);
    datas_public.prepared_request.inputs[0].type = type_11;

    char program_id[12]                      = "credits.aleo";
    datas_public.prepared_request.program_id = NULL;
    assert_int_equal(tx_parse(&datas_public, &tx), -1);
    datas_public.prepared_request.program_id = program_id;

    char function_name[15]                      = "transfer_public";
    datas_public.prepared_request.function_name = NULL;
    assert_int_equal(tx_parse(&datas_public, &tx), -1);
    datas_public.prepared_request.function_name = function_name;

    char program_id_2[12]                    = "credots.aleo";
    datas_public.prepared_request.program_id = program_id_2;
    assert_int_equal(tx_parse(&datas_public, &tx), -1);
    datas_public.prepared_request.program_id = program_id;

    char function_name_2[15]                    = "tronsfer_public";
    datas_public.prepared_request.function_name = function_name_2;
    assert_int_equal(tx_parse(&datas_public, &tx), -1);
    datas_public.prepared_request.function_name = function_name;

    assert_int_equal(tx_parse(&datas_public, &tx), 0);

    char function_name_3[26]                           = "transfer_public_to_private";
    datas_public.prepared_request.function_name_length = sizeof(function_name_3);
    datas_public.prepared_request.function_name        = function_name_3;
    datas_public.prepared_request.inputs[0].type       = type_12;
    assert_int_equal(tx_parse(&datas_public, &tx), 0);
    datas_public.prepared_request.inputs[0].type = type_11;
    assert_int_equal(tx_parse(&datas_public, &tx), -1);

    datas_public.prepared_request.inputs[0].type = type_12;
    assert_int_equal(tx_parse(&datas_public, &tx), 0);
    datas_public.prepared_request.nested_call_count = 1;
    assert_int_equal(tx_parse(&datas_public, &tx), -1);

    const uint8_t hash_record_c[96]
        = "\xf4\x69\x19\x61\x50\x7b\x8f\x32\x92\xaf\x47\xac\x64\xdf\x59\xf7"
          "\xc4\x39\xf6\xb2\x48\xa9\x55\x10\xfa\x95\xcc\x96\x25\xe7\xfd\x07"
          "\x2a\xe1\x18\xb5\x2d\x46\xd6\x0b\x96\x63\x06\x72\x73\x3d\x25\x2c"
          "\xa9\x3f\xbb\x8d\x56\xd5\x26\x1c\x0c\x4c\xbb\x8c\xf2\x92\x5d\x05"
          "\xd9\x75\x64\x3e\x43\x24\x5c\x7a\xf2\x8d\xb2\xa8\x5b\x54\x59\xb4"
          "\xb7\x7b\x38\x2a\x0c\x74\x33\x0c\x1d\x53\x7a\xa9\x22\xe1\x7c\x00";
    uint8_t                  hash_record[96];
    sign_transaction_datas_t datas_private = {
        .max_base_fee             = 100,
        .max_priority_fee         = 500,
        .fee_function_name_length = 11,
        .fee_function_name        = "fee_private",
        .fee_program_id_length    = 12,
        .fee_program_id           = "credits.aleo",
        .prepared_request
        = {.program_id_length    = 12,
           .program_id           = "credits.aleo",
           .function_name_length = 16,
           .function_name        = "transfer_private",
           .inputs_count         = 3,
           .inputs
           = {{.value_length = 96,
               .value        = hash_record,
               .type_length  = 9,
               .type         = (uint8_t *) "\x03\x07\x63\x72\x65\x64\x69\x74\x73"},
              {.value_length = 32,
               .value
               = (uint8_t *) "\x82\x48\xd5\xe8\x5a\xc4\xc1\x23\x46\xf8\x45\x8b\xd9\x39\xf1\xce"
                             "\x25\xae\x03\xe9\xc6\xcb\xc8\x86\x28\x6d\xf1\x61\x63\x0a\x75\x0c",
               .type_length = 3,
               .type        = (uint8_t *) "\x02\x00\x00"},
              {.value_length = 8,
               .value        = (uint8_t *) "\xe8\x03\x00\x00\x00\x00\x00\x00",
               .type_length  = 3,
               .type         = (uint8_t *) "\x02\x00\x0c"}}}
    };

    memcpy(hash_record, hash_record_c, 96);
    assert_int_equal(tx_parse(&datas_private, &tx), 0);

    datas_private.prepared_request.inputs[2].type = type_1;
    assert_int_equal(tx_parse(&datas_private, &tx), -1);
    datas_private.prepared_request.inputs[2].type = type_2;

    char function_name_4[26]                            = "transfer_private_to_public";
    datas_private.prepared_request.function_name_length = sizeof(function_name_4);
    datas_private.prepared_request.function_name        = function_name_4;
    datas_private.prepared_request.inputs[1].type       = type_11;
    datas_private.prepared_request.inputs[2].type       = type_1;
    assert_int_equal(tx_parse(&datas_private, &tx), 0);

    char function_name_5[11]                            = "fee_private";
    datas_private.prepared_request.function_name_length = sizeof(function_name_5);
    datas_private.prepared_request.function_name        = function_name_5;

    sign_transaction_datas_t datas_private_2 = {
        .max_base_fee             = 100,
        .max_priority_fee         = 500,
        .fee_function_name_length = 11,
        .fee_function_name        = "fee_private",
        .fee_program_id_length    = 12,
        .fee_program_id           = "credits.aleo",
        .prepared_request
        = {.program_id_length    = 12,
           .program_id           = "credits.aleo",
           .function_name_length = 11,
           .function_name        = "fee_private",
           .inputs_count         = 4,
           .inputs
           = {{.value_length = 96,
               .value        = hash_record,
               .type_length  = 9,
               .type         = (uint8_t *) "\x03\x07\x63\x72\x65\x64\x69\x74\x73"},
              {.value_length = 8,
               .value        = (uint8_t *) "\xe8\x03\x00\x00\x00\x00\x00\x00",
               .type_length  = 3,
               .type         = (uint8_t *) "\x01\x00\x0c"},
              {.value_length = 8,
               .value        = (uint8_t *) "\x64\x00\x00\x00\x00\x00\x00\x00",
               .type_length  = 3,
               .type         = (uint8_t *) "\x01\x00\x0c"},
              {.value_length = 32,
               .value
               = (uint8_t *) "\x82\x48\xd5\xe8\x5a\xc4\xc1\x23\x46\xf8\x45\x8b\xd9\x39\xf1\xce"
                             "\x25\xae\x03\xe9\xc6\xcb\xc8\x86\x28\x6d\xf1\x61\x63\x0a\x75\x0c",
               .type_length = 3,
               .type        = (uint8_t *) "\x01\x00\x02"}}}
    };

    assert_int_equal(tx_parse(&datas_private_2, &tx), 0);

    sign_transaction_datas_t datas_private_3 = {
        .max_base_fee             = 100,
        .max_priority_fee         = 500,
        .fee_function_name_length = 10,
        .fee_function_name        = "fee_public",
        .fee_program_id_length    = 12,
        .fee_program_id           = "credits.aleo",
        .prepared_request
        = {.program_id_length    = 12,
           .program_id           = "credits.aleo",
           .function_name_length = 10,
           .function_name        = "fee_public",
           .inputs_count         = 3,
           .inputs
           = {{.value_length = 8,
               .value        = (uint8_t *) "\xe8\x03\x00\x00\x00\x00\x00\x00",
               .type_length  = 3,
               .type         = (uint8_t *) "\x01\x00\x0c"},
              {.value_length = 8,
               .value        = (uint8_t *) "\x64\x00\x00\x00\x00\x00\x00\x00",
               .type_length  = 3,
               .type         = (uint8_t *) "\x01\x00\x0c"},
              {.value_length = 32,
               .value
               = (uint8_t *) "\x82\x48\xd5\xe8\x5a\xc4\xc1\x23\x46\xf8\x45\x8b\xd9\x39\xf1\xce"
                             "\x25\xae\x03\xe9\xc6\xcb\xc8\x86\x28\x6d\xf1\x61\x63\x0a\x75\x0c",
               .type_length = 3,
               .type        = (uint8_t *) "\x01\x00\x02"}}}
    };

    assert_int_equal(tx_parse(&datas_private_3, &tx), 0);

    sign_transaction_datas_t datas_batch_private = {
        .max_base_fee             = 100,
        .max_priority_fee         = 500,
        .fee_function_name_length = 11,
        .fee_function_name        = "fee_private",
        .fee_program_id_length    = 12,
        .fee_program_id           = "credits.aleo",
        .prepared_request
        = {.program_id_length    = 13,
           .program_id           = "ldg_p_28.aleo",
           .function_name_length = 18,
           .function_name        = "transfer_private_2",
           .nested_call_count    = 2,
           .inputs_count         = 4,
           .inputs
           = {{.value_length = 96,
               .value        = hash_record,
               .type_length  = 9,
               .type         = (uint8_t *) "\x03\x07\x63\x72\x65\x64\x69\x74\x73"},
              {.value_length = 96,
               .value        = hash_record,
               .type_length  = 9,
               .type         = (uint8_t *) "\x03\x07\x63\x72\x65\x64\x69\x74\x73"},
              {.value_length = 32,
               .value
               = (uint8_t *) "\x82\x48\xd5\xe8\x5a\xc4\xc1\x23\x46\xf8\x45\x8b\xd9\x39\xf1\xce"
                             "\x25\xae\x03\xe9\xc6\xcb\xc8\x86\x28\x6d\xf1\x61\x63\x0a\x75\x0c",
               .type_length = 3,
               .type        = (uint8_t *) "\x02\x00\x00"},
              {.value_length = 8,
               .value        = (uint8_t *) "\xe8\x03\x00\x00\x00\x00\x00\x00",
               .type_length  = 3,
               .type         = (uint8_t *) "\x02\x00\x0c"}}}
    };

    assert_int_equal(tx_parse(&datas_batch_private, &tx), 0);

    datas_batch_private.prepared_request.inputs[2].type = type_1;
    assert_int_equal(tx_parse(&datas_batch_private, &tx), -1);

    sign_transaction_datas_t datas_batch_private_to_public = {
        .max_base_fee             = 100,
        .max_priority_fee         = 500,
        .fee_function_name_length = 11,
        .fee_function_name        = "fee_private",
        .fee_program_id_length    = 12,
        .fee_program_id           = "credits.aleo",
        .prepared_request = {.program_id_length    = 15,
                             .program_id           = "ldg_p2p_28.aleo",
                             .function_name_length = 28,
                             .function_name        = "transfer_private_to_public_2",
                             .nested_call_count    = 2,
                             .inputs_count         = 3,
                             .inputs = {{.value_length = 96,
                                         .value        = hash_record,
                                         .type_length  = 1,
                                         .type         = (uint8_t *) "\x04"},
                                        {.value_length = 96,
                                         .value        = hash_record,
                                         .type_length  = 1,
                                         .type         = (uint8_t *) "\x04"},
                                        {.value_length = 8,
                                         .value = (uint8_t *) "\xe8\x03\x00\x00\x00\x00\x00\x00",
                                         .type_length = 3,
                                         .type        = (uint8_t *) "\x01\x00\x0c"}}}
    };

    assert_int_equal(tx_parse(&datas_batch_private_to_public, &tx), 0);

    datas_batch_private_to_public.prepared_request.inputs[2].type = type_2;
    assert_int_equal(tx_parse(&datas_batch_private_to_public, &tx), -1);
}

static void test_tx_token_parse(void **state)
{
    (void) state;
    memset(&G_context, 0, sizeof(G_context));
    tx_t tx;

    sign_transaction_datas_t datas_token_public = {
        .max_base_fee             = 100,
        .max_priority_fee         = 500,
        .fee_function_name_length = 10,
        .fee_function_name        = "fee_public",
        .fee_program_id_length    = 12,
        .fee_program_id           = "credits.aleo",
        .prepared_request         = {
                                     .program_id_length    = 20,
                                     .program_id           = "usad_stablecoin.aleo",
                                     .function_name_length = 15,
                                     .function_name        = "transfer_public",
                                     .inputs_count         = 2,
                                     .inputs
            = {{.value_length = 32,
                .value
                = (uint8_t *) "\x82\x48\xd5\xe8\x5a\xc4\xc1\x23\x46\xf8\x45\x8b\xd9\x39\xf1\xce"
                              "\x25\xae\x03\xe9\xc6\xcb\xc8\x86\x28\x6d\xf1\x61\x63\x0a\x75\x0c",
                .type_length = 3,
                .type        = (uint8_t *) "\x01\x00\x00"},
               {.value_length = 16,
                .value
                = (uint8_t *) "\xe8\x03\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
                .type_length = 3,
                .type        = (uint8_t *) "\x01\x00\x0d"}},
                                     }
    };

    assert_int_equal(tx_parse(&datas_token_public, &tx), 0);

    datas_token_public.prepared_request.inputs[1].type_length = 2;
    assert_int_equal(tx_parse(&datas_token_public, &tx), -1);
    datas_token_public.prepared_request.inputs[1].type_length = 3;

    uint8_t type_1[3]                                  = "\x01\x00\x0d";
    uint8_t type_2[3]                                  = "\x02\x00\x0d";
    uint8_t type_3[3]                                  = "\x01\x01\x0d";
    datas_token_public.prepared_request.inputs[1].type = type_2;
    assert_int_equal(tx_parse(&datas_token_public, &tx), -1);
    datas_token_public.prepared_request.inputs[1].type = type_3;
    assert_int_equal(tx_parse(&datas_token_public, &tx), -1);
    datas_token_public.prepared_request.inputs[1].type = type_1;

    uint8_t type_11[3]                                       = "\x01\x00\x00";
    uint8_t type_12[3]                                       = "\x02\x00\x00";
    char    function_name_3[26]                              = "transfer_public_to_private";
    datas_token_public.prepared_request.function_name_length = sizeof(function_name_3);
    datas_token_public.prepared_request.function_name        = function_name_3;
    datas_token_public.prepared_request.inputs[0].type       = type_12;
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
    const uint8_t            merkle_proof_c[32 * 32] = {0};
    uint8_t                  hash_record[96];
    uint8_t                  merkle_proof[32 * 32];
    sign_transaction_datas_t datas_token_private = {
        .max_base_fee             = 100,
        .max_priority_fee         = 500,
        .fee_function_name_length = 11,
        .fee_function_name        = "fee_private",
        .fee_program_id_length    = 12,
        .fee_program_id           = "credits.aleo",
        .prepared_request
        = {.program_id_length    = 20,
           .program_id           = "usad_stablecoin.aleo",
           .function_name_length = 16,
           .function_name        = "transfer_private",
           .inputs_count         = 4,
           .inputs               = {
               {.value_length = 32,
                .value
                = (uint8_t *) "\x82\x48\xd5\xe8\x5a\xc4\xc1\x23\x46\xf8\x45\x8b\xd9\x39\xf1\xce"
                              "\x25\xae\x03\xe9\xc6\xcb\xc8\x86\x28\x6d\xf1\x61\x63\x0a\x75\x0c",
                .type_length = 3,
                .type        = (uint8_t *) "\x02\x00\x00"},
               {.value_length = 16,
                .value
                = (uint8_t *) "\xe8\x03\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
                .type_length = 3,
                .type        = (uint8_t *) "\x02\x00\x0d"},
               {.value_length = 96,
                .value        = hash_record,
                .type_length  = 9,
                .type         = (uint8_t *) "\x03\x07\x63\x72\x65\x64\x69\x74\x73"},
               {.value_length = 32 * 32,
                .value        = merkle_proof,
                .type_length  = 3,
                .type         = (uint8_t *) "\x02\x01\x00"},
           }}
    };

    memcpy(hash_record, hash_record_c, sizeof(hash_record));
    memcpy(merkle_proof, merkle_proof_c, sizeof(merkle_proof));
    assert_int_equal(tx_parse(&datas_token_private, &tx), 0);

    char function_name_4[26]                                  = "transfer_private_to_public";
    datas_token_private.prepared_request.function_name_length = sizeof(function_name_4);
    datas_token_private.prepared_request.function_name        = function_name_4;
    datas_token_private.prepared_request.inputs[0].type       = type_11;
    datas_token_private.prepared_request.inputs[1].type       = type_1;
    assert_int_equal(tx_parse(&datas_token_private, &tx), 0);
    datas_token_private.prepared_request.inputs[0].type = type_12;
    assert_int_equal(tx_parse(&datas_token_private, &tx), -1);

    sign_transaction_datas_t datas_token_batch_private = {
        .max_base_fee             = 100,
        .max_priority_fee         = 500,
        .fee_function_name_length = 11,
        .fee_function_name        = "fee_private",
        .fee_program_id_length    = 12,
        .fee_program_id           = "credits.aleo",
        .prepared_request
        = {.program_id_length    = 18,
           .program_id           = "ldg_usad_p_28.aleo",
           .function_name_length = 18,
           .function_name        = "transfer_private_2",
           .nested_call_count    = 2,
           .inputs_count         = 5,
           .inputs               = {
               {.value_length = 96,
                .value        = hash_record,
                .type_length  = 9,
                .type         = (uint8_t *) "\x04"},
               {.value_length = 96,
                .value        = hash_record,
                .type_length  = 9,
                .type         = (uint8_t *) "\x04"},
               {.value_length = 32,
                .value
                = (uint8_t *) "\x82\x48\xd5\xe8\x5a\xc4\xc1\x23\x46\xf8\x45\x8b\xd9\x39\xf1\xce"
                              "\x25\xae\x03\xe9\xc6\xcb\xc8\x86\x28\x6d\xf1\x61\x63\x0a\x75\x0c",
                .type_length = 3,
                .type        = (uint8_t *) "\x02\x00\x00"},
               {.value_length = 16,
                .value
                = (uint8_t *) "\xe8\x03\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
                .type_length = 3,
                .type        = (uint8_t *) "\x02\x00\x0d"},
               {.value_length = 32 * 32,
                .value        = merkle_proof,
                .type_length  = 3,
                .type         = (uint8_t *) "\x02\x01\x00"},
           }}
    };
    memcpy(hash_record, hash_record_c, sizeof(hash_record));
    memcpy(merkle_proof, merkle_proof_c, sizeof(merkle_proof));
    assert_int_equal(tx_parse(&datas_token_batch_private, &tx), 0);

    char program_id_5[20]                                        = "ldg_usad_p2p_28.aleo";
    datas_token_batch_private.prepared_request.program_id_length = sizeof(program_id_5);
    datas_token_batch_private.prepared_request.program_id        = program_id_5;
    char function_name_5[28]                                     = "transfer_private_to_public_2";
    datas_token_batch_private.prepared_request.function_name_length = sizeof(function_name_5);
    datas_token_batch_private.prepared_request.function_name        = function_name_5;
    datas_token_batch_private.prepared_request.inputs[2].type       = type_11;
    datas_token_batch_private.prepared_request.inputs[3].type       = type_1;
    assert_int_equal(tx_parse(&datas_token_batch_private, &tx), 0);
}

static void test_tx_token_arc20_parse(void **state)
{
    (void) state;
    memset(&G_context, 0, sizeof(G_context));
    tx_t tx;

    sign_transaction_datas_t datas_arc20_public = {
        .max_base_fee             = 100,
        .max_priority_fee         = 500,
        .fee_function_name_length = 10,
        .fee_function_name        = "fee_public",
        .fee_program_id_length    = 12,
        .fee_program_id           = "credits.aleo",
        .prepared_request         = {
                                     .program_id_length    = 15,
                                     .program_id           = "arc20_usdt.aleo",
                                     .function_name_length = 15,
                                     .function_name        = "transfer_public",
                                     .inputs_count         = 2,
                                     .inputs
            = {{.value_length = 32,
                .value
                = (uint8_t *) "\x82\x48\xd5\xe8\x5a\xc4\xc1\x23\x46\xf8\x45\x8b\xd9\x39\xf1\xce"
                              "\x25\xae\x03\xe9\xc6\xcb\xc8\x86\x28\x6d\xf1\x61\x63\x0a\x75\x0c",
                .type_length = 3,
                .type        = (uint8_t *) "\x01\x00\x00"},
               {.value_length = 16,
                .value
                = (uint8_t *) "\xe8\x03\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
                .type_length = 3,
                .type        = (uint8_t *) "\x01\x00\x0d"}},
                                     }
    };

    assert_int_equal(tx_parse(&datas_arc20_public, &tx), 0);

    datas_arc20_public.prepared_request.inputs[1].type_length = 2;
    assert_int_equal(tx_parse(&datas_arc20_public, &tx), -1);
    datas_arc20_public.prepared_request.inputs[1].type_length = 3;

    uint8_t type_1[3]                                  = "\x01\x00\x0d";
    uint8_t type_2[3]                                  = "\x02\x00\x0d";
    uint8_t type_3[3]                                  = "\x01\x01\x0d";
    datas_arc20_public.prepared_request.inputs[1].type = type_2;
    assert_int_equal(tx_parse(&datas_arc20_public, &tx), -1);
    datas_arc20_public.prepared_request.inputs[1].type = type_3;
    assert_int_equal(tx_parse(&datas_arc20_public, &tx), -1);
    datas_arc20_public.prepared_request.inputs[1].type = type_1;

    uint8_t type_11[3]                                       = "\x01\x00\x00";
    uint8_t type_12[3]                                       = "\x02\x00\x00";
    char    function_name_2[26]                              = "transfer_public_to_private";
    datas_arc20_public.prepared_request.function_name_length = sizeof(function_name_2);
    datas_arc20_public.prepared_request.function_name        = function_name_2;
    datas_arc20_public.prepared_request.inputs[0].type       = type_12;
    assert_int_equal(tx_parse(&datas_arc20_public, &tx), 0);
    datas_arc20_public.prepared_request.inputs[0].type = type_11;
    assert_int_equal(tx_parse(&datas_arc20_public, &tx), -1);

    const uint8_t hash_record_c[96]
        = "\xf4\x69\x19\x61\x50\x7b\x8f\x32\x92\xaf\x47\xac\x64\xdf\x59\xf7"
          "\xc4\x39\xf6\xb2\x48\xa9\x55\x10\xfa\x95\xcc\x96\x25\xe7\xfd\x07"
          "\x2a\xe1\x18\xb5\x2d\x46\xd6\x0b\x96\x63\x06\x72\x73\x3d\x25\x2c"
          "\xa9\x3f\xbb\x8d\x56\xd5\x26\x1c\x0c\x4c\xbb\x8c\xf2\x92\x5d\x05"
          "\xd9\x75\x64\x3e\x43\x24\x5c\x7a\xf2\x8d\xb2\xa8\x5b\x54\x59\xb4"
          "\xb7\x7b\x38\x2a\x0c\x74\x33\x0c\x1d\x53\x7a\xa9\x22\xe1\x7c\x00";
    uint8_t hash_record[96];
    memcpy(hash_record, hash_record_c, sizeof(hash_record));

    // For ARC20 tokens, the record input (index 0) is not parsed for
    // transfer_private/transfer_private_to_public: the address and amount are
    // read from indexes 1 and 2 instead of 0 and 1 as for ARC22 tokens.
    sign_transaction_datas_t datas_arc20_private = {
        .max_base_fee             = 100,
        .max_priority_fee         = 500,
        .fee_function_name_length = 11,
        .fee_function_name        = "fee_private",
        .fee_program_id_length    = 12,
        .fee_program_id           = "credits.aleo",
        .prepared_request
        = {.program_id_length    = 15,
           .program_id           = "arc20_usdt.aleo",
           .function_name_length = 16,
           .function_name        = "transfer_private",
           .inputs_count         = 3,
           .inputs               = {
               {.value_length = 96,
                .value        = hash_record,
                .type_length  = 9,
                .type         = (uint8_t *) "\x03\x07\x63\x72\x65\x64\x69\x74\x73"},
               {.value_length = 32,
                .value
                = (uint8_t *) "\x82\x48\xd5\xe8\x5a\xc4\xc1\x23\x46\xf8\x45\x8b\xd9\x39\xf1\xce"
                              "\x25\xae\x03\xe9\xc6\xcb\xc8\x86\x28\x6d\xf1\x61\x63\x0a\x75\x0c",
                .type_length = 3,
                .type        = (uint8_t *) "\x02\x00\x00"},
               {.value_length = 16,
                .value
                = (uint8_t *) "\xe8\x03\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
                .type_length = 3,
                .type        = (uint8_t *) "\x02\x00\x0d"},
           }}
    };

    assert_int_equal(tx_parse(&datas_arc20_private, &tx), 0);

    datas_arc20_private.prepared_request.inputs[1].type = type_11;
    assert_int_equal(tx_parse(&datas_arc20_private, &tx), -1);

    char function_name_3[26]                                  = "transfer_private_to_public";
    datas_arc20_private.prepared_request.function_name_length = sizeof(function_name_3);
    datas_arc20_private.prepared_request.function_name        = function_name_3;
    datas_arc20_private.prepared_request.inputs[1].type       = type_11;
    datas_arc20_private.prepared_request.inputs[2].type       = type_1;
    assert_int_equal(tx_parse(&datas_arc20_private, &tx), 0);

    datas_arc20_private.prepared_request.inputs[1].type = type_12;
    assert_int_equal(tx_parse(&datas_arc20_private, &tx), -1);

    sign_transaction_datas_t datas_arc20_batch_private = {
        .max_base_fee             = 100,
        .max_priority_fee         = 500,
        .fee_function_name_length = 11,
        .fee_function_name        = "fee_private",
        .fee_program_id_length    = 12,
        .fee_program_id           = "credits.aleo",
        .prepared_request
        = {.program_id_length    = 20,
           .program_id           = "ldg_arc20_p_213.aleo",
           .function_name_length = 18,
           .function_name        = "transfer_private_2",
           .nested_call_count    = 2,
           .inputs_count         = 5,
           .inputs               = {
               {.value_length = 31,
                .value        = (uint8_t *) "arc20_eth\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
                                            "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
                .type_length  = 3,
                .type         = (uint8_t *) "\x02\x00\x11"},
               {.value_length = 96,
                .value        = hash_record,
                .type_length  = 9,
                .type         = (uint8_t *) "\x06"},
               {.value_length = 96,
                .value        = hash_record,
                .type_length  = 9,
                .type         = (uint8_t *) "\x06"},
               {.value_length = 32,
                .value
                = (uint8_t *) "\x82\x48\xd5\xe8\x5a\xc4\xc1\x23\x46\xf8\x45\x8b\xd9\x39\xf1\xce"
                              "\x25\xae\x03\xe9\xc6\xcb\xc8\x86\x28\x6d\xf1\x61\x63\x0a\x75\x0c",
                .type_length = 3,
                .type        = (uint8_t *) "\x02\x00\x00"},
               {.value_length = 16,
                .value
                = (uint8_t *) "\xe8\x03\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
                .type_length = 3,
                .type        = (uint8_t *) "\x02\x00\x0d"},
           }}
    };
    memcpy(hash_record, hash_record_c, sizeof(hash_record));
    assert_int_equal(tx_parse(&datas_arc20_batch_private, &tx), 0);

    char program_id_5[22]                                        = "ldg_arc20_p2p_213.aleo";
    datas_arc20_batch_private.prepared_request.program_id_length = sizeof(program_id_5);
    datas_arc20_batch_private.prepared_request.program_id        = program_id_5;
    char function_name_5[28]                                     = "transfer_private_to_public_2";
    datas_arc20_batch_private.prepared_request.function_name_length = sizeof(function_name_5);
    datas_arc20_batch_private.prepared_request.function_name        = function_name_5;
    datas_arc20_batch_private.prepared_request.inputs[3].type       = type_11;
    datas_arc20_batch_private.prepared_request.inputs[4].type       = type_1;
    assert_int_equal(tx_parse(&datas_arc20_batch_private, &tx), 0);
}

// ─── credits.aleo staking ────────────────────────────────────────────────────
// Address vectors below were produced by the application's own bech32_convert_bits() +
// bech32_encode(), so the expected strings are exactly what the review screen renders.

/* aleo1sfydt6z6cnqjx3hcgk9ajw03ecj6uqlfcm9u3p3gdhckzcc2w5xqv3v3pe */
static uint8_t ADDR_VALIDATOR[32]
    = "\x82\x48\xd5\xe8\x5a\xc4\xc1\x23\x46\xf8\x45\x8b\xd9\x39\xf1\xce"
      "\x25\xae\x03\xe9\xc6\xcb\xc8\x86\x28\x6d\xf1\x61\x63\x0a\x75\x0c";
static const char ADDR_VALIDATOR_STR[]
    = "aleo1sfydt6z6cnqjx3hcgk9ajw03ecj6uqlfcm9u3p3gdhckzcc2w5xqv3v3pe";

/* aleo1quvzjwjtt3kharaqk8pd8e84qctjsw22tdk8mr5lkrqa9cl5q5psq6tr7y (not the signer's address) */
static uint8_t ADDR_WITHDRAWAL[32]
    = "\x07\x18\x29\x3a\x4b\x5c\x6d\x7e\x8f\xa0\xb1\xc2\xd3\xe4\xf5\x06"
      "\x17\x28\x39\x4a\x5b\x6c\x7d\x8e\x9f\xb0\xc1\xd2\xe3\xf4\x05\x03";

/* aleo17rk74elyu80dhkx46t8uejwxc0qtmw4hkjc6a2ag5k3fl8yejczslgp4l4 */
static uint8_t ADDR_STAKER[32]
    = "\xf0\xed\xea\xe7\xe4\xe1\xde\xdb\xd8\xd5\xd2\xcf\xcc\xc9\xc6\xc3"
      "\xc0\xbd\xba\xb7\xb4\xb1\xae\xab\xa8\xa5\xa2\x9f\x9c\x99\x96\x05";
static const char ADDR_STAKER_STR[]
    = "aleo17rk74elyu80dhkx46t8uejwxc0qtmw4hkjc6a2ag5k3fl8yejczslgp4l4";

/* aleo1k7349nakx72h3q3gm8zg6f0dksnkp9p9ha93lrx6zu2gnvjzpyrsj28j54 (Speculos m/44'/683'/0'/0') */
static uint8_t ADDR_SIGNER[32]
    = "\xb7\xa3\x52\xcf\xb6\x37\x95\x78\x82\x28\xd9\xc4\x8d\x25\xed\xb4"
      "\x27\x60\x94\x25\xbf\x4b\x1f\x8c\xda\x17\x14\x89\xb2\x42\x09\x07";
static const char ADDR_SIGNER_STR[]
    = "aleo1k7349nakx72h3q3gm8zg6f0dksnkp9p9ha93lrx6zu2gnvjzpyrsj28j54";

// Seed returned by sys_hdkey_derive on Speculos for m/44'/683'/0'/0', from which ADDR_SIGNER derives
static uint8_t SIGNER_SEED[32]
    = "\xcd\x28\x45\x51\xff\x6f\x3f\x39\x3d\x74\xac\x8b\x78\x2a\x04\xec"
      "\x9c\x56\xe4\xa0\xa6\x87\xd2\x3f\xe6\x89\xf3\x64\x22\x1e\x15\xe6";

// ADDR_VALIDATOR with bit 255 set. Only the low FIELD_MODULUS_BITS bits of an address input are
// signed, so this renders as a different address than it commits to and must be refused.
static uint8_t ADDR_NON_CANONICAL[32]
    = "\x82\x48\xd5\xe8\x5a\xc4\xc1\x23\x46\xf8\x45\x8b\xd9\x39\xf1\xce"
      "\x25\xae\x03\xe9\xc6\xcb\xc8\x86\x28\x6d\xf1\x61\x63\x0a\x75\x8c";

// 123456789 as a little endian u64
static uint8_t AMOUNT_LE[8] = "\x15\xcd\x5b\x07\x00\x00\x00\x00";
#define AMOUNT_VALUE (123456789ULL)

static uint8_t TYPE_ADDRESS_PUBLIC[3]  = "\x01\x00\x00";
static uint8_t TYPE_ADDRESS_PRIVATE[3] = "\x02\x00\x00";
static uint8_t TYPE_U64_PUBLIC[3]      = "\x01\x00\x0c";
static uint8_t TYPE_U64_PRIVATE[3]     = "\x02\x00\x0c";
static uint8_t TYPE_U128_PUBLIC[3]     = "\x01\x00\x0d";
static uint8_t TYPE_STRUCT_PUBLIC[3]   = "\x01\x01\x00";

// Queues one sys_hdkey_derive call returning SIGNER_SEED with the given status
static void expect_signer_derivation(bolos_err_t status)
{
    will_return(sys_hdkey_derive, SIGNER_SEED);
    will_return(sys_hdkey_derive, status);
}

static void test_tx_staking_bond_parse(void **state)
{
    (void) state;
    memset(&G_context, 0, sizeof(G_context));
    tx_t tx;

    // The withdrawal address is checked against the signer's address, derived from the BIP32 path
    uint32_t signer_path[4] = {0x8000002c, 0x800002ab, 0x80000000, 0x80000000};
    memcpy(G_context.bip32_path, signer_path, sizeof(signer_path));
    G_context.bip32_path_len = 4;
    will_return_always(cx_bn_lock, CX_OK);
    will_return_always(cx_ecpoint_alloc, CX_OK);
    will_return_always(cx_ecpoint_init, CX_OK);
    will_return_always(cx_ecpoint_rnd_scalarmul, CX_OK);
    will_return_always(cx_ecpoint_export, CX_OK);
    will_return_always(cx_ecpoint_destroy, CX_OK);
    will_return_always(cx_bn_unlock, CX_OK);

    sign_transaction_datas_t datas_bonding = {
        .max_base_fee             = 100,
        .max_priority_fee         = 500,
        .fee_function_name_length = 10,
        .fee_function_name        = "fee_public",
        .fee_program_id_length    = 12,
        .fee_program_id           = "credits.aleo",
        .prepared_request
        = {.program_id_length    = 12,
           .program_id           = "credits.aleo",
           .function_name_length = 11,
           .function_name        = "bond_public",
           .inputs_count         = 3,
           .inputs
           = {{.value_length = 32,
               .value        = ADDR_VALIDATOR,
               .type_length  = 3,
               .type         = TYPE_ADDRESS_PUBLIC},
              {.value_length = 32,
               .value        = ADDR_SIGNER,
               .type_length  = 3,
               .type         = TYPE_ADDRESS_PUBLIC},
              {.value_length = 8, .value = AMOUNT_LE, .type_length = 3, .type = TYPE_U64_PUBLIC}}}
    };

    // Happy path: every field reaching the review screen must be decoded correctly.
    expect_signer_derivation(SWO_OK);
    assert_int_equal(tx_parse(&datas_bonding, &tx), 0);
    assert_int_equal(tx.type, TX_STAKING_BOND);
    assert_string_equal(tx.staking.validator_address, ADDR_VALIDATOR_STR);
    assert_string_equal(tx.staking.withdrawal_address, ADDR_SIGNER_STR);
    assert_int_equal(tx.staking.amount, AMOUNT_VALUE);
    // bond_public has no staker input, so that screen field must stay empty
    assert_int_equal(tx.staking.staker_address[0], '\0');

    // The withdrawal address must be the signer's one
    datas_bonding.prepared_request.inputs[1].value = ADDR_WITHDRAWAL;
    expect_signer_derivation(SWO_OK);
    assert_int_equal(tx_parse(&datas_bonding, &tx), -1);
    // A signer address derivation failure must be propagated
    datas_bonding.prepared_request.inputs[1].value = ADDR_SIGNER;
    expect_signer_derivation(0x0001);
    assert_int_equal(tx_parse(&datas_bonding, &tx), -1);
    // Validator and withdrawal must not be transposed: the signer as validator is refused.
    datas_bonding.prepared_request.inputs[0].value = ADDR_SIGNER;
    datas_bonding.prepared_request.inputs[1].value = ADDR_VALIDATOR;
    expect_signer_derivation(SWO_OK);
    assert_int_equal(tx_parse(&datas_bonding, &tx), -1);
    datas_bonding.prepared_request.inputs[0].value = ADDR_VALIDATOR;
    datas_bonding.prepared_request.inputs[1].value = ADDR_SIGNER;
    expect_signer_derivation(SWO_OK);
    assert_int_equal(tx_parse(&datas_bonding, &tx), 0);

    // inputs_count must match the database entry for bond_public
    datas_bonding.prepared_request.inputs_count = 2;
    assert_int_equal(tx_parse(&datas_bonding, &tx), -1);
    datas_bonding.prepared_request.inputs_count = 4;
    assert_int_equal(tx_parse(&datas_bonding, &tx), -1);
    datas_bonding.prepared_request.inputs_count = 3;

    // Both addresses are public inputs
    datas_bonding.prepared_request.inputs[0].type = TYPE_ADDRESS_PRIVATE;
    assert_int_equal(tx_parse(&datas_bonding, &tx), -1);
    datas_bonding.prepared_request.inputs[0].type = TYPE_STRUCT_PUBLIC;
    assert_int_equal(tx_parse(&datas_bonding, &tx), -1);
    datas_bonding.prepared_request.inputs[0].type = TYPE_U64_PUBLIC;
    assert_int_equal(tx_parse(&datas_bonding, &tx), -1);
    datas_bonding.prepared_request.inputs[0].type = TYPE_ADDRESS_PUBLIC;

    datas_bonding.prepared_request.inputs[1].type = TYPE_ADDRESS_PRIVATE;
    assert_int_equal(tx_parse(&datas_bonding, &tx), -1);
    datas_bonding.prepared_request.inputs[1].type = TYPE_ADDRESS_PUBLIC;

    // Address type and value lengths are exact
    datas_bonding.prepared_request.inputs[0].type_length = 2;
    assert_int_equal(tx_parse(&datas_bonding, &tx), -1);
    datas_bonding.prepared_request.inputs[0].type_length  = 3;
    datas_bonding.prepared_request.inputs[0].value_length = 31;
    assert_int_equal(tx_parse(&datas_bonding, &tx), -1);
    datas_bonding.prepared_request.inputs[0].value_length = 33;
    assert_int_equal(tx_parse(&datas_bonding, &tx), -1);
    datas_bonding.prepared_request.inputs[0].value_length = 32;

    // A non-canonical address encoding is displayed but not signed, so it must be refused
    datas_bonding.prepared_request.inputs[0].value = ADDR_NON_CANONICAL;
    assert_int_equal(tx_parse(&datas_bonding, &tx), -1);
    datas_bonding.prepared_request.inputs[0].value = ADDR_VALIDATOR;
    datas_bonding.prepared_request.inputs[1].value = ADDR_NON_CANONICAL;
    assert_int_equal(tx_parse(&datas_bonding, &tx), -1);
    datas_bonding.prepared_request.inputs[1].value = ADDR_SIGNER;

    // The amount is a public u64
    datas_bonding.prepared_request.inputs[2].type = TYPE_U64_PRIVATE;
    expect_signer_derivation(SWO_OK);
    assert_int_equal(tx_parse(&datas_bonding, &tx), -1);
    datas_bonding.prepared_request.inputs[2].type = TYPE_U128_PUBLIC;
    expect_signer_derivation(SWO_OK);
    assert_int_equal(tx_parse(&datas_bonding, &tx), -1);
    datas_bonding.prepared_request.inputs[2].type         = TYPE_U64_PUBLIC;
    datas_bonding.prepared_request.inputs[2].value_length = 16;
    expect_signer_derivation(SWO_OK);
    assert_int_equal(tx_parse(&datas_bonding, &tx), -1);
    datas_bonding.prepared_request.inputs[2].value_length = 8;

    // Amount boundaries must survive the little endian decode
    uint8_t amount_zero[8]                         = "\x00\x00\x00\x00\x00\x00\x00\x00";
    uint8_t amount_max[8]                          = "\xff\xff\xff\xff\xff\xff\xff\xff";
    datas_bonding.prepared_request.inputs[2].value = amount_zero;
    expect_signer_derivation(SWO_OK);
    assert_int_equal(tx_parse(&datas_bonding, &tx), 0);
    assert_int_equal(tx.staking.amount, 0);
    datas_bonding.prepared_request.inputs[2].value = amount_max;
    expect_signer_derivation(SWO_OK);
    assert_int_equal(tx_parse(&datas_bonding, &tx), 0);
    assert_true(tx.staking.amount == UINT64_MAX);
    datas_bonding.prepared_request.inputs[2].value = AMOUNT_LE;

    // An unknown function on a known program must not resolve to a staking parser
    char unknown_function[11]                    = "bind_public";
    datas_bonding.prepared_request.function_name = unknown_function;
    assert_int_equal(tx_parse(&datas_bonding, &tx), -1);
    datas_bonding.prepared_request.function_name = "bond_public";

    expect_signer_derivation(SWO_OK);
    assert_int_equal(tx_parse(&datas_bonding, &tx), 0);
}

static void test_tx_staking_unbond_parse(void **state)
{
    (void) state;
    memset(&G_context, 0, sizeof(G_context));
    tx_t tx;

    sign_transaction_datas_t datas_bonding = {
        .max_base_fee             = 100,
        .max_priority_fee         = 500,
        .fee_function_name_length = 10,
        .fee_function_name        = "fee_public",
        .fee_program_id_length    = 12,
        .fee_program_id           = "credits.aleo",
        .prepared_request
        = {.program_id_length    = 12,
           .program_id           = "credits.aleo",
           .function_name_length = 13,
           .function_name        = "unbond_public",
           .inputs_count         = 2,
           .inputs
           = {{.value_length = 32,
               .value        = ADDR_STAKER,
               .type_length  = 3,
               .type         = TYPE_ADDRESS_PUBLIC},
              {.value_length = 8, .value = AMOUNT_LE, .type_length = 3, .type = TYPE_U64_PUBLIC}}}
    };

    assert_int_equal(tx_parse(&datas_bonding, &tx), 0);
    assert_int_equal(tx.type, TX_STAKING_UNBOND);
    assert_string_equal(tx.staking.staker_address, ADDR_STAKER_STR);
    assert_int_equal(tx.staking.amount, AMOUNT_VALUE);
    // unbond_public displays Staker and Amount only; the bond-only fields must stay empty
    assert_int_equal(tx.staking.validator_address[0], '\0');
    assert_int_equal(tx.staking.withdrawal_address[0], '\0');

    datas_bonding.prepared_request.inputs_count = 1;
    assert_int_equal(tx_parse(&datas_bonding, &tx), -1);
    datas_bonding.prepared_request.inputs_count = 3;
    assert_int_equal(tx_parse(&datas_bonding, &tx), -1);
    datas_bonding.prepared_request.inputs_count = 2;

    datas_bonding.prepared_request.inputs[0].type = TYPE_ADDRESS_PRIVATE;
    assert_int_equal(tx_parse(&datas_bonding, &tx), -1);
    datas_bonding.prepared_request.inputs[0].type = TYPE_ADDRESS_PUBLIC;

    datas_bonding.prepared_request.inputs[0].value_length = 31;
    assert_int_equal(tx_parse(&datas_bonding, &tx), -1);
    datas_bonding.prepared_request.inputs[0].value_length = 32;

    datas_bonding.prepared_request.inputs[0].value = ADDR_NON_CANONICAL;
    assert_int_equal(tx_parse(&datas_bonding, &tx), -1);
    datas_bonding.prepared_request.inputs[0].value = ADDR_STAKER;

    datas_bonding.prepared_request.inputs[1].type = TYPE_U64_PRIVATE;
    assert_int_equal(tx_parse(&datas_bonding, &tx), -1);
    datas_bonding.prepared_request.inputs[1].type = TYPE_U64_PUBLIC;

    datas_bonding.prepared_request.inputs[1].value_length = 4;
    assert_int_equal(tx_parse(&datas_bonding, &tx), -1);
    datas_bonding.prepared_request.inputs[1].value_length = 8;

    assert_int_equal(tx_parse(&datas_bonding, &tx), 0);
}

static void test_tx_staking_claim_parse(void **state)
{
    (void) state;
    memset(&G_context, 0, sizeof(G_context));
    tx_t tx;

    sign_transaction_datas_t datas_bonding = {
        .max_base_fee             = 100,
        .max_priority_fee         = 500,
        .fee_function_name_length = 10,
        .fee_function_name        = "fee_public",
        .fee_program_id_length    = 12,
        .fee_program_id           = "credits.aleo",
        .prepared_request         = {.program_id_length    = 12,
                                     .program_id           = "credits.aleo",
                                     .function_name_length = 19,
                                     .function_name        = "claim_unbond_public",
                                     .inputs_count         = 1,
                                     .inputs               = {{.value_length = 32,
                                                               .value        = ADDR_STAKER,
                                                               .type_length  = 3,
                                                               .type         = TYPE_ADDRESS_PUBLIC}}}
    };

    assert_int_equal(tx_parse(&datas_bonding, &tx), 0);
    assert_int_equal(tx.type, TX_STAKING_CLAIM);
    assert_string_equal(tx.staking.staker_address, ADDR_STAKER_STR);
    // claim_unbond_public carries no amount; the review screen must not inherit a stale one
    assert_int_equal(tx.staking.amount, 0);
    assert_int_equal(tx.staking.validator_address[0], '\0');
    assert_int_equal(tx.staking.withdrawal_address[0], '\0');

    datas_bonding.prepared_request.inputs_count = 0;
    assert_int_equal(tx_parse(&datas_bonding, &tx), -1);
    datas_bonding.prepared_request.inputs_count = 2;
    assert_int_equal(tx_parse(&datas_bonding, &tx), -1);
    datas_bonding.prepared_request.inputs_count = 1;

    datas_bonding.prepared_request.inputs[0].type = TYPE_ADDRESS_PRIVATE;
    assert_int_equal(tx_parse(&datas_bonding, &tx), -1);
    datas_bonding.prepared_request.inputs[0].type = TYPE_U64_PUBLIC;
    assert_int_equal(tx_parse(&datas_bonding, &tx), -1);
    datas_bonding.prepared_request.inputs[0].type = TYPE_ADDRESS_PUBLIC;

    datas_bonding.prepared_request.inputs[0].type_length = 2;
    assert_int_equal(tx_parse(&datas_bonding, &tx), -1);
    datas_bonding.prepared_request.inputs[0].type_length = 3;

    datas_bonding.prepared_request.inputs[0].value_length = 31;
    assert_int_equal(tx_parse(&datas_bonding, &tx), -1);
    datas_bonding.prepared_request.inputs[0].value_length = 32;

    datas_bonding.prepared_request.inputs[0].value = ADDR_NON_CANONICAL;
    assert_int_equal(tx_parse(&datas_bonding, &tx), -1);
    datas_bonding.prepared_request.inputs[0].value = ADDR_STAKER;

    assert_int_equal(tx_parse(&datas_bonding, &tx), 0);
}

int main()
{
    const struct CMUnitTest tests[] = {cmocka_unit_test(test_tx_extract),
                                       cmocka_unit_test(test_tx_parse),
                                       cmocka_unit_test(test_tx_token_parse),
                                       cmocka_unit_test(test_tx_token_arc20_parse),
                                       cmocka_unit_test(test_tx_staking_bond_parse),
                                       cmocka_unit_test(test_tx_staking_unbond_parse),
                                       cmocka_unit_test(test_tx_staking_claim_parse)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}
