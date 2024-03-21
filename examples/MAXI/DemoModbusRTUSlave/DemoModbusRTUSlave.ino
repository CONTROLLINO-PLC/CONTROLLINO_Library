#include <Controllino.h>  /* Usage of CONTROLLINO library allows you to use CONTROLLINO_xx aliases in your sketch. */
#include "ModbusRtu.h"    /* Usage of ModBusRtu library allows you to implement the Modbus RTU protocol in your sketch. */
/*
   CONTROLLINO - Modbus RTU protocol Slave example for MAXI and MEGA, Version 01.00

   The sketch is relevant only for CONTROLLINO variants MAXI and MEGA (because of necessity of RS485 interface)!

   This sketch is intended as an example of the communication between devices via RS485 with utilization the ModbusRTU protocol.
   In this example the CONTROLLINO  is used as the Modbus slave!
   For more information about the Modbus protocol visit the website: http://modbus.org/

   Modbus master device can read Modbus 16bit registers (provided by the slave):
    0 - analog CONTROLLINO_A0 value (0 - 1024)
    1 - digital CONTROLLINO_D0 value (0/1)
    2 - Modbus messages received
    3 - Modbus messages transmitted

   Modbus master device can write Modbus 16bit registers:
    4 - relay CONTROLLINO_R0 (0/1)
    5 - relay CONTROLLINO_R1 (0/1)
    6 - relay CONTROLLINO_R2 (0/1)
    7 - relay CONTROLLINO_R3 (0/1)

   To easily evaluate this example you need a second CONTROLLINO as Modbus master running DemoModbusRTUMaster example sketch.
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

// This MACRO defines Modbus slave address.
// For any Modbus slave devices are reserved addresses in the range from 1 to 247.
// Important note only address 0 is reserved for a Modbus master device!
#define SlaveModbusAdd  1

// The object ControllinoModbuSlave of the class Modbus is initialized with three parameters.
// The first parametr specifies the address of the Modbus slave device.
Modbus ControllinoModbusSlave(SlaveModbusAdd); // Default serial interface is Serial3 where is located the RS485 on MAXI and MEGA models.

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

// The setup function runs once when you press reset (CONTROLLINO RST button) or connect the CONTROLLINO to the PC
// In the setup function is carried out port setting and initialization of communication of the Modbus slave protocol.
void setup()
{
    pinMode(CONTROLLINO_R0, OUTPUT);    // Set the pin CONTROLLINO_R0 as output.
    pinMode(CONTROLLINO_R1, OUTPUT);    // Set the pin CONTROLLINO_R1 as output.
    pinMode(CONTROLLINO_R2, OUTPUT);    // Set the pin CONTROLLINO_R2 as output.
    pinMode(CONTROLLINO_R3, OUTPUT);    // Set the pin CONTROLLINO_R3 as output.

    pinMode(CONTROLLINO_D0, INPUT);     // Set the pin CONTROLLINO_D0 as input - Read digital value at the pin D0.
    pinMode(CONTROLLINO_A0, INPUT);     // Set the pin CONTROLLINO_A0 as input - Read analog value at the pin A0

    ControllinoModbusSlave.begin(SerialBaudRate, SerialConfig); // Init Modbus RTU with the defined serial configs
}

// The loop function runs over and over again forever
void loop()
{
    // This instance of the class Modbus checks if there are any incoming data.
    // If any frame was received. This instance checks if a received frame is Ok.
    // If the received frame is Ok the instance poll writes or reads corresponding values to the internal registers (ModbusSlaveRegisters).
    // Main parameters of the instance poll are address of the internal registers and number of internal registers.
    ControllinoModbusSlave.poll(ModbusSlaveRegisters, 8);

    // While are not received or sent any data, the Modbus slave device periodically reads the values of analog and digital outputs.
    // Also it updates the other values of registers.
    ModbusSlaveRegisters[0] = analogRead(CONTROLLINO_A0);           // Read the analog input CONTROLLINO_A0.
    ModbusSlaveRegisters[1] = digitalRead(CONTROLLINO_D0);          // Read the digital input CONTROLLINO_A0.
    ModbusSlaveRegisters[2] = ControllinoModbusSlave.getInCnt();    // Read the number of incoming messages.
    ModbusSlaveRegisters[3] = ControllinoModbusSlave.getOutCnt();   // Read the number of outcoming messages

    digitalWrite(CONTROLLINO_R0, ModbusSlaveRegisters[4]); // Update the relay output CONTROLLINO_R0 - ON/OFF
    digitalWrite(CONTROLLINO_R1, ModbusSlaveRegisters[5]); // Update the relay output CONTROLLINO_R1 - ON/OFF
    digitalWrite(CONTROLLINO_R2, ModbusSlaveRegisters[6]); // Update the relay output CONTROLLINO_R2 - ON/OFF
    digitalWrite(CONTROLLINO_R3, ModbusSlaveRegisters[7]); // Update the relay output CONTROLLINO_R3 - ON/OFF
}

/* End of the example. Visit us at https://controllino.biz/ or contact us at info@controllino.biz if you have any questions or troubles. */

/* 2017-03-31: The sketch was successfully tested with Arduino 1.8.1, Controllino Library 1.1.0 and CONTROLLINO MAXI and MEGA. */


