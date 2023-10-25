from openpaygo import generate_token, TokenType
from firmware.test.model import Device

# Device definition
device_settings = {
    'serial': 'OSM10000001',
    'starting_code': 407592873,
    'secret_key': '47a01268b629e1b027fe20c99309643f',
    'restricted_digit_set': False,
    'value_divider': 1,
    'count': 0
}
# Token definition
token_type = TokenType.ADD_TIME  # ADD_TIME, SET_TIME, DISABLE_PAYG
token_value = 7  # Set to 0 if DISABLE_PAYG

# We get the new token and update the count
device = Device(device_settings)
device.count, new_token = generate_token(
    secret_key=device.secret_key,
    count=device.count,
    value=token_value,
    token_type=TokenType.ADD_TIME,
    starting_code=device.starting_code,
    value_divider=device.value_divider,
    restricted_digit_set=device.restricted_digit_set
)

print('Token: ' + new_token)
