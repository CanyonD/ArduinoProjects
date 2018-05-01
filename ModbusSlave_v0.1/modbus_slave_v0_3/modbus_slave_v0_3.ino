#include <Modbus.h>
#include <ModbusSerial.h>

// RS-485
const uint8_t     PIN_direction_TX_RX = 2;

// ModbusSerial object
ModbusSerial mb;

// Device ID
const int SlaveID = 10;

// Modbus Registers Offsets
const int COIL_BUTTON_1 = 1; 
const int COIL_BUTTON_2 = 2; 
const int COIL_BUTTON_3 = 3; 
const int COIL_BUTTON_4 = 4; 
const int COIL_RELAY_4 = 5; 
const int COIL_RELAY_3 = 6; 
const int COIL_RELAY_2 = 7; 
const int COIL_RELAY_1 = 8; 
const int RESET_COUNT_RELAY_1 = 9; 
const int RESET_COUNT_RELAY_2 = 10;
const int RESET_COUNT_RELAY_3 = 11;
const int RESET_COUNT_RELAY_4 = 12;
const int LAMP1_COIL = 13; 

// Used Pins
const int pinButton_1 = 10;   // !!!TODO!!!
const int pinButton_2 = 9;    // !!!TODO!!!
const int pinButton_3 = 3;
const int pinButton_4 = 4;
const int pinRelay_4 = 5;
const int pinRelay_3 = 6;
const int pinRelay_2 = 7;
const int pinRelay_1 = 8;
const int ledPin = 13;

// States
int stateButton_1;
int stateButton_2;
int stateButton_3;
int stateButton_4;
int stateRelay_1 = LOW;
int stateRelay_2 = LOW;
int stateRelay_3 = LOW;
int stateRelay_4 = LOW;

// Counts
long int countButton_1 = 0;
long int countButton_2 = 0;
long int countButton_3 = 0;
long int countButton_4 = 0;

// Timers
long saved_time_button_1 = 0;
long saved_time_button_2 = 0;
long saved_time_button_3 = 0;
long saved_time_button_4 = 0;

const long debounce = 500;

void setup() {
  // RS-485
  pinMode(PIN_direction_TX_RX, OUTPUT);
  digitalWrite(PIN_direction_TX_RX, LOW);
  
  // Config Modbus Serial (port, speed, byte format) 
//  mb.config(&Serial, 38400, SERIAL_8N1);
  mb.config( &Serial, 38400, PIN_direction_TX_RX );
  // Set the Slave ID
  mb.setSlaveId(SlaveID);  
    
  // Set IO mode
  pinMode(ledPin, OUTPUT);

  pinMode(pinButton_1, INPUT);
  pinMode(pinButton_2, INPUT);
  pinMode(pinButton_3, INPUT);
  pinMode(pinButton_4, INPUT);
  pinMode(pinRelay_1, OUTPUT);
  pinMode(pinRelay_2, OUTPUT);
  pinMode(pinRelay_3, OUTPUT);
  pinMode(pinRelay_4, OUTPUT);
    
  // Set Coils
  mb.addCoil(LAMP1_COIL);
  mb.addCoil(RESET_COUNT_RELAY_1);
  mb.addCoil(RESET_COUNT_RELAY_2);
  mb.addCoil(RESET_COUNT_RELAY_3);
  mb.addCoil(RESET_COUNT_RELAY_4);
  
  // Set Registers
  mb.addIreg(COIL_BUTTON_1);
  mb.addIreg(COIL_BUTTON_2);
  mb.addIreg(COIL_BUTTON_3);
  mb.addIreg(COIL_BUTTON_4);
  mb.addIreg(COIL_RELAY_1);
  mb.addIreg(COIL_RELAY_2);
  mb.addIreg(COIL_RELAY_3);
  mb.addIreg(COIL_RELAY_4);
}

void loop() {
  // Call once inside loop() - all magic here
  mb.task();
   
  // Attach ledPin to LAMP1_COIL register     
  digitalWrite(ledPin, mb.Coil(LAMP1_COIL));

  if (countButton_1 > 0 && mb.Coil(RESET_COUNT_RELAY_1) == 1) {
    countButton_1 = 0;
    mb.Ireg(COIL_BUTTON_1, 0);
    mb.Coil(RESET_COUNT_RELAY_1, false);
  }
  
  if (countButton_2 > 0 && mb.Coil(RESET_COUNT_RELAY_2) == 1) {
    countButton_2 = 0;
    mb.Ireg(COIL_BUTTON_2, 0);
    mb.Coil(RESET_COUNT_RELAY_2, false);
  }
  
  if (countButton_3 > 0 && mb.Coil(RESET_COUNT_RELAY_3) == 1) {
    countButton_3 = 0;
    mb.Ireg(COIL_BUTTON_3, 0);
    mb.Coil(RESET_COUNT_RELAY_3, false);
  }
  
  if (countButton_4 > 0 && mb.Coil(RESET_COUNT_RELAY_4) == 1) {
    countButton_4 = 0;
    mb.Ireg(COIL_BUTTON_4, 0);
    mb.Coil(RESET_COUNT_RELAY_4, false);
  }

  stateButton_1 = digitalRead(pinButton_1);
  if ( stateButton_1 == HIGH && (millis() - saved_time_button_1 > debounce) ) {
    if (stateRelay_1 == HIGH) {
      stateRelay_1 = LOW;
    } else {
      stateRelay_1 = HIGH;
      countButton_1++;
      mb.Ireg(COIL_BUTTON_1, countButton_1);
    }
      saved_time_button_1 = millis();
      mb.Ireg(COIL_RELAY_1, stateRelay_1);
    
      digitalWrite(pinRelay_1, stateRelay_1);
  }
  
  stateButton_2 = digitalRead(pinButton_2);
  if ( stateButton_2 == HIGH && (millis() - saved_time_button_2 > debounce) ) {
    if (stateRelay_2 == HIGH) {
      stateRelay_2 = LOW;
    } else {
      stateRelay_2 = HIGH;
      countButton_2++;
      mb.Ireg(COIL_BUTTON_2, countButton_2);
    }
      saved_time_button_2 = millis();
      mb.Ireg(COIL_RELAY_2, stateRelay_2);
    
      digitalWrite(pinRelay_2, stateRelay_2);
  }
  
  stateButton_3 = digitalRead(pinButton_3);
  if ( stateButton_3 == HIGH && (millis() - saved_time_button_3 > debounce) ) {
    if (stateRelay_3 == HIGH) {
      stateRelay_3 = LOW;
    } else {
      stateRelay_3 = HIGH;
      countButton_3++;
      mb.Ireg(COIL_BUTTON_3, countButton_3);
    }
      saved_time_button_3 = millis();
      mb.Ireg(COIL_RELAY_3, stateRelay_3);
    
      digitalWrite(pinRelay_3, stateRelay_3);
  }

  stateButton_4 = digitalRead(pinButton_4);
  if ( stateButton_4 == HIGH && (millis() - saved_time_button_4 > debounce) ) {
    if (stateRelay_4 == HIGH) {
      stateRelay_4 = LOW;
    } else {
      stateRelay_4 = HIGH;
      countButton_4++;
      mb.Ireg(COIL_BUTTON_4, countButton_4);
    }
      saved_time_button_4 = millis();
      mb.Ireg(COIL_RELAY_4, stateRelay_4);
    
      digitalWrite(pinRelay_4, stateRelay_4);
  }
}
