#pragma once

// defines
#define Lengths 20

// Arduino base libraries

// third party libraries
#include <Keypad.h>

// OpenSmartMeter libraries
#include "lcd_init.hpp"
#include "mem_init.hpp"
#include "sts_token.hpp"

char customKey;
bool is_STSmode = false;  // by default on OpenPAYGO Token

bool passWORD_set;

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
char Data[Lengths];

int lcd_count = 0;
byte data_count = 0;
byte dt = 0;
byte parameters = 0;
byte token_used = 0;
String password = "1234";

extern bool is_STSmode;
//    for meter
unsigned long sts_eeprom_fetched = 0;
unsigned long token_eeprom_location = 20;
unsigned long eeprom_location_cnt = 40;

unsigned long sts_value = 0;
unsigned long sts_mode = 0;
unsigned long Mode_select = 0;

void buss() {
  digitalWrite(buzzer, HIGH);
  delay(10);
  digitalWrite(buzzer, LOW);
}

void check_tokenused() {
  token_eeprom_location = mem.readLong(eeprom_location_cnt);
  if (token_eeprom_location > 4000) {
    mem.writeLong(eeprom_location_cnt, 40);
    token_eeprom_location = 40;
  }
  // token_eeprom_location = token_eeprom_location +1;
  unsigned int locationcount;
  for (locationcount = 40; locationcount < token_eeprom_location;
       locationcount++) {
    sts_eeprom_fetched = mem.readLong(locationcount);
    String ee_fetched = String(sts_eeprom_fetched);
    String sts_keyco = sts_data.substring(0, 10);
    String keyco = ee_fetched.substring(0, 10);

    long conv_keyco = keyco.toInt();
    long conv_sts_keyco = sts_keyco.toInt();
    if ((conv_keyco == conv_sts_keyco)) {
      if (c_chek == 0) {
        warntime = warn_now;
        c_chek = 1;
      }
      lcd.setCursor(0, 0);
      lcd.print("  WARNING!!!!!  ");
      lcd.setCursor(0, 1);
      lcd.print("   USED TOKEN   ");
      digitalWrite(buzzer, HIGH);
      token_used = 1;
    }
  }
}

void STS_keypad() {
  customKey = customKeypad.getKey();

  if (customKey == '*') {  // Clear screen and reset keypad.
    preset_keyprocess();
    sts_mode == 0;
  }

  else if (customKey == 'A') {  // SCROLL up device parameter on LCD.
    if (sts_mode)
      sts_mode = false;
    parameters = parameters + 1;
  } else if (customKey == 'B' &&
             parameters) {  // SCROLL down device parameter on LCD
    if (sts_mode)
      sts_mode = false;
    parameters = parameters - 1;
  } else if (customKey == '#' && sts_mode == 1) {  // Enter Key is Pressed.
    if (data_count == 20) {  // Check if 20digits token command.
      Serial2.println("# Token Entered\n");
      sts_data = Data;
      check_tokenused();
      if (token_used == 0) {
        STStoken_decode();
      }
    } else if (data_count == 3) {  // Check if 3digits command.
      Serial2.println("# Command entered\n");
      check_meterCMD();  // Confirm 3 digits command.
      preset_keyprocess();
    } else {  // ERROR, only 20 digits and 3 digits are allowed.
      Serial2.println("Invalid command\n");
      digitalWrite(buzzer, HIGH);
      lcd.setCursor(0, 0);
      lcd.print("INVALID !!!");
      delay(1000);
      lcd.clear();
      digitalWrite(buzzer, LOW);
      preset_keyprocess();
    }
  } else if (customKey != NO_KEY &&
             (0x30 >= customKey <=
              0x39)) {  // Detect Numeric Keys (0-9) to display on LCD.
    if (!sts_mode) {    // If first Key, set screen to display keys pressed.
      dt = 0;
      sts_value = lcd_count = parameters = 0;
      delay(20);
      lcd.clear();
      buss();
      lcd.setCursor(0, 0);
      sts_mode = 1;
    }
    if (customKey) {
      if (dt < 21) {
        delay(20);
        buss();
        dt++;
        if (dt < 17) {
          Data[data_count] = customKey;
          lcd.setCursor(lcd_count, 0);
          lcd.print(Data[data_count]);
          data_count++;
        }
        if (lcd_count > 15) {
          lcd_count = 0;
        }
        if (dt > 16) {
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

    if (data_count < 19 && data_count > 3) {
      if (sts_data == password) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("MODE: ");
        data_count = 0;
        lcd_count = 8;
        dt = 0;
      }
      if (sts_data == "112") {
        Mode_select = 1;
      }

      if (sts_data == "122") {
        Mode_select = 2;
      }
    }
  }
}

void preset_keyprocess() {
  data_count = lcd_count = dt = sts_value = parameters = 0;
  lcd.clear();
  buss();  // emit a sound to inform that it is ready to receive the token
  lcd.setCursor(0, 0);
  sts_mode = 1;
  if (customKeypad.getState() == HOLD) {
    lcd.println("Password: ");
  } else {
    lcd.print("TOKEN: ");
  }
}

void check_meterCMD(void) {
  bool authERR = false;
  lcd.clear();
  if (!is_STSmode && sts_data == "112") {  // Change to STS mode.
    if (passWORD_set) {
      is_STSmode = true;
      lcd.print("STS MODE");
      delay(1000);
      passWORD_set = false;
    } else
      authERR = true;
  } else if (is_STSmode && sts_data == "122") {  // Change to OpenPAYGO mode.
    if (passWORD_set) {
      is_STSmode = false;
      lcd.print("OPENPAYGO MODE");
      delay(1000);
      passWORD_set = false;
    } else
      authERR = true;
  }
  // else if( sts_data == /* custom command*/)
  // {
  //
  // }
  else {
    digitalWrite(buzzer, HIGH);
    lcd.print("INVALID COMMAND");
    delay(1000);
    digitalWrite(buzzer, LOW);
  }

  if (authERR == true) {
    digitalWrite(buzzer, HIGH);
    lcd.print("PASSWORD REQUIRED");
    delay(1000);
    digitalWrite(buzzer, LOW);
  }
}
