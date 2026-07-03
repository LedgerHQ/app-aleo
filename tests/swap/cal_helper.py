from ledger_app_clients.exchange.cal_helper import CurrencyConfiguration
from ragger.bip import pack_derivation_path
from ragger.utils import create_currency_config
from application_client.currency_utils import ALEO_PATH

# Define a configuration for each currency used in our tests: native coins and tokens

# ALEO token currency definition
ALEO_CONF = create_currency_config("ALEO", "Aleo", sub_coin_config=None)
# Serialized derivation path for the Aleo app
ALEO_PACKED_DERIVATION_PATH = pack_derivation_path(ALEO_PATH)
# Coin configuration mock as stored in CAL for the SWAP feature
ALEO_CURRENCY_CONFIGURATION = CurrencyConfiguration(ticker="ALEO",
                                                    conf=ALEO_CONF,
                                                    packed_derivation_path=ALEO_PACKED_DERIVATION_PATH)

# ALEO USAD token configuration - USAD with 6 decimals
ALEO_USAD_CONF = create_currency_config("USAD", "Aleo", sub_coin_config=("USAD", 6))
ALEO_USAD_PACKED_DERIVATION_PATH = ALEO_PACKED_DERIVATION_PATH
ALEO_USAD_CURRENCY_CONFIGURATION = CurrencyConfiguration(ticker="USAD",
                                                         conf=ALEO_USAD_CONF,
                                                         packed_derivation_path=ALEO_USAD_PACKED_DERIVATION_PATH)

# ALEO USDCx token configuration - USAD with 6 decimals
ALEO_USDCX_CONF = create_currency_config("USDCx", "Aleo", sub_coin_config=("USDCx", 6))
ALEO_USDCX_PACKED_DERIVATION_PATH = ALEO_PACKED_DERIVATION_PATH
ALEO_USDCX_CURRENCY_CONFIGURATION = CurrencyConfiguration(ticker="USDCx",
                                                          conf=ALEO_USDCX_CONF,
                                                          packed_derivation_path=ALEO_USDCX_PACKED_DERIVATION_PATH)
