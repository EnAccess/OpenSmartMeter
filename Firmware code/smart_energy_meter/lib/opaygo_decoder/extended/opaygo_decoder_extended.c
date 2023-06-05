//
//  opaygo_decoder_extended.c
//  OPAYGO-Unix
//
//  Created by Benjamin David on 17/09/2019.
//  Copyright © 2019 Solaris Offgrid. All rights reserved.
//

#include "opaygo_decoder_extended.h"


int32_t GetActivationValueFromExtendedToken(uint64_t InputToken, uint16_t *MaxCount, uint32_t StartingCode, char SECRET_KEY[16]) {
    
    uint32_t StartingCodeBase = GetTokenBaseExtended(StartingCode);
    uint32_t TokenBase = GetTokenBaseExtended(InputToken);
    uint64_t CurrentToken = PutBaseInTokenExtended(StartingCode, TokenBase);
    uint64_t MaskedToken;
    int MaxCountTry;
    
    int32_t Value = (int32_t)DecodeBaseExtended(StartingCodeBase, TokenBase);
    
    if(Value == COUNTER_SYNC_VALUE) {
        MaxCountTry = *MaxCount + MAX_TOKEN_JUMP_COUNTER_SYNC;
    } else {
        MaxCountTry = *MaxCount + MAX_TOKEN_JUMP;
    }
    
    for (int Count=0; Count <= MaxCountTry; Count++) {
        MaskedToken = PutBaseInTokenExtended(CurrentToken, TokenBase);
        if(MaskedToken == InputToken && Count > *MaxCount) {
            *MaxCount = Count;
            return Value;
        }
        CurrentToken = GenerateOPAYGOTokenExtended(CurrentToken, SECRET_KEY);
    }
    
    return -1;
}
