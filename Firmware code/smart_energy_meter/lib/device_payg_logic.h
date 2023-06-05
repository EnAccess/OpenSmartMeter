#ifndef device_payg_logic_h
#define device_payg_logic_h

#if (defined(__APPLE__) && defined(__MACH__)) /* MacOS X Framework build */
    #include <sys/types.h>
#endif

#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "unix_device/device_functions.h"
#include "opaygo_decoder/opaygo_core.h"
#include "device_parameters.h"

void UpdateDeviceStatusFromTokenValue(int TokenValue, int ActivationCount);
void UpdateInvalidTokenWaitingPeriod(void);
void SetTime(int NumberOfDaysToAdd);
void AddTime(int NumberOfDaysToAdd);
bool IsActive(void);
bool TokenEntryAllowed(void);

#endif
