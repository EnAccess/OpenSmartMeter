#ifndef opaygo_value_utils_h
#define opaygo_value_utils_h

#include <stdio.h>
#include <stdint.h>

#if (defined(__APPLE__) && defined(__MACH__)) /* MacOS X Framework build */
    #include <sys/types.h>
#endif

int DecodeBase(int StartingCodeBase, int TokenBase);
int GetTokenBase(uint32_t Token);
uint32_t PutBaseInToken(uint32_t Token, int TokenBase);

#endif
