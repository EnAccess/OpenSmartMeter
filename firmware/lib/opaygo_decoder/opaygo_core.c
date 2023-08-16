#include "opaygo_core.h"
#include "siphash.h"


uint32_t extractBits(uint32_t source, unsigned from, unsigned to)
{
    uint32_t  mask = ( (1<<(to-from+1))-1) << from;
    return (source & mask) >> from;
}

uint32_t ConvertHashToToken(uint64_t this_hash) {
    // We split the hash in two parts
    uint32_t hi_hash, lo_hash;
    hi_hash = this_hash >> 32;
    lo_hash = this_hash & 0xFFFFFFFF;

    // We XOR the two halves together
    hi_hash ^= lo_hash;

    // We reduce it to 30 bits
    uint32_t result = extractBits(hi_hash, 2, 32);
    // We reduce it to 29.5bits
    if(result > 999999999) {
        result = result - 73741825;
    }
    return result;
}

uint32_t GenerateOPAYGOToken(uint32_t LastToken, unsigned char SECRET_KEY[16]) {
    uint8_t a[8];

    a[0] = LastToken >> 24;
    a[1] = LastToken >> 16;
    a[2] = LastToken >>  8;
    a[3] = LastToken;
    a[4] = LastToken >> 24;
    a[5] = LastToken >> 16;
    a[6] = LastToken >>  8;
    a[7] = LastToken;

    uint64_t ThisHash = siphash24(a, 8, SECRET_KEY);

    // We return the conformed token
    return ConvertHashToToken(ThisHash);
}
