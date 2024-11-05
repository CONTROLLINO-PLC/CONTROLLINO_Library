#include <Wire.h>
#include <Controllino.h>

// These values defined shall be the same in the master and slave

#define NUMBER_DIGITAL_SYNC  5    // Always less than 7 to guarantize use the same pins available in all the kind controllinos 
#define NUMBER_ANALOGS_SYNC  2    // Always 2 to guarantize use the same pins available in all the kind controllinos

// Slave address
const int slaveAddress = 8;

void setup() {
  // Initialize I2C communication as Master
    Wire.begin();
    
    // Initialize serial communication for debugging
    Serial.begin(9600);
    
    
    // Set digital pins as input or output
    for (int i = 0; i < NUMBER_DIGITAL_SYNC; i++) 
    {
        pinMode(CONTROLLINO_D0 + i, INPUT);   // Digital inputs 
        pinMode(CONTROLLINO_R0 + i, OUTPUT);  // Digital outputs
    }
    
    for (int i = 0; i < NUMBER_ANALOGS_SYNC; i++) 
    {  
        pinMode(CONTROLLINO_D0 + NUMBER_DIGITAL_SYNC + i, OUTPUT); // PWM analog outputs
        pinMode(CONTROLLINO_A0 + i, INPUT);                 // Analog inputs
    }
}

void loop() {
    // Start the I2C communication with the slave 
    Wire.beginTransmission(slaveAddress);

    // Reading and sending analog input values to the slave
    for (int i = 0; i < NUMBER_ANALOGS_SYNC; i++) 
    {  
        int analogValue = analogRead(CONTROLLINO_A0 + i); // Read analog inputs of two bytes length 

        Wire.write(analogValue >> 8);   // Send analog input values to the slave
        Wire.write(analogValue & 0xFF); 
    }

    // Reading and sending digital input values to the slave
    int digitalInputStates = 0;

    for (int i = 0; i < NUMBER_DIGITAL_SYNC; i++) 
    {
        if (digitalRead(CONTROLLINO_D0 + i) == HIGH) // Read the state of digital inputs
        {
            digitalInputStates |= (1 << i); // Combine the digital input states into a single byte
        }
    }

    Wire.write(digitalInputStates); // Send digital input states to the slave

    Wire.endTransmission();

    // Request the digital output states from the slave
    Wire.requestFrom(slaveAddress, 5);

    if (Wire.available()) // Check if data is available to read
    {
        

        // Read requested buffer take the first two registers, the analog inputs in the slave 
        //  and writes its values to local  PWM outputs
        for (int i = 0; i < NUMBER_ANALOGS_SYNC; i++) 
        { 
            // In this line the master reads the analog values register by register uniting the two bytes in a single int
            // then writes the value to the PWM output pin that is after the digital outputs
            analogWrite(CONTROLLINO_D0 + NUMBER_DIGITAL_SYNC + i, (Wire.read() << 8) | Wire.read());
        }
        
        int digitalOutputStates = Wire.read();

        // Set digital output states based on received input states from the slave 
        for (int i = 0; i < NUMBER_DIGITAL_SYNC; i++) 
        {
            // In this line the digitalOutputStates is separed into diferent bits to write them to the digital output pins
            digitalWrite(CONTROLLINO_R0 + i, (digitalOutputStates & (1 << i)) ? HIGH : LOW);
        }
    }

  delay(500);
}
