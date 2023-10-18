#pragma once

// defines

// Arduino base libraries

// third party libraries

// OpenSmartMeter libraries
#include "mem_operation.hpp"
#include "power.hpp"
#include "relay.hpp"

float billing = 0.0;
byte fault = 0;

void credit_formular() {
  if (true_power > 0) {
    ENERGY = ENERGY + ((true_power) /
                       (3600000));  // energy per KWH(energy gotten per sec)
    billing = (true_power * tariff) / 3600000;  // cost per sec
    if (creditt > billing) {
      creditt = (creditt - billing);
    }
  }
}

void credit_reminder() {
  if (creditt > 1 && creditt < 200) {
    digitalWrite(red_led, HIGH);
    digitalWrite(green_led, LOW);
    delay(80);
    digitalWrite(red_led, LOW);
    delay(80);
  }
  if (creditt < 1 || fault == 1) {
    digitalWrite(red_led, HIGH);
    relay_off();
  }
  if (creditt > 200 && fault == 0) {
    digitalWrite(red_led, LOW);
    relay_on();
    digitalWrite(green_led, HIGH);
  }
}

// For Opaygo

void AddCreditt(int ActivationValue) {
  mem.writeLong(credit_eeprom_location, creditt);  // write present credit
  creditt = mem.readLong(credit_eeprom_location);  // fetch previous credit //
  // add new if any to old credit //
  creditt += ActivationValue;
  mem.writeLong(credit_eeprom_location, creditt);
  get_credit = 1;
}

void SetCreditt(int ActivationValue) {
  creditt = ActivationValue;
  mem.writeLong(credit_eeprom_location, creditt);  // write present credit
  creditt = mem.readLong(credit_eeprom_location);  // fetch previous credit //
  // add new if any to old credit //
  creditt = ActivationValue;
  mem.writeLong(credit_eeprom_location, creditt);
  get_credit = 1;
}
