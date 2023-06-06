#pragma once

// defines

// Arduino base libraries

// third party libraries

// OpenSmartMeter libraries
#include "credit.hpp"
#include "lcd_display.hpp"
#include "power.hpp"
#include "sts_token.hpp"
#include "token_management.hpp"

float pulsetime_now = 0.0;
float prev_pulsetime = 0.0;

byte token_ok = 0;

float btValue = 0.0;

byte fault_written = 0;

unsigned int over_voltage = 250;
unsigned int over_load = 25000;
unsigned int low_voltage = 150;
unsigned int low_freq = 45;

char selectKey;
bool is_STSmode = false;  // by default on OpenPAYGO Token

void urgeent() {
  if (true_power < 15) {
    true_power = 0;
  }

  pulsetime_now = millis();
  if (pulsetime_now - prev_pulsetime > 1000) {
    prev_pulsetime = pulsetime_now;
    credit_formular();
  }

  STS_keypad();
  warn_now = millis();
  if (c_chek == 1) {
    if (warn_now - warntime > 1500) {
      sts_data = "";
      sts_mode = 0;
      data_count = 0;
      sts_value = 0;
      sts_accept = 0;
      topupnew = 0;
      topup = 0;
      c_chek = 0;
      token_ok = 0;
      token_used = 0;
      warntime = warn_now;
      digitalWrite(buzzer, LOW);
    }
  }
  if (parameters > 0) {
    parameters_display();
  }
  if ((sts_mode == 0) && (parameters == 0) && (btt > 2) && (tp_fetch == 0)) {
    lcd_update();
  }

  btValue = analogRead(battery_input);
  btt = btValue * 0.07331280915044247787610619469027;
  if (btt < 2) {
    fault = 1;
    if (fault_written == 0) {
      mem.writeLong(tamper_location, 03);
      fault_written = 1;
    }
    lcd.setCursor(0, 0);
    lcd.print("TERMINAL  COVER ");
    lcd.setCursor(0, 1);
    lcd.print("      OPEN      ");
    digitalWrite(buzzer, HIGH);
    delay(20);
    digitalWrite(buzzer, LOW);
  }
  if (btt > 2) {
    fault = 0;
    fault_written = 0;
  }
  if (mains_input_value > 50) {
    if ((freq < low_freq) || (mains_input_value > over_voltage) ||
        (mains_input_value < low_voltage) || (true_power > over_load)) {
      fault = 1;
    }
    if (((freq > low_freq) || (mains_input_value < over_voltage) ||
         (mains_input_value > low_voltage) || (true_power < over_load)) &&
        (btt > 2)) {
      fault = 0;
    }
  }
}

void select_mode() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SELECT MODE:\n\n 0: PAYGO TOKEN MODE \n\n 1: STS MODE");
  selectKey = customKeypad.getKey();
  while ((selectKey = !'0') && (selectKey = !'1')) {
    selectKey = customKeypad.getKey();
    delay(100);
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  if (selectKey == '0') {
    lcd.print("OpenPAYGO Mode");
    is_STSmode = false;
  }

  if (selectKey == '1') {
    lcd.print("STS Mode");
    is_STSmode = true;
  }
  delay(2000);
}
