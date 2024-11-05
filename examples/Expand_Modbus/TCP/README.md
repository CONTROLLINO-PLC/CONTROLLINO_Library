
# Server-Client Modbus TCP Control with Controllino

## Introduction

This project demonstrates how to set up a Modbus TCP client-server communication between two Controllino devices (or similar Arduino-based devices). The client and server codes work together to synchronize inputs and outputs, enabling real-time control and monitoring of digital and analog I/O points over an Ethernet network.
This Arduino sketch sets up a Server-Client relationship using Modbus TCP to control inputs and outputs between a master device (Controllino) and a slave device. The master reads the inputs from the slave and writes them to its own outputs, and vice versa, ensuring synchronized operation between the two devices.

## Client-Server Synchronization Overview

The primary goal of this setup is to **synchronize digital and analog inputs and outputs between the server and a Modbus TCP client**. The client sends requests to read from and write to the server's coils and registers, enabling real-time control and monitoring.

In this setup:

- **The Server** runs on a Controllino device and manages the physical inputs and outputs. It listens for commands from a client and updates the I/O states accordingly.
- **The Client** also runs on a Controllino device or another compatible device. It reads inputs from the server and writes outputs to the server, effectively controlling the server's I/O points remotely.

**Synchronization Flow Diagram:**

```plaintext
+----------------+                       +----------------+
|  Modbus Client |                       |  Modbus Server |
|                |                       | (Controllino)  |
+-------+--------+                       +--------+-------+
        |                                         |
        |   Read Discrete Inputs/Register Values  |
        |---------------------------------------->|
        |                                         |
        |   Write to Coils/Holding Registers      |
        |<----------------------------------------|
        |                                         |
        +-----------------------------------------+
```

### Digital Input/Output Synchronization

- **Process:**
  1. **Client Reads Discrete Inputs**: The client reads the state of digital inputs (discrete inputs) from the server to monitor the status of connected devices (e.g., switches, sensors), and turns it related outputs according to their state or value.
  2. **Client Writes to Coils**: The client writes to the server's coils to control digital outputs (e.g., turning on/off relays or LEDs), depending on its own inputs states.
  3. **Server Updates Outputs**: Upon receiving write requests, the server updates the physical digital outputs accordingly.
  
### Analog Input/Output Synchronization

- **Process:**
  1. **Client Reads Input Registers**: The client reads analog inputs from the server to monitor values from sensors (e.g., temperature, pressure, humidity).
  2. **Client Writes to Holding Registers**: The client writes values to the server's holding registers to control analog outputs (e.g., adjusting motor speeds or valve positions).
  3. **Server Updates Outputs**: The server updates the physical analog outputs based on the values written by the client.

### Key Concepts

- **Inputs**: Represent the physical states of sensors, switches, or other input devices connected to the server. These are read by the client.
- **Outputs**: Represent the actuators, lights, or other devices controlled by the client. The client sends commands to the server to update these outputs.

### Input-Output Synchronization

The client and server codes work in tandem to ensure that the digital and analog I/O points are synchronized:

1. **Digital Inputs and Outputs**:
   - The **server** reads the state of its digital inputs (e.g., switches, sensors) and makes this data available to the client.
   - The **client** reads these inputs from the server and can control the corresponding digital outputs on the server (e.g., turning on/off a relay).

2. **Analog Inputs and Outputs**:
   - The **server** reads the values from its analog inputs (e.g., sensors providing variable readings like temperature or light intensity) and makes these values available to the client.
   - The **client** can read these analog inputs and write values to the server's analog outputs (e.g., adjusting the brightness of a light or the speed of a motor).

### Real-Time Control and Monitoring

- **Server**: Acts as the "data holder" for all I/O points. It listens on a specified IP address and port for incoming Modbus TCP requests from the client. The server's role is to provide the current state of its inputs and execute commands to change the state of its outputs.
  
- **Client**: Acts as the "controller." It connects to the server, retrieves the current state of the server's inputs, and sends commands to update the server's outputs. This continuous loop of reading and writing ensures that the server's I/O points are always synchronized with the client’s control logic.

## Code Explanation

### Server Code

The server code configures the Controllino device as a Modbus TCP server:

1. **Ethernet and Modbus Initialization**:
   - Configures the Ethernet interface with a static IP address.
   - Initializes the Modbus TCP server on port 502.

