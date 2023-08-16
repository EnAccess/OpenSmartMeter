// #pragma once
#ifndef credit_hpp
#define credit_hpp
// defines

// Arduino base libraries

// third party libraries

// OpenSmartMeter libraries
#include "power.hpp"
#include "relay.hpp"

float billing = 0.0;
unsigned int tariff = 70;
float creditt = 0.0;
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
#endif
