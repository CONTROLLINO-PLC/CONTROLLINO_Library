/** @file Controllino.cpp
 *  @brief CONTROLLINO PLC extension for the Arduino IDE - implementation
 *
 *  CONTROLLINO - First software open-source PLC (ARDUINO compatible)
 *  
 *  https://controllino.biz/
 *  https://github.com/CONTROLLINO-PLC/CONTROLLINO_Library
 *  https://www.arduino.cc/
 *
 *  @author CONTROLLINO design team
 *  @version   3.0.5
 *  @date      2018-11-15
 *  @bug No known bugs.
 */


#include "Controllino.h"
#include <stdint.h>

boolean isRTCInitialized = false;

char Controllino_RTC_init(unsigned char aChipSelect)
{
	return Controllino_RTC_init();
}

char Controllino_RTC_init( void )
{
	unsigned char SPISetting; // variable to hold SPI setting
	//Store current SPI setting  
	SPI.begin();
    SPISetting = SPCR;
	Controllino_RTCSSInit(); 
    // start the SPI library:
	SPI.begin();
	SPI.setBitOrder(MSBFIRST);
	SPI.setDataMode(SPI_MODE0); 
    // SW reset of RTC
    Controllino_SetRTCSS(HIGH);
    SPI.transfer(0x10); //0x10 is Control_1 register
	SPI.transfer(B01011000);    // SW reset command
	Controllino_SetRTCSS(LOW);
	//Return the SPI settings to previous state
	SPCR = SPISetting;
    isRTCInitialized = true;
	return 0;
}

#define ARRAYSIZE 7 //Six time values and skipped index
//=====================================
char Controllino_SetTimeDate(unsigned char aDay, unsigned char aWeekDay,unsigned char aMonth, unsigned char aYear, unsigned char aHour, unsigned char aMinute, unsigned char aSecond)
{
	unsigned char TimeDate [ARRAYSIZE]={aSecond,aMinute,aHour,aDay,aWeekDay,aMonth,aYear}; //we use a zero in the middle of the array as thats how the data are stored inside the the chip
	unsigned char i,a,b,temp;//Help variable i is used to control for cycle and a,b are used to prepare time data for the RTC chip.
	unsigned char SPISetting; // variable to hold SPI setting
	if (isRTCInitialized)
	{
	//Store SPI setting
	SPISetting = SPCR;
	// start the SPI library:
	SPI.begin();
	SPI.setBitOrder(MSBFIRST);
	SPI.setDataMode(SPI_MODE0);
	//go through all the time values
	for(i = 0; i < ARRAYSIZE;i++)
	{
		if (i == 0)    //seconds
		{
			b = TimeDate[i]/40; //get the 40s
			b = b<<6;
			a = TimeDate[i]%40; // get the rest
			temp = a/20; //get the 20s
			temp = temp<<5;
			b = b+temp;
			a = a%20; // get the rest
			temp = a/10; //get the 10s
			temp = temp<<4;
			b = b+temp;
			a = a%10; // get the rest
		}
		
		if (i == 1)    //minutes
		{
			b = TimeDate[i]/40; //get the 40s
			b = b<<6;
			a = TimeDate[i]%40; // get the rest
			temp = a/20; //get the 20s
			temp = temp<<5;
			b = b+temp;
			a = a%20; // get the rest
			temp = a/10; //get the 10s
			temp = temp<<4;
			b = b+temp;
			a = a%10; // get the rest
		}
		
		if (i == 2)    //hours
		{
			b = TimeDate[i]/10; //get the 10s
			b = b<<4;
			a = TimeDate[i]%10; // get the rest
			
		}
		
		if (i == 3)    //days
		{
			b = TimeDate[i]/20; //get the 20s
			b = b<<5;
			a = TimeDate[i]%20; // get the rest
			temp = a/10; //get the 10s
			temp = temp<<4;
			b = b+temp;
			a = a%10; // get the rest
		}
		
		if (i == 4)    //weekdays
		{
			b = 0;
			a = TimeDate[i];
		}
		
		if (i == 5)    //months
		{
			b = TimeDate[i]/10; //get the 10s
			b = b<<4;
			a = TimeDate[i]%10; // get the rest
		}
		
		if (i == 6)    //years
		{
			b = TimeDate[i]/80; //get the 80s
			b = b<<7;
			a = TimeDate[i]%80; // get the rest
			temp = a/40; //get the 40s
			temp = temp<<6;
			b = b+temp;
			a = TimeDate[i]%40; // get the rest
			temp = a/20; //get the 20s
			temp = temp<<5;
			b = b+temp;
			a = a%20; // get the rest
			temp = a/10; //get the 10s
			temp = temp<<4;
			b = b+temp;
			a = a%10; // get the rest
		}
    
    
		TimeDate[i]= a + b;   
		Controllino_SetRTCSS(HIGH);
		SPI.transfer(i + 0x12); //0x12 is starting address for write 
		SPI.transfer(TimeDate[i]);
		Controllino_SetRTCSS(LOW);
	}
	//Return the SPI settings to previous state
	SPCR = SPISetting;
	return 0; 
	}
	else
	{
		return -1; //RTC chip was initialized properly, return -1
	}
}

