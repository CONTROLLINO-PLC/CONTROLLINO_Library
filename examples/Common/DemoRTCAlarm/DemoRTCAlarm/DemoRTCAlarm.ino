#include <SPI.h>
#include <Controllino.h>  /* Usage of CONTROLLINO library allows you to use CONTROLLINO_xx aliases in your sketch. */

/*
  CONTROLLINO - Demonstration of Real Time Clock usage, Version 02.00

  Periodically reads out the date and time information from the RTC chip.
  As it is done each 5 seconds, the number of seconds read out from the chip should be always for 5 seconds higher.
  Compatible with CONTROLLINO MINI, MAXI and MEGA.

  IMPORTANT INFORMATION!
  Please, select proper target board in Tools->Board->Controllino MINI/MAXI/MEGA before Upload to your CONTROLLINO.

  Do not forget to properly setup the mechanical switch at your CONTROLLINO MINI!
  
  (Refer to https://github.com/CONTROLLINO-PLC/CONTROLLINO_Library if you do not see the CONTROLLINOs in the Arduino IDE menu Tools->Board.)

  You need only your PC, CONTROLLINO and USB cable. 
  
  Created 12 Jan 2017, Updated 15.11.2018
  by Lukas

  https://controllino.biz/
  
  (Check https://github.com/CONTROLLINO-PLC/CONTROLLINO_Library for the latest CONTROLLINO related software stuff.)
*/

bool alarmActive;

// The setup function runs once when you press reset (CONTROLLINO RST button) or connect power supply (USB or external 12V/24V) to the CONTROLLINO.
void setup() {
  alarmActive = 0;
  // initialize serial communication at 9600 bits per second
  Serial.begin(9600);
  
  sei();
  
  Controllino_RTC_init();

  Controllino_ClearAlarm();

  /* set time and date by separate values values to the RTC chip */
  /* Day, WeekDay, Month, Year, Hour, Minute, Second); */
  Controllino_SetTimeDate(12,4,1,17,15,41,56); 
  
  /* or use another possibility and define the time and date by strings, e.g. "Nov 15 2018", "11:41:02" */
  /* following example uses predefined C macros __DATE__ and __TIME__ which represent compilation time */
  //Controllino_SetTimeDateStrings(__DATE__, __TIME__); /* set compilation time to the RTC chip */

  pinMode(CONTROLLINO_RTC_INTERRUPT, INPUT_PULLUP);

  Controllino_SetAlarm(15,42);
}

// the loop function runs over and over again forever
void loop() {
  int n;  
  Serial.print("Day: ");n = Controllino_GetDay(); Serial.println(n);
  
  Serial.print("WeekDay: ");n = Controllino_GetWeekDay(); Serial.println(n);
  
  Serial.print("Month: ");n = Controllino_GetMonth(); Serial.println(n);

  Serial.print("Year: ");n = Controllino_GetYear(); Serial.println(n);

  Serial.print("Hour: ");n = Controllino_GetHour(); Serial.println(n);

  Serial.print("Minute: "); n = Controllino_GetMinute(); Serial.println(n);

  Serial.print("Second: ");n = Controllino_GetSecond(); Serial.println(n);

  Serial.print("Int pin state: "); Serial.println(digitalRead(CONTROLLINO_RTC_INTERRUPT));

  Serial.print("Alarm state: "); Serial.println(alarmActive);
  
  if (digitalRead(CONTROLLINO_RTC_INTERRUPT) = 1) {
      Serial.println("Alarm triggered! Resetting...");
      Controllino_ClearAlarm();
    }
  
  delay(5000);        
}

/* End of the example. Visit us at https://controllino.biz/ or https://github.com/CONTROLLINO-PLC/CONTROLLINO_Library or contact us at info@controllino.biz if you have any questions or troubles. */

/* 2018-09-20: The sketch was successfully tested with Arduino 1.8.5, Controllino Library 3.0.2 and CONTROLLINO MINI, MAXI and MEGA. */
