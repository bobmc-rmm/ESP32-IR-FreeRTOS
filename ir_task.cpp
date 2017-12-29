// -*- C++ -*-
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
/**@file  ir_task.cpp 
 * @brief Infrared receiver ISR and task() to decode it 
 * 
 */ 
#include "Globals.h"

enum { // timings for a 'Sony' remote
   eStart1 = 2100, eStart2 = 2800, // should be 2400
   eSpace1=300, eSpace2=900,	   // should be 600
   eLow1 = eSpace1, eLow2 = eSpace2, // should be 600
   eHigh1=1000, eHigh2=1600,	     // should be 1200
   eMaxCode
};

enum { bit_err, bit_start, bit_high, bit_low }; // for printf

static struct _b1{
   bool reset;
   int index;
   int bits; 
   int mask;
   int bit_enum;			// 'E', 'S', 'H','L'
}hbits; // for pulse times to bits

static char *bit_titles[4] = { "Err", "Start", "High", "Low"};

static void IRAM_ATTR isr_IR();
static void ir_decode(int pulse);
static QueueHandle_t ir_queue;

static int icount;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
/// \brief task to decode infrared pulse times from ISR queue
void ir_task( void * parameter ){
   float fv=0;
   const TickType_t xDelay = 600 / portTICK_PERIOD_MS;
   int pulse;

   ir_queue = xQueueCreate( 100, sizeof( int ) );
   
   if(ir_queue == NULL){
      Serial.println("Error creating the ir_queue");
   }
   
   Serial.print("ir_task...");
   // setup ISR to trigger RISING|FALLING
   pinMode(PIN_IR,INPUT);
   attachInterrupt(PIN_IR, isr_IR, CHANGE);
   hbits.reset = true;

   for(;;){
      if( xQueueReceive(ir_queue, &pulse, portMAX_DELAY) ){
	 ir_decode(pulse);
      }
   }

   printf("Ending task 1\n");
   vTaskDelete( NULL );
} 

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
/// \brief ISR edge trigged by 38khz IR device
static void IRAM_ATTR isr_IR(){
   static uint32_t t0, t1;
   int intval;

   t1 = micros();
   intval = t1 - t0;
   t0 = t1;

   xQueueSendFromISR( ir_queue, &intval, NULL);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/// \brief called from ir_task() to interpret timings
static void ir_decode(int pulse){
   static int w1,w2;

   if( hbits.reset ){
      hbits.reset = false;
      hbits.index = 0;
      hbits.bits = 0;
      hbits.mask = 1;
      hbits.bit_enum = bit_start;
   }

   if( pulse > eStart1 ){
      if( hbits.bits != 0 ){
	 printf(": \t\t %08x\n",hbits.bits);
      }
      hbits.reset = true;
      hbits.bit_enum = bit_start;
      if( pulse > eStart2 ){
	 hbits.bit_enum = bit_err;
	 w1 = 0;
      }
      else{
	 w1 = pulse;
      }
      w2 = 0;
      return;
   }

   if(hbits.index > 30 ){
      printf("\t...overflow %d\n",hbits.index);
      hbits.reset = true;
      return;
   }

   if( pulse > eHigh1 && pulse < eHigh2 ){
      hbits.index++;
      hbits.bits |= hbits.mask;
      hbits.mask <<= 1;
      w1 = pulse;
      hbits.bit_enum = bit_high;

   }
   else{
      if( pulse > eLow1 && pulse < eLow2 ){
	 if( w1 == 0 ){
	    w1 = pulse;
	    hbits.index++;
	    hbits.bits &=  ~hbits.mask;
	    hbits.mask <<= 1;
	    hbits.bit_enum = bit_low;
	 }
	 else {
	    w2 = pulse;
	    printf("%d \t %d \t %d \t%s\n",icount++,w1,w2,
		   bit_titles[hbits.bit_enum]);
	    w1 = w2 = 0; 
	 }
      }
   }
   return;
}

// file ir_task.cpp
