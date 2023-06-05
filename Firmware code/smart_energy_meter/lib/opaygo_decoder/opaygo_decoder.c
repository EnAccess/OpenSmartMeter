#include "opaygo_decoder.h"

#define CHECK_BIT(variable, position) ((variable) & (1<<(position)))
#define SET_BIT(variable,position,value) (variable & ~(1<<position)) | (value<<position)


bool IsInUsedCounts(int Count, uint16_t MaxCount, uint16_t UsedCounts) {
    uint16_t RelativeCount = MaxCount - Count - 1;
    if(Count % 2 && Count <= MaxCount) {
        return true;
    }
    if(CHECK_BIT(UsedCounts, RelativeCount) || Count == MaxCount) {
        return true;
    }
    return false;
}

bool IsCountValid(int Count, uint16_t MaxCount, int Value, uint16_t UsedCounts) {
    if(Value == COUNTER_SYNC_VALUE) {
        // We don't need to check if it's below the max as those tokens aren't tried anyway
        if (Count > MaxCount - MAX_TOKEN_JUMP) {
            return true;
        }
    } else if (Count > MaxCount) {
        return true;
    } else if(MAX_UNUSED_OLDER_TOKENS > 0 && Count != 0) {
        // We check that the count is in the range for unused older token or above
        if (Count > MaxCount - MAX_UNUSED_OLDER_TOKENS) {
            // If it is, we check that its ADD_CREDIT and that this count was not used
            if (Count % 2 == 0 && !IsInUsedCounts(Count, MaxCount, UsedCounts)) {
                return true;
            }
        }
    }
    return false;
}

void MarkCountAsUsed(int Count, uint16_t *MaxCount, uint16_t *UsedCounts, int Value) {
    uint16_t NewUsedCount = 0;
    if(Count % 2 || Value == COUNTER_SYNC_VALUE || Value == PAYG_DISABLE_VALUE) {
        // If it was a SET_CREDIT token or it was Counter Sync then we mark all past tokens as used
        for(int i=0; i < MAX_UNUSED_OLDER_TOKENS; i++) {
            *UsedCounts = SET_BIT(*UsedCounts, i, 1);
        }
    } else {
        if(Count > *MaxCount) {
            uint16_t RelativeCount = Count - *MaxCount;
            if(RelativeCount > MAX_UNUSED_OLDER_TOKENS) {
                // We cannot mark more tokens as used than the max allowed older tokens
                RelativeCount = MAX_UNUSED_OLDER_TOKENS;
            } else {
                NewUsedCount = SET_BIT(NewUsedCount, (RelativeCount-1), 1);
            }
            // We move all of the tokens marked as used to their new position relative to the new max count
            for(int i=RelativeCount+1; i < MAX_UNUSED_OLDER_TOKENS - RelativeCount; i++) {
                NewUsedCount = SET_BIT(NewUsedCount, i, CHECK_BIT(*UsedCounts, i-RelativeCount));
            }
            *UsedCounts = NewUsedCount;
        } else {
            // If the token is older than the max one, we just mark this one as used but don't shift any other tokens
            *UsedCounts = SET_BIT(*UsedCounts, (*MaxCount - Count - 1), 1);
        }
    }
    if(Count > *MaxCount) {
        *MaxCount = Count;
    }
}


TokenData GetDataFromToken(uint64_t InputToken, uint16_t *MaxCount, uint16_t *UsedCounts, uint32_t StartingCode, unsigned char SECRET_KEY[16]) {

#ifdef RESTRICTED_DIGIT_SET_MODE
    InputToken = ConvertFromFourDigitToken(InputToken);
#endif
    uint16_t StartingCodeBase = GetTokenBase(StartingCode);
    uint16_t TokenBase = GetTokenBase((uint32_t)InputToken);
    uint32_t CurrentToken = PutBaseInToken(StartingCode, TokenBase);
    uint32_t MaskedToken;
    int MaxCountTry;
    int MinCountTry;
    int Value = DecodeBase(StartingCodeBase, TokenBase);
    TokenData output;
    bool ValidOlderToken = false;

    if(Value == COUNTER_SYNC_VALUE) {
        MaxCountTry = *MaxCount + MAX_TOKEN_JUMP_COUNTER_SYNC;
    } else {
        MaxCountTry = *MaxCount + MAX_TOKEN_JUMP;
    }

    for (int Count=0; Count <= MaxCountTry; Count++) {
        MaskedToken = PutBaseInToken(CurrentToken, TokenBase);
        if(MaskedToken == InputToken) {
            if(IsCountValid(Count, *MaxCount, Value, *UsedCounts)) {
                MarkCountAsUsed(Count, MaxCount, UsedCounts, Value);
                output.Value = Value;
                output.Count = Count;
                return output;
            } else {
                ValidOlderToken = true;
            }
        }
        CurrentToken = GenerateOPAYGOToken(CurrentToken, SECRET_KEY);
    }
    if(ValidOlderToken) {
        output.Value = -2;
    } else {
        output.Value = -1;
    }
    output.Count = *MaxCount;
    return output;
}
