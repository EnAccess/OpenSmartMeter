#pragma once

// defines

// Arduino base libraries
#include "Arduino.h"

// third party libraries

// OpenSmartMeter libraries
#include "led_buzzer.hpp"
#include "mem_operation.hpp"
#include "time_management.hpp"
extern "C" {
#include "opaygo_decoder.h"
}

#define BLINK_PERIOD 250
#define DEBUG
// #define RESTRICTED_DIGIT_SET_MODE

#ifdef RESTRICTED_DIGIT_SET_MODE
#  define TOKEN_LENGTH 15
#else
#  define TOKEN_LENGTH 9
#endif

int InvalidTokenCount = 0;

bool TokenEntryAllowed() {
  if (TokenEntryLockedUntil > GetTimeInSeconds()) {
    return false;
  } else {
    return true;
  }
}

void UpdateInvalidTokenWaitingPeriod() {
  uint32_t Now = GetTimeInSeconds();

  // We check that it does not become unbearably long
  if (InvalidTokenCount > 11) {
    InvalidTokenCount = 11;
  }

  // We add some forgiveness for the first 2 errors
  if (InvalidTokenCount > 2) {
    TokenEntryLockedUntil = Now + pow(2, InvalidTokenCount - 2) * 60;
  }
}

void UpdateDeviceStatusFromTokenValue(int TokenValue, int ActivationCount) {
  // TokenValue in case of OpenPaygo Energy-based is consider directly as a
  // credit to add or set and in case of OpenPaygo Time-based it is consider as
  // a number of day to add or to set
  if (TokenValue == -1) {
    InvalidTokenCount++;
    UpdateInvalidTokenWaitingPeriod();
    BlinkRedLED(10, BLINK_PERIOD);
  } else if (TokenValue == -2) {
    BlinkGreenLED(1, BLINK_PERIOD);  // We blink the green LED once to show that
                                     // the token was valid but isnt anymore
  } else {
    InvalidTokenCount = 0;
    if (TokenValue == COUNTER_SYNC_VALUE) {
      BlinkGreenLED(
          3,
          BLINK_PERIOD);  // We blink green twice to show that the token is good
    } else if (TokenValue == PAYG_DISABLE_VALUE) {
      PAYGEnabled = false;
      BlinkGreenLED(5, BLINK_PERIOD);  // We blink green twice to show that the
                                       // device is active forever
    } else {
      if (ActivationCount % 2) {
        PAYGEnabled = true;
        if (Mode_select == 2) {  // Module in OpenPaygo Energy-based
          SetCreditt(TokenValue);
        } else {  // Module in OpenPaygo Time-based
          SetTime(TokenValue);
        }
      } else {
        if (Mode_select == 2) {  // Module in OpenPaygo Energy-based
          AddCreditt(TokenValue);
        } else {  // Module in OpenPaygo Time-based
          AddTime(TokenValue);
        }
      }
      BlinkGreenLED(
          2,
          BLINK_PERIOD);  // We blink green twice to show that the token is good
    }
    StoreActivationVariables();  // We store in every case
  }
}

bool IsActive() {
  if (PAYGEnabled) {
    if (Mode_select == 2) {
      if (creditt < 1 || fault == 1) {
        return false;
      } else {
        return true;
      }
    }

    if (Mode_select == 3) {
      if (ActiveUntil > GetTimeInSeconds()) {
        return true;
      } else {
        return false;
      }
    }
  }
  return true;
}

uint64_t WaitForTokenEntry() {
  uint64_t TempToken = 0;
  bool NoToken = true;
  int LastKey;

  while (NoToken) {
    LastKey = GetKeyPressed();
    if (LastKey == STAR_KEY) {
      if (TokenEntryAllowed()) {
        NoToken = false;
      } else {
        BlinkRedLED(1, BLINK_PERIOD);
#ifdef DEBUG
        printf("\nToken entry locked for %" PRIu32 "seconds",
               TokenEntryLockedUntil - GetTimeInSeconds());
#endif
      }
    } else if (LastKey == HASH_KEY) {
      if (IsActive()) {
        BlinkGreenLED(1, BLINK_PERIOD);
#ifdef DEBUG
        if (Mode_select == 2) {  // Module in OpenPaygo Energy-based
          mesure();
          if ((mains_input_value > 50)) {
            credit_reminder();
          }
          if ((mains_input_value < 50)) {
            digitalWrite(red_led, LOW);
            digitalWrite(green_led, LOW);
          }
          urgeent();
          printf("\nEnergy Left: %f KWH", ENERGY);
        } else {  // Module in OpenPaygo Time-based
          printf("\nTime Left: %" PRIu32 "seconds",
                 ActiveUntil - GetTimeInSeconds());
        }

#endif
      } else {
        BlinkRedLED(1, BLINK_PERIOD);
      }
    }
  }
  for (int i = 0; i < TOKEN_LENGTH; i++) {
    // We add the last key pressed to the token (as integer) if needed
    TempToken += GetKeyPressed() * pow(10, (TOKEN_LENGTH - 1) - i);
  }
  return TempToken;
}
