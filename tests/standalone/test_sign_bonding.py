from typing import Any

import pytest
from application_client.command_sender import CommandSender
from application_client.response_unpacker import (
    unpack_sign_transaction_response,
)
from ragger.backend.interface import BackendInterface
from ragger.error import ExceptionRAPDU, StatusWords
from ragger.navigator import NavInsID
from ragger.navigator.navigation_scenario import NavigateWithScenario


def check_response(received: dict, expected: dict) -> bool:
    for key in expected:
        if key not in received:
            return False
        if type(received[key]) is not type(expected[key]):
            return False
        if isinstance(received[key], list):
            index = 0
            for item in received[key]:
                if item != expected[key][index]:
                    return False
                index += 1
        elif isinstance(received[key], dict):
            if not check_response(received[key], expected[key]):
                return False
        elif received[key] != expected[key]:
            print("Error " + str(received[key]) + " != " + str(expected[key]))
            return False
    return True


def forge_bond_public(
    max_base_fee: int,
    max_priority_fee: int,
    validator_address: str,
    withdrawal_address: str,
    amount: int,
    program_checksum: str = "",
) -> dict:

    data: dict[str, Any] = {
        "type": "intent",
        "max_base_fee": max_base_fee,
        "max_priority_fee": max_priority_fee,
        "fee_program_id": "credits.aleo",
        "fee_function_name": "fee_public",
    }
    data["request"] = {
        "network_id": "mainnet",
        "program_id": "credits.aleo",
        "function_name": "bond_public",
    }
    data["request"]["inputs"] = [
        {"type": "address.public", "value": validator_address},
        {"type": "address.public", "value": withdrawal_address},
        {"type": "u64.public", "value": amount},
    ]
    data["request"]["nested_call_count"] = 0
    data["request"]["program_checksum"] = program_checksum

    return data


def forge_unbond_public(
    max_base_fee: int,
    max_priority_fee: int,
    staker_address: str,
    amount: int,
    program_checksum: str = "",
) -> dict:

    data: dict[str, Any] = {
        "type": "intent",
        "max_base_fee": max_base_fee,
        "max_priority_fee": max_priority_fee,
        "fee_program_id": "credits.aleo",
        "fee_function_name": "fee_public",
    }
    data["request"] = {
        "network_id": "mainnet",
        "program_id": "credits.aleo",
        "function_name": "unbond_public",
    }
    data["request"]["inputs"] = [
        {"type": "address.public", "value": staker_address},
        {"type": "u64.public", "value": amount},
    ]
    data["request"]["nested_call_count"] = 0
    data["request"]["program_checksum"] = program_checksum

    return data


def forge_claim_unbond_public(
    max_base_fee: int,
    max_priority_fee: int,
    staker_address: str,
    program_checksum: str = "",
) -> dict:

    data: dict[str, Any] = {
        "type": "intent",
        "max_base_fee": max_base_fee,
        "max_priority_fee": max_priority_fee,
        "fee_program_id": "credits.aleo",
        "fee_function_name": "fee_public",
    }
    data["request"] = {
        "network_id": "mainnet",
        "program_id": "credits.aleo",
        "function_name": "claim_unbond_public",
    }
    data["request"]["inputs"] = [
        {"type": "address.public", "value": staker_address},
    ]
    data["request"]["nested_call_count"] = 0
    data["request"]["program_checksum"] = program_checksum

    return data


def forge_public_fee(base_fee: int, priority_fee: int, execution_id: str, program_checksum: str = "") -> dict:
    data: dict[str, Any] = {"type": "fee"}
    data["request"] = {
        "network_id": "mainnet",
        "program_id": "credits.aleo",
        "function_name": "fee_public",
    }
    data["request"]["inputs"] = [
        {"type": "u64.public", "value": base_fee},
        {"type": "u64.public", "value": priority_fee},
        {"type": "field.public", "value": execution_id},
    ]
    data["request"]["nested_call_count"] = 0
    data["request"]["program_checksum"] = program_checksum

    return data


