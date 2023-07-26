//
//  opaygo_core_extended.c
//  OPAYGO-Unix
//
//  Created by Benjamin David on 17/09/2019.
//  Copyright © 2019 Solaris Offgrid. All rights reserved.
//

#include "opaygo_core_extended.h"

#include "opaygo_core.h"
#include "siphash.h"


uint64_t extractBitsExtended(uint64_t source, unsigned from, unsigned to)
{
    unsigned long long mask = ( (1ull<<(to-from+1ull))-1ull) << from;
    return (source & mask) >> 24;
}

uint64_t ConvertHashToTokenExtended(uint64_t this_hash) {
    // We reduce it to 40 bits
    uint64_t result = extractBitsExtended(this_hash, 24, 64);
    // We reduce it to 39.5bits
    if(result > 999999999999) {
        result = result - 99511627777;
    }
    return result;
}

uint64_t GenerateOPAYGOTokenExtended(uint64_t LastToken, char SECRET_KEY[16]) {
    
    uint8_t a[8];
    
    a[0] = LastToken >> 56;
    a[1] = LastToken >> 48;
    a[2] = LastToken >> 40;
    a[3] = LastToken >> 32;
    a[4] = LastToken >> 24;
    a[5] = LastToken >> 16;
    a[6] = LastToken >>  8;
    a[7] = LastToken;
    
    uint64_t ThisHash = siphash24(a, 8, SECRET_KEY);
    
    // We return the conformed token
    return ConvertHashToTokenExtended(ThisHash);
}

uint32_t DecodeBaseExtended(uint32_t StartingCodeBase, uint32_t TokenBase) {
    if(TokenBase < StartingCodeBase) {
        return TokenBase + 1000000 - StartingCodeBase;
    } else {
        return TokenBase - StartingCodeBase;
    }
}

uint32_t GetTokenBaseExtended(uint64_t Token) {
    return (Token % 1000000);
}

uint64_t PutBaseInTokenExtended(uint64_t Token, uint32_t TokenBase) {
    return Token - (Token % 1000000) + TokenBase;
}
