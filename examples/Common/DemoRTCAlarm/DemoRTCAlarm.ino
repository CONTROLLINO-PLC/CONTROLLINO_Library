#include <SPI.h>
#include <Controllino.h>

/*
  CONTROLLINO - Demonstration of Real Time Clock Alarm usage, Version 02.00

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

// The setup function runs once when you press reset (CONTROLLINO RST button) or connect power supply (USB or external 12V/24V) to the CONTROLLINO.
void setup() {
  int mRetval = 0;
  // initialize serial communication at 9600 bits per second
  Serial.begin(9600);
  mRetval = Controllino_RTC_init();
  Serial.println("Init retval ");
  Serial.println(mRetval, HEX);
  mRetval = Controllino_ClearAlarm();
  Serial.println("ClearAlarm retval ");
  Serial.println(mRetval, HEX);
  /* set time and date by separate values values to the RTC chip */
  /* Day, WeekDay, Month, Year, Hour, Minute, Second); */
  mRetval = Controllino_SetTimeDate(12, 4, 1, 17, 15, 58, 0);
  Serial.println("SetTime retval ");
  Serial.println(mRetval, HEX);
  pinMode(CONTROLLINO_RTC_INTERRUPT, INPUT_PULLUP);
  mRetval = Controllino_SetMinuteAlarm(59);
  Serial.println("SetAlarm retval ");
  Serial.println(mRetval, HEX);

}

// the loop function runs over and over again forever
void loop() {
  static int i = 0;
  int mMinutes = Controllino_GetMinute();
  Serial.print(mMinutes, DEC);
  Serial.print(";");
  Serial.println(i++);
  delay(1000);
  if (digitalRead(CONTROLLINO_RTC_INTERRUPT) == 0) {
    Controllino_ClearAlarm();
    Serial.println("Alarm triggered!");
  };
}

/* End of the example. Visit us at https://controllino.biz/ or https://github.com/CONTROLLINO-PLC/CONTROLLINO_Library or contact us at info@controllino.biz if you have any questions or troubles. */
/* 2024-04-18 the sketch was succesfully tested with CONTROLLINO MEGA */
