#include <Controllino.h>  /* Usage of CONTROLLINO library allows you to use CONTROLLINO_xx aliases in your sketch. */

/*
  CONTROLLINO - Port Manipulation for MINI, Version 01.00
  The sketch is relevant only for CONTROLLINO variant MINI!

  Turns on and off four digital outputs of CONTROLLINO, which also blinks with CONTROLLINO´s status LEDs for the outputs.
  It explains how to control the microcontroller´s pins directly via port registers.
  
  This know how may be useful if you need to control the lines in more efficient way.

  Please, see https://www.arduino.cc/en/Reference/PortManipulation for some theoretical background.

  List of non-Arduino pins used in CONTROLLINO devices:
  ----------------------------------------------------
   MINI - none (= all used pins have their Arduino and CONTROLLINO "nickname")
   MAXI - RS485 serial bus /RE (output)
        - RS485 serial bus  DE (output)
        - Ethernet /SS (output) - Ethernet library handles this pin for you
        - Ethernet /INT (input)
        - Overload /OVL (input)
        - Real Time Clock RTC /SS (output)
        - Real Time Clock RTC /INT (input)
   MEGA - the same like for MAXI plus
        - Digital Outputs 20,21,22 and 23 (output)

  IMPORTANT INFORMATION!
  Please, select proper target board in Tools->Board->Controllino MINI before Upload to your CONTROLLINO.
  (Please, refer to https://controllino.biz/programming/ if you do not see the CONTROLLINOs in the Arduino IDE menu Tools->Board.)

  This example sketch does not require any additional parts, equipment, wires or power supply. Just your CONTROLLINO, USB cable (type A-B) and a PC.
 
  Created 13 Dec 2016
  by Lukas

  https://controllino.biz/
*/

// the setup function runs once when you press reset (CONTROLLINO RST button) or connect the CONTROLLINO to the PC
void setup() {
  
  // lets initialize CONTROLLINO_D0 - CONTROLLINO_D3 as output pins
  // please note that they are mapped to microcontroller´s port D and pins PD4, PD5, PD6 and PD7 
  DDRD = DDRD | B11110000;  // safe way how to set register bits 7,6,5,4 of the port D to "1" in one shot and do not influence bits 3,2,1,0
  // logical "1" (HIGH) in the "direction" register configures the appropriate pin as an output
  
  // The same configuration done pin by pin looks like this
  DDRD = DDRD | B10000000;  // Port D, pin 7 is configured as output - corresponds with Digital Output 3 of CONTROLLINO MINI
  DDRD = DDRD | B01000000;  // Port D, pin 6 is configured as output - corresponds with Digital Output 2 of CONTROLLINO MINI
  DDRD = DDRD | B00100000;  // Port D, pin 5 is configured as output - corresponds with Digital Output 1 of CONTROLLINO MINI
  DDRD = DDRD | B00010000;  // Port D, pin 4 is configured as output - corresponds with Digital Output 0 of CONTROLLINO MINI
}

// the loop function runs over and over again forever
void loop() {
  PORTD = PORTD | B11110000;  // sets Digital Outputs 0,1,2,3 in one shot to HIGH -> turns the LEDs ON
  delay(1000);                // wait for 1 second 
  PORTD = PORTD & B00001111;  // sets Digital Outputs 0,1,2,3 in one shot to LOW -> turns the LEDs OFF
  delay(1000); 
  PORTD = PORTD | B00010000;  // sets Digital Output 0 to HIGH -> turns the LED ON
  delay(1000);                // wait for 1 second 
  PORTD = PORTD & B11101111;  // sets Digital Output 0 to LOW -> turns the LED OFF
  delay(1000); 
  PORTD = PORTD | B00100000;  // sets Digital Output 1 to HIGH -> turns the LED ON
  delay(1000);                // wait for 1 second 
  PORTD = PORTD & B11011111;  // sets Digital Output 1 to LOW -> turns the LED OFF
  delay(1000); 
  PORTD = PORTD | B01000000;  // sets Digital Output 2 to HIGH -> turns the LED ON
  delay(1000);                // wait for 1 second 
  PORTD = PORTD & B10111111;  // sets Digital Output 2 to LOW -> turns the LED OFF
  delay(1000); 
  PORTD = PORTD | B10000000;  // sets Digital Output 3 to HIGH -> turns the LED ON
  delay(1000);                // wait for 1 second 
  PORTD = PORTD & B01111111;  // sets Digital Output 3to LOW -> turns the LED OFF
  delay(1000); 
  
  
  // please, visit https://controllino.biz/downloads/   
  // if you want to know more about the mapping of the CONTROLLINO
  // digital outputs to the microcontroller´s ports and pins
}

/* End of the example. Visit us at https://controllino.biz/ or contact us at info@controllino.biz if you have any questions or troubles. */

/* 2016-12-14: The sketch was successfully tested with Arduino 1.6.13, Controllino Library 1.0.3 and CONTROLLINO MINI. */

