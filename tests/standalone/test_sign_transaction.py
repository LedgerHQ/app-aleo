import pytest
import time

from ledgered.devices import DeviceType, Device
from ragger.error import ExceptionRAPDU, StatusWords
from ragger.backend.interface import BackendInterface
from ragger.navigator.navigation_scenario import NavigateWithScenario
from ragger.navigator import NavInsID

from application_client.command_sender import CommandSender
from application_client.response_unpacker import unpack_sign_transaction_response


def check_response(received: dict, expected: dict) -> bool:
    for key in expected.keys():
        if key not in received.keys():
            return False
        if type(received[key]) == list:
            index = 0
            for item in received[key]:
                if item != expected[key][index]:
                    return False
                index += 1
        elif type(received[key]) == dict:
            return check_response(received[key], expected[key])
        elif received[key] != expected[key]:
            return False
    return True


def forge_public_transfer(max_base_fee: int, max_priority_fee: int, address_to: str, amount: int, program_checksum: str = '') -> dict:

    data = {'type' : 'intent',
            'max_base_fee' : max_base_fee, 'max_priority_fee' : max_priority_fee,
            'fee_program_id' : 'credits.aleo', 'fee_function_name' : 'fee_public'}
    data['request'] = {'network_id' : 'mainnet', 'program_id' : 'credits.aleo', 'function_name' : 'transfer_public'}
    data['request']['inputs'] = [{'type' : 'address.public', 'value' : address_to},
                                 {'type' : 'u64.public',     'value' : amount}]
    data['request']['nested_call_count'] = 0
    data['request']['program_checksum']  = program_checksum

    return data


def forge_private_transfer(max_base_fee: int, max_priority_fee: int, record: list[str], address_to: str, amount: int, program_checksum: str = '') -> dict:

    data = {'type' : 'intent',
            'max_base_fee' : max_base_fee, 'max_priority_fee' : max_priority_fee,
            'fee_program_id' : 'credits.aleo', 'fee_function_name' : 'fee_private'}
    data['request'] = {'network_id' : 'mainnet', 'program_id' : 'credits.aleo', 'function_name' : 'transfer_private'}
    data['request']['inputs'] = [{'type' : 'credits.record',  'value' : record},
                                 {'type' : 'address.private', 'value' : address_to},
                                 {'type' : 'u64.private',     'value' : amount}]
    data['request']['nested_call_count'] = 0
    data['request']['program_checksum']  = program_checksum

    return data


def forge_public_fee(base_fee: int, priority_fee: int, execution_id: str, program_checksum: str = '') -> dict:
    data = {'type' : 'fee'}
    data['request'] = {'network_id' : 'mainnet', 'program_id' : 'credits.aleo', 'function_name' : 'fee_public'}
    data['request']['inputs'] = [{'type' : 'u64.public',    'value' : base_fee},
                                 {'type' : 'u64.public',    'value' : priority_fee},
                                 {'type' : 'field.public',  'value' : execution_id}]
    data['request']['nested_call_count'] = 0
    data['request']['program_checksum']  = program_checksum

    return data


def forge_private_fee(base_fee: int, priority_fee: int, record: list[str], execution_id: str, program_checksum: str = '') -> dict:
    data = {'type' : 'fee'}
    data['request'] = {'network_id' : 'mainnet', 'program_id' : 'credits.aleo', 'function_name' : 'fee_private'}
    data['request']['inputs'] = [{'type' : 'credits.record', 'value' : record},
                                 {'type' : 'u64.public',     'value' : base_fee},
                                 {'type' : 'u64.public',     'value' : priority_fee},
                                 {'type' : 'field.public',   'value' : execution_id}]
    data['request']['nested_call_count'] = 0
    data['request']['program_checksum']  = program_checksum

    return data


