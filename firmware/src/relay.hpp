#ifndef relay_hpp
#define relay_hpp

// defines

// Arduino base libraries

// third party libraries

// OpenSmartMeter libraries
#include "global_defines.hpp"

byte off_exec = 0;
byte on_exec = 0;

void relay_on() {
  if (on_exec == 0) {
    digitalWrite(relaya, HIGH);
    digitalWrite(relayb, LOW);
    delay(200);
    digitalWrite(relaya, LOW);
    digitalWrite(relayb, LOW);
    on_exec = 1;
    off_exec = 0;
  }
}

void relay_off() {
  if (off_exec == 0) {
    digitalWrite(relaya, LOW);
    digitalWrite(relayb, HIGH);
    delay(200);
    digitalWrite(relaya, LOW);
    digitalWrite(relayb, LOW);
    off_exec = 1;
    on_exec = 0;
  }
}

#endif