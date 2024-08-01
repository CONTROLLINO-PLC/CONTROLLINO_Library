// This sketch configures and manages a Modbus RTU server using the Controllino Mega to facilitate communication  
// between digital and analog inputs/outputs via the Modbus protocol.  


// In this code, consecutive series of digital inputs and outputs are associated with Modbus registers of the   
// digital inputs and coils types, respectively. The same is done for analog inputs and outputs concerning   
// the input registers and holding registers. Both digital inputs and outputs of each type are selected   
// consecutively, making it possible to access all of them using the labels defined in the Controllino.h class   
// and to traverse them easily through a for loop. For example, a total of 11 outputs CONTROLLINO_A0-CONTROLLINO_A10   
// are used, which represent pins 54 to 64 of the Controllino. In this way, it is possible to access the   
// device's inputs and outputs remotely via Modbus in a practical and standardized manner. 

#include <Controllino.h>  
#include <ArduinoRS485.h>  
#include <ArduinoModbus.h>  

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

void setup()  
{  
  // Begin serial communication at a baud rate of 9600 for debugging purposes  
  Serial.begin(9600);  

  // Start the Modbus RTU Server with Slave ID 1 and baud rate of 9600  
  if (!ModbusRTUServer.begin(1, 9600))  
  {  
    Serial.println("Error starting Modbus RTU Server!");  // Log error if server fails to start  
    while (1);  // Stop execution if Modbus server initialization fails  
  }  

  // Configure the Modbus RTU Server's coil and register mapping  
  ModbusRTUServer.configureCoils(0x00, NUMBER_INPUTS_COILS); // Set up coils starting at address 0 with the specified number  
  ModbusRTUServer.configureDiscreteInputs(0x00, NUMBER_INPUTS_COILS); // Configure discrete inputs for monitoring  
  ModbusRTUServer.configureHoldingRegisters(0x00, NUMBER_IN_HOLD_REGISTERS); // Set up holding registers for PWM outputs  
  ModbusRTUServer.configureInputRegisters(0x00, NUMBER_IN_HOLD_REGISTERS); // Configure input registers for analog readings  

  // Setup digital input and output pins for the specified number of coils  
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

void loop()  
{  
  // Continuously check for incoming Modbus requests and respond accordingly  
  int packetReceived = ModbusRTUServer.poll();  
  if (packetReceived)  
  {  
    // Process each digital coil and update corresponding outputs  
    for (int i = 0; i < NUMBER_INPUTS_COILS; i++)  
    {  
      // Read the state of the corresponding coil and update the output pin state  
      bool coilValue = ModbusRTUServer.coilRead(i);  
      digitalWrite(CONTROLLINO_R0 + i, coilValue ? HIGH : LOW); // Set the output pin based on coil state  
      
      // Read the state of the digital input pin and update the corresponding coil register  
      bool inputState = digitalRead(CONTROLLINO_A0 + i);  
      ModbusRTUServer.coilWrite(i, inputState ? 1 : 0); // Update coil state based on input pin value  
    }  

    // Update the analog output pins and input registers based on the holding registers  
    for (int i = 0; i < NUMBER_IN_HOLD_REGISTERS; i++)  
    {  
      // Write the value stored in the holding register to the corresponding PWM output  
      analogWrite(CONTROLLINO_D0 + i, ModbusRTUServer.holdingRegisterRead(i));  

      // Read the value from the corresponding analog input and update the input register  
      ModbusRTUServer.inputRegisterWrite(i, analogRead(FIRST_ANALOG_INPUT_PIN + i)); // Write the analog value to input register  
    }  
  }  
}  