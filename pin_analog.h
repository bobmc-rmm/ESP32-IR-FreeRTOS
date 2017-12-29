// -*- C++ -*-
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
/**@file  pin_analog.h 
 * @brief analog input or output
 * 
 */ 
#if !defined pin_analog_h
#    define pin_analog_h

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
class PinAnalog {
   u8t pin;
   int lower;			// lower limit
   int upper;			// upper limit
   
   int freq = 5000;
   int ledChannel = 0;
   int resolution = 8;
   int dutyCycle = 0;
   bool isOutput;

public:
   PinAnalog( u8t pinNum, int _lower, int _upper){
      lower = _lower;
      upper = _upper;
      pin = pinNum;
      pinMode(pin,INPUT);
      isOutput = false;		// input constructed
   }

   PinAnalog( u8t pinNum ){
      pin = pinNum;
      ledcSetup(ledChannel, freq, resolution);
      ledcAttachPin(pin, ledChannel);
      ledcWrite(ledChannel, dutyCycle);
      isOutput = true;		// output constructed
   }

   void write( u16t value ){
      if( isOutput ){
	 dutyCycle = map(value, 0, 4095, 0, 255);
	 ledcWrite(ledChannel, dutyCycle);
      }
   }
   
   bool tooHigh(int sense) const { return (sense > upper);}
   bool tooLow(int sense) const { return (sense < lower);}

   int read_raw(void){
      if( isOutput ) return 0;
      return (analogRead(pin) & 0x7fff);
   }
   
   /// convert raw AtoD to real-world value
   float convert( int raw ){
      return raw * 1.0;		// TBD
   }
   
}; // class PinAnalog

#endif // pin_analog_h