def test_sign_transaction_errors(backend: BackendInterface, scenario_navigator: NavigateWithScenario) -> None:
    client = CommandSender(backend)

    with pytest.raises(ExceptionRAPDU) as e:
        backend.exchange_raw(bytes.fromhex("E0060000"))
    assert e.value.status == StatusWords.SWO_WRONG_DATA_LENGTH

    with pytest.raises(ExceptionRAPDU) as e:
        backend.exchange_raw(bytes.fromhex("E006000013048000002c800000008000000080000000FFFF"))
    assert e.value.status == StatusWords.SWO_INSUFFICIENT_MEMORY

    with pytest.raises(ExceptionRAPDU) as e:
        backend.exchange_raw(bytes.fromhex("E006000113048000002c8000000080000000800000000000"))
    assert e.value.status == StatusWords.SWO_CONDITIONS_NOT_SATISFIED
    
    backend.exchange_raw(bytes.fromhex("E006000014048000002c800000008000000080000000000211"))
    with pytest.raises(ExceptionRAPDU) as e:
        backend.exchange_raw(bytes.fromhex("E0060001022233"))
    assert e.value.status == StatusWords.SWO_WRONG_DATA_LENGTH

    tx_datas = forge_public_fee(500, 100, "7266375125414209082394925781071362722506946030314916664133746682226945366259field")
    
    with pytest.raises(ExceptionRAPDU) as e:
        with client.sign_transaction(tx_datas=tx_datas):
            scenario_navigator.review_reject()

    assert e.value.status == StatusWords.SWO_CONDITIONS_NOT_SATISFIED

    with pytest.raises(ExceptionRAPDU) as e:
        backend.exchange_raw(bytes.fromhex("E0060000130b8000002c8000000080000000800000000000"))
    assert e.value.status == StatusWords.SWO_WRONG_DATA_LENGTH


def test_sign_transaction_refused(backend: BackendInterface, scenario_navigator: NavigateWithScenario) -> None:
    client = CommandSender(backend)
    tx_datas = forge_public_transfer(500, 100, "aleo1sfydt6z6cnqjx3hcgk9ajw03ecj6uqlfcm9u3p3gdhckzcc2w5xqv3v3pe", 1000)
    tx_datas['path'] = "m/44'/683'/0'/0'"
    
    with pytest.raises(ExceptionRAPDU) as e:
        with client.sign_transaction(tx_datas=tx_datas):
            scenario_navigator.review_reject()

    assert e.value.status == StatusWords.SWO_PERMISSION_DENIED
    assert len(e.value.data) == 0

    if scenario_navigator.device.is_nano:
        instruction = NavInsID.BOTH_CLICK
    else:
        instruction = NavInsID.USE_CASE_REVIEW_TAP
    scenario_navigator.navigator.navigate_until_text(navigate_instruction=instruction,
                                                     validation_instructions=None,
                                                     text="Transaction rejected",
                                                     timeout=3,
                                                     screen_change_before_first_instruction=False,
                                                     screen_change_after_last_instruction=True)


def test_sign_transaction_fee_timeout(backend: BackendInterface, scenario_navigator: NavigateWithScenario) -> None:
    client = CommandSender(backend)
    tx_datas = forge_public_transfer(500, 100, "aleo1sfydt6z6cnqjx3hcgk9ajw03ecj6uqlfcm9u3p3gdhckzcc2w5xqv3v3pe", 1000)
    tx_datas['path'] = "m/44'/683'/0'/0'/0'"
    with client.sign_transaction(tx_datas=tx_datas):
        scenario_navigator.review_approve_with_spinner("Calculating fees")

    response = client.get_async_response().data
    unpacked = unpack_sign_transaction_response(response)
    expected = {'structure_type': 42,
                'version': 1,
                'signature': {'pk_sig': '52635af7828f4ceadb4fb51ce14255a4fe2f8a29985d0ba4554f79da61f52007',
                              'pr_sig': '0778a6061189b5c56da8b5a6881197e52ad01f45960dcb1cd769a0799e3cda09'},
                'tvk': 'b535d8905fba7d5151378ab894b12e18778656b894f4eab8b35c7511098f9312',
                'tpk': '6c33aa8071756445369c0f6cb3546e477af732bc1703b4aa2124a7c9e1b65207',
                'gammas_count': 0
    }
    assert(check_response(unpacked, expected))
    if scenario_navigator.device.is_nano:
        instruction = NavInsID.BOTH_CLICK
    else:
        instruction = NavInsID.USE_CASE_REVIEW_TAP
    scenario_navigator.navigator.navigate_until_text(navigate_instruction=instruction,
                                                     validation_instructions=None,
                                                     text="Transaction rejected",
                                                     timeout=20,
                                                     screen_change_before_first_instruction=False,
                                                     screen_change_after_last_instruction=True)


