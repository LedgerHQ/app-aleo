import pytest
from ledger_app_clients.exchange.test_runner import ExchangeTestRunner, ALL_TESTS_EXCEPT_MEMO_THORSWAP_AND_FEES
from ledger_app_clients.exchange.cal_helper import CurrencyConfiguration
from ragger.error import ExceptionRAPDU
from ragger.utils import create_currency_config

from application_client.currency_utils import ALEO_PATH
from application_client.command_sender import CommandSender

from . import cal_helper as cal

def forge_public_transfer(max_base_fee: int, max_priority_fee: int, address_to: str,
                          amount: int, program_checksum: str = '') -> dict:

    data = {'type' : 'intent',
            'max_base_fee' : max_base_fee, 'max_priority_fee' : max_priority_fee,
            'fee_program_id' : 'credits.aleo', 'fee_function_name' : 'fee_public'}
    data['request'] = {'network_id' : 'mainnet', 'program_id' : 'credits.aleo', 'function_name' : 'transfer_public'}
    data['request']['inputs'] = [{'type' : 'address.public', 'value' : address_to},
                                 {'type' : 'u64.public',     'value' : amount}]
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


class GenericAleoTests(ExchangeTestRunner):
    # The coin configuration of our currency. Replace by your own
    currency_configuration = cal.ALEO_CURRENCY_CONFIGURATION
    # A valid template address of a supposed trade partner.
    valid_destination_1 = "aleo1sfydt6z6cnqjx3hcgk9ajw03ecj6uqlfcm9u3p3gdhckzcc2w5xqv3v3pe"
    # A memo to use associated with the destination address if applicable.
    valid_destination_memo_1 = ""
    # A second valid template address of a supposed trade partner.
    valid_destination_2 = "9fc3da866e7dF3a1c57ade1a97c9f00a70f010c8"
    # A second memo to use associated with the destination address if applicable.
    valid_destination_memo_2 = ""
    # The address of the Speculos seed on the ALEO_PATH.
    valid_refund = "aleo1k7349nakx72h3q3gm8zg6f0dksnkp9p9ha93lrx6zu2gnvjzpyrsj28j54"
    valid_refund_memo = ""

    # Values we ask the ExchangeTestRunner to use in the test setup
    valid_send_amount_1 = 1000
    valid_send_amount_2 = 666
    valid_fees_1 = 100
    valid_fees_2 = 200

    # Fake addresses to test the address rejection code.
    fake_refund = "abcdabcd"
    fake_refund_memo = "bla"
    fake_payout = "abcdabcd"
    fake_payout_memo = "bla"
    
    def perform_final_tx(self, destination, send_amount, fees, memo):
        client = CommandSender(self.backend)
        tx_datas = forge_public_transfer(0, fees, destination, send_amount)
        tx_datas['path'] = "m/44'/683'/0'/0'"
        with client.sign_transaction(tx_datas=tx_datas):
            pass
        rapdu = client.get_async_response()

        if fees != 0:
            tx_datas = forge_public_fee(0, fees, "7266375125414209082394925781071362722506946030314916664133746682226945366259field")
            with client.sign_transaction(tx_datas=tx_datas):
                pass
            rapdu = client.get_async_response()


class TestsAleo:

    @pytest.mark.parametrize('test_to_run', ALL_TESTS_EXCEPT_MEMO_THORSWAP_AND_FEES)
    def test_aleo(self, backend, exchange_navigation_helper, test_to_run):
        GenericAleoTests(backend, exchange_navigation_helper).run_test(test_to_run)
