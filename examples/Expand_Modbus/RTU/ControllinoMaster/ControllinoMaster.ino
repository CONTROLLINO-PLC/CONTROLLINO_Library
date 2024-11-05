// In this code, consecutive series of digital inputs and outputs are associated with Modbus registers of the  
// digital inputs and coils types, respectively. The same is done for analog inputs and outputs concerning  
// the input registers and holding registers. Both digital inputs and outputs of each type are selected  
// consecutively, making it possible to access all of them using the labels defined in the Controllino.h class  
// and to access them easily through a for loop. This makes it possible to associate the inputs and outputs of this   
// Controllino master with those of a slave that has the equivalent code created in this repository. By using both   
// codes, it is possible to easily synchronize both Controllinos using the same labels and a similar, standardized   
// program.   
 
#include <Controllino.h>  
#include <ArduinoRS485.h>  
#include <ArduinoModbus.h>  

#define CONTROLLINO_MEGA_SLAVE  1  
#define NUMBER_INPUTS_COILS  11    // Number of digital inputs and coils in both master and slave  
#define NUMBER_IN_HOLD_REGISTERS 5 // Number of input and holding registers in both master and slave  

void setup() {  
  Serial.begin(9600); // Begin serial communication at 9600 baud rate for debugging  
  while (!Serial); // Wait until the serial port is ready  

  // Setup digital input and output pins for the specified number of coils  
  for (int i = 0; i < NUMBER_INPUTS_COILS; i++)  
  {  
    pinMode(CONTROLLINO_A0 + i, INPUT);  // Configure digital input pins (A0 to A10) as INPUT  
    pinMode(CONTROLLINO_R0 + i, OUTPUT); // Configure digital output pins (R0 to R10) as OUTPUT  
    digitalWrite(CONTROLLINO_R0 + i, LOW); // Initialize output pins to LOW (off state)  
  }  

  // Setup analog input and output pins for holding registers  
  for (int i = 0; i < NUMBER_IN_HOLD_REGISTERS; i++)  
  {  
    pinMode(CONTROLLINO_A11 + i, INPUT); // Set analog input pins (A11 to A15) as INPUT  
    pinMode(CONTROLLINO_D0 + i, OUTPUT); // Set analog output pins (D0 to D4) as OUTPUT  
    analogWrite(CONTROLLINO_D0 + i, 0); // Initialize analog outputs to minimum value (0V)   
  }  
  
  Serial.println("Modbus RTU Client Toggle"); // Log message indicating the start of the Modbus RTU Client  

  // Initialize Modbus RTU Client with a baud rate of 9600  
  if (!ModbusRTUClient.begin(9600)) {  
    Serial.println("Error starting Modbus RTU Client!"); // Error log if client fails to start  
    while (1); // Stop execution if Modbus client fails to initialize  
  }  
}  

void loop() {  
  
  // Arrays to hold the state of digital inputs from the slave and holding registers  
  int slaveInputs[NUMBER_INPUTS_COILS];   
  int slaveInRegisters[NUMBER_IN_HOLD_REGISTERS];  
  
  // Read Modbus Controllino Mega slave digital inputs  
  for (int i = 0; i < NUMBER_INPUTS_COILS; i++)   
  {  
    // Read discrete inputs from the slave device  
    slaveInputs[i] = ModbusRTUClient.discreteInputRead(CONTROLLINO_MEGA_SLAVE, i);  
    
    // Check for errors while reading inputs  
    if (slaveInputs[i] < 0) {  
      Serial.print("Error reading slave input A");  
      Serial.print(i);  
      Serial.println("!"); // Log error message if reading fails  
    }  
    
    // Synchronize the state of slave inputs to the corresponding master outputs  
    digitalWrite(CONTROLLINO_R0 + i, slaveInputs[i]);  

    // Write the current state of the master digital input to the slave output  
    if (!ModbusRTUClient.coilWrite(CONTROLLINO_MEGA_SLAVE, i, digitalRead(CONTROLLINO_A0 + i))) {  
      Serial.print("Error writing slave output R");  
      Serial.print(i);  
      Serial.println("!"); // Log error message if writing fails  
    }  
  }  

  // Read and synchronize holding registers with the slave device  
  for (int i = 0; i < NUMBER_IN_HOLD_REGISTERS; i++)   
  {  
    // Read input registers from the slave device  
    slaveInRegisters[i] = ModbusRTUClient.inputRegisterRead(CONTROLLINO_MEGA_SLAVE, i);  
    
    // Check for errors while reading input registers  
    if (slaveInRegisters[i] < 0) {  
      Serial.print("Error reading slave input register A");  
      Serial.print(i + 11); // Offset by 11 to match the register numbering  
      Serial.println("!"); // Log error message if reading fails  
    }  

    // Write the value from the slave input register to the master analog output  
    analogWrite(CONTROLLINO_D0 + i, slaveInRegisters[i]);  
    
    // Synchronize master holding registers to slave holding registers and update the corresponding analog output  
    if (!ModbusRTUClient.holdingRegisterWrite(CONTROLLINO_MEGA_SLAVE, i, analogRead(CONTROLLINO_A11 + i))) {  
      Serial.print("Error writing slave holding register D");  
      Serial.print(i);  
      Serial.println("!"); // Log error message if writing fails  
    }  
  }  

  delay(500); // Delay 500 milliseconds before repeating the loop to avoid overwhelming the bus  
} 