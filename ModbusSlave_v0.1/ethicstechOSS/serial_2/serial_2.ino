#include <Modbus.h>
#include <ModbusSerial.h>

// Modbus Registers Offsets (0-9999)
const int LAMP1_COIL = 13; 
//const int SWITCH_ISTS = 9; 

const int COIL_BUTTON = 4; 
const int COIL_RELAY = 5; 

// Used Pins
const int ledPin = 13;
//const int switchPin = 3;

const int pinButton = 4;
const int pinRelay = 5;

int stateRelay = LOW;
int stateButton;
long int countButton = 0;
//int previous = LOW;
long saved_time = 0;
long debounce = 500;

// RS-485
const uint8_t     PIN_direction_TX_RX = 2;

// ModbusSerial object
ModbusSerial mb;

void setup() {
    pinMode(PIN_direction_TX_RX, OUTPUT);
    digitalWrite(PIN_direction_TX_RX, LOW);
  
    // Config Modbus Serial (port, speed, byte format) 
//    mb.config(&Serial, 38400, SERIAL_8N1);
    mb.config( &Serial, 38400, PIN_direction_TX_RX );
    // Set the Slave ID (1-247)
    mb.setSlaveId(10);  
    
    // Set ledPin mode
    pinMode(ledPin, OUTPUT);
    // Add LAMP1_COIL register - Use addCoil() for digital outputs
    mb.addCoil(LAMP1_COIL);
    // Add SWITCH_ISTS register - Use addIsts() for digital inputs 
//    mb.addIsts(COIL_BUTTON);
//    mb.addIsts(COIL_RELAY);

    mb.addIreg(COIL_BUTTON);
    mb.addIreg(COIL_RELAY);

    pinMode(pinButton, INPUT);
    pinMode(pinRelay, OUTPUT);
}

void loop() {
   // Call once inside loop() - all magic here
   mb.task();
   
   // Attach ledPin to LAMP1_COIL register     
   digitalWrite(ledPin, mb.Coil(LAMP1_COIL));

   //Attach switchPin to SWITCH_ISTS register     
//   mb.Ists(SWITCH_ISTS, digitalRead(switchPin));

   stateButton = digitalRead(pinButton);
   if ( stateButton == HIGH && (millis() - saved_time > debounce) ) {
     if (stateRelay == HIGH) {
         stateRelay = LOW;
     } else {
         stateRelay = HIGH;
         countButton++;
         mb.Ireg(COIL_BUTTON, countButton);
     }
     saved_time = millis();
     mb.Ireg(COIL_RELAY, stateRelay);
     digitalWrite(pinRelay, stateRelay );
   }
   
//   digitalWrite(pinRelay, stateRelay);

//   digitalWrite(pinRelay, mb.Coil(COIL_RELAY));

//   mb.Ists(COIL_BUTTON, digitalRead(pinButton));

   //Attach switchPin to SWITCH_ISTS register     
//   mb.Ists(SWITCH_ISTS, digitalRead(switchPin));

}
