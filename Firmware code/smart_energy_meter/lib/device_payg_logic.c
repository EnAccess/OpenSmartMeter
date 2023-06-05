#include "device_payg_logic.h"

int InvalidTokenCount = 0;

void UpdateDeviceStatusFromTokenValue(int TokenValue, int ActivationCount) {
    if(TokenValue == -1) {
        InvalidTokenCount++;
        UpdateInvalidTokenWaitingPeriod();
        BlinkRedLED(10);
    } else if(TokenValue == -2) {
        BlinkGreenLED(1); // We blink the green LED once to show that the token was valid but isnt anymore
    } else {
        InvalidTokenCount = 0;
        if(TokenValue == COUNTER_SYNC_VALUE) {
            BlinkGreenLED(3); // We blink green twice to show that the token is good
        } else if(TokenValue == PAYG_DISABLE_VALUE) {
            PAYGEnabled = false;
            BlinkGreenLED(5); // We blink green twice to show that the device is active forever
        } else {
            if(ActivationCount % 2) {
                PAYGEnabled = true;
                SetTime(TokenValue);
            } else {
                AddTime(TokenValue);
            }
            BlinkGreenLED(2); // We blink green twice to show that the token is good
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
