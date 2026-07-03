import pytest

from ragger.error import ExceptionRAPDU, StatusWords
from ragger.backend.interface import BackendInterface
from ragger.navigator.navigation_scenario import NavigateWithScenario
from ragger.navigator import NavInsID

from application_client.command_sender import CommandSender, InsType
from application_client.response_unpacker import (
    unpack_sign_transaction_response,
    unpack_get_tvk_response,
)


def check_response(received: dict, expected: dict) -> bool:
    for key in expected.keys():
        if key not in received.keys():
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


def forge_public_transfer(
    max_base_fee: int,
    max_priority_fee: int,
    address_to: str,
    amount: int,
    program_checksum: str = "",
) -> dict:

    data = {
        "type": "intent",
        "max_base_fee": max_base_fee,
        "max_priority_fee": max_priority_fee,
        "fee_program_id": "credits.aleo",
        "fee_function_name": "fee_public",
    }
    data["request"] = {
        "network_id": "mainnet",
        "program_id": "credits.aleo",
        "function_name": "transfer_public",
    }
    data["request"]["inputs"] = [
        {"type": "address.public", "value": address_to},
        {"type": "u64.public", "value": amount},
    ]
    data["request"]["nested_call_count"] = 0
    data["request"]["program_checksum"] = program_checksum

    return data


def forge_private_transfer(
    max_base_fee: int,
    max_priority_fee: int,
    record: list[str],
    address_to: str,
    amount: int,
    program_checksum: str = "",
) -> dict:

    data = {
        "type": "intent",
        "max_base_fee": max_base_fee,
        "max_priority_fee": max_priority_fee,
        "fee_program_id": "credits.aleo",
        "fee_function_name": "fee_private",
    }
    data["request"] = {
        "network_id": "mainnet",
        "program_id": "credits.aleo",
        "function_name": "transfer_private",
    }
    data["request"]["inputs"] = [
        {"type": "credits.record", "value": record},
        {"type": "address.private", "value": address_to},
        {"type": "u64.private", "value": amount},
    ]
    data["request"]["nested_call_count"] = 0
    data["request"]["program_checksum"] = program_checksum

    return data


def forge_batch_private_transfer(
    max_base_fee: int,
    max_priority_fee: int,
    external_record: list[str],
    address_to: str,
    amount: int,
    program_checksum: str = "",
) -> dict:
    data = {
        "type": "intent",
        "max_base_fee": max_base_fee,
        "max_priority_fee": max_priority_fee,
        "fee_program_id": "credits.aleo",
        "fee_function_name": "fee_private",
    }
    data["request"] = {
        "network_id": "mainnet",
        "program_id": "ldg_p_28.aleo",
        "function_name": "transfer_private_2",
    }
    data["request"]["inputs"] = [
        {"type": "external_record", "value": external_record[0]},
        {"type": "external_record", "value": external_record[1]},
        {"type": "address.private", "value": address_to},
        {"type": "u64.private", "value": amount},
    ]
    data["request"]["nested_call_count"] = 2
    data["request"]["program_checksum"] = program_checksum

    return data


def forge_nested_call_join(r0: list[str], r1: list[str], program_checksum: str = ""):
    data = {"type": "nested_call"}
    data["request"] = {
        "network_id": "mainnet",
        "program_id": "credits.aleo",
        "function_name": "join",
    }
    data["request"]["inputs"] = [
        {"type": "credits.record", "value": r0},
        {"type": "credits.record", "value": r1},
    ]
    data["request"]["nested_call_count"] = 0
    data["request"]["program_checksum"] = program_checksum

    return data


def forge_nested_call_private_transfer(
    record: list[str], address_to: str, amount: int, program_checksum: str = ""
):
    data = {"type": "nested_call"}
    data["request"] = {
        "network_id": "mainnet",
        "program_id": "credits.aleo",
        "function_name": "transfer_private",
    }
    data["request"]["inputs"] = [
        {"type": "credits.record", "value": record},
        {"type": "address.private", "value": address_to},
        {"type": "u64.private", "value": amount},
    ]
    data["request"]["nested_call_count"] = 0
    data["request"]["program_checksum"] = program_checksum

    return data


def forge_public_fee(
    base_fee: int, priority_fee: int, execution_id: str, program_checksum: str = ""
) -> dict:
    data = {"type": "fee"}
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


def forge_private_fee(
    base_fee: int,
    priority_fee: int,
    record: list[str],
    execution_id: str,
    program_checksum: str = "",
) -> dict:
    data = {"type": "fee"}
    data["request"] = {
        "network_id": "mainnet",
        "program_id": "credits.aleo",
        "function_name": "fee_private",
    }
    data["request"]["inputs"] = [
        {"type": "credits.record", "value": record},
        {"type": "u64.public", "value": base_fee},
        {"type": "u64.public", "value": priority_fee},
        {"type": "field.public", "value": execution_id},
    ]
    data["request"]["nested_call_count"] = 0
    data["request"]["program_checksum"] = program_checksum

    return data


