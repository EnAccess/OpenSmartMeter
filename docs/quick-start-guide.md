# Getting Started

You have now an OpenSmartMeter in your hands and you want to test it? Here is how.

## Initial configuration

ToDo: Do a tutorial on how to access the technical mode via the keypad of the OpenSmartMeter. Make pictures of the OSM.

ToDo: How to set up secret key, starting code, etc.

## Testing OpenPAYGO tokens

After plugging the OSM, it will appear the following screen:
ToDo: Provide picture

By default, the OSM is deactivated, which means it doesn't provide any energy on its output.
If the initial default configuration has been applied, you can test tokens available on the Excel sheet "Token examples.xlsx" (Find it here: OpenSmartMeter\firmware\test).
If you want to create other token:

1. Open the repository on an IDE with python3.9 or above.
2. Install the python requirements (in OpenSmartMeter/firmware/test):
   pip3 install -r requirements.txt
3. Change initial parameters in "test_openpaygo.py"
Token_value can be days or kWh, depending on the initial default configuration you have defined on the OSM itself.
4. Run script and get your desired token in console output.
