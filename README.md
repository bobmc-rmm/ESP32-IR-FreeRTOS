# ESP32-IR-FreeRTOS
ESP32, Arduino IDE, FreeRTOS Tasks+Queues+ISR

This is a demo/test of ESP32 infrared receiver decoding using FreeRTOS tools in Arduino build.

There is an edge-triggered ISR which times the IR device pulses in microseconds.  It pushes these timings to the IR task queue.  The IR task waits for a stream of timings on it's input queue. It converts these timing to bit-mapped words. 

File IR_bits.jpg shows a snippet of the serial monitor output. It was done with Phillips resort set to 'Sony'. The timings are:-
2400=Start, 1200=High, 600=Low, 600=space.  The actual measurements vary because of ISR response and/or IR noise

Hardware:  Huzzah32, TSOP28328, VS1838B