def test_sign_transaction_errors(
    backend: BackendInterface, scenario_navigator: NavigateWithScenario
) -> None:
    client = CommandSender(backend)

    with pytest.raises(ExceptionRAPDU) as e:
        backend.exchange_raw(bytes.fromhex("E0060000"))
    assert e.value.status == StatusWords.SWO_WRONG_DATA_LENGTH

    with pytest.raises(ExceptionRAPDU) as e:
        backend.exchange_raw(
            bytes.fromhex("E006000013048000002c800000008000000080000000FFFF")
        )
    assert e.value.status == StatusWords.SWO_INSUFFICIENT_MEMORY

    with pytest.raises(ExceptionRAPDU) as e:
        backend.exchange_raw(
            bytes.fromhex("E006000113048000002c8000000080000000800000000000")
        )
    assert e.value.status == StatusWords.SWO_CONDITIONS_NOT_SATISFIED

    backend.exchange_raw(
        bytes.fromhex("E006000014048000002c800000008000000080000000000211")
    )
    with pytest.raises(ExceptionRAPDU) as e:
        backend.exchange_raw(bytes.fromhex("E0060001022233"))
    assert e.value.status == StatusWords.SWO_WRONG_DATA_LENGTH

    tx_datas = forge_public_fee(
        500,
        100,
        "7266375125414209082394925781071362722506946030314916664133746682226945366259field",
    )

    with pytest.raises(ExceptionRAPDU) as e:
        with client.sign_transaction(tx_datas=tx_datas):
            scenario_navigator.review_reject()

    assert e.value.status == StatusWords.SWO_CONDITIONS_NOT_SATISFIED

    with pytest.raises(ExceptionRAPDU) as e:
        backend.exchange_raw(
            bytes.fromhex("E0060000130b8000002c8000000080000000800000000000")
        )
    assert e.value.status == StatusWords.SWO_WRONG_DATA_LENGTH


def test_sign_transaction_refused(
    backend: BackendInterface, scenario_navigator: NavigateWithScenario
) -> None:
    client = CommandSender(backend)
    tx_datas = forge_public_transfer(
        500,
        100,
        "aleo1sfydt6z6cnqjx3hcgk9ajw03ecj6uqlfcm9u3p3gdhckzcc2w5xqv3v3pe",
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
        validation_instructions=None,
        text="Transaction rejected",
        timeout=3,
        screen_change_before_first_instruction=False,
        screen_change_after_last_instruction=True,
    )


