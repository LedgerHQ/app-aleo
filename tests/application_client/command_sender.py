from enum import IntEnum
from typing import Generator, List, Optional
from contextlib import contextmanager

from ragger.backend.interface import BackendInterface, RAPDU
from ragger.bip import pack_derivation_path


MAX_APDU_LEN: int = 255

CLA: int = 0xE0

class P1(IntEnum):
    # Parameter 1 for APDU get without confirmation.
    P1_GET_WITHOUT_CONFIRMATION = 0x00
    # Parameter 1 for APDU get with confirmation.
    P1_GET_WITH_CONFIRMATION = 0x01
    # Parameter 1 for APDU sign mode root (intent).
    P1_SIGN_MODE_ROOT = 0x00
    # Parameter 1 for APDU sign mode nester call (following a root).
    P1_SIGN_MODE_NESTED_CALL = 0x01
    # Parameter 1 for APDU sign mode fee.
    P1_SIGN_MODE_FEE = 0x02

class P2(IntEnum):
    # Parameter 2 for APDU begin chunk.
    P2_BEGIN = 0x00
    # Parameter 2 for APDU next chunk.
    P2_CONTINUE = 0x01

class InsType(IntEnum):
    CMD_GET_VERSION      = 0x03
    CMD_GET_APP_NAME     = 0x04
    CMD_GET_ADDRESS      = 0x05
    CMD_SIGN_TRANSACTION = 0x06
    CMD_GET_VIEW_KEY     = 0x07

class Errors(IntEnum):
    SW_DISPLAY_BIP32_PATH_FAIL   = 0xb001
    """
    SWO_WRONG_DATA_LENGTH        = 0x6a87
    SWO_INVALID_CLA              = 0x6e00
    SWO_INCORRECT_P1_P2          = 0x6a86
    SWO_SUCCESS                  = 0x9000
    SWO_INVALID_INS              = 0x6d00
    SWO_CONDITIONS_NOT_SATISFIED = 0x6985
    SWO_SUCCESS                  = 0x9000
    SWO_INCORRECT_DATA           = 0x6a80
    SWO_INSUFFICIENT_MEMORY      = 0x6a84
    SWO_PERMISSION_DENIED        = 0x69f0
    """


def split_message(message: bytes, max_size: int) -> List[bytes]:
    return [message[x:x + max_size] for x in range(0, len(message), max_size)]


class CommandSender:
    def __init__(self, backend: BackendInterface) -> None:
        self.backend = backend


    def get_app_and_version(self) -> RAPDU:
        return self.backend.exchange(cla=0xB0,  # specific CLA for BOLOS
                                     ins=0x01,  # specific INS for get_app_and_version
                                     p1=0x00,
                                     p2=0x00,
                                     data=b"")


    def get_version(self) -> RAPDU:
        return self.backend.exchange(cla=CLA,
                                     ins=InsType.CMD_GET_VERSION,
                                     p1=0x00,
                                     p2=0x00,
                                     data=b"")


    def get_app_name(self) -> RAPDU:
        return self.backend.exchange(cla=CLA,
                                     ins=InsType.CMD_GET_APP_NAME,
                                     p1=0x00,
                                     p2=0x00,
                                     data=b"")

    def get_address_without_confirmation(self, path: str) -> RAPDU:
        return self.backend.exchange(cla=CLA,
                                     ins=InsType.CMD_GET_ADDRESS,
                                     p1=P1.P1_GET_WITHOUT_CONFIRMATION,
                                     p2=0x00,
                                     data=pack_derivation_path(path))

    @contextmanager
    def get_address_with_confirmation(self, path: str) -> Generator[None, None, None]:
        with self.backend.exchange_async(cla=CLA,
                                         ins=InsType.CMD_GET_ADDRESS,
                                         p1=P1.P1_GET_WITH_CONFIRMATION,
                                         p2=0x00,
                                         data=pack_derivation_path(path)) as response:
            yield response

    @contextmanager
    def get_view_key(self, path: str) -> Generator[None, None, None]:
        with self.backend.exchange_async(cla=CLA,
                                         ins=InsType.CMD_GET_VIEW_KEY,
                                         p1=P1.P1_GET_WITH_CONFIRMATION,
                                         p2=0x00,
                                         data=pack_derivation_path(path)) as response:
            yield response


    # Retrieve the last asynchronous response from the backend
    def get_async_response(self) -> Optional[RAPDU]:
        return self.backend.last_async_response
