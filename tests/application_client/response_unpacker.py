from typing import Tuple
from struct import unpack

from .transaction import Transaction

# remainder, data_len, data
def pop_sized_buf_from_buffer(buffer:bytes, size:int) -> Tuple[bytes, bytes]:
    return buffer[size:], buffer[0:size]

# remainder, data_len, data
def pop_size_prefixed_buf_from_buf(buffer:bytes) -> Tuple[bytes, int, bytes]:
    data_len = buffer[0]
    return buffer[1+data_len:], data_len, buffer[1:data_len+1]

# Unpack from response:
# response = app_name (var)
def unpack_get_app_name_response(response: bytes) -> str:
    return response.decode("ascii")

# Unpack from response:
# response = MAJOR (1)
#            MINOR (1)
#            PATCH (1)
def unpack_get_version_response(response: bytes) -> Tuple[int, int, int]:
    assert len(response) == 3
    major, minor, patch = unpack("BBB", response)
    return (major, minor, patch)

# Unpack from response:
# response = format_id (1)
#            app_name_raw_len (1)
#            app_name_raw (var)
#            version_raw_len (1)
#            version_raw (var)
#            unused_len (1)
#            unused (var)
def unpack_get_app_and_version_response(response: bytes) -> Tuple[str, str]:
    response, _ = pop_sized_buf_from_buffer(response, 1)
    response, _, app_name_raw = pop_size_prefixed_buf_from_buf(response)
    response, _, version_raw = pop_size_prefixed_buf_from_buf(response)
    response, _, _ = pop_size_prefixed_buf_from_buf(response)

    assert len(response) == 0

    return app_name_raw.decode("ascii"), version_raw.decode("ascii")

# Unpack from response:
# response = address_len (1)
#            address (var)
def unpack_get_address_response(response: bytes) -> Tuple[int, bytes]:
    response, address_len, address = pop_size_prefixed_buf_from_buf(response)

    assert address_len == 63
    assert len(response) == 0

    return address_len, address

# Unpack from response:
# response = view_key_len (1)
#            view_key (var)
def unpack_get_view_key_response(response: bytes) -> Tuple[int, bytes]:
    response, view_key_len, view_key = pop_size_prefixed_buf_from_buf(response)

    assert view_key_len == 53
    assert len(response) == 0

    return view_key_len, view_key

# Unpack from response:
def unpack_sign_transaction_response(response: bytes) -> dict:
    tx = Transaction()
    return tx.unpack_response(response.hex())