def test_sign_transaction_fee_timeout(
    backend: BackendInterface, scenario_navigator: NavigateWithScenario
) -> None:
    client = CommandSender(backend)
    tx_datas = forge_public_transfer(
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
    if scenario_navigator.device.is_nano:
        instruction = NavInsID.LEFT_CLICK
    else:
        instruction = NavInsID.USE_CASE_REVIEW_TAP
    scenario_navigator.navigator.navigate_until_text(
        navigate_instruction=instruction,
        validation_instructions=None,
        text="Transaction rejected",
        timeout=20,
        screen_change_before_first_instruction=False,
        screen_change_after_last_instruction=True,
    )


def test_sign_transaction_wrong_fee(
    backend: BackendInterface, scenario_navigator: NavigateWithScenario
) -> None:
    client = CommandSender(backend)
    tx_datas = forge_public_transfer(
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
        5000,
        1000,
        "7266375125414209082394925781071362722506946030314916664133746682226945366259field",
    )

    with pytest.raises(ExceptionRAPDU) as e:
        with client.sign_transaction(tx_datas=tx_datas):
            pass
    assert e.value.status == StatusWords.SWO_INCORRECT_DATA


def test_sign_transaction_transfer_public(
    backend: BackendInterface, scenario_navigator: NavigateWithScenario
) -> None:
    client = CommandSender(backend)
    tx_datas = forge_public_transfer(
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
            validation_instructions=None,
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


def test_sign_transaction_transfer_private(
    backend: BackendInterface, scenario_navigator: NavigateWithScenario
) -> None:
    client = CommandSender(backend)
    record = [
        "3614797564276936744957924747041031196891698846785520060979425601577054464500field",
        "2426895214035216932245297778850989035038538961658726507442215877484415082794field",
        "0220642863446832956019507279394572297489712696240584424406852292692897199577field",
    ]
    tx_datas = forge_private_transfer(
        500,
        100,
        record,
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
        "gammas_count": 1,
        "gammas": ["b0bfc7d7c4fd471833c6d4dd6bd061b3a728a31594b75ef8e424a2de7f883003"],
    }
    assert check_response(unpacked, expected)

    tx_datas = forge_private_fee(
        500,
        100,
        record,
        "7266375125414209082394925781071362722506946030314916664133746682226945366259field",
    )
    with client.sign_transaction(tx_datas=tx_datas):
        if scenario_navigator.device.is_nano:
            instruction = NavInsID.BOTH_CLICK
        else:
            instruction = NavInsID.USE_CASE_REVIEW_TAP
        scenario_navigator.navigator.navigate_until_text(
            navigate_instruction=instruction,
            validation_instructions=None,
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
        "gammas_count": 1,
        "gammas": ["b0bfc7d7c4fd471833c6d4dd6bd061b3a728a31594b75ef8e424a2de7f883003"],
    }
    assert check_response(unpacked, expected)


def test_sign_transaction_transfer_private_zero_fees(
    backend: BackendInterface, scenario_navigator: NavigateWithScenario
) -> None:
    client = CommandSender(backend)
    record = [
        "3614797564276936744957924747041031196891698846785520060979425601577054464500field",
        "2426895214035216932245297778850989035038538961658726507442215877484415082794field",
        "0220642863446832956019507279394572297489712696240584424406852292692897199577field",
    ]
    tx_datas = forge_private_transfer(
        0,
        0,
        record,
        "aleo1sfydt6z6cnqjx3hcgk9ajw03ecj6uqlfcm9u3p3gdhckzcc2w5xqv3v3pe",
        1000,
    )
    tx_datas["path"] = "m/44'/683'/0'/0'"
    with client.sign_transaction(tx_datas=tx_datas):
        scenario_navigator.review_approve()

    response = client.get_async_response().data
    unpacked = unpack_sign_transaction_response(response)
    expected = {
        "structure_type": 42,
        "version": 1,
        "signature": {
            "pk_sig": "1d4c4b28dd6ce05ab520f00b71c081d480684c746a7d8f3b0a3a68d410ce840e",
            "pr_sig": "3a8a3cfee21ce108285cca4cc50abb5ac9044acf26959ddb7722cbb968bdc310",
        },
        "gammas_count": 1,
        "gammas": ["b0bfc7d7c4fd471833c6d4dd6bd061b3a728a31594b75ef8e424a2de7f883003"],
    }
    assert check_response(unpacked, expected)


def test_sign_transaction_transfer_batch_private(
    backend: BackendInterface, scenario_navigator: NavigateWithScenario
) -> None:
    client = CommandSender(backend)

    tx_datas = {"type": "get_tvk", "path": "m/44'/683'/0'/0'", "index": 0}
    response = client.get_tvk(tx_datas=tx_datas).data
    unpacked = unpack_get_tvk_response(response)
    tvk_0 = unpacked["tvk"]
    tx_datas = {"type": "get_tvk", "path": "m/44'/683'/0'/0'", "index": 1}
    response = client.get_tvk(tx_datas=tx_datas).data
    unpacked = unpack_get_tvk_response(response)
    tvk_1 = unpacked["tvk"]
    tx_datas = {"type": "get_tvk", "path": "m/44'/683'/0'/0'", "index": 2}
    response = client.get_tvk(tx_datas=tx_datas).data
    unpacked = unpack_get_tvk_response(response)
    tvk_2 = unpacked["tvk"]

    external_record = [
        "d5f4b9312020d52c6752cb927e00771b300e8742e7cbe2cffe79a2a9f1641e03f1020000b6a58dc9bd8dc99591a5d1cd0503100019000000000000806381fe03232753113751635f57729543c73e2ab3641901f57fec18b1e560b28b80000000",
        "d5f4b9312020d52c6752cb927e00771b300e8742e7cbe2cffe79a2a9f1641e03f1020000b6a58dc9bd8dc99591a5d1cd0503100019000000000000c0eed4b40239bab0f49f52a8a88613dc6ea6aec32d2d23df9a005edf7d940ecfb980000000",
    ]
    tx_datas = forge_batch_private_transfer(
        500,
        100,
        external_record,
        "aleo1sfydt6z6cnqjx3hcgk9ajw03ecj6uqlfcm9u3p3gdhckzcc2w5xqv3v3pe",
        1000,
        "e9fb1007c069e11dda4a4c3f6e1d5a8c6fcbfb0a1f556ff629719f095902e107",
    )
    tx_datas["path"] = "m/44'/683'/0'/0'"
    with client.sign_transaction(tx_datas=tx_datas):
        scenario_navigator.review_approve_with_spinner("Prepare Tx 5/15")

    response = client.get_async_response().data
    unpacked = unpack_sign_transaction_response(response)
    expected = {
        "structure_type": 42,
        "version": 1,
        "signature": {
            "pk_sig": "1d4c4b28dd6ce05ab520f00b71c081d480684c746a7d8f3b0a3a68d410ce840e",
            "pr_sig": "3a8a3cfee21ce108285cca4cc50abb5ac9044acf26959ddb7722cbb968bdc310",
        },
        "tvk": tvk_0,
        "gammas_count": 0,
    }
    assert check_response(unpacked, expected)

    record = [
        "3614797564276936744957924747041031196891698846785520060979425601577054464500field",
        "2426895214035216932245297778850989035038538961658726507442215877484415082794field",
        "0220642863446832956019507279394572297489712696240584424406852292692897199577field",
    ]
    tx_datas = forge_nested_call_join(
        record,
        record,
        "e9fb1007c069e11dda4a4c3f6e1d5a8c6fcbfb0a1f556ff629719f095902e107",
    )
    with client.sign_transaction(tx_datas=tx_datas):
        if scenario_navigator.device.is_nano:
            instruction = NavInsID.BOTH_CLICK
        else:
            instruction = NavInsID.USE_CASE_REVIEW_TAP
        scenario_navigator.navigator.navigate_until_text(
            navigate_instruction=instruction,
            validation_instructions=None,
            text="Prepare Tx 10/15",
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
        "tvk": tvk_1,
        "gammas_count": 2,
        "gammas": [
            "b0bfc7d7c4fd471833c6d4dd6bd061b3a728a31594b75ef8e424a2de7f883003",
            "b0bfc7d7c4fd471833c6d4dd6bd061b3a728a31594b75ef8e424a2de7f883003",
        ],
    }
    assert check_response(unpacked, expected)

    tx_datas = forge_nested_call_private_transfer(
        record,
        "aleo1sfydt6z6cnqjx3hcgk9ajw03ecj6uqlfcm9u3p3gdhckzcc2w5xqv3v3pe",
        1000,
        "e9fb1007c069e11dda4a4c3f6e1d5a8c6fcbfb0a1f556ff629719f095902e107",
    )
    with client.sign_transaction(tx_datas=tx_datas):
        if scenario_navigator.device.is_nano:
            instruction = NavInsID.BOTH_CLICK
        else:
            instruction = NavInsID.USE_CASE_REVIEW_TAP
        scenario_navigator.navigator.navigate_until_text(
            navigate_instruction=instruction,
            validation_instructions=None,
            text="Calculating fees",
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
        "tvk": tvk_2,
        "gammas_count": 1,
        "gammas": ["b0bfc7d7c4fd471833c6d4dd6bd061b3a728a31594b75ef8e424a2de7f883003"],
    }
    assert check_response(unpacked, expected)

    tx_datas = forge_private_fee(
        500,
        100,
        record,
        "7266375125414209082394925781071362722506946030314916664133746682226945366259field",
    )
    with client.sign_transaction(tx_datas=tx_datas):
        if scenario_navigator.device.is_nano:
            instruction = NavInsID.BOTH_CLICK
        else:
            instruction = NavInsID.USE_CASE_REVIEW_TAP
        scenario_navigator.navigator.navigate_until_text(
            navigate_instruction=instruction,
            validation_instructions=None,
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
        "gammas_count": 1,
        "gammas": ["b0bfc7d7c4fd471833c6d4dd6bd061b3a728a31594b75ef8e424a2de7f883003"],
    }
    assert check_response(unpacked, expected)


def test_sign_transaction_transfer_batch_private_zero_fees(
    backend: BackendInterface, scenario_navigator: NavigateWithScenario
) -> None:
    client = CommandSender(backend)

    tx_datas = {"type": "get_tvk", "path": "m/44'/683'/0'/0'", "index": 0}
    response = client.get_tvk(tx_datas=tx_datas).data
    unpacked = unpack_get_tvk_response(response)
    tvk_0 = unpacked["tvk"]
    tx_datas = {"type": "get_tvk", "path": "m/44'/683'/0'/0'", "index": 1}
    response = client.get_tvk(tx_datas=tx_datas).data
    unpacked = unpack_get_tvk_response(response)
    tvk_1 = unpacked["tvk"]
    tx_datas = {"type": "get_tvk", "path": "m/44'/683'/0'/0'", "index": 2}
    response = client.get_tvk(tx_datas=tx_datas).data
    unpacked = unpack_get_tvk_response(response)
    tvk_2 = unpacked["tvk"]

    external_record = [
        "d5f4b9312020d52c6752cb927e00771b300e8742e7cbe2cffe79a2a9f1641e03f1020000b6a58dc9bd8dc99591a5d1cd0503100019000000000000806381fe03232753113751635f57729543c73e2ab3641901f57fec18b1e560b28b80000000",
        "d5f4b9312020d52c6752cb927e00771b300e8742e7cbe2cffe79a2a9f1641e03f1020000b6a58dc9bd8dc99591a5d1cd0503100019000000000000c0eed4b40239bab0f49f52a8a88613dc6ea6aec32d2d23df9a005edf7d940ecfb980000000",
    ]
    tx_datas = forge_batch_private_transfer(
        0,
        0,
        external_record,
        "aleo1sfydt6z6cnqjx3hcgk9ajw03ecj6uqlfcm9u3p3gdhckzcc2w5xqv3v3pe",
        1000,
        "e9fb1007c069e11dda4a4c3f6e1d5a8c6fcbfb0a1f556ff629719f095902e107",
    )
    tx_datas["path"] = "m/44'/683'/0'/0'"
    with client.sign_transaction(tx_datas=tx_datas):
        scenario_navigator.review_approve_with_spinner("Prepare Tx 5/15")

    response = client.get_async_response().data
    unpacked = unpack_sign_transaction_response(response)
    expected = {
        "structure_type": 42,
        "version": 1,
        "signature": {
            "pk_sig": "1d4c4b28dd6ce05ab520f00b71c081d480684c746a7d8f3b0a3a68d410ce840e",
            "pr_sig": "3a8a3cfee21ce108285cca4cc50abb5ac9044acf26959ddb7722cbb968bdc310",
        },
        "tvk": tvk_0,
        "gammas_count": 0,
    }
    assert check_response(unpacked, expected)

    record = [
        "3614797564276936744957924747041031196891698846785520060979425601577054464500field",
        "2426895214035216932245297778850989035038538961658726507442215877484415082794field",
        "0220642863446832956019507279394572297489712696240584424406852292692897199577field",
    ]
    tx_datas = forge_nested_call_join(
        record,
        record,
        "e9fb1007c069e11dda4a4c3f6e1d5a8c6fcbfb0a1f556ff629719f095902e107",
    )
    with client.sign_transaction(tx_datas=tx_datas):
        if scenario_navigator.device.is_nano:
            instruction = NavInsID.BOTH_CLICK
        else:
            instruction = NavInsID.USE_CASE_REVIEW_TAP
        scenario_navigator.navigator.navigate_until_text(
            navigate_instruction=instruction,
            validation_instructions=None,
            text="Prepare Tx 10/15",
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
        "tvk": tvk_1,
        "gammas_count": 2,
        "gammas": [
            "b0bfc7d7c4fd471833c6d4dd6bd061b3a728a31594b75ef8e424a2de7f883003",
            "b0bfc7d7c4fd471833c6d4dd6bd061b3a728a31594b75ef8e424a2de7f883003",
        ],
    }
    assert check_response(unpacked, expected)

    tx_datas = forge_nested_call_private_transfer(
        record,
        "aleo1sfydt6z6cnqjx3hcgk9ajw03ecj6uqlfcm9u3p3gdhckzcc2w5xqv3v3pe",
        1000,
        "e9fb1007c069e11dda4a4c3f6e1d5a8c6fcbfb0a1f556ff629719f095902e107",
    )
    with client.sign_transaction(tx_datas=tx_datas):
        if scenario_navigator.device.is_nano:
            instruction = NavInsID.BOTH_CLICK
        else:
            instruction = NavInsID.USE_CASE_REVIEW_TAP
        scenario_navigator.navigator.navigate_until_text(
            navigate_instruction=instruction,
            validation_instructions=None,
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
        "tvk": tvk_2,
        "gammas_count": 1,
        "gammas": ["b0bfc7d7c4fd471833c6d4dd6bd061b3a728a31594b75ef8e424a2de7f883003"],
    }
    assert check_response(unpacked, expected)


def test_sign_transaction_transfer_batch_private_timeout(
    backend: BackendInterface, scenario_navigator: NavigateWithScenario
) -> None:
    client = CommandSender(backend)

    tx_datas = {"type": "get_tvk", "path": "m/44'/683'/0'/0'", "index": 0}
    response = client.get_tvk(tx_datas=tx_datas).data
    unpacked = unpack_get_tvk_response(response)
    tvk_0 = unpacked["tvk"]
    tx_datas = {"type": "get_tvk", "path": "m/44'/683'/0'/0'", "index": 1}
    response = client.get_tvk(tx_datas=tx_datas).data
    unpacked = unpack_get_tvk_response(response)
    tvk_1 = unpacked["tvk"]

    external_record = [
        "d5f4b9312020d52c6752cb927e00771b300e8742e7cbe2cffe79a2a9f1641e03f1020000b6a58dc9bd8dc99591a5d1cd0503100019000000000000806381fe03232753113751635f57729543c73e2ab3641901f57fec18b1e560b28b80000000",
        "d5f4b9312020d52c6752cb927e00771b300e8742e7cbe2cffe79a2a9f1641e03f1020000b6a58dc9bd8dc99591a5d1cd0503100019000000000000c0eed4b40239bab0f49f52a8a88613dc6ea6aec32d2d23df9a005edf7d940ecfb980000000",
    ]
    tx_datas = forge_batch_private_transfer(
        500,
        100,
        external_record,
        "aleo1sfydt6z6cnqjx3hcgk9ajw03ecj6uqlfcm9u3p3gdhckzcc2w5xqv3v3pe",
        1000,
        "e9fb1007c069e11dda4a4c3f6e1d5a8c6fcbfb0a1f556ff629719f095902e107",
    )
    tx_datas["path"] = "m/44'/683'/0'/0'"
    with client.sign_transaction(tx_datas=tx_datas):
        scenario_navigator.review_approve_with_spinner("Prepare Tx 5/15")

    response = client.get_async_response().data
    unpacked = unpack_sign_transaction_response(response)
    expected = {
        "structure_type": 42,
        "version": 1,
        "signature": {
            "pk_sig": "1d4c4b28dd6ce05ab520f00b71c081d480684c746a7d8f3b0a3a68d410ce840e",
            "pr_sig": "3a8a3cfee21ce108285cca4cc50abb5ac9044acf26959ddb7722cbb968bdc310",
        },
        "tvk": tvk_0,
        "gammas_count": 0,
    }
    assert check_response(unpacked, expected)

    record = [
        "3614797564276936744957924747041031196891698846785520060979425601577054464500field",
        "2426895214035216932245297778850989035038538961658726507442215877484415082794field",
        "0220642863446832956019507279394572297489712696240584424406852292692897199577field",
    ]
    tx_datas = forge_nested_call_join(
        record,
        record,
        "e9fb1007c069e11dda4a4c3f6e1d5a8c6fcbfb0a1f556ff629719f095902e107",
    )
    with client.sign_transaction(tx_datas=tx_datas):
        if scenario_navigator.device.is_nano:
            instruction = NavInsID.BOTH_CLICK
        else:
            instruction = NavInsID.USE_CASE_REVIEW_TAP
        scenario_navigator.navigator.navigate_until_text(
            navigate_instruction=instruction,
            validation_instructions=None,
            text="Transaction rejected",
            timeout=20,
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
        "tvk": tvk_1,
        "gammas_count": 2,
        "gammas": [
            "b0bfc7d7c4fd471833c6d4dd6bd061b3a728a31594b75ef8e424a2de7f883003",
            "b0bfc7d7c4fd471833c6d4dd6bd061b3a728a31594b75ef8e424a2de7f883003",
        ],
    }
    assert check_response(unpacked, expected)


def test_sign_transaction_transfer_batch_private_wrong_nc(
    backend: BackendInterface, scenario_navigator: NavigateWithScenario
) -> None:
    client = CommandSender(backend)

    tx_datas = {"type": "get_tvk", "path": "m/44'/683'/0'/0'", "index": 0}
    response = client.get_tvk(tx_datas=tx_datas).data
    unpacked = unpack_get_tvk_response(response)
    tvk_0 = unpacked["tvk"]
    tx_datas = {"type": "get_tvk", "path": "m/44'/683'/0'/0'", "index": 1}
    response = client.get_tvk(tx_datas=tx_datas).data
    unpacked = unpack_get_tvk_response(response)
    tvk_1 = unpacked["tvk"]
    tx_datas = {"type": "get_tvk", "path": "m/44'/683'/0'/0'", "index": 2}
    response = client.get_tvk(tx_datas=tx_datas).data
    unpacked = unpack_get_tvk_response(response)
    tvk_2 = unpacked["tvk"]

    external_record = [
        "d5f4b9312020d52c6752cb927e00771b300e8742e7cbe2cffe79a2a9f1641e03f1020000b6a58dc9bd8dc99591a5d1cd0503100019000000000000806381fe03232753113751635f57729543c73e2ab3641901f57fec18b1e560b28b80000000",
        "d5f4b9312020d52c6752cb927e00771b300e8742e7cbe2cffe79a2a9f1641e03f1020000b6a58dc9bd8dc99591a5d1cd0503100019000000000000c0eed4b40239bab0f49f52a8a88613dc6ea6aec32d2d23df9a005edf7d940ecfb980000000",
    ]
    tx_datas = forge_batch_private_transfer(
        500,
        100,
        external_record,
        "aleo1sfydt6z6cnqjx3hcgk9ajw03ecj6uqlfcm9u3p3gdhckzcc2w5xqv3v3pe",
        1000,
        "e9fb1007c069e11dda4a4c3f6e1d5a8c6fcbfb0a1f556ff629719f095902e107",
    )
    tx_datas["path"] = "m/44'/683'/0'/0'"
    with client.sign_transaction(tx_datas=tx_datas):
        scenario_navigator.review_approve_with_spinner("Prepare Tx 5/15")

    response = client.get_async_response().data
    unpacked = unpack_sign_transaction_response(response)
    expected = {
        "structure_type": 42,
        "version": 1,
        "signature": {
            "pk_sig": "1d4c4b28dd6ce05ab520f00b71c081d480684c746a7d8f3b0a3a68d410ce840e",
            "pr_sig": "3a8a3cfee21ce108285cca4cc50abb5ac9044acf26959ddb7722cbb968bdc310",
        },
        "tvk": tvk_0,
        "gammas_count": 0,
    }
    assert check_response(unpacked, expected)

    record = [
        "3614797564276936744957924747041031196891698846785520060979425601577054464500field",
        "2426895214035216932245297778850989035038538961658726507442215877484415082794field",
        "0220642863446832956019507279394572297489712696240584424406852292692897199577field",
    ]
    tx_datas = forge_nested_call_join(
        record,
        record,
        "e9fb1007c069e11dda4a4c3f6e1d5a8c6fcbfb0a1f556ff629719f095902e107",
    )
    with client.sign_transaction(tx_datas=tx_datas):
        if scenario_navigator.device.is_nano:
            instruction = NavInsID.BOTH_CLICK
        else:
            instruction = NavInsID.USE_CASE_REVIEW_TAP
        scenario_navigator.navigator.navigate_until_text(
            navigate_instruction=instruction,
            validation_instructions=None,
            text="Prepare Tx 10/15",
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
        "tvk": tvk_1,
        "gammas_count": 2,
        "gammas": [
            "b0bfc7d7c4fd471833c6d4dd6bd061b3a728a31594b75ef8e424a2de7f883003",
            "b0bfc7d7c4fd471833c6d4dd6bd061b3a728a31594b75ef8e424a2de7f883003",
        ],
    }
    assert check_response(unpacked, expected)

    record = [
        "3614797564276936744957924747041031196891698846785520060979425601577054464500field",
        "2426895214035216932245297778850989035038538961658726507442215877484415082794field",
        "0220642863446832956019507279394572297489712696240584424406852292692897199577field",
    ]
    tx_datas = forge_nested_call_join(
        record,
        record,
        "e9fb1007c069e11dda4a4c3f6e1d5a8c6fcbfb0a1f556ff629719f095902e107",
    )
    with client.sign_transaction(tx_datas=tx_datas):
        if scenario_navigator.device.is_nano:
            instruction = NavInsID.BOTH_CLICK
        else:
            instruction = NavInsID.USE_CASE_REVIEW_TAP
        scenario_navigator.navigator.navigate_until_text(
            navigate_instruction=instruction,
            validation_instructions=None,
            text="Calculating fees",
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
        "tvk": tvk_2,
        "gammas_count": 2,
        "gammas": [
            "b0bfc7d7c4fd471833c6d4dd6bd061b3a728a31594b75ef8e424a2de7f883003",
            "b0bfc7d7c4fd471833c6d4dd6bd061b3a728a31594b75ef8e424a2de7f883003",
        ],
    }
    assert check_response(unpacked, expected)

    tx_datas = forge_nested_call_private_transfer(
        record,
        "aleo1sfydt6z6cnqjx3hcgk9ajw03ecj6uqlfcm9u3p3gdhckzcc2w5xqv3v3pe",
        1000,
        "e9fb1007c069e11dda4a4c3f6e1d5a8c6fcbfb0a1f556ff629719f095902e107",
    )

    with pytest.raises(ExceptionRAPDU) as e:
        with client.sign_transaction(tx_datas=tx_datas):
            pass
    assert e.value.status == StatusWords.SWO_CONDITIONS_NOT_SATISFIED


def test_sign_transaction_transfer_batch_private_wrong_tvk(
    backend: BackendInterface, scenario_navigator: NavigateWithScenario
) -> None:
    client = CommandSender(backend)

    tx_datas = {"type": "get_tvk", "path": "m/44'/683'/0'/0'", "index": 0}
    response = client.get_tvk(tx_datas=tx_datas).data
    unpacked = unpack_get_tvk_response(response)
    tvk_0 = unpacked["tvk"]
    tx_datas = {"type": "get_tvk", "path": "m/44'/683'/0'/0'", "index": 1}
    response = client.get_tvk(tx_datas=tx_datas).data
    unpacked = unpack_get_tvk_response(response)
    tvk_1 = unpacked["tvk"]

    external_record = [
        "d5f4b9312020d52c6752cb927e00771b300e8742e7cbe2cffe79a2a9f1641e03f1020000b6a58dc9bd8dc99591a5d1cd0503100019000000000000806381fe03232753113751635f57729543c73e2ab3641901f57fec18b1e560b28b80000000",
        "d5f4b9312020d52c6752cb927e00771b300e8742e7cbe2cffe79a2a9f1641e03f1020000b6a58dc9bd8dc99591a5d1cd0503100019000000000000c0eed4b40239bab0f49f52a8a88613dc6ea6aec32d2d23df9a005edf7d940ecfb980000000",
    ]
    tx_datas = forge_batch_private_transfer(
        500,
        100,
        external_record,
        "aleo1sfydt6z6cnqjx3hcgk9ajw03ecj6uqlfcm9u3p3gdhckzcc2w5xqv3v3pe",
        1000,
        "e9fb1007c069e11dda4a4c3f6e1d5a8c6fcbfb0a1f556ff629719f095902e107",
    )
    tx_datas["path"] = "m/44'/683'/0'/0'"
    with client.sign_transaction(tx_datas=tx_datas):
        scenario_navigator.review_approve_with_spinner("Prepare Tx 5/15")

    response = client.get_async_response().data
    unpacked = unpack_sign_transaction_response(response)
    expected = {
        "structure_type": 42,
        "version": 1,
        "signature": {
            "pk_sig": "1d4c4b28dd6ce05ab520f00b71c081d480684c746a7d8f3b0a3a68d410ce840e",
            "pr_sig": "3a8a3cfee21ce108285cca4cc50abb5ac9044acf26959ddb7722cbb968bdc310",
        },
        "tvk": tvk_0,
        "gammas_count": 0,
    }
    assert check_response(unpacked, expected)

    record = [
        "3614797564276936744957924747041031196891698846785520060979425601577054464500field",
        "2426895214035216932245297778850989035038538961658726507442215877484415082794field",
        "0220642863446832956019507279394572297489712696240584424406852292692897199577field",
    ]
    tx_datas = forge_nested_call_join(
        record,
        record,
        "e9fb1007c069e11dda4a4c3f6e1d5a8c6fcbfb0a1f556ff629719f095902e107",
    )
    with client.sign_transaction(tx_datas=tx_datas):
        if scenario_navigator.device.is_nano:
            instruction = NavInsID.BOTH_CLICK
        else:
            instruction = NavInsID.USE_CASE_REVIEW_TAP
        scenario_navigator.navigator.navigate_until_text(
            navigate_instruction=instruction,
            validation_instructions=None,
            text="Prepare Tx 10/15",
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
        "tvk": tvk_1,
        "gammas_count": 2,
        "gammas": [
            "b0bfc7d7c4fd471833c6d4dd6bd061b3a728a31594b75ef8e424a2de7f883003",
            "b0bfc7d7c4fd471833c6d4dd6bd061b3a728a31594b75ef8e424a2de7f883003",
        ],
    }
    assert check_response(unpacked, expected)

    tx_datas = forge_nested_call_private_transfer(
        record,
        "aleo1sfydt6z6cnqjx3hcgk9ajw03ecj6uqlfcm9u3p3gdhckzcc2w5xqv3v3pe",
        1000,
        "e9fb1007c069e11dda4a4c3f6e1d5a8c6fcbfb0a1f556ff629719f095902e107",
    )

    with pytest.raises(ExceptionRAPDU) as e:
        with client.sign_transaction(tx_datas=tx_datas):
            pass
    assert e.value.status == StatusWords.SWO_INCORRECT_DATA


# ============================================================================
# GET_TVK negative tests
# ============================================================================


def test_get_tvk_invalid_p1(backend: BackendInterface) -> None:
    """GET_TVK with an invalid P1 value (neither 0x00 nor 0x01) must return SWO_INCORRECT_P1_P2."""
    # Build a valid BIP32 path payload: path_len=4, m/44'/683'/0'/0'
    path_data = bytes.fromhex("048000002c800002ab8000000080000000")
    with pytest.raises(ExceptionRAPDU) as e:
        backend.exchange(
            cla=0xE0,
            ins=InsType.CMD_GET_TVK,
            p1=0x02,  # invalid P1
            p2=0x00,
            data=path_data,
        )
    assert e.value.status == StatusWords.SWO_INCORRECT_P1_P2


def test_get_tvk_empty_data(backend: BackendInterface) -> None:
    """GET_TVK with empty data must return SWO_WRONG_DATA_LENGTH."""
    with pytest.raises(ExceptionRAPDU) as e:
        backend.exchange(cla=0xE0, ins=InsType.CMD_GET_TVK, p1=0x00, p2=0x00, data=b"")
    assert e.value.status == StatusWords.SWO_WRONG_DATA_LENGTH


def test_get_tvk_derived_without_seed(backend: BackendInterface) -> None:
    """GET_TVK in derived mode (P1=0x01) without prior seed call must return SWO_INCORRECT_DATA
    because no r_list has been initialized yet."""
    # Build path + index=1 payload
    path_data = bytes.fromhex("048000002c800002ab8000000080000000") + bytes([1])
    with pytest.raises(ExceptionRAPDU) as e:
        backend.exchange(
            cla=0xE0,
            ins=InsType.CMD_GET_TVK,
            p1=0x01,  # derived mode
            p2=0x00,
            data=path_data,
        )
    assert e.value.status == StatusWords.SWO_INCORRECT_DATA


def test_get_tvk_derived_index_skip(backend: BackendInterface) -> None:
    """GET_TVK derived with a skipped index must fail.
    First seed index 0, then request index 2 (skipping 1)."""
    client = CommandSender(backend)
    # Seed with index 0 (success = no exception raised)
    tx_datas = {"type": "get_tvk", "path": "m/44'/683'/0'/0'", "index": 0}
    client.get_tvk(tx_datas=tx_datas)

    # Skip index 1, request index 2 directly
    path_data = bytes.fromhex("048000002c800002ab8000000080000000") + bytes([2])
    with pytest.raises(ExceptionRAPDU) as e:
        backend.exchange(
            cla=0xE0, ins=InsType.CMD_GET_TVK, p1=0x01, p2=0x00, data=path_data
        )
    assert e.value.status == StatusWords.SWO_INCORRECT_DATA


def test_get_tvk_derived_missing_index_byte(backend: BackendInterface) -> None:
    """GET_TVK in derived mode (P1=0x01) with path but no index byte must return SWO_INCORRECT_DATA."""
    client = CommandSender(backend)
    # First seed so r_list is alive (success = no exception raised)
    tx_datas = {"type": "get_tvk", "path": "m/44'/683'/0'/0'", "index": 0}
    client.get_tvk(tx_datas=tx_datas)

    # Send derived request with path only (no index byte)
    path_data = bytes.fromhex("048000002c800002ab8000000080000000")
    with pytest.raises(ExceptionRAPDU) as e:
        backend.exchange(
            cla=0xE0, ins=InsType.CMD_GET_TVK, p1=0x01, p2=0x00, data=path_data
        )
    assert e.value.status == StatusWords.SWO_INCORRECT_DATA


def test_sign_transaction_get_tvk_timeout(
    backend: BackendInterface, scenario_navigator: NavigateWithScenario
) -> None:
    client = CommandSender(backend)

    tx_datas = {"type": "get_tvk", "path": "m/44'/683'/0'/0'", "index": 0}
    client.get_tvk(tx_datas=tx_datas).data

    if scenario_navigator.device.is_nano:
        instruction = NavInsID.LEFT_CLICK
    else:
        instruction = NavInsID.USE_CASE_REVIEW_TAP
    scenario_navigator.navigator.navigate_until_text(
        navigate_instruction=instruction,
        validation_instructions=None,
        text="Aleo",
        timeout=15,
        screen_change_before_first_instruction=False,
        screen_change_after_last_instruction=True,
    )
