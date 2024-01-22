---
sidebar_position: 1
---

# Getting Started

You have now an OpenSmartMeter in your hands and you want to test it? Here is how.

## Initial configuration

ToDo: Do a tutorial on how to access the technical mode via the keypad of the OpenSmartMeter. Make pictures of the OSM.

ToDo: How to set up secret key, starting code, etc.

## Testing OpenPAYGO tokens

After plugging the OSM, it will appear the following screen:
ToDo: Provide a picture

By default, the OSM is deactivated, which means it doesn't provide any energy in its output.
If the initial default configuration has been applied, you can test tokens available here:

**Default configuration:**

```sh
Starting Code: 407592873
Secret Key: 47a01268b629e1b027fe20c99309643f
```

| Time/Energy     | Serial OSM10000001 |
| --------------- | ------------------ |
| add 1 day/kWh   | 287 923 874        |
| add 30 days/kWh | 743 630 903        |
| set 7 days/kWh  | 844 231 880        |
| unlock forever  | 304 054 873        |

:::warning Remark

We cannot jump back to a previous token if a later token has been already entered.

:::

If you want to create other tokens:

1. Open the repository on an IDE with python3.9 or above.
2. Install the Python requirements (in OpenSmartMeter/firmware/test):
   `pip3 install -r requirements.txt`
3. Change initial parameters in "test_openpaygo.py"
   Token_value can be days or kWh, depending on the initial default configuration you have defined on the OSM itself.
4. Run the script and get your desired token in the console output.