2. **Input/Output Configuration**:
   - Sets up the digital and analog pins as inputs or outputs based on the Controllino model.
   - Configures Modbus coils and registers to manage these inputs and outputs.

3. **Main Loop**:
   - Continuously listens for Modbus TCP requests from the client.
   - Processes read/write commands to update the physical I/O states on the Controllino.

### Client Code

The client code configures another Controllino device as a Modbus TCP client:

1. **Ethernet and Modbus Initialization**:
   - Configures the Ethernet interface and establishes a connection to the server's IP address.

2. **Main Loop**:
   - Continuously reads the digital and analog inputs from the server.
   - Writes to the server’s digital and analog outputs based on the client's control logic.

3. **Input/Output Synchronization**:
   - Digital inputs from the server are mapped to digital outputs on the client.
   - Analog inputs from the server are mapped to analog outputs on the client.

## Example Scenario

Imagine you have two Controllino devices:

- **Controllino A (Server)**: Connected to various sensors and actuators.
- **Controllino B (Client)**: Remotely controlling the sensors and actuators on Controllino A.

The client reads the state of a button connected to Controllino A (server) and turns on a light connected to the same device. If the client detects that the button is pressed (input), it sends a command to turn on the light (output). This real-time synchronization allows for responsive and automated control of industrial systems.

## Client Code Explanation

### Libraries

- `Controllino.h`: Provides support for using Controllino hardware.
- `SPI.h`: Enables SPI communication, which is necessary for Ethernet communication.
- `Ethernet.h`: Handles the Ethernet connection required for Modbus TCP.
- `ArduinoRS485.h`: A dependency for Modbus communication.
- `ArduinoModbus.h`: Allows for Modbus communication over TCP.

### Network Configuration

```cpp
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 1, 1);
EthernetClient ethernetClient;
ModbusTCPClient modbusTCPClient(ethernetClient);
IPAddress server(192, 168, 1, 10);
```

- **MAC Address**: A unique identifier for the Ethernet hardware.
- **IP Address**: The static IP assigned to the Controllino master on the local network.
- **Server IP**: The IP address of the Modbus slave device.

These settings establish the communication parameters for the Modbus TCP connection.

### Input and Output Definitions

```cpp
#ifdef CONTROLLINO_MAXI
  #define NUMBER_INPUTS_COILS  5
  #define NUMBER_IN_HOLD_REGISTERS 5
  #define FIRST_ANALOG_INPUT_PIN  CONTROLLINO_A5
#endif

#ifdef CONTROLLINO_MEGA
  #define NUMBER_INPUTS_COILS  11
  #define NUMBER_IN_HOLD_REGISTERS 5
  #define FIRST_ANALOG_INPUT_PIN  CONTROLLINO_A11 
#endif
```

- **NUMBER_INPUTS_COILS**: Number of digital inputs to be tracked and synchronized.
- **NUMBER_IN_HOLD_REGISTERS**: Number of analog inputs (holding registers) to be managed.
- **FIRST_ANALOG_INPUT_PIN**: Defines the first analog input pin to be used for analog input.

This section is configured based on the specific Controllino device (MAXI or MEGA) being used. The number of inputs and outputs must match between the master and slave to ensure proper synchronization.

### `setup()`: Initialization Function

```cpp
void setup() {
  Serial.begin(9600);
  Ethernet.begin(mac, ip);

  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found. Sorry, can't run without hardware. :(");
    while (true) {
      delay(1);
    }
  }

  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }
}
```

- **Serial Communication**: Initializes serial communication for debugging.
- **Ethernet Initialization**: Starts the Ethernet connection using the predefined MAC and IP address.
- **Hardware Check**: Verifies if the Ethernet shield is present. If not, the program halts.
- **Link Status Check**: Checks if the Ethernet cable is connected and provides feedback.

This function sets up the basic environment needed for Modbus communication over Ethernet.

### `loop()`: Main Control Loop

The `loop()` function is where the main operations occur. It handles the connection to the Modbus TCP server and synchronizes inputs and outputs between the master and slave.

#### 1. **Connection Management**

```cpp
if (!modbusTCPClient.connected()) {
  Serial.println("Attempting to connect to Modbus TCP server");
  
  if (!modbusTCPClient.begin(server, 502)) {
    Serial.println("Modbus TCP Client failed to connect!");
  } else {
    Serial.println("Modbus TCP Client connected");
  }
}
```

