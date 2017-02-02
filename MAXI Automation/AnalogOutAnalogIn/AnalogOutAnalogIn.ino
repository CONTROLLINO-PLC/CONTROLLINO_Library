#include <Controllino.h>  /* Usage of CONTROLLINO library allows you to use CONTROLLINO_xx aliases in your sketch. */

/*
  CONTROLLINO - Analog Out Analog In, Version 01.00

  Simple menu which alows you to test special CONTROLLINO MAXI Automation features 0-10V analog inputs and 0-10V / 0-20mA analog outputs.
  Please use Tools-Serial Monitor (9600 baud)

  IMPORTANT INFORMATION!
  Please, select proper target board in Tools->Board->CONTROLLINO MAXI Automation before Upload to your CONTROLLINO.
  (Please, refer to https://github.com/CONTROLLINO-PLC/CONTROLLINO_Library if you do not see the CONTROLLINOs in the Arduino IDE menu Tools->Board.)

  This example sketch requires external 24V power supply for your CONTROLLINO. 
 
  Created 2 Feb 2017
  by Lukas

  https://controllino.biz/

  (Check https://github.com/CONTROLLINO-PLC/CONTROLLINO_Library for the latest CONTROLLINO related software stuff.)
*/

int incomingByte = 0;   // for incoming serial data
int analogOutPercent0 = 0;   // initial value 0-100 percent
int analogOutPercent1 = 0;   // initial value 0-100 percent
int showMenu = 1;
int measureAI0 = 0;
int measureAI1 = 0;


void setup() {
  // initialize serial communications at 9600 bps
  Serial.begin(9600);

  pinMode(CONTROLLINO_AO0, OUTPUT);
  pinMode(CONTROLLINO_AO1, OUTPUT);
  pinMode(CONTROLLINO_AI12, INPUT);
  pinMode(CONTROLLINO_AI13, INPUT);
  
}


