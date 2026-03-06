import pytest

from ragger.error import ExceptionRAPDU, StatusWords
from ragger.backend.interface import BackendInterface
from ragger.navigator.navigation_scenario import NavigateWithScenario

from application_client.command_sender import CommandSender
from application_client.response_unpacker import unpack_get_address_response


# In this test we check that the CMD_GET_ADDRESS works in non-confirmation mode
def test_get_address_no_confirm(backend: BackendInterface) -> None:
    test_list = [
        {'path' : "m/44'/683'/0'/0'", 'address' : "aleo1tqf8vya0zrwlzmmck04cq4vv7lkcgje5srr8us84wttmedc4cv9q80wam3"},
        {'path' : "m/44'/683'/1'/0'", 'address' : "aleo1k4js0rx67xqpft3rese85zlayet3zqx4g0kyn8lqcu6hqwuvt5zq3397la"},
        {'path' : "m/44'/683'/911'/0'", 'address' : "aleo159srl98sugxj789a0ak3vmrs9sxlq6364etvdzfmrnzrvh28dvgss3gsva"},
        {'path' : "m/44'/683'/255'/255'", 'address' : "aleo125vymm425xfvth0ytef75506sadmdsu7hpwq6qe60yjcuj02ks8s74wcrn"},
        {'path' : "m/44'/683'/2147483647'/0'", 'address' : "aleo1ul3jenvpax4uve8kh5g43km4usmfew6cjqzgv3uxzwfp5edpfvfqjc9p42"},
    ]
    for test in test_list:
        client = CommandSender(backend)
        response = client.get_address_without_confirmation(path=test['path']).data
        _, address = unpack_get_address_response(response)

        assert address.decode("utf-8") == test['address']


# In this test we check that the CMD_GET_ADDRESS works in confirmation mode
def test_get_address_confirm_accepted(backend: BackendInterface, scenario_navigator: NavigateWithScenario) -> None:
    client = CommandSender(backend)
    path = "m/44'/683'/0'/0'"
    with client.get_address_with_confirmation(path=path):
        scenario_navigator.address_review_approve()

    response = client.get_async_response().data
    _, address = unpack_get_address_response(response)

    assert address.decode("utf-8") == "aleo1tqf8vya0zrwlzmmck04cq4vv7lkcgje5srr8us84wttmedc4cv9q80wam3"


# In this test we check that the CMD_GET_ADDRESS in confirmation mode replies an error if the user refuses
def test_get_address_confirm_refused(backend: BackendInterface, scenario_navigator: NavigateWithScenario) -> None:
    client = CommandSender(backend)
    path = "m/44'/683'/0'/0'"

    with pytest.raises(ExceptionRAPDU) as e:
        with client.get_address_with_confirmation(path=path):
            scenario_navigator.address_review_reject()

    # Assert that we have received a refusal
    assert e.value.status == StatusWords.SWO_PERMISSION_DENIED
    assert len(e.value.data) == 0
