#include <Controllino.h>  /* Usage of CONTROLLINO library allows you to use CONTROLLINO_xx aliases in your sketch. */
#include "ModbusRtu.h"    /* Usage of ModBusRtu library allows you to implement the Modbus RTU protocol in your sketch. */
/*
   CONTROLLINO - Modbus RTU protocol Master example for MAXI and MEGA, Version 01.00
   
   The sketch is relevant only for CONTROLLINO variants MAXI and MEGA (because of necessity of RS485 interface)!

   This sketch is intended as an example of the communication between devices via RS485 with utilization the ModbusRTU protocol.
   In this example the CONTROLLINO  is used as the Modbus master!
   For more information about the Modbus protocol visit the website: http://modbus.org/
   
   Modbus master device periodically reads Modbus 16bit registers (provided by the slave) and prints the current state to the debug Serial:
    0 - analog CONTROLLINO_A0 value (0 - 1024)
    1 - digital CONTROLLINO_D0 value (0/1)
    2 - Modbus messages received
    3 - Modbus messages transmitted

   Modbus master device periodically writes and toggles Modbus 16bit registers:
    4 - relay CONTROLLINO_R0 (0/1)
    5 - relay CONTROLLINO_R1 (0/1)
    6 - relay CONTROLLINO_R2 (0/1)
    7 - relay CONTROLLINO_R3 (0/1)

   To easily evaluate this example you need a second CONTROLLINO as Modbus slave running DemoModbusRTUSlave example sketch.
   Please note that both CONTROLLINOs need 12/24V external supply and you need to interconnect GND, -, + signals of RS485 screw terminal.

   Modbus Master-Slave library for Arduino (ModbusRtu.h) was taken from the website: https://github.com/smarmengol/Modbus-Master-Slave-for-Arduino
   It was necessary to modify setting of the PORTJ for pins DE and RE control. These pins are located at the PORJ and on the pins PIN6(DE) and PIN5(RE).

  IMPORTANT INFORMATION!
  Please, select proper target board in Tools->Board->Controllino MAXI/MEGA before Upload to your CONTROLLINO.
  (Please, refer to https://github.com/CONTROLLINO-PLC/CONTROLLINO_Library if you do not see the CONTROLLINOs in the Arduino IDE menu Tools->Board.)

  Created 30 March 2017
  by David

  https://controllino.biz/

  (Check https://github.com/CONTROLLINO-PLC/CONTROLLINO_Library for the latest CONTROLLINO related software stuff.)
*/

// Serial configuration settings
#define SerialBaudRate 19200
#define SerialConfig SERIAL_8N1 // 8 bits de datos parity:even 1 stop bit

// This MACRO defines Modbus master address.
// For any Modbus slave devices are reserved addresses in the range from 1 to 247.
// Important note only address 0 is reserved for a Modbus master device!
#define MasterModbusAdd  0
#define SlaveModbusAdd  1

// The object ControllinoModbuSlave of the class Modbus is initialized with three parameters.
// The parameter specifies the address of the Modbus device.
Modbus ControllinoModbusMaster(MasterModbusAdd); // Default serial interface is Serial3 where is located the RS485 on MAXI and MEGA models.

// This uint16 array specified internal registers in the Modbus slave device.
// Each Modbus device has particular internal registers that are available for the Modbus master.
// In this example sketch internal registers are defined as follows:
// (ModbusSlaveRegisters 0 - 3 read only and ModbusSlaveRegisters 4 - 7 write only from the Master perspective):
// ModbusSlaveRegisters[0] - Read an analog value from the CONTROLLINO_A0 - returns value in the range from 0 to 1023.
// ModbusSlaveRegisters[1] - Read an digital value from the CONTROLLINO_D0 - returns only the value 0 or 1.
// ModbusSlaveRegisters[2] - Read the number of incoming messages - Communication diagnostic.
// ModbusSlaveRegisters[3] - Read the number of number of outcoming messages - Communication diagnostic.
// ModbusSlaveRegisters[4] - Sets the Relay output CONTROLLINO_R0 - only the value 0 or 1 is accepted.
// ModbusSlaveRegisters[5] - Sets the Relay output CONTROLLINO_R1 - only the value 0 or 1 is accepted.
// ModbusSlaveRegisters[6] - Sets the Relay output CONTROLLINO_R2 - only the value 0 or 1 is accepted.
// ModbusSlaveRegisters[7] - Sets the Relay output CONTROLLINO_R3 - only the value 0 or 1 is accepted.
uint16_t ModbusSlaveRegisters[8];

