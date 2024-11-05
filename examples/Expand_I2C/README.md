# I2C Synchronization Between Two Controllinos

## Introduction

This project enables communication and synchronization between two Controllino devices over I2C. One device acts as the master and the other as the slave. The master reads analog and digital inputs, sends these values to the slave, and receives updated outputs from the slave. This setup is useful for distributed control systems where multiple devices need to work together in a coordinated manner.

## How Synchronization Works

1. **Master Controllino:**
   - Reads analog and digital inputs.
   - Sends these input values to the slave.
   - Receives updated output states from the slave and updates its own outputs accordingly.

2. **Slave Controllino:**
   - Receives input values from the master.
   - Reads its own analog and digital inputs.
   - Sends its input values back to the master and updates its outputs based on the received data.

The I2C bus is used for this communication, allowing for efficient data transfer between the master and slave devices.

## Master Code Breakdown

### Includes and Definitions

```cpp
#include <Wire.h>
#include <Controllino.h>

// Configuration values
#define NUMBER_DIGITAL_SYNC  5
#define NUMBER_ANALOGS_SYNC  2
const int slaveAddress = 8;
```

- **Includes:**
  - `Wire.h`: Manages I2C communication.
  - `Controllino.h`: Provides Controllino-specific functions and pin definitions.

- **Configuration:**
  - `NUMBER_DIGITAL_SYNC`: Number of digital pins used for synchronization.
  - `NUMBER_ANALOGS_SYNC`: Number of analog pins used for synchronization.
  - `slaveAddress`: I2C address of the slave device.

### Setup Function

```cpp
void setup() {
  Wire.begin(); // Initialize I2C communication as Master
  Serial.begin(9600); // Initialize serial communication for debugging

  // Configure digital and analog pins
  for (int i = 0; i < NUMBER_DIGITAL_SYNC; i++) {
    pinMode(CONTROLLINO_D0 + i, INPUT);   // Set digital pins as inputs
    pinMode(CONTROLLINO_R0 + i, OUTPUT);  // Set other digital pins as outputs
  }
  
  for (int i = 0; i < NUMBER_ANALOGS_SYNC; i++) {
    pinMode(CONTROLLINO_D0 + NUMBER_DIGITAL_SYNC + i, OUTPUT); // Set PWM output pins
    pinMode(CONTROLLINO_A0 + i, INPUT); // Set analog pins as inputs
  }
}
```

- **`Wire.begin()`:** Initializes the I2C bus in master mode.
- **`Serial.begin(9600)`:** Starts serial communication for debugging.
- **Pin Configuration:**
  - **Digital Pins:**
    - **Inputs:** Sets `CONTROLLINO_D0` to `CONTROLLINO_D4` as digital inputs to read external signals.
    - **Outputs:** Sets `CONTROLLINO_R0` to `CONTROLLINO_R4` as digital outputs to control external devices.
  - **Analog Pins:**
    - **PWM Outputs:** Configures `CONTROLLINO_D5` to `CONTROLLINO_D6` as PWM outputs to control devices based on analog values.
    - **Inputs:** Configures `CONTROLLINO_A0` and `CONTROLLINO_A1` as analog inputs to read sensor values.

### Loop Function

```cpp
void loop() {
  Wire.beginTransmission(slaveAddress);

  // Send analog input values to the slave
  for (int i = 0; i < NUMBER_ANALOGS_SYNC; i++) {
    int analogValue = analogRead(CONTROLLINO_A0 + i); // Read analog input
    Wire.write(analogValue >> 8);   // Send high byte of the analog value
    Wire.write(analogValue & 0xFF); // Send low byte of the analog value
  }

  // Send digital input values to the slave
  int digitalInputStates = 0;
  for (int i = 0; i < NUMBER_DIGITAL_SYNC; i++) {
    if (digitalRead(CONTROLLINO_D0 + i) == HIGH) {
      digitalInputStates |= (1 << i); // Combine digital input states into a single byte
    }
  }
  Wire.write(digitalInputStates); // Send digital input states to the slave

  Wire.endTransmission();

  // Request output states from the slave
  Wire.requestFrom(slaveAddress, 5);

  if (Wire.available()) {
    int digitalOutputStates = Wire.read(); // Read the digital output states

    // Update PWM outputs with received analog values
    for (int i = 0; i < NUMBER_ANALOGS_SYNC; i++) {
      int analogValue = (Wire.read() << 8) | Wire.read(); // Combine high and low bytes
      analogWrite(CONTROLLINO_D

```markdown
### Loop Function (Continued)

```cpp
      analogWrite(CONTROLLINO_D0 + NUMBER_DIGITAL_SYNC + i, analogValue); // Update PWM output with the received value
    }

    // Update digital outputs based on received states
    for (int i = 0; i < NUMBER_DIGITAL_SYNC; i++) {
      bool state = (digitalOutputStates & (1 << i)) ? HIGH : LOW; // Extract the state for each digital output
      digitalWrite(CONTROLLINO_D0 + i, state); // Set the digital output pin state
    }
  }

  delay(500); // Delay for stability
}
```

- **Sending Data to Slave:**
  - **Analog Inputs:** Reads analog values from `CONTROLLINO_A0` and `CONTROLLINO_A1`, and sends these values in two bytes (high and low).
  - **Digital Inputs:** Reads the state of digital inputs (`CONTROLLINO_D0` to `CONTROLLINO_D4`). Combines these states into a single byte where each bit represents an input pin.