- **Connection Check**: Ensures the Modbus client is connected to the server.
- **Reconnect Logic**: If disconnected, it attempts to reconnect to the server on port 502 (standard Modbus TCP port).

This block ensures that the master is always connected to the slave, ready to exchange data.

#### 2. **Synchronizing Digital Inputs and Outputs**

```cpp
int slaveInputs[NUMBER_INPUTS_COILS];

for (int i = 0; i < NUMBER_INPUTS_COILS; i++) {
  slaveInputs[i] = modbusTCPClient.discreteInputRead(i);

  if (slaveInputs[i] < 0) {
    Serial.print("Error reading slave input A");
    Serial.print(i);
    Serial.println("!");
  }

  digitalWrite(CONTROLLINO_R0 + i, slaveInputs[i]);

  if (!modbusTCPClient.coilWrite(i, digitalRead(CONTROLLINO_A0 + i))) {
    Serial.print("Error writing slave output R");
    Serial.print(i);
    Serial.println("!");
  }
}
```

- **Read Slave Inputs**: The master reads the state of digital inputs from the slave device.
- **Error Handling**: If reading fails, an error message is printed.
- **Write to Master Outputs**: The read values are then written to the corresponding outputs on the master.
- **Write to Slave Outputs**: The master writes its own digital input states back to the slave’s outputs, ensuring synchronization.

This block handles the real-time exchange of digital signals, ensuring that the master’s outputs mirror the slave’s inputs and vice versa.

#### 3. **Synchronizing Analog Inputs and Holding Registers**

```cpp
int slaveInRegisters[NUMBER_IN_HOLD_REGISTERS];

for (int i = 0; i < NUMBER_IN_HOLD_REGISTERS; i++) {
  slaveInRegisters[i] = modbusTCPClient.inputRegisterRead(i);

  if (slaveInRegisters[i] < 0) {
    Serial.print("Error reading slave input register A");
    Serial.print(i + 11);
    Serial.println("!");
  }

  analogWrite(CONTROLLINO_D0 + i, slaveInRegisters[i]);

  if (!modbusTCPClient.holdingRegisterWrite(i, analogRead(CONTROLLINO_A11 + i))) {
    Serial.print("Error writing slave holding register D");
    Serial.print(i);
    Serial.println("!");
  }
}
```

- **Read Slave Analog Inputs**: The master reads the analog input registers from the slave.
- **Error Handling**: Similar to digital inputs, errors in reading are logged.
- **Write to Master Analog Outputs**: The values from the slave’s analog inputs are written to the master’s analog outputs.
- **Write to Slave Holding Registers**: The master updates the slave’s holding registers with its own analog input values.

This section ensures that analog data, such as sensor readings, is synchronized between the master and slave devices.

Here is the improved and more detailed `README.md` explaining the code and its synchronization with a Modbus TCP client:

## Server Code Splanation

### Library Inclusions

At the beginning of the code, essential libraries are included to provide necessary functionalities:

```cpp
#include <Controllino.h>
#include <SPI.h>
#include <Ethernet.h>
#include <ArduinoRS485.h> // Dependency for ArduinoModbus
#include <ArduinoModbus.h>
```

**Explanation:**

- **`Controllino.h`**: Enables easy access to Controllino-specific pin mappings and functions.
- **`SPI.h`**: Allows for SPI communication between the microcontroller and Ethernet shield.
- **`Ethernet.h`**: Manages Ethernet networking capabilities for TCP/IP communication.
- **`ArduinoRS485.h` & `ArduinoModbus.h`**: Together facilitate Modbus protocol communication over TCP/IP networks.

### Server Network Configuration

Proper network configuration is critical for establishing successful communication between the server and client.

```cpp
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // MAC address
IPAddress ip(192, 168, 1, 10); // Static IP address for the server

EthernetServer ethernetServer(502); // Initialize Ethernet server on port 502
ModbusTCPServer modbusTCPServer; // Initialize Modbus TCP server
```

**Explanation:**

- **MAC Address (`mac[]`)**: A unique identifier for the Ethernet interface. Ensure this address does not conflict with other devices on the network.
- **IP Address (`ip`)**: The server's static IP address. It should be within the range of your local network and not conflict with other devices.
- **Ethernet Server (`ethernetServer`)**: Listens for incoming connections on **port 502**, the standard port for Modbus TCP communication.
- **Modbus TCP Server (`modbusTCPServer`)**: Handles Modbus protocol-specific requests and responses over the established Ethernet connection.

