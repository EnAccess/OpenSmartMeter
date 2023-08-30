// defines

// Arduino base libraries
#include <Arduino.h>

// third party libraries

// OpenSmartMeter libraries
#include "credit.hpp"
#include "global_defines.hpp"
#include "helpers.hpp"
#include "lcd_display.hpp"
#include "lcd_init.hpp"
#include "mem_init.hpp"
#include "opaygo_functions.hpp"
#include "power.hpp"
#include "relay.hpp"
#include "remote.hpp"
#include "sts_token.hpp"
#include "thingsboard.hpp"
#include "time_management.hpp"
#include "token_management.hpp"

extern "C" {
#include "opaygo_decoder.h"
}

uint64_t InputToken;
TokenData Output;
uint32_t StartingCode = 123456789;
unsigned char SECRET_KEY[16] = {0xa2, 0x9a, 0xb8, 0x2e, 0xdc, 0x5f, 0xbb, 0xc4,
                                0x1e, 0xc9, 0x53, 0xf,  0x6d, 0xac, 0x86, 0xb1};
// unsigned char SECRET_KEY[16] = {...};

// meter_const_config deviceConfig;
HardwareSerial Serial2(PA3, PA2);

byte fe1[8] = {0b00011, 0b00011, 0b00011, 0b00011,
               0b00011, 0b11111, 0b11111, 0b11111};

byte fe3[8] = {0b11111, 0b11111, 0b11111, 0b00011,
               0b00011, 0b00011, 0b00011, 0b00011};

byte fe2[8] = {0b11111, 0b11111, 0b00000, 0b00000,
               0b00000, 0b01100, 0b01100, 0b01111};

byte fe4[8] = {0b01111, 0b01100, 0b01100, 0b00000,
               0b00000, 0b00000, 0b00000, 0b00000};

