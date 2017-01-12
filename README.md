# CONTROLLINO

### FIRST SOFTWARE OPEN-SOURCE PLC (ARDUINO compatible)
https://controllino.biz/

This GitHub repository contains all you need to start with CONTROLLINO devices programming in Arduino IDE.
* Arduino IDE extension package for Boards Manager
  * See our [Installation guide](#installation-guide)
* CONTROLLINO library for Library Manager
  * See our [Installation guide](#installation-guide)
  * See our [Reference manual](#reference-manual)
* Examples how to use special features of the CONTROLLINOs
  * When the library is installed in the Arduino IDE, you can find them i the File->Examples menu

Please note that CONTROLLINO exists in [3 different variants](#controllino-mini).

### CONTROLLINO MINI
* Microcontroller: ATmega328
* Atmel Clock Speed: 16 MHz
* RTC
* 1x serial Interface
* 1x SPI Interface
* 1x I2C Interface
* Input current Max. 8A
* 6x Relays Outputs – 230V / 6A
* 8x Analog/Digital Inputs
* 8x Digital Outputs – 2A @12V or 24V

### CONTROLLINO MAXI
* Microcontroller: ATmega2560
* Clock Speed: 16 MHz
* RTC
* Ethernet Connector
* 2x serial Interface
* 1x RS485 Interface
* 1x I2C Interface
* 1x SPI Interface
* Input current Max. 20A
* 10x Relays Outputs – 230V / 6A
* 12x Analog/Digital Inputs
* 12x Digital Outputs – 2A @12V or 24V

### CONTROLLINO MEGA
* Microcontroller: ATmega2560
* Clock Speed: 16 MHz
* RTC
* Ethernet Connector
* 2x serial Interface
* 1x RS485 Interface
* 1x I2C Interface
* 1x SPI Interface
* Input current Max. 30A
* 16x Relays Outputs – 230V / 6A
* 21x Analog/Digital Inputs
* 12x Digital Outputs – High Side Switch – 2A @12V or 24V
* 12x Digital Outputs – Half-Bridge - 2A @12V or 24V

## Reference manual

Please, make sure that you have included the CONTROLLINO library in your sketch.
So, you can see `#include <Controllino.h>` at the top of your sketch.

### Aliases for the pins
Of course, you can use Arduino native aliases for the pin numbers like A0, or 2, but CONTROLLINO library provides you its own set of aliases.
It is always combination of CONTROLLINO_ and the text description of the screw terminal you can see on the device. 

For example, for MINI we have defined following aliases:

` CONTROLLINO_D0`

` CONTROLLINO_D1`

` CONTROLLINO_D2`

` CONTROLLINO_D3`

` CONTROLLINO_D4`

` CONTROLLINO_D5`

` CONTROLLINO_D6`

` CONTROLLINO_D7`

 
` CONTROLLINO_A0`

` CONTROLLINO_A1`

` CONTROLLINO_A2`

` CONTROLLINO_A3`

` CONTROLLINO_A4`

` CONTROLLINO_A5` 

` CONTROLLINO_IN0`

` CONTROLLINO_IN1`


### RTC functions
Please, make sure that you have included also the SPI library in your sketch.
So, you can see `#include <SPI.h>` at the top of your sketch. 
It should be added automatically with the `#include <Controllino.h>`.

**Do not forget to properly setup the mechanical switch at your MINI!**

##### Initializes RTC library, SPI bus and RTC chip (RV-2123)
 *  @param aChipSelect is ignored. Kept for backwards compatibility only.
 *  @return Always returns 0
 *  
`char Controllino_RTC_init(unsigned char aChipSelect)`

##### Sets time and date to the RTC chip (RV-2123)
 *  See [RTC chip manual](http://www.microcrystal.com/images/_Product-Documentation/02_Oscillator_&_RTC_Modules/02_Application_Manual/RV-2123-C2_App-Manual.pdf) for more information.
 *  @param aDay day 01 - 31
 *  @param aWeekDay weekday 00 - 06
 *  @param aMonth month 01 - 12
 *  @param aYear year 00 - 99
 *  @param aHour hours 01 - 12, or 00 - 23 (depending of 12H/24H mode)
 *  @param aMinute minutes 00 - 59
 *  @param aSecond seconds 00 - 59
 *  @return Returns 0 when succeeded, or -1 if the RTC library was not initialized before
 *  
`char Controllino_SetTimeDate(unsigned char aDay, unsigned char aWeekDay,unsigned char aMonth, unsigned char aYear, unsigned char aHour, unsigned char aMinute, unsigned char aSecond);`

##### Reads out the time and date from the RTC chip (RV-2123)
 *  See [RTC chip manual](http://www.microcrystal.com/images/_Product-Documentation/02_Oscillator_&_RTC_Modules/02_Application_Manual/RV-2123-C2_App-Manual.pdf) for more information.
 *  @param aDay pointer to day 01 - 31
 *  @param aWeekDay pointer to weekday 00 - 06
 *  @param aMonth pointer to month 01 - 12
 *  @param aYear pointer to year 00 - 99
 *  @param aHour pointer to hours 01 - 12, or 00 - 23 (depending of 12H/24H mode)
 *  @param aMinute pointer to minutes 00 - 59
 *  @param aSecond pointer to seconds 00 - 59
 *  @return Returns 0 when succeeded, or -1 if the RTC library was not initialized before
 *  
`char Controllino_ReadTimeDate(unsigned char *aDay, unsigned char *aWeekDay, unsigned char *aMonth, unsigned char *aYear, unsigned char *aHour, unsigned char *aMinute, unsigned char *aSecond)`

##### Reads out the day value from the RTC chip (RV-2123)
 *  @return Returns day 01 - 31, or -1 if the RTC library was not initialized before
 *  
`char Controllino_GetDay( void )`

##### Reads out the weekday value from the RTC chip (RV-2123)
 *  @return Returns weekday 00 - 06, or -1 if the RTC library was not initialized before
 *  
`char Controllino_GetWeekDay( void )`

#####  Reads out the month value from the RTC chip (RV-2123)
 *  @return Returns month 01 - 12, or -1 if the RTC library was not initialized before
 *  
`char Controllino_GetMonth( void )`

##### Reads out the year value from the RTC chip (RV-2123)
 *  @return Returns year 00 - 99, or -1 if the RTC library was not initialized before
 *  
`char Controllino_GetYear( void )`

##### Reads out the hours value from the RTC chip (RV-2123)
 *  @return Returns hours 01 - 12, or 00 - 23 (depending of 12H/24H mode), or -1 if the RTC library was not initialized before
 *  
`char Controllino_GetHour( void )`

##### Reads out the minutes value from the RTC chip (RV-2123)
 *  @return Returns minutes 00 - 59, or -1 if the RTC library was not initialized before
 *  
`char Controllino_GetMinute( void )`

##### Reads out the seconds value from the RTC chip (RV-2123)
 *  @return Returns seconds 00 - 59, or -1 if the RTC library was not initialized before
 *  
`char Controllino_GetSecond( void )`

##### Reads time and date from RTC chip and prints it on serial line
 *  This function expects that the Serial was initialized before calling it.
 *  Format is DD/MM/YY   HH:MM:SS
 *  @return Returns seconds 0, or -1 if the RTC library was not initialized before
 *  
`char Controllino_PrintTimeAndDate( void )`


### RS485 functions

Please note that RS485 interface is present ony in MAXI and MEGA variants.

##### Initialization of the RS485 bus
 *  Serial3 still needs to be initialized separately. This only inits RE and DE pins.
 *  @return Always returns 0 
 *  
`char Controllino_RS485Init( void )`

##### Control of RS485 bus RE signal 
 *  @param mode 0 for RS485 Receive Enable Active, 1 for Receive Enable Inactive 
 *  @return Returns 0 when succeeded, -1 for unsupported mode
 *  
`char Controllino_SwitchRS485RE(char mode)`

##### Control of RS485 bus DE signal 
 *  @param mode 0 for RS485 Data Transmission Enable Inactive, 1 for Data Transmission Enable Active
 *  @return Returns 0 when succeeded, -1 for unsupported mode
 *  
`char Controllino_SwitchRS485DE(char mode)`

## Installation guide

1. General requirements
 * PC with Arduino IDE (1.6.4 or newer) (Windows, Linux, MAC)
 * Internet connection

2. CONTROLLINO library
 * Start Arduino IDE, navigate to Sketch–>Include Library–>Manage Libraries
 * In the Library Manager type CONTROLLINO into the filter text box and search for CONTROLLINO library
 * When found, select the latest version and install it. The installation process should be fully automated
 * When finished - check in Sketch–>Include Library menu that you can see the CONTROLLINO library there
 * You can also check if you can see the set of CONTROLLINO examples in File->Examples->CONTROLLINO

3. CONTROLLINO boards hardware definition
 * Navigate to File–>Preferences 
 * Copy-paste the following link to the Additional Boards Manager URLs: 
https://raw.githubusercontent.com/CONTROLLINO-PLC/CONTROLLINO_Library/master/Boards/package_ControllinoHardware_index.json
 * Press OK button
 * Then navigate to Tools–>Board: “Foo“–>Boards Manager
 * In the Boards Manager type CONTROLLINO into the filter text box and search for CONTROLLINO boards 
 * When found, select the latest version and install it. The installation process should be fully automated
 * When finished - check in Tools–>Board: “Foo“–> menu that you can see the CONTROLLINO boards there