def test_sign_transaction_bond_public(backend: BackendInterface, scenario_navigator: NavigateWithScenario) -> None:
    client = CommandSender(backend)
    tx_datas = forge_bond_public(
        500,
        100,
        "aleo1sfydt6z6cnqjx3hcgk9ajw03ecj6uqlfcm9u3p3gdhckzcc2w5xqv3v3pe",
        "aleo1ktwldl75earvxjur7devnqvdccjeuqa6807078klkg0a0l6ayq8qu9xzg4",
        1000,
    )
    tx_datas["path"] = "m/44'/683'/0'/0'"
    with client.sign_transaction(tx_datas=tx_datas):
        scenario_navigator.review_approve_with_spinner("Calculating fees")

    response = client.get_async_response().data
    unpacked = unpack_sign_transaction_response(response)
    expected = {
        "structure_type": 42,
        "version": 1,
        "signature": {
            "pk_sig": "1d4c4b28dd6ce05ab520f00b71c081d480684c746a7d8f3b0a3a68d410ce840e",
            "pr_sig": "3a8a3cfee21ce108285cca4cc50abb5ac9044acf26959ddb7722cbb968bdc310",
        },
        "gammas_count": 0,
    }
    assert check_response(unpacked, expected)

    tx_datas = forge_public_fee(
        500,
        100,
        "7266375125414209082394925781071362722506946030314916664133746682226945366259field",
    )
    with client.sign_transaction(tx_datas=tx_datas):
        if scenario_navigator.device.is_nano:
            instruction = NavInsID.BOTH_CLICK
        else:
            instruction = NavInsID.USE_CASE_REVIEW_TAP
        scenario_navigator.navigator.navigate_until_text(
            navigate_instruction=instruction,
            validation_instructions=[],
            text="Transaction signed",
            timeout=3,
            screen_change_before_first_instruction=False,
            screen_change_after_last_instruction=True,
        )

    response = client.get_async_response().data
    unpacked = unpack_sign_transaction_response(response)
    expected = {
        "structure_type": 42,
        "version": 1,
        "signature": {
            "pk_sig": "1d4c4b28dd6ce05ab520f00b71c081d480684c746a7d8f3b0a3a68d410ce840e",
            "pr_sig": "3a8a3cfee21ce108285cca4cc50abb5ac9044acf26959ddb7722cbb968bdc310",
        },
        "gammas_count": 0,
    }
    assert check_response(unpacked, expected)


def test_sign_transaction_bond_public_testnet(backend: BackendInterface, scenario_navigator: NavigateWithScenario) -> None:
    client = CommandSender(backend)
    tx_datas = forge_bond_public(
        500,
        100,
        "aleo1sfydt6z6cnqjx3hcgk9ajw03ecj6uqlfcm9u3p3gdhckzcc2w5xqv3v3pe",
        "aleo1ktwldl75earvxjur7devnqvdccjeuqa6807078klkg0a0l6ayq8qu9xzg4",
        1000,
    )
    tx_datas["request"]["network_id"] = "testnet"
    tx_datas["path"] = "m/44'/683'/0'/0'"
    with client.sign_transaction(tx_datas=tx_datas):
        scenario_navigator.review_approve_with_spinner("Calculating fees")

    response = client.get_async_response().data
    unpacked = unpack_sign_transaction_response(response)
    expected = {
        "structure_type": 42,
        "version": 1,
        "signature": {
            "pk_sig": "1d4c4b28dd6ce05ab520f00b71c081d480684c746a7d8f3b0a3a68d410ce840e",
            "pr_sig": "3a8a3cfee21ce108285cca4cc50abb5ac9044acf26959ddb7722cbb968bdc310",
        },
        "gammas_count": 0,
    }
    assert check_response(unpacked, expected)

    tx_datas = forge_public_fee(
        500,
        100,
        "7266375125414209082394925781071362722506946030314916664133746682226945366259field",
    )
    with client.sign_transaction(tx_datas=tx_datas):
        if scenario_navigator.device.is_nano:
            instruction = NavInsID.BOTH_CLICK
        else:
            instruction = NavInsID.USE_CASE_REVIEW_TAP
        scenario_navigator.navigator.navigate_until_text(
            navigate_instruction=instruction,
            validation_instructions=[],
            text="Transaction signed",
            timeout=3,
            screen_change_before_first_instruction=False,
            screen_change_after_last_instruction=True,
        )

    response = client.get_async_response().data
    unpacked = unpack_sign_transaction_response(response)
    expected = {
        "structure_type": 42,
        "version": 1,
        "signature": {
            "pk_sig": "1d4c4b28dd6ce05ab520f00b71c081d480684c746a7d8f3b0a3a68d410ce840e",
            "pr_sig": "3a8a3cfee21ce108285cca4cc50abb5ac9044acf26959ddb7722cbb968bdc310",
        },
        "gammas_count": 0,
    }
    assert check_response(unpacked, expected)


