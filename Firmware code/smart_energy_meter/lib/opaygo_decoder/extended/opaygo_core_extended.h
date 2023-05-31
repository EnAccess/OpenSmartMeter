//
//  opaygo_core_extended.h
//  OPAYGO-Unix
//
//  Created by Benjamin David on 17/09/2019.
//  Copyright © 2019 Solaris Offgrid. All rights reserved.
//

#ifndef opaygo_core_extended_h
#define opaygo_core_extended_h

#if (defined(__APPLE__) && defined(__MACH__)) /* MacOS X Framework build */
    #include <sys/types.h>
#endif

#include <stdint.h>
#include <stdio.h>

uint64_t GenerateOPAYGOTokenExtended(uint64_t LastToken, char SECRET_KEY[16]);
uint32_t DecodeBaseExtended(uint32_t StartingCodeBase, uint32_t TokenBase);
uint32_t GetTokenBaseExtended(uint64_t Token);
uint64_t PutBaseInTokenExtended(uint64_t Token, uint32_t TokenBase);

#endif /* opaygo_core_extended_h */
