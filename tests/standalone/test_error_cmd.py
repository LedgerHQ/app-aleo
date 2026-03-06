import pytest

from ragger.error import ExceptionRAPDU, StatusWords
from ragger.backend.interface import BackendInterface

from application_client.command_sender import CLA, InsType, P1, P2, Errors


# Ensure the app returns an error when a bad CLA is used
def test_bad_cla(backend: BackendInterface) -> None:
    with pytest.raises(ExceptionRAPDU) as e:
        backend.exchange(cla=CLA + 1, ins=InsType.CMD_GET_VERSION)
    assert e.value.status == StatusWords.SWO_INVALID_CLA


# Ensure the app returns an error when a bad INS is used
def test_bad_ins(backend: BackendInterface) -> None:
    with pytest.raises(ExceptionRAPDU) as e:
        backend.exchange(cla=CLA, ins=0xff)
    assert e.value.status == StatusWords.SWO_INVALID_INS


# Ensure the app returns an error when a bad P1 or P2 is used
def test_wrong_p1p2(backend: BackendInterface) -> None:
    with pytest.raises(ExceptionRAPDU) as e:
        backend.exchange(cla=CLA, ins=InsType.CMD_GET_VERSION, p1=0x01, p2=0x00)
    assert e.value.status == StatusWords.SWO_INCORRECT_P1_P2
    with pytest.raises(ExceptionRAPDU) as e:
        backend.exchange(cla=CLA, ins=InsType.CMD_GET_VERSION, p1=0x00, p2=0x01)
    assert e.value.status == StatusWords.SWO_INCORRECT_P1_P2
    with pytest.raises(ExceptionRAPDU) as e:
        backend.exchange(cla=CLA, ins=InsType.CMD_GET_APP_NAME, p1=0x01, p2=0x00)
    assert e.value.status == StatusWords.SWO_INCORRECT_P1_P2
    with pytest.raises(ExceptionRAPDU) as e:
        backend.exchange(cla=CLA, ins=InsType.CMD_GET_APP_NAME, p1=0x00, p2=0x01)
    assert e.value.status == StatusWords.SWO_INCORRECT_P1_P2


# Ensure the app returns an error when a bad data length is used
def test_wrong_data_length(backend: BackendInterface) -> None:
    # APDUs must be at least 4 bytes: CLA, INS, P1, P2.
    with pytest.raises(ExceptionRAPDU) as e:
        backend.exchange_raw(bytes.fromhex("E00300"))
    assert e.value.status == StatusWords.SWO_WRONG_DATA_LENGTH
    # APDUs advertises a too long length
    with pytest.raises(ExceptionRAPDU) as e:
        backend.exchange_raw(bytes.fromhex("E003000005"))
    assert e.value.status == StatusWords.SWO_WRONG_DATA_LENGTH
