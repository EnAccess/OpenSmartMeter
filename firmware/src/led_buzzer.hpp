#pragma once

// defines

// Arduino base libraries
#include "Arduino.h"

// third party libraries

// OpenSmartMeter libraries
#include "global_defines.hpp"

void ChangeLedState(int ledPin) {
  digitalRead(ledPin) == LOW ? digitalWrite(ledPin, HIGH)
                             : digitalWrite(ledPin, LOW);
}

void BlinkLED(int LedPin, int NumberOfBlinks, int BlinkPeriode) {
  int i;
  for (i = 0; i < NumberOfBlinks; i++) {
    ChangeLedState(LedPin);
    delay(BlinkPeriode);
    ChangeLedState(LedPin);
    delay(BlinkPeriode);
  }
}

void BlinkRedLED(int NumberOfBlinks, int BlinkPeriode) {
  BlinkLED(red_led, NumberOfBlinks, BlinkPeriode);
}

void BlinkGreenLED(int NumberOfBlinks, int BlinkPeriode) {
  BlinkLED(green_led, NumberOfBlinks, BlinkPeriode);
}

void buss() {
  digitalWrite(buzzer, HIGH);
  delay(10);
  digitalWrite(buzzer, LOW);
}