void loop() {
  int analogOut = 0;   // 0 - 255 to be set
  long int analogOutVoltage = 0;   // 0 - 10 000 mV to be set
  long int analogOutCurrent = 0;   // 0 - 20 000 uA to be set

  if ( showMenu == 1 )
    {
      Serial.println("------- Version 01.00 --------------------");
      Serial.println("CONTROLLINO AUTOMATION example menu: ");
      Serial.println("------------------------------------------");
      Serial.println(" > press 'o' to increase Analog Out 0 for 5% ");
      Serial.println(" > press 'p' to reduce Analog Out 0 for 5% ");
      Serial.println("");
      Serial.println(" > press '+' to increase Analog Out 1 for 5% ");
      Serial.println(" > press '-' to reduce Analog Out 1 for 5% ");
      Serial.println("");
      Serial.println(" > press 's' to START measurement of AI_10V_0 ");
      Serial.println(" > press 'd' to STOP measurement of AI_10V_0 ");
      Serial.println("");
      Serial.println(" > press 'f' to START measurement of AI_10V_1 ");
      Serial.println(" > press 'g' to STOP measurement of AI_10V_1 ");
      Serial.println("------------------------------------------");
      Serial.println("---- H or h shows this menu---------------");
      Serial.println("------------------------------------------");
      showMenu = 0;
    }

   if ( measureAI0 == 1 )
    {
      int val = 0;
      int analogIn = 0;
      val = analogRead(CONTROLLINO_AI12);    // read the input pin
      Serial.print("Analog input 0-10V 0,  ADC  is "); 
      Serial.print(val);
      analogIn = map(val, 0, 1023, 0, 10000);
      Serial.print(" ,Analog input 0 0-10V  is "); 
      Serial.print(analogIn);
      Serial.println(" mV");
      Serial.println("------------------------------------------");
    }

  if ( measureAI1 == 1 )
    {
      int val = 0;
      int analogIn = 0;
      val = analogRead(CONTROLLINO_AI13);    // read the input pin
      Serial.print("Analog input 0-10V 1,  ADC  is ");  
      Serial.print(val);
      analogIn = map(val, 0, 1023, 0, 10000);
      Serial.print(" ,Analog input 0 0-10V  is "); 
      Serial.print(analogIn);
      Serial.println(" mV");
      Serial.println("------------------------------------------");
    }

  if (Serial.available() > 0) 
    {
      // read the incoming byte:
      incomingByte = Serial.read();

      switch(incomingByte)
        {
          case 'H':
          case 'h':
            showMenu = 1;
            break;
          
          case 'o':
            analogOutPercent0 = analogOutPercent0 + 5;
            if (analogOutPercent0 > 100) {analogOutPercent0 = 100;}
            analogOut = map(analogOutPercent0, 0, 100, 0, 255);
            analogWrite(CONTROLLINO_AO0, analogOut);
            Serial.print("AO0 increased to "); 
            Serial.print(analogOutPercent0);
            Serial.print(" %, PWM duty cycle to "); 
            Serial.print(analogOut);
            Serial.print(" , Analog voltage "); 
            analogOutVoltage = map(analogOut, 0, 255, 0, 10000);
            Serial.print(analogOutVoltage);
            Serial.print(" mV, or Analog current ");
            analogOutCurrent = map(analogOut, 0, 255, 0, 20000);
            Serial.print(analogOutCurrent);
            Serial.println(" uA");
            break;
          case 'p':
            analogOutPercent0 = analogOutPercent0 - 5;
            if (analogOutPercent0 < 0) {analogOutPercent0 = 00;}
            analogOut = map(analogOutPercent0, 0, 100, 0, 255);
            analogWrite(CONTROLLINO_AO0, analogOut);
            Serial.print("AO0 reduced to "); 
            Serial.print(analogOutPercent0);
            Serial.print(" %, PWM duty cycle to "); 
            Serial.print(analogOut);
            Serial.print(" , Analog voltage "); 
            analogOutVoltage = map(analogOut, 0, 255, 0, 10000);
            Serial.print(analogOutVoltage);
            Serial.print(" mV, or Analog current ");
            analogOutCurrent = map(analogOut, 0, 255, 0, 20000);
            Serial.print(analogOutCurrent);
            Serial.println(" uA");
            break; 
          case '+':
            analogOutPercent1 = analogOutPercent1 + 5;
            if (analogOutPercent1 > 100) {analogOutPercent1 = 100;}
            analogOut = map(analogOutPercent1, 0, 100, 0, 255);
            analogWrite(CONTROLLINO_AO1, analogOut);
            Serial.print("AO1 increased to "); 
            Serial.print(analogOutPercent1);
            Serial.print(" %, PWM duty cycle to "); 
            Serial.print(analogOut);
            Serial.print(" , Analog voltage "); 
            analogOutVoltage = map(analogOut, 0, 255, 0, 10000);
            Serial.print(analogOutVoltage);
            Serial.print(" mV, or Analog current ");
            analogOutCurrent = map(analogOut, 0, 255, 0, 20000);
            Serial.print(analogOutCurrent);
            Serial.println(" uA");
            break;
          case '-':
            analogOutPercent1 = analogOutPercent1 - 5;
            if (analogOutPercent1 < 0) {analogOutPercent1 = 00;}
            analogOut = map(analogOutPercent1, 0, 100, 0, 255);
            analogWrite(CONTROLLINO_AO1, analogOut);
            Serial.print("AO1 reduced to "); 
            Serial.print(analogOutPercent0);
            Serial.print(" %, PWM duty cycle to "); 
            Serial.print(analogOut);
            Serial.print(" , Analog voltage "); 
            analogOutVoltage = map(analogOut, 0, 255, 0, 10000);
            Serial.print(analogOutVoltage);
            Serial.print(" mV, or Analog current ");
            analogOutCurrent = map(analogOut, 0, 255, 0, 20000);
            Serial.print(analogOutCurrent);
            Serial.println(" uA");
            break; 


          case 's':
            measureAI0 = 1;
            break;
          case 'd':
            measureAI0 = 0;
            break;
          case 'f':
            measureAI1= 1;
            break;
          case 'g':
            measureAI1= 0;
            break;
            
          default:
            Serial.println("FLUSH"); 
            break; 
        }
    }
  delay(1000);
}


/* End of the example. Visit us at https://controllino.biz/ or contact us at info@controllino.biz if you have any questions or troubles. */

/* 2017-02-02: The sketch was successfully tested with Arduino 1.8.1, Controllino Library 1.1.0 and CONTROLLINO MAXI Automation */

