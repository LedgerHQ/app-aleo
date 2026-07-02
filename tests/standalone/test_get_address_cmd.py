import pytest

from ragger.error import ExceptionRAPDU, StatusWords
from ragger.backend.interface import BackendInterface
from ragger.navigator.navigation_scenario import NavigateWithScenario

from application_client.command_sender import CommandSender
from application_client.response_unpacker import unpack_get_address_response


# In this test we check that the CMD_GET_ADDRESS works in non-confirmation mode
def test_get_address_no_confirm(backend: BackendInterface) -> None:
    test_list = [
        {
            "path": "m/44'/683'/0'/0'",
            "address": "aleo1k7349nakx72h3q3gm8zg6f0dksnkp9p9ha93lrx6zu2gnvjzpyrsj28j54",
        },
        {
            "path": "m/44'/683'/1'/0'",
            "address": "aleo1tmp0fupalv9yn0waxx2saanaq44arvqyz8drltdz8nc4yxytcsxquc66t6",
        },
        {
            "path": "m/44'/683'/911'/0'",
            "address": "aleo14xxsl4w2l5ac905986cwfff5kjj6f9ml9e53rjkzasneyx34tc8scdu53v",
        },
        {
            "path": "m/44'/683'/255'/255'",
            "address": "aleo13z7g8tfee8yzcwn458y98cr6c04ylauxkg4fg9c625du6hpaluxs3ynj48",
        },
        {
            "path": "m/44'/683'/2147483647'/0'",
            "address": "aleo1m45zd4j7g40mdkfmmhcqrsxpf93ktcwkrum7png43f5syqvu9u8ssmq39g",
        },
    ]
    for test in test_list:
        client = CommandSender(backend)
        response = client.get_address_without_confirmation(path=test["path"]).data
        _, address = unpack_get_address_response(response)

        assert address.decode("utf-8") == test["address"]


# In this test we check that the CMD_GET_ADDRESS works in confirmation mode
def test_get_address_confirm_accepted(
    backend: BackendInterface, scenario_navigator: NavigateWithScenario
) -> None:
    client = CommandSender(backend)
    path = "m/44'/683'/0'/0'"
    with client.get_address_with_confirmation(path=path):
        scenario_navigator.address_review_approve()

    response = client.get_async_response().data
    _, address = unpack_get_address_response(response)

    assert (
        address.decode("utf-8")
        == "aleo1k7349nakx72h3q3gm8zg6f0dksnkp9p9ha93lrx6zu2gnvjzpyrsj28j54"
    )


# In this test we check that the CMD_GET_ADDRESS in confirmation mode replies an error if the user refuses
def test_get_address_confirm_refused(
    backend: BackendInterface, scenario_navigator: NavigateWithScenario
) -> None:
    client = CommandSender(backend)
    path = "m/44'/683'/0'/0'"

    with pytest.raises(ExceptionRAPDU) as e:
        with client.get_address_with_confirmation(path=path):
            scenario_navigator.address_review_reject()

    # Assert that we have received a refusal
    assert e.value.status == StatusWords.SWO_PERMISSION_DENIED
    assert len(e.value.data) == 0