- **Receiving Data from Slave:**
  - Requests 5 bytes from the slave: 2 bytes for each analog output and 1 byte for digital outputs.
  - **Analog Outputs:** Reads the received analog values (two bytes per value), combines them into an integer, and sets the PWM outputs (`CONTROLLINO_D5` and `CONTROLLINO_D6`).
  - **Digital Outputs:** Reads the state byte and sets the digital outputs (`CONTROLLINO_D0` to `CONTROLLINO_D4`) according to the received states.

- **Delay:** Adds a half-second delay between iterations to ensure stable communication.

## Slave Code Breakdown

### Definitions

```cpp
#include <Wire.h>
#include <Controllino.h>

// Configuration values
#define NUMBER_DIGITAL_SYNC  5
#define NUMBER_ANALOGS_SYNC  2
```

- **Includes:**
  - `Wire.h`: Manages I2C communication.
  - `Controllino.h`: Provides Controllino-specific functions and pin definitions.

- **Configuration:**
  - `NUMBER_DIGITAL_SYNC` and `NUMBER_ANALOGS_SYNC` must match the master configuration to ensure proper synchronization.

### Setup

```cpp
void setup() {
  Wire.begin(8); // Initialize I2C communication as Slave with address 8

  // Configure digital and analog pins
  for (int i = 0; i < NUMBER_DIGITAL_SYNC; i++) {
    pinMode(CONTROLLINO_D0 + i, INPUT);   // Set digital pins as inputs
    pinMode(CONTROLLINO_R0 + i, OUTPUT);  // Set other digital pins as outputs
  }

  for (int i = 0; i < NUMBER_ANALOGS_SYNC; i++) {
    pinMode(CONTROLLINO_D0 + NUMBER_DIGITAL_SYNC + i, OUTPUT); // Set PWM output pins
    pinMode(CONTROLLINO_A0 + i, INPUT); // Set analog pins as inputs
  }

  // Register event handlers
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
}
```

- **`Wire.begin(8)`:** Initializes the I2C bus as a slave with address 8.
- **Pin Configuration:**
  - **Digital Pins:**
    - **Inputs:** Sets `CONTROLLINO_D0` to `CONTROLLINO_D4` as inputs to read signals from external sources.
    - **Outputs:** Sets `CONTROLLINO_R0` to `CONTROLLINO_R4` as outputs to control external devices.
  - **Analog Pins:**
    - **PWM Outputs:** Configures `CONTROLLINO_D5` to `CONTROLLINO_D6` as PWM outputs for analog signal control.
    - **Inputs:** Configures `CONTROLLINO_A0` and `CONTROLLINO_A1` as analog inputs to read sensor values.

- **Event Handlers:**
  - **`Wire.onReceive(receiveEvent)`:** Registers the handler for data received from the master.
  - **`Wire.onRequest(requestEvent)`:** Registers the handler for data requests from the master.

### Receive Event Handler

```cpp
void receiveEvent(int bytes) {
  if (Wire.available()) {
    // Update PWM outputs with received analog values
    for (int i = 0; i < NUMBER_ANALOGS_SYNC; i++) {
      int highByte = Wire.read(); // Read high byte
      int lowByte = Wire.read();  // Read low byte
      int analogValue = (highByte << 8) | lowByte; // Combine bytes into an integer
      analogWrite(CONTROLLINO_D0 + NUMBER_DIGITAL_SYNC + i, analogValue); // Update PWM output
    }

    int digitalOutputStates = Wire.read(); // Read the digital input states from the master

    // Update digital outputs based on received states
    for (int i = 0; i < NUMBER_DIGITAL_SYNC; i++) {
      bool state = (digitalOutputStates & (1 << i)) ? HIGH : LOW; // Extract state for each output
      digitalWrite(CONTROLLINO_D0 + i, state); // Set the digital output pin state
    }
  }
}
```

- **Processing Received Data:**
  - **Analog Values:** Reads two bytes for each analog value, combines them into an integer, and updates the PWM outputs (`CONTROLLINO_D5` and `CONTROLLINO_D6`).
  - **Digital States:** Reads the byte containing digital output states and updates the digital outputs (`CONTROLLINO_D0` to `CONTROLLINO_D4`).

### Request Event Handler

```cpp
void requestEvent() {
  // Send analog input values to the master
  for (int i = 0; i < NUMBER_ANALOGS_SYNC; i++) {
    int analogValue = analogRead(CONTROLLINO_A0 + i); // Read analog input value
    Wire.write(analogValue >> 8);   // Send high byte
    Wire.write(analogValue & 0xFF); // Send low byte
  }

  // Send digital input values to the master
  int digitalInputStates = 0;
  for (int i = 0; i < NUMBER_DIGITAL_SYNC; i++) {
    if (digitalRead(CONTROLLINO_D0 + i) == HIGH) {
      digitalInputStates |= (1 << i); // Combine digital input states into a single byte
    }
  }
  Wire.write(digitalInputStates); // Send digital input states to the master
}
```

- **Sending Data to Master:**
  - **Analog Inputs:** Reads analog values from `CONTROLLINO_A0` and `CONTROLLINO_A1`, splits these into high and low bytes, and sends them to the master.
  - **Digital Inputs:** Reads digital input states (`CONTROLLINO_D0` to `CONTROLLINO_D4`), combines them into a byte, and sends this byte to the master.

## Conclusion

This setup allows two Controllinos to synchronize their inputs and outputs over the I2C bus. The master reads inputs, sends them to the slave, and updates its outputs based on the slave's responses. The slave processes the received data, updates its own outputs, and sends back its input values. This system is ideal for applications requiring multiple controllers to operate in harmony.
