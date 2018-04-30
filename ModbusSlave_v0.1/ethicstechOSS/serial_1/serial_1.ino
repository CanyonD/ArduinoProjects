/*
  Modbus-Arduino Example - Lamp (Modbus Serial)
  Copyright by André Sarmento Barbosa
  http://github.com/andresarmento/modbus-arduino
*/
 
#include <Modbus.h>
#include <ModbusSerial.h>

// Modbus Registers Offsets (0-9999)
const int LAMP1_COIL = 5; 
// Used Pins
const int ledPin = 13;

//Modbus Registers Offsets (0-9999)
const int SWITCH_ISTS = 9; 
//Used Pins
const int switchPin = 3;

// ModbusSerial object
ModbusSerial mb;

void setup() {
    // Config Modbus Serial (port, speed, byte format) 
    mb.config(&Serial, 38400, SERIAL_8N1);
    // Set the Slave ID (1-247)
    mb.setSlaveId(10);  
    
    // Set ledPin mode
    pinMode(ledPin, OUTPUT);
    // Add LAMP1_COIL register - Use addCoil() for digital outputs
    mb.addCoil(LAMP1_COIL);
    // Add SWITCH_ISTS register - Use addIsts() for digital inputs 
    mb.addIsts(SWITCH_ISTS);
}

void loop() {
   // Call once inside loop() - all magic here
   mb.task();
   
   // Attach ledPin to LAMP1_COIL register     
   digitalWrite(ledPin, mb.Coil(LAMP1_COIL));

   //Attach switchPin to SWITCH_ISTS register     
   mb.Ists(SWITCH_ISTS, digitalRead(switchPin));
}