char Controllino_ReadTimeDate(unsigned char *aDay, unsigned char *aWeekDay, unsigned char *aMonth, unsigned char *aYear, unsigned char *aHour, unsigned char *aMinute, unsigned char *aSecond)
{
	unsigned char i,a,b,c,d,e; //help variables a,b are used to store modified time information for a while during readout, and i is used for for cycle
	unsigned int n; //n stores exact unmodified recieved data from the chip
	int timeDate [ARRAYSIZE]; //second,minute,hour,weekday,day,month,year
	unsigned char SPISetting; // variable to hold SPI setting	
	if (isRTCInitialized)
	{
		//Store SPI setting
		SPISetting = SPCR;
		// start the SPI library:
		SPI.begin();
		SPI.setBitOrder(MSBFIRST);
		SPI.setDataMode(SPI_MODE0);
		for(i = 0; i < ARRAYSIZE;i++)
		{
			Controllino_SetRTCSS(HIGH);
			SPI.transfer(i + 0x92); //0x92 is starting address for read
			n = SPI.transfer(0x00);       
			Controllino_SetRTCSS(LOW);
			a = n & B00001111;   
			if(i == 2) //hours, 24 hours mode
			{  
				c = (n & B00100000)>>5;
                b = (n & B00010000)>>4;
				timeDate[i] = a + (b * 10) + (c * 20);
			}
			else if(i == 3) //day
			{ 
				
				c = (n & B00100000)>>5;
				b = (n & B00010000)>>4;
				timeDate[i] = a + (b * 10) + (c * 20);
       
			}
			else if(i == 4) //weekday
			{ 
				timeDate[i] = a;
			}
			else if(i == 5) //month
			{ 
				b = (n & B00010000)>>4;
				timeDate[i] = a + (b * 10);
			}
			else if(i == 6) //year
			{ 
				e = (n & B10000000)>>7;
				d = (n & B01000000)>>6;
				c = (n & B00100000)>>5;
				b = (n & B00010000)>>4;
				timeDate[i] = a + (b * 10) + (c * 20) + (d * 40) + (e * 80);
			}
			else //minute,second
			{  
				d = (n & B01000000)>>6;
				c = (n & B00100000)>>5;
				b = (n & B00010000)>>4;
				timeDate[i] = a + (b * 10) + (c * 20) + (d * 40);
			}
		}
		//if supplied pointer was NOT NULL, return the recieved data
		if (aDay != NULL)
		{
			*aDay = timeDate[3];
		}
		
		if (aWeekDay != NULL)
		{
			*aWeekDay = timeDate[4];
		}
		
		if (aMonth != NULL)
		{
			*aMonth = timeDate[5];
		}
		
		if (aYear != NULL)
		{
			*aYear = timeDate[6];
		}
		
		if (aHour != NULL)
		{
			*aHour = timeDate[2];
		}
		
		if (aMinute != NULL)
		{
			*aMinute = timeDate[1];
		}
		
		if (aSecond != NULL)
		{
			*aSecond = timeDate[0];
		}
		//Return the SPI settings to previous state
		SPCR = SPISetting;
		return 0;
	}
	else
	{
		return -1;//RTC chip was initialized properly, return -1
	}
}

char Controllino_GetDay( void )
{
	unsigned char day;
	if (Controllino_ReadTimeDate(&day, NULL, NULL, NULL, NULL, NULL, NULL) >= 0)
	{		
		return day;
	}
	else
	{
		return -1;//RTC chip was initialized properly, return -1
	}
}

