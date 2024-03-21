#include <Controllino.h>  /* Usage of CONTROLLINO library allows you to use CONTROLLINO_xx aliases in your sketch. */

/*
  CONTROLLINO - Analog Inputs, Version 01.00

  Reads out converted analog values from a screw terminal input A0 of CONTROLLINO.
  Compatible with CONTROLLINO MINI, MAXI and MEGA.

  IMPORTANT INFORMATION!
  Please, select proper target board in Tools->Board->Controllino MINI/MAXI/MEGA before Upload to your CONTROLLINO.
  
  (Refer to https://github.com/CONTROLLINO-PLC/CONTROLLINO_Library if you do not see the CONTROLLINOs in the Arduino IDE menu Tools->Board.)

  This example sketch requires external 12V or 24V power supply for your CONTROLLINO and a potentiometer.
  You can also try to connect screw terminal A0 input directly to 12V/24V or GND to see max/min values. 


                                                 ------------------------
  12V/24V -----------------o---------------------| 12V/24V              |
  external                 |                     |                      |
  supply                   |                     |                      |
                        --------                 |      CONTROLLINO     |
                        | POTI |-----------------| A0                   |
                        --------                 |                      |
                           |                     |                      |
                           |                     |                      |
                           |                     |                      |
  GND ---------------------o---------------------| GND                  |
                                                 ------------------------

  
  Created 9 Jan 2017
  by Lukas

  https://controllino.biz/
  
  (Check https://github.com/CONTROLLINO-PLC/CONTROLLINO_Library for the latest CONTROLLINO related software stuff.)
*/

// The setup function runs once when you press reset (CONTROLLINO RST button) or connect power supply (USB or external 12V/24V) to the CONTROLLINO.
void setup() {
  // initialize necessary pin as input pin
  pinMode(CONTROLLINO_A0, INPUT);
  // please note that CONTROLLINO MINI has 5 more analog inputs A1 - A5
  // please note that CONTROLLINO MAXI has 9 more analog inputs A1 - A9
  // please note that CONTROLLINO MEGA has 15 more analog inputs A1 - A15
  // All of them work in the same way like A0.

  // Initialize serial communication at 9600 bits per second.
  // Use Tools->SerialMonitor to see the messages.
  Serial.begin(9600); 
}

// the loop function runs over and over again forever
void loop() {
  int measurementResult;
  int digitalValue;
  int measurementResultVoltage;

  // read out value from A0 screw terminal input
  measurementResult = analogRead(CONTROLLINO_A0);
  // analogRead function returns a number between 0 (for 0 V = connected to ground) to 1023 for maximal voltage (which depends on power supply voltage)
    Serial.print("Analog to digital conversion at input A0 returned: ");
  Serial.println(measurementResult);

  
  // convert the result value to a voltage in case of external 12V power supply
  measurementResultVoltage = map(measurementResult, 0, 1023, 0, 16);  // because the voltage dividers are dimensioned with some safety reserve
  Serial.print("Mapping to 0-12 V scale results to: ");
  Serial.print(measurementResultVoltage);
  Serial.print(" V or ");
  measurementResultVoltage = map(measurementResult, 0, 1023, 0, 15200); // because the voltage dividers are dimensioned with some safety reserve
  Serial.print(measurementResultVoltage);
  Serial.println(" mV");

  // convert the result value to a voltage in case of external 24V power supply
  measurementResultVoltage = map(measurementResult, 0, 1023, 0, 31);  // because the voltage dividers are dimensioned with some safety reserve
  Serial.print("Mapping to 0-24 V scale results to: ");
  Serial.print(measurementResultVoltage);
  Serial.print(" V or ");
  measurementResultVoltage = map(measurementResult, 0, 1023, 0, 30400);  // because the voltage dividers are dimensioned with some safety reserve
  Serial.print(measurementResultVoltage);
  Serial.println(" mV");

  // So, if you need to calculate the measured voltage, you have to know the device´s power supply voltage!

  // read out logical representation (LOW or HIGH, 0 or 1) of the analog value
  digitalValue = digitalRead(CONTROLLINO_A0);
  Serial.print("Logical representation of the analog value is ");
  if (digitalValue == LOW)
    {
      Serial.println("LOW"); 
    }
    else 
      {
        Serial.println("HIGH"); 
      }

  // Please note that guaranteed logical levels have following thresholds:
  //   LOW:   0V – 3,6V   ( for Vin=12V)
  //   HIGH:  9V – 13,2V  ( for Vin=12V)
  
  //   LOW:   0V – 7,2V   ( for Vin=24V)
  //   HIGH: 18V – 26,4V  ( for Vin=24V)
  //
  //   logical state between these ranges is undefined!

  Serial.println("---------------------------------------------------------");
  delay(3000);  // three seconds delay for better readability of the printed results                    
}

/* End of the example. Visit us at https://controllino.biz/ or https://github.com/CONTROLLINO-PLC/CONTROLLINO_Library or contact us at info@controllino.biz if you have any questions or troubles. */

/* 2017-01-10: The sketch was successfully tested with Arduino 1.6.13, Controllino Library 1.0.0 and CONTROLLINO MINI, MAXI and MEGA. */

