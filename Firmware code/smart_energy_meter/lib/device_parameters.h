#ifndef device_parameters_h
#define device_parameters_h

#define TIME_DIVIDER 1
//#define RESTRICTED_DIGIT_SET_MODE

#ifdef RESTRICTED_DIGIT_SET_MODE
#define TOKEN_LENGTH 15
#else
#define TOKEN_LENGTH 9
#endif

#endif