def test_sign_transaction_wrong_fee(backend: BackendInterface, scenario_navigator: NavigateWithScenario) -> None:
    client = CommandSender(backend)
    tx_datas = forge_public_transfer(500, 100, "aleo1sfydt6z6cnqjx3hcgk9ajw03ecj6uqlfcm9u3p3gdhckzcc2w5xqv3v3pe", 1000)
    tx_datas['path'] = "m/44'/683'/0'/0'/0'"
    with client.sign_transaction(tx_datas=tx_datas):
        scenario_navigator.review_approve_with_spinner("Calculating fees")

    response = client.get_async_response().data
    unpacked = unpack_sign_transaction_response(response)
    expected = {'structure_type': 42,
                'version': 1,
                'signature': {'pk_sig': '52635af7828f4ceadb4fb51ce14255a4fe2f8a29985d0ba4554f79da61f52007',
                              'pr_sig': '0778a6061189b5c56da8b5a6881197e52ad01f45960dcb1cd769a0799e3cda09'},
                'tvk': 'b535d8905fba7d5151378ab894b12e18778656b894f4eab8b35c7511098f9312',
                'tpk': '6c33aa8071756445369c0f6cb3546e477af732bc1703b4aa2124a7c9e1b65207',
                'gammas_count': 0
    }
    assert(check_response(unpacked, expected))

    tx_datas = forge_public_fee(5000, 1000, "7266375125414209082394925781071362722506946030314916664133746682226945366259field")
 
    with pytest.raises(ExceptionRAPDU) as e:
        with client.sign_transaction(tx_datas=tx_datas):
            pass
    assert e.value.status == StatusWords.SWO_INCORRECT_DATA


def test_sign_transaction_transfer_public(backend: BackendInterface, scenario_navigator: NavigateWithScenario) -> None:
    client = CommandSender(backend)
    tx_datas = forge_public_transfer(500, 100, "aleo1sfydt6z6cnqjx3hcgk9ajw03ecj6uqlfcm9u3p3gdhckzcc2w5xqv3v3pe", 1000)
    tx_datas['path'] = "m/44'/683'/0'/0'/0'"
    with client.sign_transaction(tx_datas=tx_datas):
        scenario_navigator.review_approve_with_spinner("Calculating fees")

    response = client.get_async_response().data
    unpacked = unpack_sign_transaction_response(response)
    expected = {'structure_type': 42,
                'version': 1,
                'signature': {'pk_sig': '52635af7828f4ceadb4fb51ce14255a4fe2f8a29985d0ba4554f79da61f52007',
                              'pr_sig': '0778a6061189b5c56da8b5a6881197e52ad01f45960dcb1cd769a0799e3cda09'},
                'tvk': 'b535d8905fba7d5151378ab894b12e18778656b894f4eab8b35c7511098f9312',
                'tpk': '6c33aa8071756445369c0f6cb3546e477af732bc1703b4aa2124a7c9e1b65207',
                'gammas_count': 0
    }
    assert(check_response(unpacked, expected))

    tx_datas = forge_public_fee(500, 100, "7266375125414209082394925781071362722506946030314916664133746682226945366259field")
    with client.sign_transaction(tx_datas=tx_datas):
        if scenario_navigator.device.is_nano:
            instruction = NavInsID.BOTH_CLICK
        else:
            instruction = NavInsID.USE_CASE_REVIEW_TAP
        scenario_navigator.navigator.navigate_until_text(navigate_instruction=instruction,
                                                         validation_instructions=None,
                                                         text="Transaction signed",
                                                         timeout=3,
                                                         screen_change_before_first_instruction=False,
                                                         screen_change_after_last_instruction=True)

    response = client.get_async_response().data
    unpacked = unpack_sign_transaction_response(response)
    expected = {'structure_type': 42,
                'version': 1,
                'signature': {'pk_sig': '52635af7828f4ceadb4fb51ce14255a4fe2f8a29985d0ba4554f79da61f52007',
                              'pr_sig': '0778a6061189b5c56da8b5a6881197e52ad01f45960dcb1cd769a0799e3cda09'},
                'tvk': 'aaab438b86c993c90d6bafdf80af63ca8ffe6aefe2cc6f7ef1876d19fe488512',
                'tpk': '82ec6ee9302e9bc5731270438a9319c935eb695df74cbd18631f2e4e2a54740b',
                'gammas_count': 0
    }
    assert(check_response(unpacked, expected))


