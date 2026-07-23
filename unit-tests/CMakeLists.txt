cmake_minimum_required(VERSION 3.10)

if(${CMAKE_VERSION} VERSION_LESS 3.10)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

# project information
project(unit_tests
        VERSION 0.1
        DESCRIPTION "Unit tests for Ledger Nano application"
        LANGUAGES C)


# guard against bad build-type strings
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

include(CTest)
ENABLE_TESTING()

# specify C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)
add_definitions(-DPRINTF=)
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -pedantic -g -O0 --coverage")

set(GCC_COVERAGE_LINK_FLAGS "--coverage -lgcov")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

# guard against in-source builds
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt. ")
endif()

if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
  cmake_policy(SET CMP0135 NEW)
endif()

set(INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/install)
include(ExternalProject)
ExternalProject_Add(
  openssl
  URL https://www.openssl.org/source/openssl-1.1.1k.tar.gz
  URL_HASH SHA256=892a0875b9872acd04a9fde79b1f943075d5ea162415de3047c327df33fbaee5
  CONFIGURE_COMMAND ./Configure
    "CC=${CMAKE_C_COMPILER}"
    "CFLAGS=${OPENSSL_CFLAGS}"
    no-afalgeng no-aria no-asan no-asm no-async
    no-autoalginit no-autoerrinit no-autoload-config
    no-bf no-buildtest-c++ no-camellia no-capieng no-cast
    no-chacha no-cmac no-cms no-comp no-crypto-mdebug
    no-crypto-mdebug-backtrace no-ct no-deprecated no-des
    no-devcryptoeng no-dgram no-dh no-dsa no-dso no-dtls
    no-ecdh no-egd no-engine no-err no-external-tests
    no-filenames no-fuzz-afl no-fuzz-libfuzzer no-gost
    no-heartbeats no-hw no-idea no-makedepend no-md2 no-md4
    no-mdc2 no-msan no-multiblock no-nextprotoneg no-ocb
    no-ocsp no-pinshared no-poly1305 no-posix-io no-psk
    no-rc2 no-rc4 no-rc5 no-rdrand no-rfc3779 no-scrypt
    no-sctp no-seed no-shared no-siphash no-sm2 no-sm3
    no-sm4 no-sock no-srp no-srtp no-sse2 no-ssl
    no-ssl3-method no-ssl-trace no-stdio no-tests no-threads
    no-tls no-ts no-ubsan no-ui-console no-unit-test
    no-whirlpool no-zlib no-zlib-dynamic
    linux-x86_64
    --prefix=${INSTALL_DIR}
  BUILD_COMMAND make
  INSTALL_COMMAND make install_sw
  BUILD_IN_SOURCE 1
)

add_compile_definitions(TEST)

include_directories(${INSTALL_DIR}/include)
link_directories(${INSTALL_DIR}/lib)

link_libraries(ssl crypto)

# ── Include directories ──────────────────────────────────────────────────────
# stubs/ must come first to intercept hardware-specific SDK headers
include_directories(stubs)
include_directories(stubs/cx)
include_directories(stubs/nbgl)
include_directories(.)
include_directories(../src)
include_directories(../src/crypto)
include_directories(../src/account)
include_directories(../src/db)
include_directories(../src/format)
include_directories(../src/helper)
include_directories(../src/swap)
include_directories(../src/token)
include_directories(../src/transaction)
include_directories($ENV{BOLOS_SDK}/lib_standard_app)
include_directories($ENV{BOLOS_SDK}/include)
include_directories($ENV{BOLOS_SDK}/lib_tlv)
include_directories($ENV{BOLOS_SDK}/lib_cxng/include)
include_directories(${INSTALL_DIR}/include)

# ── SDK shared libraries ─────────────────────────────────────────────────────
add_library(base58         SHARED $ENV{BOLOS_SDK}/lib_standard_app/base58.c)
add_library(bip32          SHARED $ENV{BOLOS_SDK}/lib_standard_app/bip32.c)
add_library(buffer         SHARED $ENV{BOLOS_SDK}/lib_standard_app/buffer.c)
add_library(read           SHARED $ENV{BOLOS_SDK}/lib_standard_app/read.c)
add_library(write          SHARED $ENV{BOLOS_SDK}/lib_standard_app/write.c)
add_library(format         SHARED $ENV{BOLOS_SDK}/lib_standard_app/format.c)
add_library(varint         SHARED $ENV{BOLOS_SDK}/lib_standard_app/varint.c)
add_library(apdu_parser    SHARED $ENV{BOLOS_SDK}/lib_standard_app/parser.c)
add_library(stubs_os_utils SHARED stubs/os_utils_stubs.c)
add_library(tlv_library    SHARED $ENV{BOLOS_SDK}/lib_tlv/tlv_library.c)

add_library(stubs_ox SHARED 
  stubs/ox_bn_stubs.c
  stubs/ox_ec_stubs.c
  stubs/cx/cx_mpi.c
  stubs/cx/cx_bn.c
  stubs/cx/cx_ecpoint.c
  stubs/cx/cx_ec_domain.c
  stubs/cx/cx_twisted_edwards.c
  stubs/cx/cx_montgomery.c
  stubs/cx/cx_weierstrass.c)

