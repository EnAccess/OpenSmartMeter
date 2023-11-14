#pragma once

// defines

// Arduino base libraries

// third party libraries

// OpenSmartMeter libraries
#include "credit.hpp"
#include "mem_init.hpp"
#include "token_management.hpp"
// #include "helpers.hpp"

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

uint32_t convertArrayToUint32(int arrayToConvert[9]) {
  int i = 0;
  uint32_t uint32 = 0;
  for (i = 0; i < 9; i++) {
    uint32 = 10 * uint32 + arrayToConvert[i];
  }
  return (uint32);
}

uint32_t convertByteArrayToUint32(uint8_t arrayToConvert[4]) {
  uint32_t low1 = arrayToConvert[0];
  uint32_t low2 = arrayToConvert[1];
  low2 = low2 << 8;
  uint32_t high1 = arrayToConvert[2];
  high1 = high1 << 16;
  uint32_t high2 = arrayToConvert[3];
  high2 = high2 << 24;
  uint32_t result = low1 + low2 + high1 + high2;
  return (result);
}

void convertUint32ToUint8Array(uint32_t uint32ToConvert,
                               uint8_t arrayBytes[4]) {
  byte low1 = uint32ToConvert;
  byte low2 = uint32ToConvert >> 8;
  byte high1 = uint32ToConvert >> 16;
  byte high2 = uint32ToConvert >> 24;
  arrayBytes[0] = low1;
  arrayBytes[1] = low2;
  arrayBytes[2] = high1;
  arrayBytes[3] = high2;
}
