#pragma once

// defines
#define Lengths 20

// Arduino base libraries

// third party libraries
#include <Keypad.h>

// OpenSmartMeter libraries
#include "led_buzzer.hpp"
#include "time_management.hpp"
#include "token_management.hpp"

#define STAR_KEY -1
#define HASH_KEY -2
#define NON_ACCEPTED_KEY -3

char customKey;

// keypad
const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {{'1', '2', '3', 'A'},
                             {'4', '5', '6', 'B'},
                             {'7', '8', '9', 'C'},
                             {'*', '0', '#', 'D'}};
byte rowPins[ROWS] = {PB9, PB8, PB1, PB0};
byte colPins[COLS] = {PA7, PA6, PA5, PC13};
Keypad customKeypad =
    Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

void FactorySetting() {
  lcd.clear();
  lcd.setCursor(0, 0);
  data_count = 0;
  lcd_count = 8;
  dt = 0;
  switch (setting_mode) {
    case 1:
      lcd.println(
          "001:Reinitialisation  011:Configuration  021:Change password");
      setting_mode = 2;
      break;

    case 2:  // Reinitialize values
      if (sts_data == "001") {
        fault = 0;
        billing = 0.0;
        true_power = 0.0;
        ENERGY = 0.0;
        freq = 0.0;
        curr = 0.0;
        powerFactor = 0.0;
        mains_input_value = 0.0;

        InvalidTokenCount = 0;
        TokenCount = 1;
        UsedTokens = 0;
        PAYGEnabled = true;
        ActiveUntil = 0;
        TokenEntryLockedUntil = 0;
        nbDisconnections = 0;
        StoreActivationVariables();

        topup = 0.0;
        topupnew = 0;

        warntime = 0;
        warn_now = 0;

        timeInitializationRtc = 0;
        hours = 0;
        minutes = 0;
        seconds = 0;
        rtcday = 0;
        nw_month_cnt = 0;
        rtcmonth = 0;
        rtcnewmonth = 0;
        billing_date = 0;

        thingsboard_check = 0;

        lastmonth_KWH = 0.0;
        initializeTime();

        lcdtime_now = 0;
        prev_lcdtime = 0;
        cnt = 0;
        lcd_count = 0;
        data_count = 0;
        dt = 0;
        parameters = 0;
        token_used = 0;

        sts_eeprom_fetched = 0;
        token_eeprom_location = 20;
        eeprom_location_cnt = 40;

        sts_value = 0;
        sts_mode = 0;

        lcd.println("Values reinitialize");
        delay(2000);
        lcd.clear();
        lcd.setCursor(0, 0);
        setting_mode = 0;
      }

      if (sts_data == "011") {  // Configure mode
        lcd.println("112:STS  122:Energy-based  132:Timed-based");
        setting_mode = 3;
      }

      if (sts_data == "021") {  // Change Password
        lcd.println("Old password: ");
        setting_mode = 4;
      }
      break;

    case 3:
      if (sts_data == "112") {
        Mode_select = 1;  // For STS Mode (Energy-based)
        lcd.println("configured to STS Mode");
        delay(2000);
        lcd.clear();
        lcd.setCursor(0, 0);
      }

      if (sts_data == "122") {
        Mode_select = 2;  // For OpenPaygo Energy-based
        lcd.println("configured to OPaygo Energy-based Mode");
        delay(2000);
        lcd.clear();
        lcd.setCursor(0, 0);
      }

      if (sts_data == "132") {
        Mode_select = 3;  // For OpenPaygo Time-based
        lcd.println("configured to OPaygo Time-based Mode");
        delay(2000);
        lcd.clear();
        lcd.setCursor(0, 0);
      }
      setting_mode = 0;
      break;

    case 4:  // Enter new password
      if (sts_data == password) {
        lcd.println("New password: ");
        setting_mode = 5;
      }
      break;

    case 5:  // confirm new password
      new_password = sts_data;
      lcd.println("confirm new password: ");
      setting_mode = 6;
      break;

    case 6:  // Password Changed
      if (sts_data == new_password) {
        password = new_password;
        lcd.println("password changed ");
        delay(2000);
        lcd.clear();
        lcd.setCursor(0, 0);
        setting_mode = 0;
      }
      break;
  }
}
// For STS

void STS_keypad() {
  customKey = customKeypad.getKey();
  if (customKey == '*') {
    data_count = 0;
    lcd_count = 12;
    dt = 0;
    sts_value = 0;
    delay(20);
    lcd.clear();
    buss();  // emit a sound to inform that it is ready to receive the token
    lcd.setCursor(0, 0);
    sts_mode = 1;
    if (customKeypad.getState() == HOLD) {
      lcd.println("Enter Password to start factory setting: ");
      setting_mode = 1;
    } else {
      lcd.print("TOKEN: ");
      setting_mode = 0;
    }
  }

  if (sts_mode == 1 && customKey != '*' && customKey != '#' &&
      customKey != 'D') {
    if (customKey) {
      if (dt < 21) {
        delay(20);
        buss();
        dt++;
        if (dt < 5) {
          Data[data_count] = customKey;
          lcd.setCursor(lcd_count, 0);
          lcd.print(Data[data_count]);
          data_count++;
        }
        if (lcd_count > 15) {
          lcd_count = 0;
        }
        if (dt > 4) {
          Data[data_count] = customKey;
          lcd.setCursor(lcd_count, 1);
          lcd.print(Data[data_count]);
          data_count++;
        }
        lcd_count++;
        sts_value = data_count;
      }
    }
  }
  if (customKey == 'D') {
    sts_data = "";
    sts_mode = 0;
    sts_value = 0;
  }

  if (customKey == 'A' && sts_mode == 0) {
    parameters = parameters + 1;
  }

  if (customKey == '#' && sts_mode == 1) {
    sts_data = Data;
    if (data_count > 19) {
      check_tokenused();
      if (token_used == 0) {
        STStoken_decode();
      }
    }

    if (data_count < 19 && data_count >= 3) {
      if (setting_mode == 1) {
        if (sts_data == password) {
          FactorySetting();
        }
      } else if (setting_mode > 1) {
        FactorySetting();
      }
    }
  }
}

// For Opaygo

int GetKeyPressed()
/*
 * returns the key pressed on the Keypad remote as a int (this function forces
 * to put the key in a char, good practice)
 */
{
  char incomingByte;
  incomingByte = customKeypad.getKey();
  Serial.print("incoming byte = ");
  Serial.println(incomingByte);
  switch (incomingByte) {
    case '*':
      return STAR_KEY;
      break;

    case '#':
      return HASH_KEY;
      break;

    default:
      return (int)(incomingByte - '0');  // this_char is now an int
      break;
  }
}
