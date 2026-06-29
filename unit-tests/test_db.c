#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <cmocka.h>

#include "cx_errors.h"
#include "db.h"

static void test_db(void **state)
{
    (void) state;

    int      status = 0;
    char     program_id[32 + 1];
    uint8_t  program_length = 0;
    char     function_name[32 + 1];
    uint8_t  function_name_length = 0;
    field_t *token_id             = NULL;

    // db_get_function_parameters
    function_parameters_t *function_parameters;

    strncpy(program_id, "credits.aleo", sizeof(program_id));
    strncpy(function_name, "transfer_public", sizeof(function_name));
    program_length       = strlen(program_id);
    function_name_length = strlen(function_name);
    status               = db_get_function_parameters(
        program_id, program_length, function_name, function_name_length, &function_parameters);
    assert_int_equal(status, 0);

    strncpy(function_name, "transfer_publi", sizeof(function_name));
    function_name_length = strlen(function_name);
    status               = db_get_function_parameters(
        program_id, program_length, function_name, function_name_length, &function_parameters);
    assert_int_equal(status, -1);

    strncpy(program_id, "usad_stablecoin.aleo", sizeof(program_id));
    strncpy(function_name, "transfer_public", sizeof(function_name));
    program_length       = strlen(program_id);
    function_name_length = strlen(function_name);
    status               = db_get_function_parameters(
        program_id, program_length, function_name, function_name_length, &function_parameters);
    assert_int_equal(status, 0);

    strncpy(program_id, "usdcx_stablecoin.aleo", sizeof(program_id));
    strncpy(function_name, "transfer_private", sizeof(function_name));
    program_length       = strlen(program_id);
    function_name_length = strlen(function_name);
    status               = db_get_function_parameters(
        program_id, program_length, function_name, function_name_length, &function_parameters);
    assert_int_equal(status, 0);

    strncpy(program_id, "usad_stablecoin.aleo", sizeof(program_id));
    strncpy(function_name, "transfer_publi", sizeof(function_name));
    program_length       = strlen(program_id);
    function_name_length = strlen(function_name);
    status               = db_get_function_parameters(
        program_id, program_length, function_name, function_name_length, &function_parameters);
    assert_int_equal(status, -1);

    strncpy(program_id, "usad_stablecoin.aleo", sizeof(program_id));
    strncpy(function_name, "transfer_publit", sizeof(function_name));
    program_length       = strlen(program_id);
    function_name_length = strlen(function_name);
    status               = db_get_function_parameters(
        program_id, program_length, function_name, function_name_length, &function_parameters);
    assert_int_equal(status, -1);

    // db_get_token_display_info
    token_display_info_t *token_display_info;
    token_id = (field_t *) &FIELD_ZERO;
    strncpy(program_id, "usad_stablecoin.aleo", sizeof(program_id));
    program_length = strlen(program_id);
    status = db_get_token_display_info(program_id, program_length, token_id, &token_display_info);
    assert_int_equal(status, 0);

    strncpy(program_id, "usad_stablecoi.aleo", sizeof(program_id));
    program_length = strlen(program_id);
    status = db_get_token_display_info(program_id, program_length, token_id, &token_display_info);
    assert_int_equal(status, -1);

    strncpy(program_id, "usad_stablecois.aleo", sizeof(program_id));
    program_length = strlen(program_id);
    status = db_get_token_display_info(program_id, program_length, token_id, &token_display_info);
    assert_int_equal(status, -1);

    token_id = (field_t *) &FIELD_ONE;
    strncpy(program_id, "usad_stablecoin.aleo", sizeof(program_id));
    program_length = strlen(program_id);
    status = db_get_token_display_info(program_id, program_length, token_id, &token_display_info);
    assert_int_equal(status, -1);
}

int main()
{
    const struct CMUnitTest tests[] = {cmocka_unit_test(test_db)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}