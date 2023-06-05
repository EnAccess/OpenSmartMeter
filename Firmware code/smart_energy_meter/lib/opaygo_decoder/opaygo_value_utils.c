#include "opaygo_value_utils.h"


int DecodeBase(int StartingCodeBase, int TokenBase) {
    if((TokenBase - StartingCodeBase) < 0) {
        return TokenBase + 1000 - StartingCodeBase;
    } else {
        return TokenBase - StartingCodeBase;
    }
}

int GetTokenBase(uint32_t Token) {
    return (Token % 1000);
}

uint32_t PutBaseInToken(uint32_t Token, int TokenBase) {
    return Token - (Token % 1000) + TokenBase;
}
