#ifndef _MultiSerial_h
#define _MultiSerial_h

#ifdef __cplusplus
#include <variant.h>
#endif

/*
	| Serial |   | TX arduino pin |   | RX arduino pin |   | SERCOM |
	==================================================================
	| Serial2      18                   19                   4      |
	| Serial3      16                   17                   1      |
	| Serial4      14                   15                   5      |
	==================================================================
*/

extern Uart Serial2;
extern Uart Serial3;
extern Uart Serial4;


#endif