### Device Configuration

The configuration adjusts based on whether a **Controllino MAXI** or **Controllino MEGA** is used:

```cpp
#ifdef CONTROLLINO_MAXI
  #define NUMBER_INPUTS_COILS  5
  #define NUMBER_IN_HOLD_REGISTERS 5
  #define FIRST_ANALOG_INPUT_PIN  CONTROLLINO_A5
#endif

#ifdef CONTROLLINO_MEGA
  #define NUMBER_INPUTS_COILS  11
  #define NUMBER_IN_HOLD_REGISTERS 5
  #define FIRST_ANALOG_INPUT_PIN  CONTROLLINO_A11 
#endif
```

**Explanation:**

- **`NUMBER_INPUTS_COILS`**: Sets the number of digital inputs and outputs (coils) to be managed.
  - **MAXI**: Manages 5 digital inputs/outputs.
  - **MEGA**: Manages 11 digital inputs/outputs.
- **`NUMBER_IN_HOLD_REGISTERS`**: Defines the number of analog inputs and outputs (holding registers) to be managed. Both devices manage 5 analog inputs/outputs.
- **`FIRST_ANALOG_INPUT_PIN`**: Specifies the starting pin number for analog inputs.
  - **MAXI**: Starts from `CONTROLLINO_A5`.
  - **MEGA**: Starts from `CONTROLLINO_A11`.

**Note:** Ensure that the same device type and configuration are used on both server and client sides to maintain proper synchronization.

### `setup()`: Initialization

The `setup()` function initializes serial communication, configures Ethernet and Modbus servers, and sets up the input/output pins.

```cpp
void setup() {
  Serial.begin(9600);
  
  // Ethernet Initialization
  Ethernet.begin(mac, ip);
  
  // Hardware and Link Status Checks
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found. Sorry, can't run without hardware. :(");
    while (true) { delay(1); }
  }
  
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }
  
  // Modbus Server Initialization
  ethernetServer.begin();
  
  if (!modbusTCPServer.begin()) {
    Serial.println("Failed to start Modbus TCP Server!");
    while (1);
  }
  
  // Modbus Coils and Registers Configuration
  modbusTCPServer.configureCoils(0x00, NUMBER_INPUTS_COILS);
  modbusTCPServer.configureDiscreteInputs(0x00, NUMBER_INPUTS_COILS);
  modbusTCPServer.configureHoldingRegisters(0x00, NUMBER_IN_HOLD_REGISTERS);
  modbusTCPServer.configureInputRegisters(0x00, NUMBER_IN_HOLD_REGISTERS);
  
  // Pin Configuration
  for (int i = 0; i < NUMBER_INPUTS_COILS; i++) {
    pinMode(CONTROLLINO_A0 + i, INPUT);
    pinMode(CONTROLLINO_R0 + i, OUTPUT);
    digitalWrite(CONTROLLINO_R0 + i, LOW);
  }
  
  for (int i = 0; i < NUMBER_IN_HOLD_REGISTERS; i++) {
    pinMode(FIRST_ANALOG_INPUT_PIN + i, INPUT);
    pinMode(CONTROLLINO_D0 + i, OUTPUT);
    analogWrite(CONTROLLINO_D0 + i, 0);
  }
}
```

#### Ethernet Initialization

```cpp
Ethernet.begin(mac, ip);
```

- **Purpose**: Establishes the Ethernet connection using the specified MAC and IP addresses.
- **Functionality**:
  - Configures the network settings for the device.
  - Enables the device to communicate over the local network via Ethernet.

#### Hardware and Link Status Checks

```cpp
if (Ethernet.hardwareStatus() == EthernetNoHardware) {
  Serial.println("Ethernet shield was not found. Sorry, can't run without hardware. :(");
  while (true) { delay(1); }
}

if (Ethernet.linkStatus() == LinkOFF) {
  Serial.println("Ethernet cable is not connected.");
}
```

- **Purpose**: Ensures that the necessary hardware is present and properly connected.
- **Functionality**:
  - Checks if the Ethernet shield is attached and functional.
  - Verifies that the Ethernet cable is connected and the network link is active.
  - Provides error messages and halts execution if issues are detected.

#### Modbus Server Initialization

```cpp
ethernetServer.begin();

if (!modbusTCPServer.begin()) {
  Serial.println("Failed to start Modbus TCP Server!");
  while (1);
}
```