def test_sign_transaction_transfer_private(backend: BackendInterface, scenario_navigator: NavigateWithScenario) -> None:
    client = CommandSender(backend)
    record = ["3614797564276936744957924747041031196891698846785520060979425601577054464500field",
              "2426895214035216932245297778850989035038538961658726507442215877484415082794field",
              "0220642863446832956019507279394572297489712696240584424406852292692897199577field"]
    tx_datas = forge_private_transfer(500, 100, record, "aleo1sfydt6z6cnqjx3hcgk9ajw03ecj6uqlfcm9u3p3gdhckzcc2w5xqv3v3pe", 1000)
    tx_datas['path'] = "m/44'/683'/0'/0'/0'"
    with client.sign_transaction(tx_datas=tx_datas):
        scenario_navigator.review_approve_with_spinner("Calculating fees")

    response = client.get_async_response().data
    unpacked = unpack_sign_transaction_response(response)
    expected = {'structure_type': 42,
                'version': 1,
                'signature': {'pk_sig' : '52635af7828f4ceadb4fb51ce14255a4fe2f8a29985d0ba4554f79da61f52007',
                              'pr_sig' : '0778a6061189b5c56da8b5a6881197e52ad01f45960dcb1cd769a0799e3cda09'},
                'tvk': 'a8d1529eb6df81004a28af86347cae9005ea04244f4c1a816688e165cc18680a',
                'tpk': 'a24c8e1be3a48c5342784d5ddd92d470fc6db93ea680dd62ccc3e5c10fadf609',
                'gammas_count': 1,
                'gammas': ['5da12afb13f3b9d1dd29a5cd0e0e180e6db92002dabf9a8bd5d83f8ce831fc03']
    }
    assert(check_response(unpacked, expected))

    tx_datas = forge_private_fee(500, 100, record, "7266375125414209082394925781071362722506946030314916664133746682226945366259field")
    with client.sign_transaction(tx_datas=tx_datas):
        if scenario_navigator.device.is_nano:
            instruction = NavInsID.BOTH_CLICK
        else:
            instruction = NavInsID.USE_CASE_REVIEW_TAP
        scenario_navigator.navigator.navigate_until_text(navigate_instruction=instruction,
                                                         validation_instructions=None,
                                                         text="Transaction signed",
                                                         timeout=3,
                                                         screen_change_before_first_instruction=False,
                                                         screen_change_after_last_instruction=True)

    response = client.get_async_response().data
    unpacked = unpack_sign_transaction_response(response)
    expected = {'structure_type': 42,
                'version': 1,
                'signature': {'pk_sig' : '52635af7828f4ceadb4fb51ce14255a4fe2f8a29985d0ba4554f79da61f52007',
                              'pr_sig' : '0778a6061189b5c56da8b5a6881197e52ad01f45960dcb1cd769a0799e3cda09'},
                'tvk': 'e731d9987a156e31f5478109b10cadca099914263c7436efe1609e819c7b3105',
                'tpk': '28d6d3fa69ffb808bc5b06043b64dddca07225fc1e4d285cf7766baa54f26c00',
                'gammas_count': 1,
                'gammas': ['5da12afb13f3b9d1dd29a5cd0e0e180e6db92002dabf9a8bd5d83f8ce831fc03']
    }
    assert(check_response(unpacked, expected))


def test_sign_transaction_transfer_private_zero_fees(backend: BackendInterface, scenario_navigator: NavigateWithScenario) -> None:
    client = CommandSender(backend)
    record = ["3614797564276936744957924747041031196891698846785520060979425601577054464500field",
              "2426895214035216932245297778850989035038538961658726507442215877484415082794field",
              "0220642863446832956019507279394572297489712696240584424406852292692897199577field"]
    tx_datas = forge_private_transfer(0, 0, record, "aleo1sfydt6z6cnqjx3hcgk9ajw03ecj6uqlfcm9u3p3gdhckzcc2w5xqv3v3pe", 1000)
    tx_datas['path'] = "m/44'/683'/0'/0'/0'"
    with client.sign_transaction(tx_datas=tx_datas):
        scenario_navigator.review_approve()

    response = client.get_async_response().data
    unpacked = unpack_sign_transaction_response(response)
    expected = {'structure_type': 42,
                'version': 1,
                'signature': {'pk_sig' : '52635af7828f4ceadb4fb51ce14255a4fe2f8a29985d0ba4554f79da61f52007',
                              'pr_sig' : '0778a6061189b5c56da8b5a6881197e52ad01f45960dcb1cd769a0799e3cda09'},
                'tvk': 'a8d1529eb6df81004a28af86347cae9005ea04244f4c1a816688e165cc18680a',
                'tpk': 'a24c8e1be3a48c5342784d5ddd92d470fc6db93ea680dd62ccc3e5c10fadf609',
                'gammas_count': 1,
                'gammas': ['5da12afb13f3b9d1dd29a5cd0e0e180e6db92002dabf9a8bd5d83f8ce831fc03']
    }
    assert(check_response(unpacked, expected))