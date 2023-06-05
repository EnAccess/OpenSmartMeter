#include "device_functions.h"
#include "getch.h"

// Device parameters (to be stored in Flash/EEPROM)
uint16_t TokenCount = 1;
uint16_t UsedTokens = 0;
bool PAYGEnabled = true;
uint32_t ActiveUntil = 0;
uint32_t TokenEntryLockedUntil = 0;

// WARNING: THIS SECRET KEY AND STARTING CODE IS ONLY HERE AS AN EXAMPLE AND SHOULD NEVER BE USED IN PRODUCTION
uint32_t StartingCode = 123456789;
// char SECRET_KEY[16] = {0xa2, 0x9a, 0xb8, 0x2e, 0xdc, 0x5f, 0xbb, 0xc4, 0x1e, 0xc9, 0x53, 0xf, 0x6d, 0xac, 0x86, 0xb1};
char SECRET_KEY[16] = {...};


// This is just for displaying the activation status in the simulater WaitForTokenEntry function
extern bool IsActive(void);

void BlinkRedLED(int NumberOfBlinks) {
    printf("\nRed LED Blinked %d times!\n", NumberOfBlinks);
}

void BlinkGreenLED(int NumberOfBlinks) {
    printf("\nGreen LED Blinked %d times!\n", NumberOfBlinks);
}

int GetKeyPressed() {
    char this_char = getch();
    switch (this_char) {
        case '*':
            return STAR_KEY;
            break;
            
        case '#':
            return HASH_KEY;
            break;
            
        default:
            return (int) (this_char - '0'); // this_char is now an int
            break;
    }
}


void LoadActivationVariables() {
    // We load TokenCount (& UsedTokens if needed)
    // We load PAYGEnabled
    // We load ActiveUntil
    // We load TokenEntryLockedUntil
}

void StoreActivationVariables() {
    // We store TokenCount (& UsedTokens if needed)
    // We store PAYGEnabled
    // We store ActiveUntil
    // We store TokenEntryLockedUntil
}

uint32_t GetTimeInSeconds() {
    return (uint32_t) time(NULL);
}
