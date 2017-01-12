#include <SPI.h>
#include <Controllino.h>  /* Usage of CONTROLLINO library allows you to use CONTROLLINO_xx aliases in your sketch. */

/*
  CONTROLLINO - Demonstration of RS485 interface usage, Version 01.00

  Compatible with CONTROLLINO MAXI and MEGA.

  IMPORTANT INFORMATION!
  Please, select proper target board in Tools->Board->Controllino MAXI/MEGA before Upload to your CONTROLLINO.
  
  (Refer to https://github.com/CONTROLLINO-PLC/CONTROLLINO_Library if you do not see the CONTROLLINOs in the Arduino IDE menu Tools->Board.)

  This example sketch requires external 12V or 24V power supply for your CONTROLLINO and RS485 communication partner device (e.g. another CONTROLLINO :-) ). 

  Created 12 Jan 2017
  by Lukas

  https://controllino.biz/
  
  (Check https://github.com/CONTROLLINO-PLC/CONTROLLINO_Library for the latest CONTROLLINO related software stuff.)
*/

/* This function will test RS485 functionality. Possible modes are 0-2. 0 is all enabled, sended message should come back, 1 means RE is low,  so message should not be recieved, 
2 means DE is disabled, so no message should be sent at all.*/
void TestRS485 (int mode)
{
  DDRJ = DDRJ | B01100000;
  PORTJ = PORTJ & B10011111;
  /*pinMode(CONTROLLINO_RS485_TX, OUTPUT);
  pinMode(CONTROLLINO_RS485_RX, INPUT);
  pinMode(CONTROLLINO_RS485_RE, OUTPUT);
  pinMode(CONTROLLINO_RS485_DE, OUTPUT);*/
  switch (mode)
  {
    case 0:
      PORTJ = PORTJ & B10011111;
      PORTJ = PORTJ | B01000000;
      /*digitalWrite (CONTROLLINO_RS485_RE, HIGH);
      digitalWrite (CONTROLLINO_RS485_DE, HIGH);*/
      delay (10);
      Serial.println ("Sending test message, expected to return;"); 
      Serial3.print("UUUUU Controllino RS485 test Message.UUUUU");
      break;
      
    case 1:
      PORTJ = PORTJ & B10011111;
      PORTJ = PORTJ | B01100000;
      /*digitalWrite (CONTROLLINO_RS485_RE, LOW);
      digitalWrite (CONTROLLINO_RS485_DE, HIGH);*/
      delay (10);
      Serial.println ("Sending test message, not expected to return;");
      Serial3.print("UUUUU Controllino RS485 test Message.UUUUU");
      break;
      
    case 2:
      PORTJ = PORTJ & B10011111;
      PORTJ = PORTJ | B00100000;
     /* digitalWrite (CONTROLLINO_RS485_RE, HIGH);
      digitalWrite (CONTROLLINO_RS485_DE, LOW);*/
      delay (10);
      Serial.println ("Sending test message, not expected to be sended;"); 
      Serial3.print("UUUUU Controllino RS485 test Message.UUUUU");
      break;
      
    default:
      Serial.println("Wrong mode!");
      return; 
  }
}

/* This function enters loop and waits for any incoming data on serial 3.*/
void RecieveRS485()
{
  Serial.println("Recieving RS485.");
  while(true)
  {
    if (Serial3.available()) 
    {
      // print the new byte:
      Serial.print((char)Serial3.read()); 
    }
  }
}

void setup() {
  /* Here we initialize USB serial at 9600 baudrate for reporting */
  Serial.begin(9600);
  /* Here we initialize RS485 serial at 9600 baudrate for communication */
  Serial3.begin(9600);
  /* This will initialize Controllino RS485 pins */
  Controllino_RS485Init();

}

void loop() {
  /* Use only one of the two functions, either send or recieve */
  // RecieveRS485();
  
  /* Send test. 2 seconds apart sending messages with 3 types of different settings. Check the function comment for more infromation */
  TestRS485(0);
  delay(2000);
  TestRS485(1);
  delay(2000);
  TestRS485(2);
  delay(2000);

}

/* 2017-01-12: The sketch was successfully tested with Arduino 1.6.13, Controllino Library 1.0.0 and CONTROLLINO MAXI and MEGA. */
