class Device(object):
    def __init__(self, device_settings):
        self.serial = device_settings.get("serial", None)
        self.starting_code = device_settings.get("starting_code", None)
        self.secret_key = device_settings.get("secret_key", None)
        self.restricted_digit_set = device_settings.get("restricted_digit_set", None)
        self.value_divider = device_settings.get("value_divider", None)
        self.count = device_settings.get("count", None)