void setup() {
  Serial2.begin(115200);
  Serial2.print("Device Powered! \n");

  lcd.begin(16, 2);
  pinMode(buzzer, OUTPUT);
  pinMode(relaya, OUTPUT);
  pinMode(relayb, OUTPUT);
  pinMode(red_led, OUTPUT);
  pinMode(green_led, OUTPUT);

  // Display Intro Screen.
  digitalWrite(buzzer, HIGH);
  lcd.createChar(0, fe1);
  lcd.createChar(1, fe2);
  lcd.createChar(2, fe3);
  lcd.createChar(3, fe4);
  lcd.setCursor(0, 0);
  lcd.print("    FIRST      ");
  lcd.setCursor(0, 1);

  lcd.print("    ELECTRIC CO.   ");
  lcd.setCursor(1, 0);
  lcd.write(byte(0));
  lcd.setCursor(2, 0);
  lcd.write(byte(1));
  lcd.setCursor(1, 1);
  lcd.write(byte(2));
  lcd.setCursor(2, 1);
  lcd.write(byte(3));
  Serial2.print("LCD Setup complete! \n");

  Serial2.print("Setting up AFE... \n");
  ATM90E26.begin(9600);
  AFE_chip.SET_register_values();
  delay(1000);
  Serial2.print("AFE Setup Complete! \n");
  delay(2000);
  lcd.clear();
  digitalWrite(buzzer, LOW);

  // Begin Device Initialization.
  lcd.setCursor(0, 0);
  lcd.print(" SYSTEM BOOTING ");
  lcd.setCursor(0, 1);
  lcd.print(" #------------#");

  // Configure GSM modem.
  Serial2.println("Initializing modem...");
  Serial1.begin(115200);
  delay(1500);
  Serial1.write("AT+IPR=9600\r\n");
  Serial1.end();
  Serial1.begin(9600);
  modem.restart();
  String modemInfo = modem.getModemInfo();
  lcd.setCursor(0, 0);
  lcd.print("                     ");
  lcd.setCursor(0, 1);
  lcd.print("                    ");

  Serial2.print("Modem: ");
  Serial2.println(modemInfo);
  lcd.setCursor(0, 0);
  lcd.print("ModemID ");
  lcd.setCursor(0, 1);
  lcd.print(modemInfo);
  delay(2000);
  lcd.clear();

  // Configure RTC
  while (rtc.begin() == false) {
    lcd.print("Couldn't find RTC");
    delay(500);
  }

  if (!rtc.isrunning()) {
    // If not running, enable and set Dat&time.
    rtc.adjust(
        DateTime(2020, 1, 1, 3, 0, 0));  //<YEAR>,<MTH>,<DAY>,<HR><MIN><SEC>
    delay(500);
  }
  initializeTime();

  // Configure AFE.
  ATM90E26.begin(9600);
  AFE_chip.SET_register_values();
  delay(500);

  // Get Meter AFE checksum
  lcd.setCursor(0, 0);
  lcd.print("CSOne :         ");
  lcd.setCursor(8, 0);
  lcd.print(AFE_chip.FETCH_MeterCSOne());

  lcd.setCursor(0, 1);
  lcd.print("CSTwo :         ");
  lcd.setCursor(8, 1);
  lcd.print(AFE_chip.FETCH_MeterCSTwo());
  delay(3000);

  // Configure EEPROM.
  // mem.writeLong(credit_eeprom_location, 200);
  //  mem.writeLong(eeprom_location_cnt, token_eeprom_location);
  //  delay(20);
  //  mem.writeLong(token_eeprom_location, 200);
  //  delay(20);
  creditt = mem.readLong(credit_eeprom_location);
  if (creditt >= 0) {
    mem.writeLong(credit_eeprom_location,
                  creditt);  // Not sure why this is needed. Should be removed.
  }

  /* TODO: Get device configuration parameters from eeprom */
  // meter_const_config deviceConfig;     //Meter configuration struct.
  //
  // deviceConfig =  mem.readLong(deviceConfig_location);
  // while(deviceConfig.status == NOT_SET_CONFIG)
  // {
  //   Serial2.println("Device configuration required...\n");
  //   STEPS:
  //    1. Start timer and wait for command from serial 2 containing the
  //    configuration parameters.
  //    2. Verify Configuration parameters and save into struct "deviceConfig"
  //    3. Save struct into the EEPROM.
  // }

  delay(10);
  relay_on();
#if defined(TIM1)
  TIM_TypeDef* Instance = TIM1;
#else
  TIM_TypeDef* Instance = TIM2;
#endif
  HardwareTimer* MyTim = new HardwareTimer(Instance);
  MyTim->setOverflow(20, HERTZ_FORMAT);
  switch (Mode_select) {
    case 0:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.println("No Configuration! ");
      while (Mode_select == 0)  // wait for mode configuration
      {
        STS_keypad();
        delay(20);
      }
      break;

    case 1:
      MyTim->attachInterrupt(urgeent);
      MyTim->resume();
      break;

    case 2:
      /*OpenPayGo Token initializing code; */
      printf("Welcome to the OPAYGO Device\n");
      printf(
          "We're waiting for the * character to start recording the key "
          "presses.\n(Press the '#' key to see the device activation "
          "status)\n\n");
      LoadActivationVariables();  // We load the activation variableS
      break;
  }
}

void loop() {
  if (Mode_select == 1) {
    /**RUN CODE FOR STS MODE**/
    mesure();  // Read AFE parameters.
    if ((mains_input_value > 50)) {
      credit_reminder();
    }
    if ((mains_input_value < 50)) {
      digitalWrite(red_led, LOW);
      digitalWrite(green_led, LOW);
    }
    get_time();  // Update Date and Time.
    if ((sts_mode == 0) && (mains_input_value > 50)) {
      gsm_func();
    }
  }

  if (Mode_select == 2) {
    // We wait for a token to be entered
    InputToken = WaitForTokenEntry();
    // We get the activation value from the token

    Output = GetDataFromToken(
        InputToken, &TokenCount, &UsedTokens, StartingCode,
        SECRET_KEY);  // We get the activation value from the token

    printf("\n(Token entered: %llu)", InputToken);
    printf("\n(Activation Value from Token: %d)",
           Output.Value);  // Activation Value found in the token
    printf("\n(Count: %d)", Output.Count);        // Count found in the token
    printf("\n(Max Count: %d)", TokenCount);      // Count found in the token
    printf("\n(Used Tokens: %d)\n", UsedTokens);  // Count found in the token

    UpdateDeviceStatusFromTokenValue(Output.Value, Output.Count);
  }
}