def test_sign_transaction_unbond_public(backend: BackendInterface, scenario_navigator: NavigateWithScenario) -> None:
    client = CommandSender(backend)
    tx_datas = forge_unbond_public(
        500,
        100,
        "aleo1sfydt6z6cnqjx3hcgk9ajw03ecj6uqlfcm9u3p3gdhckzcc2w5xqv3v3pe",
        1000,
    )
    tx_datas["path"] = "m/44'/683'/0'/0'"
    with client.sign_transaction(tx_datas=tx_datas):
        scenario_navigator.review_approve_with_spinner("Calculating fees")

    response = client.get_async_response().data
    unpacked = unpack_sign_transaction_response(response)
    expected = {
        "structure_type": 42,
        "version": 1,
        "signature": {
            "pk_sig": "1d4c4b28dd6ce05ab520f00b71c081d480684c746a7d8f3b0a3a68d410ce840e",
            "pr_sig": "3a8a3cfee21ce108285cca4cc50abb5ac9044acf26959ddb7722cbb968bdc310",
        },
        "gammas_count": 0,
    }
    assert check_response(unpacked, expected)

    tx_datas = forge_public_fee(
        500,
        100,
        "7266375125414209082394925781071362722506946030314916664133746682226945366259field",
    )
    with client.sign_transaction(tx_datas=tx_datas):
        if scenario_navigator.device.is_nano:
            instruction = NavInsID.BOTH_CLICK
        else:
            instruction = NavInsID.USE_CASE_REVIEW_TAP
        scenario_navigator.navigator.navigate_until_text(
            navigate_instruction=instruction,
            validation_instructions=[],
            text="Transaction signed",
            timeout=3,
            screen_change_before_first_instruction=False,
            screen_change_after_last_instruction=True,
        )

    response = client.get_async_response().data
    unpacked = unpack_sign_transaction_response(response)
    expected = {
        "structure_type": 42,
        "version": 1,
        "signature": {
            "pk_sig": "1d4c4b28dd6ce05ab520f00b71c081d480684c746a7d8f3b0a3a68d410ce840e",
            "pr_sig": "3a8a3cfee21ce108285cca4cc50abb5ac9044acf26959ddb7722cbb968bdc310",
        },
        "gammas_count": 0,
    }
    assert check_response(unpacked, expected)


