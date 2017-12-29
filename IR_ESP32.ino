// -*- C++ -*-
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
/**@file 
 * @brief Arduino ESP32 FreeRTOS Infrared test/demo
 * @author RMM.2573098
 * 
 *
 */ 
#include "Globals.h"
#include "pin_analog.h"

void setup(){ 

   Serial.begin(115200);
   delay(1000);
   Serial.println("ESP32 IR Receiver Test");
   
   const int stack1 = 10000, priority = 1;

   xTaskCreate(ir_task, "IR_TASK", stack1, NULL, priority, NULL);
   
} 
 
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
/// something to do while the IR task is running
void loop(){
   //puts("_rc02b looping..." );
   PinAnalog *pLed;
   pLed = new PinAnalog(PIN_LED1);
   
   for( u8t i = 0; i < 240; i += 20 ){
      pLed->write(i);
      delay(1000/4);
   }
      
}

// IR_ESP32.ino