char Controllino_GetWeekDay( void )
{
	unsigned char weekday;
	if (Controllino_ReadTimeDate(NULL, &weekday, NULL, NULL, NULL, NULL, NULL) >= 0)
	{		
		return weekday;
	}
	else
	{
		return -1;//RTC chip was initialized properly, return -1
	}
}

char Controllino_GetMonth( void )
{
	unsigned char month;
	if (Controllino_ReadTimeDate(NULL, NULL, &month, NULL, NULL, NULL, NULL) >= 0)
	{
		return month;
	}
	else
	{
		return -1;//RTC chip was initialized properly, return -1
	}
}

char Controllino_GetYear( void )
{
	unsigned char year;
	if (Controllino_ReadTimeDate(NULL, NULL, NULL, &year, NULL, NULL, NULL) >= 0)
	{
		return year;
	}
	else
	{
		return -1;//RTC chip was initialized properly, return -1
	}
}

char Controllino_GetHour( void )
{
	unsigned char hour;
	if (Controllino_ReadTimeDate(NULL, NULL, NULL, NULL, &hour, NULL, NULL) >= 0)
	{	
		return hour;
	}
	else
	{
		return -1;//RTC chip was initialized properly, return -1
	}
}


char Controllino_GetMinute( void )
{
	unsigned char minute;
	if (Controllino_ReadTimeDate(NULL, NULL, NULL, NULL, NULL, &minute, NULL) >= 0)
	{		
		return minute;
	}
	else
	{
		return -1;//RTC chip was initialized properly, return -1
	}
}

char Controllino_GetSecond( void )
{
	unsigned char second;
	if (Controllino_ReadTimeDate(NULL, NULL, NULL, NULL, NULL, NULL, &second) >= 0)
	{	
		return second;
	}
	else
	{
		return -1;//RTC chip was initialized properly, return -1
	}
}

char Controllino_PrintTimeAndDate( void )
{
	unsigned char day, weekday, month, year, hour, minute, second;
	if (Controllino_ReadTimeDate(&day, &weekday, &month, &year, &hour, &minute, &second) >= 0)
	{
		Serial.print(day);
		Serial.print("/");
		Serial.print(month);
		Serial.print("/");
		Serial.print(year);
		Serial.print("   ");
		Serial.print(hour);
		Serial.print(":");
		Serial.print(minute);
		Serial.print(":");
		Serial.println(second);
		return 0;
	}
	else
	{
		return -1;//RTC chip was initialized properly, return -1
	}
}

/* This function is taken from https://github.com/adafruit/RTClib */
/* Released to the public domain! Enjoy! */
static uint8_t conv2d(const char* p)
{
    uint8_t v = 0;
    if ('0' <= *p && *p <= '9')
        v = *p - '0';
    return 10 * v + *++p - '0';
}

const uint8_t daysInMonth []  = { 31,28,31,30,31,30,31,31,30,31,30,31 };
/* This function is taken from https://github.com/adafruit/RTClib */
/* Released to the public domain! Enjoy! */
/* number of days since 2000/01/01, valid for 2001..2099 */
static uint16_t date2days(uint16_t y, uint8_t m, uint8_t d)
{
    if (y >= 2000)
        y -= 2000;
    uint16_t days = d;
    for (uint8_t i = 1; i < m; ++i)
        days += *(daysInMonth + i - 1);
    if (m > 2 && y % 4 == 0)
        ++days;
    return days + 365 * y + (y + 3) / 4 - 1;
}

/* This function is taken from https://github.com/adafruit/RTClib */
/* Released to the public domain! Enjoy! */
uint8_t dayOfTheWeek(uint8_t yOff,uint8_t  m, uint8_t d)
{    
    uint16_t day = date2days(yOff + 2000, m, d);
    return (day + 6) % 7; // Jan 1, 2000 is a Saturday, i.e. returns 6
}

/* This function is inspired by https://github.com/adafruit/RTClib */
/* Released to the public domain! Enjoy! */
char Controllino_SetTimeDateStrings(const char* date, const char* time)
{
    // sample input: date = "Dec 26 2009", time = "12:34:56"
    uint8_t yOff, m, d, hh, mm, ss;
    
    yOff = conv2d(date + 9);
    // Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec 
    switch (date[0]) {
        case 'J': m = (date[1] == 'a') ? 1 : ((date[2] == 'n') ? 6 : 7); break;
        case 'F': m = 2; break;
        case 'A': m = date[2] == 'r' ? 4 : 8; break;
        case 'M': m = date[2] == 'r' ? 3 : 5; break;
        case 'S': m = 9; break;
        case 'O': m = 10; break;
        case 'N': m = 11; break;
        case 'D': m = 12; break;
    }
    d = conv2d(date + 4);
    hh = conv2d(time);
    mm = conv2d(time + 3);
    ss = conv2d(time + 6);

    return Controllino_SetTimeDate(d,dayOfTheWeek(yOff, m, d),m,yOff,hh,mm,ss);
}

