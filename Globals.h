// -*- C++ -*-
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
/**@file  Globals.h 
 * @brief symbols given wide scope
 * 
 */ 
#if !defined Globals_h
#    define Globals_h

#include <Arduino.h>
#include <freertos/queue.h>

enum { PIN_IR=A2, PIN_LED1=A11 };

typedef uint8_t  u8t;
typedef uint16_t u16t;

void ir_task( void * parameter );

#endif // Globals.h

