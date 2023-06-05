#include "restricted_digit_set_mode.h"

void StoreNBitsInArray(bool* BooleanArray, uint64_t Data, uint8_t NumberOfBits, uint8_t BufferStartBit)
{
    for (int i = 0; i < NumberOfBits; i++)
    {
        BooleanArray[i+BufferStartBit] = !!(Data & (1 << ((NumberOfBits - 1 - i)))); ;
    }
}

uint32_t GetINTFromBooleanArray(bool* BooleanArray, uint8_t StartPosition, uint8_t Lenght)
{
    uint32_t Buffer = 0;
    for (int i = StartPosition; i < StartPosition+Lenght; i++) {
        Buffer = (Buffer << 1) + (BooleanArray[i] ? 1 : 0);
    }
    return Buffer;
}

uint32_t ConvertFromFourDigitToken(uint64_t FourDigitToken) {
    // This is made for 15 digit long codes converting to 9.
    // For other lengths the code needs to be adapted
    bool TokenBooleanArray[30] = {0};
    uint8_t ThisDigit;
    
    for(int i = 0; i < 15; i++) {
        ThisDigit = (FourDigitToken % 10) - 1;
        StoreNBitsInArray(TokenBooleanArray, ThisDigit, 2, (14-i)*2);
        FourDigitToken /= 10;
    }
    
    return GetINTFromBooleanArray(TokenBooleanArray, 0, 30);
}