add_dependencies(stubs_ox openssl)
target_link_libraries(stubs_ox PUBLIC ssl crypto)

add_library(stubs_nbgl     SHARED stubs/nbgl/nbgl_use_case_stubs.c)
add_library(stubs_os_hdkey SHARED stubs/os_hdkey_stubs.c)

# ── App shared libraries ─────────────────────────────────────────────────────
add_library(bits          SHARED ../src/crypto/bits.c)
add_library(bigint_256    SHARED ../src/crypto/bigint_256.c)
add_library(fp256         SHARED ../src/crypto/fp256.c)
add_library(field         SHARED ../src/crypto/field.c)
add_library(scalar        SHARED ../src/crypto/scalar.c)
add_library(group         SHARED ../src/crypto/group.c)
add_library(poseidon      SHARED ../src/crypto/poseidon.c ../src/crypto/poseidon_parameters.c)
add_library(bech32        SHARED ../src/crypto/bech32.c)
add_library(bhp_1024      SHARED ../src/crypto/bhp_1024.c ../src/db/db_program_function.c ../src/db/db.c ../src/db/db_tokens.c)
add_library(account       SHARED ../src/account/account.c)
add_library(signature     SHARED ../src/account/signature.c)
add_library(transaction   SHARED ../src/transaction/tx_extract.c ../src/transaction/tx_parse.c  ../src/db/db_program_function.c ../src/db/db.c ../src/db/db_tokens.c)
add_library(send_response SHARED ../src/helper/send_response.c)
add_library(db            SHARED ../src/db/db.c ../src/db/db_program_function.c ../src/db/db_tokens.c)
add_library(formatting    SHARED ../src/format/format_u128.c ../src/swap/aleo_swap_utils.c)

# Common link set for tests that use TLV parsing
set(COMMON_LIBS buffer read write varint format cmocka gcov stubs_os_utils)

# ── test_hl_context ──────────────────────────────────────────────────────────
add_executable(test_bigint_256 test_bigint_256.c)
target_link_libraries(test_bigint_256 PUBLIC bigint_256 stubs_ox ${COMMON_LIBS})
add_test(test_bigint_256 test_bigint_256)

add_executable(test_bits test_bits.c)
target_link_libraries(test_bits PUBLIC bits fp256 bigint_256 bits stubs_ox ${COMMON_LIBS})
add_test(test_bits test_bits)

add_executable(test_fp256 test_fp256.c)
target_link_libraries(test_fp256 PUBLIC fp256 bigint_256 stubs_ox ${COMMON_LIBS})
add_test(test_fp256 test_fp256)

add_executable(test_field test_field.c)
target_link_libraries(test_field PUBLIC field fp256 bigint_256 bits stubs_ox ${COMMON_LIBS})
add_test(test_field test_field)

add_executable(test_scalar test_scalar.c)
target_link_libraries(test_scalar PUBLIC scalar field fp256 bigint_256 bits stubs_ox ${COMMON_LIBS})
add_test(test_scalar test_scalar)

add_executable(test_group test_group.c)
target_link_libraries(test_group PUBLIC group scalar field fp256 bigint_256 bits stubs_ox ${COMMON_LIBS})
add_test(test_group test_group)

add_executable(test_poseidon test_poseidon.c)
target_link_libraries(test_poseidon PUBLIC poseidon group scalar field fp256 bigint_256 bits stubs_ox ${COMMON_LIBS})
add_test(test_poseidon test_poseidon)

add_executable(test_bhp_1024 test_bhp_1024.c)
target_link_libraries(test_bhp_1024 PUBLIC bhp_1024 field fp256 bigint_256 bits stubs_ox ${COMMON_LIBS})
add_test(test_bhp_1024 test_bhp_1024)

add_executable(test_account test_account.c)
target_link_libraries(test_account PUBLIC account bhp_1024 signature stubs_os_hdkey stubs_nbgl bech32 poseidon base58 bip32 group scalar field fp256 bigint_256 bits stubs_ox ${COMMON_LIBS})
add_test(test_account test_account)

add_executable(test_signature test_signature.c)
target_link_libraries(test_signature PUBLIC signature bhp_1024 account stubs_os_hdkey stubs_nbgl bech32 poseidon base58 bip32 group scalar field fp256 bigint_256 bits stubs_ox ${COMMON_LIBS})
add_test(test_signature test_signature)

add_executable(test_transaction test_transaction.c)
target_link_libraries(test_transaction PUBLIC transaction tlv_library account stubs_os_hdkey stubs_nbgl bech32 poseidon base58 bip32 group scalar field fp256 bigint_256 bits stubs_ox ${COMMON_LIBS})
add_test(test_transaction test_transaction)

add_executable(test_send_response test_send_response.c)
target_link_libraries(test_send_response PUBLIC send_response group scalar field fp256 bigint_256 bits stubs_ox ${COMMON_LIBS})
add_test(test_send_response test_send_response)

add_executable(test_db test_db.c)
target_link_libraries(test_db PUBLIC db field fp256 bigint_256 bits stubs_ox ${COMMON_LIBS})
add_test(test_db test_db)

add_executable(test_formatting test_formatting.c)
target_link_libraries(test_formatting PUBLIC formatting ${COMMON_LIBS})
add_test(test_formatting test_formatting)
