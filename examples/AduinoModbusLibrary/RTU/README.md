
# Controllino Modbus Master-Slave Integration

## Introduction

Modbus is a popular communication protocol used in industrial automation systems. This class allows you to easily communicate with devices using the Modbus protocol on your Controllino board. This is a Modbus class designed to work with Controllino boards.

### Overview

This repository contains two Arduino sketches designed for the Controllino Mega platform, implementing a Modbus RTU communication system. One sketch acts as a Master and the other as a Slave, facilitating the synchronization of digital and analog inputs and outputs via Modbus registers. The Master reads and writes values to the Slave, ensuring that the states of inputs and outputs are consistent across both devices.

### Project Structure

Master Code: Implements the Modbus RTU Client.
Slave Code: Implements the Modbus RTU Server.
Both codes are structured similarly, allowing easy synchronization and communicatio

## Installation on Arduino IDE

1. Install the Arduino IDE from the official website: [Arduino IDE](https://www.arduino.cc/en/software)
2. Open the Arduino IDE and install the required libraries:
    - ArduinoRS485: Go to `Sketch` -> `Include Library` -> `Manage Libraries...` and search for `ArduinoRS485` to install it.
    - ArduinoModbus: Go to `Sketch` -> `Include Library` -> `Manage Libraries...` and search for `ArduinoModbus` to install it.
3. Connect your Controllino PLC to your computer using a USB cable.

### Note

The Controllino BSP, for MAXI and MEGA, inside **pins_arduino.h** contains the following definitions:

```cpp
// ArduinoRS485 library support
#define RS485_SERIAL_PORT           Serial3
#define RS485_DEFAULT_TX_PIN        14
#define CUSTOM_RS485_DEFAULT_DE_PIN 75
#define CUSTOM_RS485_DEFAULT_RE_PIN 76
```

These allows the use of the default instance of RS485 library:

```cpp
RS485Class RS485(RS485_SERIAL_PORT, RS485_DEFAULT_TX_PIN, RS485_DEFAULT_DE_PIN, RS485_DEFAULT_RE_PIN);
```

Which is also the default interface for **ModbusRTUServer** and **ModbusRTUClient** which are the default instances for RTU in ArduinoModbus library.

This allows an easy integration of the libraries and the Controllino BSP so there is not need to create an instance of RS485 and later an instance of ModbusRTUServer or ModbusRTUClient, but just use the default instances like in the library examples.

### Usage

1. Open the Arduino IDE.
2. Copy and paste the provided code into a new sketch.
3. Upload the sketch to your Controllino PLC.

## Project Description

This project is an example of using a Controllino PLC (Programmable Logic Controller) with Modbus communication. The example demonstrates how to set up, standarize and establish Modbus communication among both, Master and Slave controllinos. The Modbus registers map is builded according to the Controllino pin names declared in Controllino.h for digital inputs(coils), digital outputs (coils), analog inputs (input registers), and analog outputs (holding registers).

## Modbus Slave Code

### Code Explanation

The given code snippet is part of a loop function that handles Modbus RTU communication on a Controllino Mega PLC. It processes Modbus requests and updates the state of digital and analog inputs and outputs based on the received Modbus commands.

---

### **Setup Function**

```cpp
void setup()  
{  
    // Initialize serial communication at 9600 baud rate for debugging
    Serial.begin(9600);  

    // Start Modbus RTU server with slave ID and baud rate
    if (!ModbusRTUServer.begin(1, 9600))  
    {  
        Serial.println("ModbusRTUServer initialization failed!");  
        while (1); // Halt the program if failure occurs  
    }  

    // Setup the number of coils and registers
    ModbusRTUServer.coilsSetup(NUMBER_INPUTS_COILS);  
    ModbusRTUServer.holdingRegistersSetup(NUMBER_IN_HOLD_REGISTERS);  
    ModbusRTUServer.inputRegistersSetup(NUMBER_IN_HOLD_REGISTERS);  

    // Initialize digital pins
    for (int i = 0; i < NUMBER_INPUTS_COILS; i++)  
    {  
        pinMode(CONTROLLINO_A0 + i, INPUT); 
        pinMode(CONTROLLINO_R0 + i, OUTPUT);  
        digitalWrite(CONTROLLINO_R0 + i, LOW); // Initialize outputs to LOW  
    }  

    // Initialize analog pins
    for (int i = 0; i < NUMBER_IN_HOLD_REGISTERS; i++)  
    {  
        pinMode(CONTROLLINO_A11 + i, INPUT); 
        pinMode(CONTROLLINO_D0 + i, OUTPUT);  
        analogWrite(CONTROLLINO_D0 + i, 0); // Initialize analog outputs to 0V  
    }  
}
```

---

### **Digital Coils Initialization**

```cpp
for (int i = 0; i < NUMBER_INPUTS_COILS; i++)  
{  
    pinMode(CONTROLLINO_A0 + i, INPUT); 
    pinMode(CONTROLLINO_R0 + i, OUTPUT);  
    digitalWrite(CONTROLLINO_R0 + i, LOW); // Initialize outputs to LOW  
}  
```

**Explanation**:

- This loop initializes digital pins for coil management.
- Each pin corresponding to `CONTROLLINO_A0`, `CONTROLLINO_A1`, etc., is set to `INPUT` to read the state of connected devices.
- The corresponding output pins (`R0`, `R1`, etc.) are set to `OUTPUT` and initialized to `LOW`, ensuring they start in a known state.

---

### **Analog Pins Initialization**

```cpp
for (int i = 0; i < NUMBER_IN_HOLD_REGISTERS; i++)  
{  
    pinMode(CONTROLLINO_A11 + i, INPUT); 
    pinMode(CONTROLLINO_D0 + i, OUTPUT);  
    analogWrite(CONTROLLINO_D0 + i, 0); // Initialize analog outputs to 0V  
}  
```

**Explanation**:

- This loop sets up the analog input and output pins based on the holding registers.
- It configures the pins for reading sensor values (`A11`, `A12`, etc.) as `INPUT`, while the output pins (`D0`, `D1`, etc.) are set to `OUTPUT`.
- All analog outputs are initialized to 0V using `analogWrite`, ensuring a known starting condition.

---

### **Main Loop**

```cpp
void loop()  
{  
    // Continuously check for incoming Modbus requests and respond accordingly  
    int packetReceived = ModbusRTUServer.poll();  
    if (packetReceived)  
    {  
        // Process each digital coil and update corresponding outputs  
        for (int i = 0; i < NUMBER_INPUTS_COILS; i++)  
        {  
            // Read digital coil state
            bool coilValue = ModbusRTUServer.coilRead(i);  
            digitalWrite(CONTROLLINO_R0 + i, coilValue ? HIGH : LOW); // Update output pin state  
            
            // Read digital input state and update coil state
            bool inputState = digitalRead(CONTROLLINO_A0 + i);  
            ModbusRTUServer.coilWrite(i, inputState ? 1 : 0); // Update coil state  
        }  

        // Update the analog output pins and input registers
        for (int i = 0; i < NUMBER_IN_HOLD_REGISTERS; i++)  
        {  
            // Write holding register value to PWM output
            analogWrite(CONTROLLINO_D0 + i, ModbusRTUServer.holdingRegisterRead(i));  

            // Read value from analog input and write to input register
            ModbusRTUServer.inputRegisterWrite(i, analogRead(CONTROLLINO_A11 + i)); 
        }  
    }  
}
```

#### Modbus Polling

```cpp
int packetReceived = ModbusRTUServer.poll();
```

- `ModbusRTUServer.poll()` checks for incoming Modbus RTU requests. It returns `true` if a valid packet is received.

### **Processing Digital Coils**

```cpp
for (int i = 0; i < NUMBER_INPUTS_COILS; i++)  
{  
    // Read digital coil state
    bool coilValue = ModbusRTUServer.coilRead(i);  
    digitalWrite(CONTROLLINO_R0 + i, coilValue ? HIGH : LOW); // Update output pin state  
    
    // Read digital input state and update coil state
    bool inputState = digitalRead(CONTROLLINO_A0 + i);  
    ModbusRTUServer.coilWrite(i, inputState ? 1 : 0); // Update coil state  
}  
```

**Explanation**:

- This loop processes digital coils by reading their current state using `coilRead()`.
- The corresponding output pin (`R0`, `R1`, etc.) is updated based on the coil's state.
- It also reads the associated input pin state and updates the coil state accordingly.

---

### **Updating Analog Outputs and Input Registers**

```cpp
for (int i = 0; i < NUMBER_IN_HOLD_REGISTERS; i++)  
{  
    // Write holding register value to PWM output
    analogWrite(CONTROLLINO_D0 + i, ModbusRTUServer.holdingRegisterRead(i));  

    // Read value from analog input and write to input register
    ModbusRTUServer.inputRegisterWrite(i, analogRead(CONTROLLINO_A11 + i)); 
}  
```

**Explanation**:

- This loop handles the updates for analog outputs and input registers.
- Each holding register's value is written to the relevant PWM output pin using `analogWrite()`.
- Simultaneously, the analog input value is read and written back to the respective input register.


### Summary

- The `ModbusRTUServer.poll()` function checks for incoming Modbus RTU requests.
- The coils are read from the Modbus server and used to set the state of the digital output pins.
- The state of the digital input pins is read and written to the Modbus coil registers.
- The values of the holding registers are read from the Modbus server and used to set the analog output pins.
- The values of the analog input pins are read and written to the Modbus input registers.

## Modbus Master Code

This code demonstrates how to use a Controllino Mega to read and write Modbus registers and digital inputs/outputs. 

### Overview Master Code

This code is designed for a Controllino master device that communicates with a Controllino slave device using the Modbus RTU protocol. The master code is standardized to connect seamlessly with a slave that has a similar code structure and variable declarations. The digital inputs and outputs, as well as the analog inputs and outputs, are associated with Modbus registers, allowing for easy synchronization between the master and slave devices.The master code reads from and writes to the Modbus registers and digital inputs/outputs on the Controllino Mega slave. This example demonstrates how to establish Modbus RTU communication between a Controllino Master and a Controllino Slave. The master reads and writes digital inputs/outputs and analog inputs/PWM outputs on the slave through Modbus. declared digital inputs of master are sinchronized to digital outputs of slave and viceversa. The same sinchronizition is developed between the analog inputs / outputs declared in both modbus devices

### Code Explanation Master

#### Library Inclusions and Variable Declarations

```cpp
#include <Controllino.h>  
#include <ArduinoRS485.h>  
#include <ArduinoModbus.h>  

#define CONTROLLINO_MEGA_SLAVE  1  
#define NUMBER_INPUTS_COILS  11    // Number of digital inputs and coils in both master and slave  
#define NUMBER_IN_HOLD_REGISTERS 5 // Number of input and holding registers in both master and slave  
```

**Explanation**:

- The code begins by including necessary libraries for Controllino, RS485 communication, and Modbus protocol.
- Two constants are defined: `CONTROLLINO_MEGA_SLAVE`, which identifies the slave device, and `NUMBER_INPUTS_COILS` and `NUMBER_IN_HOLD_REGISTERS`, which specify the number of digital inputs/coils and analog input/holding registers, respectively.

#### Setup Function

```cpp
void setup() {  
  Serial.begin(9600); // Begin serial communication at 9600 baud rate for debugging  
  while (!Serial); // Wait until the serial port is ready  
```

**Explanation**:

- The `setup()` function initializes serial communication for debugging purposes, allowing the user to monitor the device's status via the serial monitor.

#### Digital Input and Output Pin Setup

```cpp
  // Setup digital input and output pins for the specified number of coils  
  for (int i = 0; i < NUMBER_INPUTS_COILS; i++)  
  {  
    pinMode(CONTROLLINO_A0 + i, INPUT);  // Configure digital input pins (A0 to A10) as INPUT  
    pinMode(CONTROLLINO_R0 + i, OUTPUT); // Configure digital output pins (R0 to R10) as OUTPUT  
    digitalWrite(CONTROLLINO_R0 + i, LOW); // Initialize output pins to LOW (off state)  
  }  
```

**Explanation**:

- This loop iterates through the defined number of digital inputs and outputs.
- Each digital input pin (from `A0` to `A10`) is configured as an `INPUT`, and each corresponding digital output pin (from `R0` to `R10`) is set as an `OUTPUT`.
- All output pins are initialized to `LOW`, ensuring they start in an off state.

#### Analog Input and Output Pin Setup

```cpp
  // Setup analog input and output pins for holding registers  
  for (int i = 0; i < NUMBER_IN_HOLD_REGISTERS; i++)  
  {  
    pinMode(CONTROLLINO_A11 + i, INPUT); // Set analog input pins (A11 to A15) as INPUT  
    pinMode(CONTROLLINO_D0 + i, OUTPUT); // Set analog output pins (D0 to D4) as OUTPUT  
    analogWrite(CONTROLLINO_D0 + i, 0); // Initialize analog outputs to minimum value (0V)   
  }  
```

**Explanation**:

- This loop initializes the analog input pins (from `A11` to `A15`) as `INPUT` and the analog output pins (from `D0` to `D4`) as `OUTPUT`.
- All analog outputs are set to 0V using `analogWrite()`, ensuring a known starting condition.

#### Modbus Client Initialization

```cpp
  Serial.println("Modbus RTU Client Toggle"); // Log message indicating the start of the Modbus RTU Client  

  // Initialize Modbus RTU Client with a baud rate of 9600  
  if (!ModbusRTUClient.begin(9600)) {  
    Serial.println("Error starting Modbus RTU Client!"); // Error log if client fails to start  
    while (1); // Stop execution if Modbus client fails to initialize  
  }  
}  
```

**Explanation**:

- A message is logged to indicate the start of the Modbus RTU client.
- The Modbus client is initialized with a baud rate of 9600. If initialization fails, an error message is printed, and the program halts.

### Main Loop

```cpp
void loop() {  
  
  // Arrays to hold the state of digital inputs from the slave and holding registers  
  int slaveInputs[NUMBER_INPUTS_COILS];   
  int slaveInRegisters[NUMBER_IN_HOLD_REGISTERS];  
```

**Explanation**:

- The `loop()` function continuously runs, holding arrays to store the state of digital inputs from the slave and the values of holding registers.

#### Reading Digital Inputs from the Slave

```cpp
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
```

**Explanation**:

- This loop reads discrete inputs from the slave device for each digital input.
- If an error occurs during reading, an error message is logged.
- The state of the slave inputs is synchronized with the corresponding master outputs (`R0`, `R1`, etc.).
- The current state of the master digital input is written to the slave's output coils. If writing fails, an error message is logged.

#### Reading and Synchronizing Holding Registers

```cpp
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
```

**Explanation**:

- This loop reads input registers from the slave device for each holding register.
- If an error occurs during reading, an error message is logged.
- Reading Input Registers
- This line attempts to read a specific input register from the slave device identified by `CONTROLLINO_MEGA_SLAVE`.
- The value read is stored in the `slaveInRegisters` array at the index `i`.

```cpp
slaveInRegisters[i] = ModbusRTUClient.inputRegisterRead(CONTROLLINO_MEGA_SLAVE, i);
```

- Error Checking
- After attempting to read, the code checks if the returned value is negative, which indicates an error occurred during the read operation.
- If an error is found, a message is printed to the serial monitor, including the specific register number that failed to read (with an offset of 11).

```cpp
if (slaveInRegisters[i] < 0) {...}
```

- Writing to Analog Output
- This line writes the value retrieved from the slave input register directly to the corresponding analog output pin on the master device.
- This allows the Controllino master to control outputs based on the slave’s input data.

```cpp
analogWrite(CONTROLLINO_D0 + i, slaveInRegisters[i]);
```

- Synchronizing Holding Registers
- This line reads an analog input from the master device and attempts to write this value to the corresponding holding register of the slave device.
- If writing to the holding register fails (returns `false`), an error message is logged to the serial monitor.

```cpp
if (!ModbusRTUClient.holdingRegisterWrite(CONTROLLINO_MEGA_SLAVE, i, analogRead(CONTROLLINO_A11 + i))) {...}
```
