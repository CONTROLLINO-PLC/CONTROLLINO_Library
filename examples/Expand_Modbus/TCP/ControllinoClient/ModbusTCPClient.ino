/*
  Ethernet Modbus TCP Client Toggle

  This sketch toggles the coil of a Modbus TCP server connected
  on and off every second.

  Circuit:
   - Any Arduino MKR Board
   - MKR ETH Shield

  created 16 July 2018
  by Sandeep Mistry
*/
#include <Controllino.h>
#include <SPI.h>
#include <Ethernet.h>

#include <ArduinoRS485.h> // ArduinoModbus depends on the ArduinoRS485 library
#include <ArduinoModbus.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
// The IP address will be dependent on your local network:
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 1, 1);

EthernetClient ethernetClient;
ModbusTCPClient modbusTCPClient(ethernetClient);

IPAddress server(192, 168, 1, 10); // update with the IP Address of your Modbus server

// Defines the amount of consecutive pins declared for digital and analog asociation according to the device to use
// Its important to use the same type of devices in master and slave, otherwise the sinchronization will missmatch
// returning an error because the both Modbus insttances will have different registers declarations
#ifdef CONTROLLINO_MAXI
  #define NUMBER_INPUTS_COILS  5    // Number of digital input coils to track  
  #define NUMBER_IN_HOLD_REGISTERS 5 // Number of analog input registers and holding registers to manage  
  #define FIRST_ANALOG_INPUT_PIN  CONTROLLINO_A5
#endif

#ifdef CONTROLLINO_MEGA
  #define NUMBER_INPUTS_COILS  11    // Number of digital input coils to track  
  #define NUMBER_IN_HOLD_REGISTERS 5 // Number of analog input registers and holding registers to manage 
  #define FIRST_ANALOG_INPUT_PIN  CONTROLLINO_A11 
#endif



void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);

  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }

}

void loop() {
  if (!modbusTCPClient.connected()) {
    // client not connected, start the Modbus TCP client
    Serial.println("Attempting to connect to Modbus TCP server");
    
    if (!modbusTCPClient.begin(server, 502)) {
      Serial.println("Modbus TCP Client failed to connect!");
    } else {
      Serial.println("Modbus TCP Client connected");
    }
  } else {
    // Once client is connected
 
    // Declare arrays to hold the state of digital inputs from the slave and holding registers  
    int slaveInputs[NUMBER_INPUTS_COILS];   
    int slaveInRegisters[NUMBER_IN_HOLD_REGISTERS];  
    
    // Read Modbus Controllino slave digital inputs  
    for (int i = 0; i < NUMBER_INPUTS_COILS; i++)   
    {  
      // Read discrete inputs from the slave device  
      slaveInputs[i] = modbusTCPClient.discreteInputRead(i);  
      
      // Check for errors while reading inputs  
      if (slaveInputs[i] < 0) {  
        Serial.print("Error reading slave input A");  
        Serial.print(i);  
        Serial.println("!"); // Log error message if reading fails  
      }  
      
      // Synchronize the state of slave inputs to the corresponding master outputs  
      digitalWrite(CONTROLLINO_R0 + i, slaveInputs[i]);  

      // Write the current state of the master digital input to the slave output  
      if (!modbusTCPClient.coilWrite( i, digitalRead(CONTROLLINO_A0 + i))) {  
        Serial.print("Error writing slave output R");  
        Serial.print(i);  
        Serial.println("!"); // Log error message if writing fails  
      }  
    }  

    // Read and synchronize holding registers with the slave device  
    for (int i = 0; i < NUMBER_IN_HOLD_REGISTERS; i++)   
    {  
      // Read input registers from the slave device  
      slaveInRegisters[i] = modbusTCPClient.inputRegisterRead(i);  
      
      // Check for errors while reading input registers  
      if (slaveInRegisters[i] < 0) {  
        Serial.print("Error reading slave input register A");  
        Serial.print(i + 11); // Offset by 11 to match the register numbering  
        Serial.println("!"); // Log error message if reading fails  
      }  

      // Write the value from the slave input register to the master analog output  
      analogWrite(CONTROLLINO_D0 + i, slaveInRegisters[i]);  
      
      // Synchronize master holding registers to slave holding registers and update the corresponding analog output  
      if (!modbusTCPClient.holdingRegisterWrite( i, analogRead(CONTROLLINO_A11 + i))) {  
        Serial.print("Error writing slave holding register D");  
        Serial.print(i);  
        Serial.println("!"); // Log error message if writing fails  
      }  
    }  
  }
}
