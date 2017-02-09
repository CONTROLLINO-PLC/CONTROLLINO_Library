
##:exclamation: NEW BOARD "CONTROLLINO MAXI Automation" AVAILABLE :exclamation:

Check out the newest board [here](#controllino-maxi-automation)!

Having trouble compiling your code:question:
* Upon releasing new version major changes have been made in board 1.0.1 and it's not compatible with library 1.0.0.
* Arduino IDE has trouble deleting old board's version from your PC and it must be deleted manually.
  * (Windows) To delete boards, press Win+R and type in this path: "%LOCALAPPDATA%\Arduino15\packages" (Arduino IDE 1.6.6 and later) or "%APPDATA%\Arduino15\packages" (Arduino IDE 1.6.5r5 and previous). New Windows Explorer window will open. Delete directory "CONTROLLINO Boards". Now you can install the newest board in Arduino IDE.
  * (Linux) To delete boards, go to: "/home/(username)/.arduino15/packages" (a.k.a. ~/.arduino15/packages) and delete directory "CONTROLLINO Boards". Now you can install the newest board in Arduino IDE.
  * (Mac OS X) To delete boards, go to: "/Users/(username)/Library/Arduino15/packages" and delete directory "CONTROLLINO Boards". Now you can install the newest board in Arduino IDE.

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
* Frequently Asked Questions
  * See our [FAQ](#faq)

Please note that CONTROLLINO exists in [4 different variants](#controllino-mini).

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

### CONTROLLINO MAXI Automation
* Microcontroller: ATmega2560
* Clock Speed: 16 MHz
* RTC
* Ethernet Connector
* 2x serial Interface
* 1x I2C Interface
* 1x SPI Interface
* Input current Max. 20A
* Input voltage 24V
* 10x Relays Outputs – 230V / 6A
* 12x Analog Inputs
* 6x Digital Inputs (2x Interrupt)
* 8x Digital Outputs – 2A @ 24V
* 2x 0-10V Inputs
* 2x 0-10V/0-20mA Outputs

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
 
 
## FAQ

**1. Automated installation through Arduino Board Manager gets 403 Forbidden in Arduino IDE for OSX**

*Answer: This issue was reported for Arduino IDE 1.6.11. In older versions - e.g. 1.6.8 it worked fine. With the latest Arduino IDE version (1.8.1) it also works fine.* 

**2. AnalogRead always returns 806 instead of 1023**

Question: A0 should read 0 and 1023 (its connected to D0 pin), but instead it reads 0 and 806. Is there anything wrong with the voltage reference or am I missing something? Btw, power supply is 24 volts.

*Answer: This is caused by the fact that CONTROLLINO actually doesn't have maximum of 24V. The maximum for 24V setting is close to 30V. So, value 1023 will be returned when you would get close to 30V power supply. See our example AnalogInputs.*

**3. Digital 20 - Digital 23 mapping**

Question: Pins Digital 20 - Digital 23 seem not to be implemented to use with digitalWrite, from Arduino lib. Is it possible to use those pins with your library? I understand, that these pins don't have an according alias on arduino, but still. I would really like to somehow circumvent using this syntax in order to set Digital 20.
`PORTD = B11110111`

*Answer: It is correct. Our library does not support these digital outputs as we have tried to do not touch Arduino Core functionality. But it may be a point for further releases...
I am afraid that you have to use this "PORTD" way to control these pins. Please check example sketch PortManipulation in the Arduino IDE for more details.*

**4. CONTROLLINO boards not showing with arduino IDE 1.6.12**

Question: Loading the Controllino Library into the Arduino 1.6.12 IDE does not show up the Controllino boards on the board types list.

*Answer: You also need to install the "CONTROLLINO Hardware" by the Boards Manager.*

**5. How to burn bootloader**

Question: Is it possible to burn bootloader in Controllino? I want to add possibility for OTA updates by Ethernet.

*Answer: Yes, it is possible. We are fully compatible with Arduino! All ICSP signals are available at the X1 pinheader connector of your CONTROLLINO. See https://controllino.biz/downloads/ for more details. You just need ICSP programmer and six wires to handle that.*

**6. Unable to locate (obsolete) hardware description files on GitHub**

Question: My Arduino IDE gives me following error message: "Error downloading ....githubusercontent.com/Controllino/ControllinoHardware/master/package_ControllinoHardware_index.json". Is it unable to locate hardware description files?

*Answer: Our GitHub repository has been moved to a new location, therefore this path is no longer valid. Please, follow our new [Installation guide](#installation-guide).*

**7. Pin header SPI bus communication is not working properly (MAXI, MEGA)**

Question: I am not using RTC and/or Ethernet built in CONTROLLINO MAXI/MEGA. I am trying to use SPI bus available at pinheader, but it sometimes work and sometimes not. Is there any conflict with RTC chip and Etherent chip connected to the same SPI bus, even if I am not using them?

*Answer: When SS pins PJ2 (RTC) and PJ3 (Wiznet) are not properly handled, it may happen that the floating pin is recognized as a valid chip select signal and the RTC chip or Wiznet chip is accidentally connected to SPI bus - MISO signal.*
*The solution is quite easy - if a user wants to use the SPI bus signals as GPIOs at the pin header, s/he must properly setup the Slave Select signals for the RTC and Wiznet.* 
*This may cause confusion of the SPI communication at the pinheader and also strange voltages at MISO (pin header X1) when using it as GPIO.*
 
*These three lines in your sketch setup function should do the job:*
 
`DDRJ   | = B00001100;` 

`PORTJ &= B11111011;`

`PORTJ | = B00001000;`

