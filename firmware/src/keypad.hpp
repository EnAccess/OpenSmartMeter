#pragma once

// defines
#define Lengths 20

// Arduino base libraries

// third party libraries
#include <Keypad.h>

// OpenSmartMeter libraries
#include "led_buzzer.hpp"

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
      lcd.println("Password: ");
    } else {
      lcd.print("TOKEN: ");
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
        Mode_select = 1;  // For STS Mode (Energy-based)
      }

      if (sts_data == "122") {
        Mode_select = 2;  // For OpenPaygo Energy-based
      }

      if (sts_data == "132") {
        Mode_select = 3;  // For OpenPaygo Time-based
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