def test_sign_transaction_unbond_public_testnet(backend: BackendInterface, scenario_navigator: NavigateWithScenario) -> None:
    client = CommandSender(backend)
    tx_datas = forge_unbond_public(
        500,
        100,
        "aleo1sfydt6z6cnqjx3hcgk9ajw03ecj6uqlfcm9u3p3gdhckzcc2w5xqv3v3pe",
        1000,
    )
    tx_datas["request"]["network_id"] = "testnet"
    tx_datas["path"] = "m/44'/683'/0'/0'"
    with client.sign_transaction(tx_datas=tx_datas):
        scenario_navigator.review_approve_with_spinner("Calculating fees")

    response = client.get_async_response().data
    unpacked = unpack_sign_transaction_response(response)
    expected = {
        "structure_type": 42,
        "version": 1,
        "signature": {
            "pk_sig": "1d4c4b28dd6ce05ab520f00b71c081d480684c746a7d8f3b0a3a68d410ce840e",
            "pr_sig": "3a8a3cfee21ce108285cca4cc50abb5ac9044acf26959ddb7722cbb968bdc310",
        },
        "gammas_count": 0,
    }
    assert check_response(unpacked, expected)

    tx_datas = forge_public_fee(
        500,
        100,
        "7266375125414209082394925781071362722506946030314916664133746682226945366259field",
    )
    with client.sign_transaction(tx_datas=tx_datas):
        if scenario_navigator.device.is_nano:
            instruction = NavInsID.BOTH_CLICK
        else:
            instruction = NavInsID.USE_CASE_REVIEW_TAP
        scenario_navigator.navigator.navigate_until_text(
            navigate_instruction=instruction,
            validation_instructions=[],
            text="Transaction signed",
            timeout=3,
            screen_change_before_first_instruction=False,
            screen_change_after_last_instruction=True,
        )

    response = client.get_async_response().data
    unpacked = unpack_sign_transaction_response(response)
    expected = {
        "structure_type": 42,
        "version": 1,
        "signature": {
            "pk_sig": "1d4c4b28dd6ce05ab520f00b71c081d480684c746a7d8f3b0a3a68d410ce840e",
            "pr_sig": "3a8a3cfee21ce108285cca4cc50abb5ac9044acf26959ddb7722cbb968bdc310",
        },
        "gammas_count": 0,
    }
    assert check_response(unpacked, expected)


def test_sign_transaction_claim_unbond_public(backend: BackendInterface, scenario_navigator: NavigateWithScenario) -> None:
    client = CommandSender(backend)
    tx_datas = forge_claim_unbond_public(500, 100, "aleo1sfydt6z6cnqjx3hcgk9ajw03ecj6uqlfcm9u3p3gdhckzcc2w5xqv3v3pe")
    tx_datas["path"] = "m/44'/683'/0'/0'"
    with client.sign_transaction(tx_datas=tx_datas):
        scenario_navigator.review_approve_with_spinner("Calculating fees")

    response = client.get_async_response().data
    unpacked = unpack_sign_transaction_response(response)
    expected = {
        "structure_type": 42,
        "version": 1,
        "signature": {
            "pk_sig": "1d4c4b28dd6ce05ab520f00b71c081d480684c746a7d8f3b0a3a68d410ce840e",
            "pr_sig": "3a8a3cfee21ce108285cca4cc50abb5ac9044acf26959ddb7722cbb968bdc310",
        },
        "gammas_count": 0,
    }
    assert check_response(unpacked, expected)

    tx_datas = forge_public_fee(
        500,
        100,
        "7266375125414209082394925781071362722506946030314916664133746682226945366259field",
    )
    with client.sign_transaction(tx_datas=tx_datas):
        if scenario_navigator.device.is_nano:
            instruction = NavInsID.BOTH_CLICK
        else:
            instruction = NavInsID.USE_CASE_REVIEW_TAP
        scenario_navigator.navigator.navigate_until_text(
            navigate_instruction=instruction,
            validation_instructions=[],
            text="Transaction signed",
            timeout=3,
            screen_change_before_first_instruction=False,
            screen_change_after_last_instruction=True,
        )

    response = client.get_async_response().data
    unpacked = unpack_sign_transaction_response(response)
    expected = {
        "structure_type": 42,
        "version": 1,
        "signature": {
            "pk_sig": "1d4c4b28dd6ce05ab520f00b71c081d480684c746a7d8f3b0a3a68d410ce840e",
            "pr_sig": "3a8a3cfee21ce108285cca4cc50abb5ac9044acf26959ddb7722cbb968bdc310",
        },
        "gammas_count": 0,
    }
    assert check_response(unpacked, expected)