- **Purpose**: Initializes the Ethernet and Modbus TCP servers to handle incoming connections and Modbus requests.
- **Functionality**:
  - Starts listening for incoming TCP connections on port 502.
  - Initializes the Modbus server to process and respond to Modbus protocol requests.
  - Provides error handling if the server fails to start.

#### Modbus Coils and Registers Configuration

```cpp
modbusTCPServer.configureCoils(0x00, NUMBER_INPUTS_COILS);
modbusTCPServer.configureDiscreteInputs(0x00, NUMBER_INPUTS_COILS);
modbusTCPServer.configureHoldingRegisters(0x00, NUMBER_IN_HOLD_REGISTERS);
modbusTCPServer.configureInputRegisters(0x00, NUMBER_IN_HOLD_REGISTERS);
```

- **Purpose**: Defines the Modbus data model by specifying the range and size of coils and registers.
- **Functionality**:
  - **Coils (`configureCoils`)**: Configures writable single-bit values used for digital outputs.
  - **Discrete Inputs (`configureDiscreteInputs`)**: Configures read-only single-bit values used for digital inputs.
  - **Holding Registers (`configureHoldingRegisters`)**: Configures writable 16-bit values used for analog outputs.
  - **Input Registers (`configureInputRegisters`)**: Configures read-only 16-bit values used for analog inputs.
- **Addressing**:
  - All configurations start at address `0x00` and span the number of inputs/outputs defined earlier.

#### Pin Configuration

**Digital Inputs and Outputs:**

```cpp
for (int i = 0; i < NUMBER_INPUTS_COILS; i++) {
  pinMode(CONTROLLINO_A0 + i, INPUT);
  pinMode(CONTROLLINO_R0 + i, OUTPUT);
  digitalWrite(CONTROLLINO_R0 + i, LOW);
}
```

- **Purpose**: Sets up digital input and output pins for use with Modbus coils and discrete inputs.
- **Functionality**:
  - **Digital Inputs (`CONTROLLINO_A0 + i`)**: Configured as inputs to read digital signals (e.g., switches, sensors).
  - **Digital Outputs (`CONTROLLINO_R0 + i`)**: Configured as outputs to control digital devices (e.g., relays, LEDs).
  - **Initialization**: All digital outputs are initialized to `LOW` (off state).

**Analog Inputs and Outputs:**

```cpp
for (int i = 0; i < NUMBER_IN_HOLD_REGISTERS; i++) {
  pinMode(FIRST_ANALOG_INPUT_PIN + i, INPUT);
  pinMode(CONTROLLINO_D0 + i, OUTPUT);
  analogWrite(CONTROLLINO_D0 + i, 0);
}
```

- **Purpose**: Sets up analog input and output pins for use with Modbus holding registers and input registers.
- **Functionality**:
  - **Analog Inputs (`FIRST_ANALOG_INPUT_PIN + i`)**: Configured as inputs to read analog signals (e.g., sensors providing variable voltage).
  - **Analog Outputs (`CONTROLLINO_D0 + i`)**: Configured as outputs to control devices requiring variable voltage (e.g., dimmable lights, speed controllers).
  - **Initialization**: All analog outputs are initialized to `0` (minimum voltage).

**Note:** Proper pin configuration ensures accurate and reliable data exchange between the server and client.

### Main Loop Function: `loop()`

The `loop()` function runs continuously after the `setup()` completes. In this code, it should handle incoming Modbus requests and process them accordingly.

```cpp
void loop() {
  EthernetClient client = ethernetServer.available(); // Check for incoming clients
  
  if (client) {
    modbusTCPServer.accept(client); // Accept the client connection
    
    // Handle Modbus requests
    while (client.connected()) {
      modbusTCPServer.poll(); // Process incoming Modbus requests
    }
    
    client.stop(); // Close the connection when done
  }
}
```

**Explanation:**

- **Client Availability Check**: The server checks if any new clients are attempting to connect.
- **Accepting Connections**: If a client is available, the server accepts the connection and starts processing requests.
- **Processing Requests (`modbusTCPServer.poll()`)**: The server listens for and processes Modbus requests from the client, updating coils and registers as needed.
- **Closing Connections**: Once all requests are processed and the client disconnects, the server closes the connection and waits for new clients.

**Note:** The `loop()` function ensures that the server is always ready to process incoming Modbus requests, facilitating continuous synchronization with the client.
