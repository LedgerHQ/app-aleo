import pytest

from ragger.error import ExceptionRAPDU, StatusWords
from ragger.backend.interface import BackendInterface
from ragger.navigator.navigation_scenario import NavigateWithScenario

from application_client.command_sender import CommandSender
from application_client.response_unpacker import unpack_get_view_key_response


# In this test we check that the CMD_GET_VIEW_KEY works in confirmation mode
def test_get_view_key_confirm_accepted(
    backend: BackendInterface, scenario_navigator: NavigateWithScenario
) -> None:
    client = CommandSender(backend)
    path = "m/44'/683'/0'/0'"
    with client.get_view_key(path=path):
        scenario_navigator.address_review_approve()

    response = client.get_async_response().data
    _, address = unpack_get_view_key_response(response)

    assert (
        address.decode("utf-8")
        == "AViewKey1fnXDtDJz1Vr8hRFXa7ZxwWA37E3TX9MrQJei691gSJkA"
    )


# In this test we check that the CMD_GET_VIEW_KEY in confirmation mode replies an error if the user refuses
def test_get_view_key_confirm_refused(
    backend: BackendInterface, scenario_navigator: NavigateWithScenario
) -> None:
    client = CommandSender(backend)
    path = "m/44'/683'/0'/0'"

    with pytest.raises(ExceptionRAPDU) as e:
        with client.get_view_key(path=path):
            scenario_navigator.address_review_reject()

    # Assert that we have received a refusal
    assert e.value.status == StatusWords.SWO_PERMISSION_DENIED
    assert len(e.value.data) == 0
