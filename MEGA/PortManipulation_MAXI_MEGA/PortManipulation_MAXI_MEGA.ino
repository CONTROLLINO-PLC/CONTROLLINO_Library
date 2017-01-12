#include <Controllino.h>  /* Usage of CONTROLLINO library allows you to use CONTROLLINO_xx aliases in your sketch. */

/*
  CONTROLLINO - Port Manipulation for MAXI and MEGA, Version 01.00
  The sketch is relevant only for CONTROLLINO variants MAXI and MEGA!

  Turns on and off four digital outputs of CONTROLLINO, which also blinks with CONTROLLINO´s status LEDs for the outputs.
  It explains how to control the microcontroller´s pins directly via port registers.
  
  This know how may be useful if you need to control the lines in a more efficient way, but it is essential if you
  need to control "non-Arduino" pins, like Digital Outputs 20,21,22 and 23 at CONTROLLINO MEGA.

  Please, see https://www.arduino.cc/en/Reference/PortManipulation for some theoretical background.

  List of non-Arduino pins used in CONTROLLINO boards:
  ----------------------------------------------------
   MINI - none (= all used pins have their Arduino and CONTROLLINO "nickname")
   MAXI - RS485 serial bus /RE (output)
        - RS485 serial bus DE (output)
        - Ethernet /SS (output) - Ethernet library handles this pin for you
        - Ethernet /INT (input)
        - Overload /OVL (input)
        - Real Time Clock RTC /SS (output)
        - Real Time Clock RTC /INT (input)
   MEGA - the same like for MAXI plus
        - Digital Outputs 20,21,22 and 23 (output)

  IMPORTANT INFORMATION!
  Please, select proper target board in Tools->Board->Controllino MAXI/MEGA before Upload to your CONTROLLINO.
  (Please, refer to https://github.com/CONTROLLINO-PLC/CONTROLLINO_Library if you do not see the CONTROLLINOs in the Arduino IDE menu Tools->Board.)

  This example sketch does not require any additional parts, equipment, wires or power supply. Just your CONTROLLINO, USB cable (type A-B) and a PC.
 
  Created 13 Dec 2016
  by Lukas

  https://controllino.biz/

  (Check https://github.com/CONTROLLINO-PLC/CONTROLLINO_Library for the latest CONTROLLINO related software stuff.)
*/

// the setup function runs once when you press reset (CONTROLLINO RST button) or connect the CONTROLLINO to the PC
void setup() {
  
  // lets initialize CONTROLLINO_D8 - CONTROLLINO_D11 as output pins
  // please note that they are mapped to microcontroller´s port B and pins PB4, PB5, PB6 and PB7 
  DDRB = DDRB | B11110000;  // safe way how to set register bits 7,6,5,4 of the port to "1" in one shot and do not influence bits 3,2,1,0
  // logical "1" (HIGH) in the "direction" register configures the appropriate pin as an output
  
  // The same configuration done pin by pin looks like this
  DDRB = DDRB | B10000000;  // Port B, pin 7 is configured as output - corresponds with Digital Output 11 of CONTROLLINO MAXI/MEGA
  DDRB = DDRB | B01000000;  // Port B, pin 6 is configured as output - corresponds with Digital Output 10 of CONTROLLINO MAXI/MEGA
  DDRB = DDRB | B00100000;  // Port B, pin 5 is configured as output - corresponds with Digital Output  9 of CONTROLLINO MAXI/MEGA
  DDRB = DDRB | B00010000;  // Port B, pin 4 is configured as output - corresponds with Digital Output  8 of CONTROLLINO MAXI/MEGA
}

// the loop function runs over and over again forever
void loop() {
  PORTB = PORTB | B11110000;  // sets Digital Outputs 8,9,10,11 in one shot to HIGH -> turns the LEDs ON
  delay(1000);                // wait for 1 second 
  PORTB = PORTB & B00001111;  // sets Digital Outputs 8,9,10,11 in one shot to LOW -> turns the LEDs OFF
  delay(1000); 
  PORTB = PORTB | B00010000;  // sets Digital Output 8 to HIGH -> turns the LED ON
  delay(1000);                // wait for 1 second 
  PORTB = PORTB & B11101111;  // sets Digital Output 8 to LOW -> turns the LED OFF
  delay(1000); 
  PORTB = PORTB | B00100000;  // sets Digital Output 9 to HIGH -> turns the LED ON
  delay(1000);                // wait for 1 second 
  PORTB = PORTB & B11011111;  // sets Digital Output 9 to LOW -> turns the LED OFF
  delay(1000); 
  PORTB = PORTB | B01000000;  // sets Digital Output 10 to HIGH -> turns the LED ON
  delay(1000);                // wait for 1 second 
  PORTB = PORTB & B10111111;  // sets Digital Output 10 to LOW -> turns the LED OFF
  delay(1000); 
  PORTB = PORTB | B10000000;  // sets Digital Output 11 to HIGH -> turns the LED ON
  delay(1000);                // wait for 1 second 
  PORTB = PORTB & B01111111;  // sets Digital Output 11 to LOW -> turns the LED OFF
  delay(1000); 
  
  
  // please, visit https://controllino.biz/downloads/   
  // if you want to know more about the mapping of the CONTROLLINO
  // digital outputs to the microcontroller´s ports and pins
}

/* End of the example. Visit us at https://controllino.biz/ or https://github.com/CONTROLLINO-PLC/CONTROLLINO_Library or contact us at info@controllino.biz if you have any questions or troubles. */

/* 2016-12-14: The sketch was successfully tested with Arduino 1.6.13, Controllino Library 1.0.0 and CONTROLLINO MAXI and MEGA. */

