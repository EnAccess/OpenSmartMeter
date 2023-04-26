#pragma once
#include "global_defines.hpp"

void STStoken_decode() {
  String keycofirmation = sts_data.substring(0, 3);
  int convertekeycofirmation = keycofirmation.toInt();
  String keycofirmationmultiplier = sts_data.substring(3, 4);
  int convertekeycofirmationmultiplier = keycofirmationmultiplier.toInt();
  confirmkey = convertekeycofirmation / convertekeycofirmationmultiplier;

  String sts_meter_no_count = sts_data.substring(4, 5);
  convertedsts_data = sts_data.toInt();
  unsigned int stsnew_meter_no_count = sts_meter_no_count.toInt();
  // int stsnew_meter_no_count = sts_meter_no_count;
  String sts_day = sts_data.substring(6, 8);
  convertedsts_day = sts_day.toInt();
  String sts_third = sts_data.substring(8, 15);
  String encoder = sts_third.substring(0, 1);
  encodernew = encoder.toInt();
  String identifier = sts_third.substring(6, 7);
  String sts_encode = sts_data.substring(15, 20);
  long tariff_MT_NO = sts_encode.toInt();
  // long tariff_gotten = ((tariff_MT_NO) - (meter_no * multiplier));
  unsigned long trueMT_NO = tariff_MT_NO - tariff;
  trueMT_NO = trueMT_NO / multiplier;

  if ((confirmkey == private_stskey) &&
      (convertedsts_data != eeprom_sts_data) && (trueMT_NO == meter_no) &&
      (meter_no_count == stsnew_meter_no_count)) {
    // to identify 3 digit topup
    if (identifier == "A") {
      String tp = sts_third.substring(1, 4);
      topup = tp.toInt();
      if (encoder == "1") {
        topupnew = topup + 25;
        dcode = (topupnew - topup) / 25;
        if (dcode == encodernew) {
          sts_accept = 1;
        } else if (dcode != encodernew) {
          sts_accept = 0;
        }
      }
      if (encoder == "2") {
        topupnew = topup + 50;
        dcode = (topupnew - topup) / 25;
        if (dcode == encodernew) {
          sts_accept = 1;
        } else if (dcode != encodernew) {
          sts_accept = 0;
        }
      }
      if (encoder == "3") {
        topupnew = topup + 39;
        dcode = (topupnew - topup) / 15;
        if (dcode == encodernew) {
          sts_accept = 1;
        } else if (dcode != encodernew) {
          sts_accept = 0;
        }
      }
    }

    // to identify 4 digit topup
    if (identifier == "C") {
      String tp = sts_third.substring(1, 5);
      topup = tp.toInt();
      if (encoder == "1") {
        topupnew = topup + 15;
        dcode = (topupnew - topup) / 15;
        if (dcode == encodernew) {
          sts_accept = 1;
        } else if (dcode != encodernew) {
          sts_accept = 0;
        }
      }
      if (encoder == "2") {
        topupnew = topup + 36;
        dcode = (topupnew - topup) / 18;
        if (dcode == encodernew) {
          sts_accept = 1;
        } else if (dcode != encodernew) {
          sts_accept = 0;
        }
      }
      if (encoder == "3") {
        topupnew = topup + 39;
        dcode = (topupnew - topup) / 13;
        if (dcode == encodernew) {
          sts_accept = 1;
        } else if (dcode != encodernew) {
          sts_accept = 0;
        }
      }
    }
    // to identify 5 digit topup
    if (identifier == "B") {
      String tp = sts_third.substring(1, 6);
      topup = tp.toInt();
      if (encoder == "1") {
        topupnew = topup + 99;
        dcode = (topupnew - topup) / 99;
        if (dcode == encodernew) {
          sts_accept = 1;
        } else if (dcode != encodernew) {
          sts_accept = 0;
        }
      }
      if (encoder == "2") {
        topupnew = topup + 70;
        dcode = (topupnew - topup) / 35;
        if (dcode == encodernew) {
          sts_accept = 1;
        } else if (dcode != encodernew) {
          sts_accept = 0;
        }
      }
      if (encoder == "3") {
        topupnew = topup + 60;
        dcode = (topupnew - topup) / 20;
        if (dcode == encodernew) {
          sts_accept = 1;
        } else if (dcode != encodernew) {
          sts_accept = 0;
        }
      }
    }

    if (sts_accept == 1) {
      if (c_chek == 0) {
        warntime = warn_now;
        c_chek = 1;
      }
      lcd.setCursor(0, 0);
      lcd.print(" TOKEN ACCEPTED ");
      lcd.setCursor(0, 1);
      lcd.print("   VIA KEYPAD   ");
      topup = topupnew;
      mem.writeLong(credit_eeprom_location, creditt);  // write present credit
      creditt =
          mem.readLong(credit_eeprom_location);  // fetch previous credit //
      creditt = creditt + topup;  // add new   if any to old credit //
      mem.writeLong(credit_eeprom_location, creditt);
      get_credit = 1;
    }
  }

  if (sts_accept == 0) {
    if (c_chek == 0) {
      warntime = warn_now;
      c_chek = 1;
    }
    lcd.setCursor(0, 0);
    lcd.print("  WARNING!!!!!  ");
    lcd.setCursor(0, 1);
    lcd.print("  INVALID TOKEN ");
    digitalWrite(buzzer, HIGH);
  }
}

