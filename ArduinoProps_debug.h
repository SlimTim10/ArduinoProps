#ifndef ARDUINOPROPS_DEBUG_H
#define ARDUINOPROPS_DEBUG_H

//#define DEBUG

#include "Arduino.h"

#ifdef DEBUG
#define dbg_init() do { Serial.begin(115200); while (!Serial); } while (0)
#define dbg_print(args...) Serial.print(args)
#define dbg_println(str) Serial.println(str)
#define dbg_printHex(num) do { Serial.print(' '); Serial.print(num, HEX); } while (0)
#define dbg_printInt(num) do { Serial.print(' '); Serial.print(num); } while (0)
#else
#define dbg_init()
#define dbg_print(args...)
#define dbg_println(str)
#define dbg_printHex(num)
#define dbg_printInt(num)
#endif

#endif // ARDUINOPROPS_DEBUG_H
