//
//  opaygo_decoder_extended.h
//  OPAYGO-Unix
//
//  Created by Benjamin David on 17/09/2019.
//  Copyright © 2019 Solaris Offgrid. All rights reserved.
//

#ifndef opaygo_decoder_extended_h
#define opaygo_decoder_extended_h

#if (defined(__APPLE__) && defined(__MACH__)) /* MacOS X Framework build */
    #include <sys/types.h>
#endif

#include <stdint.h>
#include <stdio.h>
#include "opaygo_core_extended.h"
#include "opaygo_decoder.h"

int32_t GetActivationValueFromExtendedToken(uint64_t InputToken, uint16_t *MaxCount, uint32_t StartingCode, char SECRET_KEY[16]);

#endif /* opaygo_decoder_extended_h */