// This is an structe which contains a query to an slave device
modbus_t ModbusQuery[2];

uint8_t myState; // machine state
uint8_t currentQuery; // pointer to message query

unsigned long WaitingTime;

void setup() {
   // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Serial.println("-----------------------------------------");
  Serial.println("CONTROLLINO Modbus RTU Master Test Sketch");
  Serial.println("-----------------------------------------");
  Serial.println("");
  // ModbusQuery 0: read registers
  ModbusQuery[0].u8id = SlaveModbusAdd; // slave address
  ModbusQuery[0].u8fct = 3; // function code (this one is registers read)
  ModbusQuery[0].u16RegAdd = 0; // start address in slave
  ModbusQuery[0].u16CoilsNo = 4; // number of elements (coils or registers) to read
  ModbusQuery[0].au16reg = ModbusSlaveRegisters; // pointer to a memory array in the CONTROLLINO

  // ModbusQuery 1: write a single register
  ModbusQuery[1].u8id = SlaveModbusAdd; // slave address
  ModbusQuery[1].u8fct = 6; // function code (this one is write a single register)
  ModbusQuery[1].u16RegAdd = 4; // start address in slave
  ModbusQuery[1].u16CoilsNo = 1; // number of elements (coils or registers) to write
  ModbusQuery[1].au16reg = ModbusSlaveRegisters+4; // pointer to a memory array in the CONTROLLINO
  ModbusSlaveRegisters[4] = 1; // initial value for the relays 
	
  ControllinoModbusMaster.begin(SerialBaudRate, SerialConfig); // Init Modbus RTU with the defined serial configs
  ControllinoModbusMaster.setTimeOut(5000); // if there is no answer in 5000 ms, roll over
  
  WaitingTime = millis() + 1000;
  myState = 0;
  currentQuery = 0; 
}

void loop() {
  switch( myState ) {
  case 0: 
    if (millis() > WaitingTime) myState++; // wait state
    break;
  case 1: 
    Serial.print("---- Sending query ");
    Serial.print(currentQuery);
    Serial.println(" -------------");
    ControllinoModbusMaster.query( ModbusQuery[currentQuery] ); // send query (only once)
    myState++;
	  currentQuery++;
	  if (currentQuery == 2) 
	    {
	      currentQuery = 0;
	    }
    break;
  case 2:
    ControllinoModbusMaster.poll(); // check incoming messages
    if (ControllinoModbusMaster.getState() == COM_IDLE) 
      {
        // response from the slave was received
        myState = 0;
        WaitingTime = millis() + 1000; 
        // debug printout
        if (currentQuery == 0)
          {
            // registers write was proceed
            Serial.println("---------- WRITE RESPONSE RECEIVED ----");
            Serial.println("");
          }
        if (currentQuery == 1)
          {
            // registers read was proceed
            Serial.println("---------- READ RESPONSE RECEIVED ----");
            Serial.print("Slave ");
            Serial.print(SlaveModbusAdd, DEC); 
            Serial.print(" ADC0: 0x");
            Serial.print(ModbusSlaveRegisters[0], HEX);
            Serial.print(" , Digital0: ");
            Serial.print(ModbusSlaveRegisters[1], BIN);
            Serial.print(" , ModbusCounterIn: ");
            Serial.print(ModbusSlaveRegisters[2], DEC);
            Serial.print(" , ModbusCounterOut: ");
            Serial.println(ModbusSlaveRegisters[3], DEC);
            Serial.println("-------------------------------------");
            Serial.println("");

            // toggle with the relays
            ModbusQuery[1].u16RegAdd++;
            if (ModbusQuery[1].u16RegAdd == 8)
              {
                ModbusQuery[1].u16RegAdd = 4;
                if (ModbusSlaveRegisters[4]) 
                  {
                    ModbusSlaveRegisters[4] = 0;
                  }
                  else 
                  {
                    ModbusSlaveRegisters[4] = 1;
                  }
              }
         }
    }
    break;
  }
}

/* End of the example. Visit us at https://controllino.biz/ or contact us at info@controllino.biz if you have any questions or troubles. */

/* 2017-03-31: The sketch was successfully tested with Arduino 1.8.1, Controllino Library 1.1.0 and CONTROLLINO MAXI and MEGA. */