def test_sign_transaction_claim_unbond_public_testnet(
    backend: BackendInterface, scenario_navigator: NavigateWithScenario
) -> None:
    client = CommandSender(backend)
    tx_datas = forge_claim_unbond_public(500, 100, "aleo1sfydt6z6cnqjx3hcgk9ajw03ecj6uqlfcm9u3p3gdhckzcc2w5xqv3v3pe")
    tx_datas["request"]["network_id"] = "testnet"
    tx_datas["path"] = "m/44'/683'/0'/0'"
    with client.sign_transaction(tx_datas=tx_datas):
        scenario_navigator.review_approve_with_spinner("Calculating fees")

    response = client.get_async_response().data
    unpacked = unpack_sign_transaction_response(response)
    expected = {
        "structure_type": 42,
        "version": 1,
        "signature": {
            "pk_sig": "1d4c4b28dd6ce05ab520f00b71c081d480684c746a7d8f3b0a3a68d410ce840e",
            "pr_sig": "3a8a3cfee21ce108285cca4cc50abb5ac9044acf26959ddb7722cbb968bdc310",
        },
        "gammas_count": 0,
    }
    assert check_response(unpacked, expected)

    tx_datas = forge_public_fee(
        500,
        100,
        "7266375125414209082394925781071362722506946030314916664133746682226945366259field",
    )
    with client.sign_transaction(tx_datas=tx_datas):
        if scenario_navigator.device.is_nano:
            instruction = NavInsID.BOTH_CLICK
        else:
            instruction = NavInsID.USE_CASE_REVIEW_TAP
        scenario_navigator.navigator.navigate_until_text(
            navigate_instruction=instruction,
            validation_instructions=[],
            text="Transaction signed",
            timeout=3,
            screen_change_before_first_instruction=False,
            screen_change_after_last_instruction=True,
        )

    response = client.get_async_response().data
    unpacked = unpack_sign_transaction_response(response)
    expected = {
        "structure_type": 42,
        "version": 1,
        "signature": {
            "pk_sig": "1d4c4b28dd6ce05ab520f00b71c081d480684c746a7d8f3b0a3a68d410ce840e",
            "pr_sig": "3a8a3cfee21ce108285cca4cc50abb5ac9044acf26959ddb7722cbb968bdc310",
        },
        "gammas_count": 0,
    }
    assert check_response(unpacked, expected)


def test_sign_transaction_bond_refused(backend: BackendInterface, scenario_navigator: NavigateWithScenario) -> None:
    client = CommandSender(backend)
    tx_datas = forge_bond_public(
        500,
        100,
        "aleo1sfydt6z6cnqjx3hcgk9ajw03ecj6uqlfcm9u3p3gdhckzcc2w5xqv3v3pe",
        "aleo1ktwldl75earvxjur7devnqvdccjeuqa6807078klkg0a0l6ayq8qu9xzg4",
        1000,
    )
    tx_datas["path"] = "m/44'/683'/0'/0'"

    with pytest.raises(ExceptionRAPDU) as e:
        with client.sign_transaction(tx_datas=tx_datas):
            scenario_navigator.review_reject()

    assert e.value.status == StatusWords.SWO_PERMISSION_DENIED
    assert len(e.value.data) == 0

    if scenario_navigator.device.is_nano:
        instruction = NavInsID.BOTH_CLICK
    else:
        instruction = NavInsID.USE_CASE_REVIEW_TAP
    scenario_navigator.navigator.navigate_until_text(
        navigate_instruction=instruction,
        validation_instructions=[],
        text="Transaction rejected",
        timeout=3,
        screen_change_before_first_instruction=False,
        screen_change_after_last_instruction=True,
    )
