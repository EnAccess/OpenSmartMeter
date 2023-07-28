#pragma once

// OpenSmartMeter libraries
//#include "global_defines.hpp"
#include "mem_init.hpp"
//#include "token_management.hpp"

// Arduino base libraries
#include "Arduino.h"
// Device parameters location in Flash/EEPROM
unsigned int TokenCount_eeprom_location = 6;
unsigned int UsedTokens_eeprom_location = 8;
unsigned int PAYGEnabled_eeprom_location = 10;
unsigned int ActiveUntil_eeprom_location = 11;
unsigned int TokenEntryLockedUntil_eeprom_location = 15;

// Device parameters (to be stored in Flash/EEPROM)
uint16_t TokenCount = 1;
uint16_t UsedTokens = 0;
bool PAYGEnabled = true;
uint32_t ActiveUntil = 0;
uint32_t TokenEntryLockedUntil = 0;

/*uint32_t StartingCode = 123456789;
char SECRET_KEY[16] = {0xa2, 0x9a, 0xb8, 0x2e, 0xdc, 0x5f, 0xbb, 0xc4, 0x1e, 0xc9, 0x53, 0xf, 0x6d, 0xac, 0x86, 0xb1};
*/

void LoadActivationVariables() {
    TokenCount = mem.readInt(TokenCount_eeprom_location);// We load TokenCount (& UsedTokens if needed)
    UsedTokens = mem.readInt(UsedTokens_eeprom_location);//We load UsedTokens if needed 
    PAYGEnabled = mem.read(PAYGEnabled_eeprom_location);// We load PAYGEnabled //Verify this syntax
    ActiveUntil = mem.readLong(ActiveUntil_eeprom_location);// We load ActiveUntil
    TokenEntryLockedUntil = mem.readLong(TokenEntryLockedUntil_eeprom_location);// We load TokenEntryLockedUntil
}

void StoreActivationVariables() {
    mem.writeInt(TokenCount_eeprom_location,TokenCount);// We store TokenCount (& UsedTokens if needed)
    mem.writeInt(UsedTokens_eeprom_location, UsedTokens);//We store UsedTokens if needed 
    mem.write(PAYGEnabled_eeprom_location, PAYGEnabled);// We store PAYGEnabled //Verify this syntax
    mem.writeLong(ActiveUntil_eeprom_location,ActiveUntil);// We store ActiveUntil
    mem.writeLong(TokenEntryLockedUntil_eeprom_location,TokenEntryLockedUntil);// We store TokenEntryLockedUntil
}
