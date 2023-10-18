#ifndef opaygo_decoder_h
#define opaygo_decoder_h

#if (defined(__APPLE__) && defined(__MACH__)) /* MacOS X Framework build */
#  include <sys/types.h>
#endif

#include <stdint.h>
#include <stdio.h>
#include "opaygo_core.h"
#include "opaygo_value_utils.h"
#include "restricted_digit_set_mode.h"

#define MAX_TOKEN_JUMP \
  64  // This is a jump in count so up to twice as large as the number of tokens
      // of the same type
#define MAX_TOKEN_JUMP_COUNTER_SYNC \
  100  // This is a jump in count so up to twice as large as the number of
       // tokens
#define MAX_UNUSED_OLDER_TOKENS 16  // Maximum of 16 (8 tokens of the same type)

struct TokenDataStruct {
  int Value;
  uint16_t Count;
};
typedef struct TokenDataStruct TokenData;

TokenData GetDataFromToken(uint64_t InputToken, uint16_t* MaxCount,
                           uint16_t* UsedCounts, uint32_t StartingCode,
                           unsigned char SECRET_KEY[16]);

#endif
