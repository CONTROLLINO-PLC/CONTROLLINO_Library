/*
  Ethernet Modbus TCP Server LED

  This sketch creates a Modbus TCP Server with a simulated coil.
  The value of the simulated coil is set on the LED

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
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

IPAddress ip(192, 168, 1, 10);

EthernetServer ethernetServer(502);

ModbusTCPServer modbusTCPServer;

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
  // You can use Ethernet.init(pin) to configure the CS pin
  //Ethernet.init(10);  // Most Arduino shields
  //Ethernet.init(5);   // MKR ETH shield
  //Ethernet.init(0);   // Teensy 2.0
  //Ethernet.init(20);  // Teensy++ 2.0
  //Ethernet.init(15);  // ESP8266 with Adafruit Featherwing Ethernet
  //Ethernet.init(33);  // ESP32 with Adafruit Featherwing Ethernet

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

  // start the server
  ethernetServer.begin();
  
  // start the Modbus TCP server
  if (!modbusTCPServer.begin()) {
    Serial.println("Failed to start Modbus TCP Server!");
    while (1);
  }
  
  // Configure the Modbus Server's coil and register mapping  
  modbusTCPServer.configureCoils(0x00, NUMBER_INPUTS_COILS); // Set up coils starting at address 0 with the specified number  
  modbusTCPServer.configureDiscreteInputs(0x00, NUMBER_INPUTS_COILS); // Configure discrete inputs for monitoring  
  modbusTCPServer.configureHoldingRegisters(0x00, NUMBER_IN_HOLD_REGISTERS); // Set up holding registers for PWM outputs  
  modbusTCPServer.configureInputRegisters(0x00, NUMBER_IN_HOLD_REGISTERS); // Configure input registers for analog readings  


  for (int i = 0; i < NUMBER_INPUTS_COILS; i++)  
    {  
      pinMode(CONTROLLINO_A0 + i, INPUT);  // Set digital input pins (A0 to A10) as INPUT  
      pinMode(CONTROLLINO_R0 + i, OUTPUT); // Set digital output pins (R0 to R10) as OUTPUT  
      digitalWrite(CONTROLLINO_R0 + i, LOW); // Initialize output pins to LOW (off state)  
    }  

    // Setup analog output pins for holding registers  
    for (int i = 0; i < NUMBER_IN_HOLD_REGISTERS; i++)  
    {  
      pinMode(FIRST_ANALOG_INPUT_PIN + i, INPUT); // Set analog inputs pins (A11 to A15) as INPUTS 
      pinMode(CONTROLLINO_D0 + i, OUTPUT); // Set analog output pins (D0 to D4) as OUTPUT  
      analogWrite(CONTROLLINO_D0 + i, 0); // Initialize analog outputs to minimum value (0V) 
    } 
}

   


void loop() {
  // listen for incoming clients
  EthernetClient client = ethernetServer.available();
  
  if (client) {
    // a new client connected
    Serial.println("new client");

    // let the Modbus TCP accept the connection 
    modbusTCPServer.accept(client);

    while (client.connected()) {  
      if (modbusTCPServer.poll()) // poll for Modbus TCP requests, while client connected
        executeSinchronization(); // and excetutes the sinchronization among in-out pins and modbus registers
    }

    Serial.println("client disconnected"); 
  }
}

void executeSinchronization() {

  // Process each digital coil and update corresponding outputs  
    for (int i = 0; i < NUMBER_INPUTS_COILS; i++)  
    {  
      // Read the state of the corresponding coil and update the output pin state  
      bool coilValue = modbusTCPServer.coilRead(i);  
      digitalWrite(CONTROLLINO_R0 + i, coilValue ? HIGH : LOW); // Set the output pin based on coil state  
      
      // Read the state of the digital input pin and update the corresponding coil register  
      bool inputState = digitalRead(CONTROLLINO_A0 + i);  
      modbusTCPServer.coilWrite(i, inputState ? 1 : 0); // Update coil state based on input pin value  
    }  

    // Update the analog output pins and input registers based on the holding registers  
    for (int i = 0; i < NUMBER_IN_HOLD_REGISTERS; i++)  
    {  
      // Write the value stored in the holding register to the corresponding PWM output  
      analogWrite(CONTROLLINO_D0 + i, modbusTCPServer.holdingRegisterRead(i));  

      // Read the value from the corresponding analog input and update the input register  
      modbusTCPServer.inputRegisterWrite(i, analogRead(FIRST_ANALOG_INPUT_PIN + i)); // Write the analog value to input register  
    } 



}
