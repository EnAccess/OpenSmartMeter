#pragma once

// defines

// Arduino base libraries

// third party libraries

// OpenSmartMeter libraries
#include "lcd_init.hpp"
#include "mem_init.hpp"
#include "time_management.hpp"
#include "token_management.hpp"

int tamper_log = 0;
int tamper_location = 5;

unsigned long lcdtime_now, prev_lcdtime = 0;
unsigned long cnt = 0;
float btt = 0.0;

unsigned long LCD_scroll_time =
    5000;  // 10sec according to Nigeria metering code

void lcd_update() {
  lcdtime_now = millis();
  if (lcdtime_now - prev_lcdtime >= LCD_scroll_time) {
    cnt = cnt + 1;
    prev_lcdtime = lcdtime_now;
    lcd.begin(16, 2);
  }

  // scene 1
  if (cnt == 0) {
    lcd.setCursor(0, 0);
    lcd.print("V=     ");
    lcd.setCursor(2, 0);
    lcd.print(mains_input_value);
    lcd.setCursor(7, 0);
    lcd.print(" ");

    lcd.setCursor(8, 0);
    lcd.print("C=     ");
    lcd.setCursor(10, 0);
    lcd.print(curr);
    lcd.setCursor(15, 0);
    lcd.print("A");

    lcd.setCursor(0, 1);
    lcd.print("W=          ");
    lcd.setCursor(2, 1);
    lcd.print(true_power);
    lcd.setCursor(8, 1);
    lcd.print(" ");
    lcd.setCursor(9, 1);
    lcd.print("PF=        ");
    lcd.setCursor(12, 1);
    lcd.print(powerFactor);
  }

  // scene 2
  if (cnt == 1) {
    lcd.setCursor(0, 0);
    lcd.print(" CREDIT=#       ");
    lcd.setCursor(9, 0);
    lcd.print(creditt);

    lcd.setCursor(0, 1);
    lcd.print("ENERGY=        ");
    lcd.setCursor(7, 1);
    lcd.print(ENERGY);
    lcd.setCursor(12, 1);
    lcd.print("KW/H");
  }
  if (cnt > 1) {
    cnt = 0;
  }
}

void parameters_display() {
    lcd.setCursor(0, 0);
    if (parameters == 1) {
      lcd.print("  Meter number  ");
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(3, 1);
      lcd.print(meter);
    }
    else if (parameters == 2) {
      lcd.print("date/time :     ");
      lcd.setCursor(0, 1);
      lcd.print("                ");

      lcd.setCursor(0, 1);
      lcd.print(" ");
      lcd.setCursor(1, 1);
      lcd.print(rtcday);
      lcd.setCursor(3, 1);
      lcd.print("/");

      lcd.setCursor(4, 1);
      lcd.print(hours);
      lcd.setCursor(6, 1);
      lcd.print(":");

      lcd.setCursor(7, 1);
      lcd.print(minutes);
      lcd.setCursor(9, 1);
      lcd.print(":");

      lcd.setCursor(10, 1);
      lcd.print(seconds);
      lcd.setCursor(12, 1);
      lcd.print("     ");
    }
    else if (parameters == 3) {
      lcd.print("battery level :  ");
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(6, 1);
      lcd.print(btt);
    }
    else if (parameters == 4) {
      lcd.print("TARIFF =        ");
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(6, 1);
      lcd.print(tariff);
    }
    else if (parameters == 5) {
      lcd.print("Fault event :   ");
      lcd.setCursor(0, 1);
      lcd.print("                ");
      tamper_log = mem.readLong(tamper_location);
      lcd.setCursor(6, 1);
      lcd.print(tamper_log);
    }
    else if (parameters == 6) {
      lcd.print("Last month      ");
      lcd.setCursor(0, 1);
      lcd.print("billing date :  ");
      lcd.setCursor(14, 1);
      lcd.print(lastmonth_KWH);
    }
    else if (parameters == 7) {
      lcd.print(" Last month KWH ");
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(6, 1);
      lcd.print(lastmonth_KWH);
      lcd.setCursor(12, 1);
      lcd.print("KW/H");
    }
    if (parameters > 7) {
      parameters = 0;
    }
}
