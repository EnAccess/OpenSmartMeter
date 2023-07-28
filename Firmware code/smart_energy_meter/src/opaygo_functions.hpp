#pragma once

// OpenSmartMeter libraries
#include "global_defines.hpp"
#include "mem_init.hpp"
//#include "token_management.hpp"

// Arduino base libraries
#include "Arduino.h"

#define BLINK_PERIOD 250
//#define DEBUG

#define STAR_KEY -1
#define HASH_KEY -2
#define NON_ACCEPTED_KEY -3

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

void ChangeLedState(int ledPin)
{
    digitalRead(ledPin) == LOW ? digitalWrite(ledPin, HIGH) : digitalWrite(ledPin, LOW);
}

void BlinkLED(int LedPin, int NumberOfBlinks, int BlinkPeriode) {
    int i;
    for (i = 0; i < NumberOfBlinks; i++)
    {
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


uint64_t WaitForTokenEntry() {
    uint64_t TempToken = 0;
    bool NoToken = true;
    int LastKey;
    
    while(NoToken) {
        LastKey = GetKeyPressed();
        if(LastKey == STAR_KEY) {
            if(TokenEntryAllowed()) {
                NoToken = false;
            } else {
                BlinkRedLED(1, BLINK_PERIOD);
                #ifdef DEBUG
                printf("\nToken entry locked for %d seconds", TokenEntryLockedUntil-GetTimeInSeconds());
                #endif
            }
        } else if(LastKey == HASH_KEY) {
            if(IsActive()) {
                BlinkGreenLED(1, BLINK_PERIOD);
                #ifdef DEBUG
                printf("\nTime Left: %d seconds", ActiveUntil-GetTimeInSeconds());
                #endif
            } else {
                BlinkRedLED(1, BLINK_PERIOD);
            }
        }
    }
    for(int i=0; i<TOKEN_LENGTH; i++) {
        // We add the last key pressed to the token (as integer) if needed
        TempToken += GetKeyPressed()*pow(10, (TOKEN_LENGTH-1)-i);
    }
    return TempToken;
}

void UpdateDeviceStatusFromTokenValue(int TokenValue, int ActivationCount) {
    if(TokenValue == -1) {
        InvalidTokenCount++;
        UpdateInvalidTokenWaitingPeriod();
        BlinkRedLED(10, BLINK_PERIOD);
    } else if(TokenValue == -2) {
        BlinkGreenLED(1, BLINK_PERIOD); // We blink the green LED once to show that the token was valid but isnt anymore
    } else {
        InvalidTokenCount = 0;
        if(TokenValue == COUNTER_SYNC_VALUE) {
            BlinkGreenLED(3, BLINK_PERIOD); // We blink green twice to show that the token is good
        } else if(TokenValue == PAYG_DISABLE_VALUE) {
            PAYGEnabled = false;
            BlinkGreenLED(5, BLINK_PERIOD); // We blink green twice to show that the device is active forever
        } else {
            if(ActivationCount % 2) {
                PAYGEnabled = true;
                SetTime(TokenValue);
            } else {
                AddTime(TokenValue);
            }
            BlinkGreenLED(2, BLINK_PERIOD); // We blink green twice to show that the token is good
        }
        StoreActivationVariables(); // We store in every case
    }
}


void UpdateInvalidTokenWaitingPeriod() {
    uint32_t Now = GetTimeInSeconds();
    
    // We check that it does not become unbearably long
    if(InvalidTokenCount > 11) {
        InvalidTokenCount = 11;
    }

    // We add some forgiveness for the first 2 errors
    if(InvalidTokenCount > 2) {
        TokenEntryLockedUntil = Now + pow(2, InvalidTokenCount-2)*60;
    }
}

bool TokenEntryAllowed() {
    if(TokenEntryLockedUntil > GetTimeInSeconds()) {
        return false;
    } else {
        return true;
    }
}


void AddTime(int ActivationValue) {
    uint32_t Now = GetTimeInSeconds();
    int NumberOfSecondsToActivate = (ActivationValue*3600*24) / TIME_DIVIDER;
    
    if(ActiveUntil < Now) {
        ActiveUntil = Now;
    }
    
    ActiveUntil += NumberOfSecondsToActivate; // We add the number of days (converted in seconds for to compare to our RTC time)
}

void SetTime(int ActivationValue) {
    uint32_t Now = GetTimeInSeconds();
    int NumberOfSecondsToActivate = (ActivationValue*3600*24) / TIME_DIVIDER;
    
    ActiveUntil = Now + NumberOfSecondsToActivate; // We set the number of days (converted in seconds for to compare to our RTC time)
}

bool IsActive() {
    if(PAYGEnabled) {
        if(ActiveUntil > GetTimeInSeconds()) {
            return true;
        } else {
            return false;
        }
    } else {
        return true;
    }
}


