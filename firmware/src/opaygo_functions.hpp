#pragma once

// OpenSmartMeter libraries
#include "global_defines.hpp"
#include "helpers.hpp"
#include "mem_init.hpp"
#include "time_management.hpp"

// Arduino base libraries
#include <inttypes.h>
#include "Arduino.h"
// #include <time.h>

extern "C" {
#include "opaygo_decoder.h"
}

#define BLINK_PERIOD 250
#define DEBUG

#define STAR_KEY -1
#define HASH_KEY -2
#define NON_ACCEPTED_KEY -3

#define TIME_DIVIDER 1
// #define RESTRICTED_DIGIT_SET_MODE

#ifdef RESTRICTED_DIGIT_SET_MODE
#  define TOKEN_LENGTH 15
#else
#  define TOKEN_LENGTH 9
#endif

// Device parameters location in Flash/EEPROM
unsigned int TokenCount_eeprom_location = 6;
unsigned int UsedTokens_eeprom_location = 8;
unsigned int PAYGEnabled_eeprom_location = 10;
unsigned int ActiveUntil_eeprom_location = 11;
unsigned int TokenEntryLockedUntil_eeprom_location = 15;
unsigned int LAST_TIME_STAMP_ADDRESS = 27;
unsigned int NB_DISCONNECTIONS_ADDRESS = 32;

// Device parameters (to be stored in Flash/EEPROM)
uint16_t TokenCount = 1;
uint16_t UsedTokens = 0;
bool PAYGEnabled = true;
uint32_t ActiveUntil = 0;
uint32_t TokenEntryLockedUntil = 0;
uint8_t nbDisconnections = 0;

int InvalidTokenCount = 0;

void LoadActivationVariables() {
  TokenCount =
      mem.readInt(TokenCount_eeprom_location);  // We load TokenCount (&
                                                // UsedTokens if needed)
  UsedTokens =
      mem.readInt(UsedTokens_eeprom_location);  // We load UsedTokens if needed
  switch (mem.read(PAYGEnabled_eeprom_location)) {
    case 0:
      PAYGEnabled = false;
      break;
    case 1:
      PAYGEnabled = true;
      break;
  }

  mem.read(
      PAYGEnabled_eeprom_location);  // We load PAYGEnabled //Verify this syntax
  TokenEntryLockedUntil = mem.readLong(
      TokenEntryLockedUntil_eeprom_location);  // We load TokenEntryLockedUntil
  if (Mode_select == 2) {
    creditt =
        mem.readLong(credit_eeprom_location);  // We load creditt in case of
                                               // OpenPaygo Energy-based
  } else {
    ActiveUntil =
        mem.readLong(ActiveUntil_eeprom_location);  // We load ActiveUntil
  }
}

void StoreActivationVariables() {
  mem.writeInt(TokenCount_eeprom_location,
               TokenCount);  // We store TokenCount (& UsedTokens if needed)
  mem.writeInt(UsedTokens_eeprom_location,
               UsedTokens);  // We store UsedTokens if needed
  if (PAYGEnabled) {
    mem.writeInt(PAYGEnabled_eeprom_location, 1);  // We store PAYGEnabled
  } else {
    mem.writeInt(PAYGEnabled_eeprom_location, 0);  // We store PAYGEnabled
  }

  mem.writeLong(TokenEntryLockedUntil_eeprom_location,
                TokenEntryLockedUntil);  // We store TokenEntryLockedUntil
  if (Mode_select == 2) {
    mem.writeLong(
        credit_eeprom_location,
        creditt);  // We Store creditt in case of OpenPaygo Energy-based
  } else {
    mem.writeLong(ActiveUntil_eeprom_location,
                  ActiveUntil);  // We store ActiveUntil
  }
}

void storeTimeStampEEPROM(uint32_t timeStampInSeconds) {
  mem.writeInt(LAST_TIME_STAMP_ADDRESS, timeStampInSeconds);
}

void storeNbDisconnectionsEEPROM() {
  mem.writeInt(NB_DISCONNECTIONS_ADDRESS, nbDisconnections);
}

void incrementNbDisconnectionsEeprom() {
  Serial.println("Disconnection spotted!!");  // will be displayed if DEBUG_MODE
                                              // is uncommented
  nbDisconnections =
      mem.readInt(NB_DISCONNECTIONS_ADDRESS);  // just to be sure we have the
                                               // proper nb of disconnections
  nbDisconnections++;
  storeNbDisconnectionsEEPROM();
}

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

uint32_t GetTimeInSeconds() {
  /*
   *Returns the unixtime in seconds of Now
   */
  DateTime now = rtc.now();
  uint32_t nowInSeconds =
      (now.unixtime() -
       timeInitializationRtc);  // we substract the init time so that it's
                                // easier to read when debugging, and works the
                                // same as Arduino Time mgt
  return (nowInSeconds);
}

bool TokenEntryAllowed() {
  if (TokenEntryLockedUntil > GetTimeInSeconds()) {
    return false;
  } else {
    return true;
  }
}

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

void AddTime(int ActivationValue) {
  uint32_t Now = GetTimeInSeconds();
  int NumberOfSecondsToActivate = (ActivationValue * 3600 * 24) / TIME_DIVIDER;

  if (ActiveUntil < Now) {
    ActiveUntil = Now;
  }

  ActiveUntil +=
      NumberOfSecondsToActivate;  // We add the number of days (converted in
                                  // seconds for to compare to our RTC time)
}

void SetTime(int ActivationValue) {
  uint32_t Now = GetTimeInSeconds();
  int NumberOfSecondsToActivate = (ActivationValue * 3600 * 24) / TIME_DIVIDER;

  ActiveUntil =
      Now +
      NumberOfSecondsToActivate;  // We set the number of days (converted in
                                  // seconds for to compare to our RTC time)
}

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
        if (Mode_select == 2) {
          SetCreditt(TokenValue);
        } else {
          SetTime(TokenValue);
        }
      } else {
        if (Mode_select == 2) {
          AddCreditt(TokenValue);
        } else {
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
        if (Mode_select == 2) {  //
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
        } else {
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

void initializeTime() {
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1)
      ;
  }

  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
  }

  // We check that it is not a disconnection power - Arduino
  uint32_t nvramCheck = readUint32FromNvram(TIME_INITIALIZATION_NVRAM_ADDRESS);
  if (nvramCheck == 0) {  // it is the first setup of the Arduino
    rtc.adjust(
        DateTime(F(__DATE__), F(__TIME__)));  // sets the RTC to the date & time
                                              // this sketch was compiled
    DateTime now = rtc.now();
    timeInitializationRtc = now.unixtime();
    storeUint32InNvram(timeInitializationRtc,
                       TIME_INITIALIZATION_NVRAM_ADDRESS);
  } else {  // a disconnection happened
    timeInitializationRtc = nvramCheck;
    incrementNbDisconnectionsEeprom();
    storeTimeStampEEPROM(
        GetTimeInSeconds());  // to avoid counting twice the disconnection if
                              // the previous TimeStamp was too long ago
  }
}