#if defined(CONTROLLINO_MAXI) || defined(CONTROLLINO_MEGA)

char Controllino_RS485Init( void )
{

	pinMode(CONTROLLINO_RS485_DE, OUTPUT);
  pinMode(CONTROLLINO_RS485_nRE, OUTPUT);

  Controllino_RS485RxEnable();
	
	return 0;
}

char Controllino_RS485Init( long aBaudrate )
{
  
  Controllino_RS485Init();
  /* Initialize serial port for RS485 communication */
  Serial3.begin(aBaudrate);
  
  return 0;
}

void Controllino_RS485TxEnable( void )
{
  /* disable receiver, enable transmitter */
  digitalWrite(CONTROLLINO_RS485_DE, HIGH);
  digitalWrite(CONTROLLINO_RS485_nRE, HIGH);

  return;
}

void Controllino_RS485RxEnable( void )
{
  /* disable trasmitter, enable receiver */
  digitalWrite(CONTROLLINO_RS485_DE, LOW);
  digitalWrite(CONTROLLINO_RS485_nRE, LOW);

  return;
}


/* obsolete way of RE pin handling */
char Controllino_SwitchRS485RE(char mode)
{
	if (mode == 0)
	{
		// set RE on LOW
		PORTJ &= B11011111;
	}
	else if (mode == 1)
		{
			// set RE on HIGH
			PORTJ |= B00100000;
		}
		else return -1; // Unknown mode value
	return 0;
}

/* obsolete way of DE pin handling */
char Controllino_SwitchRS485DE(char mode)
{
	if (mode == 0)
	{
		// set DE on LOW
		PORTJ &= B10111111;
	}
	else if (mode == 1)
		{
			// set DE on HIGH
			PORTJ |= B01000000;
		}
		else return -1; // Unknown mode value
	return 0;
}
#endif

char Controllino_RTCSSInit( void )
{
	#if defined(CONTROLLINO_MAXI) || defined(CONTROLLINO_MEGA) || defined(CONTROLLINO_MAXI_AUTOMATION)
	
    pinMode(CONTROLLINO_RTC_CHIP_SELECT, OUTPUT);
    pinMode(CONTROLLINO_ETHERNET_CHIP_SELECT, OUTPUT);
    
    digitalWrite(CONTROLLINO_RTC_CHIP_SELECT, LOW);       // inactive
    digitalWrite(CONTROLLINO_ETHERNET_CHIP_SELECT, HIGH); // inactive

	 return 0;
	#endif
	#if defined(CONTROLLINO_MINI)
	 pinMode(CONTROLLINO_PIN_HEADER_SS,OUTPUT);
	 digitalWrite(CONTROLLINO_PIN_HEADER_SS, LOW);
	 return 0;
	#endif
	 return -2; // No Controllino board is selected
}

char Controllino_SetRTCSS(char mode)
{
	#if defined(CONTROLLINO_MAXI) || defined(CONTROLLINO_MEGA) || defined(CONTROLLINO_MAXI_AUTOMATION)
  	if (mode == 0)
  	{
  		digitalWrite(CONTROLLINO_RTC_CHIP_SELECT, LOW);       // inactive
  	}
  	else if (mode == 1)
  		{
  			digitalWrite(CONTROLLINO_RTC_CHIP_SELECT, HIGH);       // inactive
  		}
  	else return -1; // Unknown mode value
  	return 0;
	#endif
	#if defined(CONTROLLINO_MINI)
  	if (mode == 0)
  	{
  		// set RTC SS on LOW
  		digitalWrite(CONTROLLINO_PIN_HEADER_SS, LOW);
  	}
  	else if (mode == 1)
  		{
  			// set RTC SS on HIGH
  			digitalWrite(CONTROLLINO_PIN_HEADER_SS, HIGH);
  		}
  	else return -1; // Unknown mode value
	  return 0;
	#endif
  	return -2; // No Controllino board is selected
}

