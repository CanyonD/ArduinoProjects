#include <Modbus.h>
#include <ModbusSerial.h>

// ModbusSerial object
ModbusSerial mb;

// RS-485
const uint8_t     PIN_direction_TX_RX = 10;

// Device ID
const int SlaveID = 23;

// Modbus registers
const int COIL_COUNTS[] = {1,2,3,4,5};
const int COIL_STATUS[] = {6,7,8,9,10};
const int COIL_THRESHOLD[] = {11,12,13,14,15};
const int COIL_INVERT[] = {16,17,18,19,20};
const int COIL_TIME[] = {21,22,23,24,25};
const int COIL_ADDRESS = 100;

// Used Pins
const int pinInput[]  = {A0, A1, A2, A3, A4};
const int pinOutput[] = {5, 6, 7, 8, 9};
const int ledPin = 13;

// States
int stateInput[5];
int stateOutput[] = {LOW,LOW,LOW,LOW,LOW};

// Settings
bool invertOutput[] = {false, false, false, false};
int timeOutput[] = {1,1,1,1,1};
int timeOutputTmp[] = {0,0,0,0,0};
int threshold[] = {550,550,550,550,550};

// Counts
long int countOutput[] = {0,0,0,0,0};

// Timers
long saved_time_input[] = {0,0,0,0,0};
long saved_led_time;
boolean led;

const long debounce = 500;

void setup() {
  // RS-485
  pinMode(PIN_direction_TX_RX, OUTPUT);
  digitalWrite(PIN_direction_TX_RX, LOW);
  
  // Set Led Pin
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Outputs
  for (char i = 0; i < 5; i++) {
    pinMode(pinOutput[i], OUTPUT);
    digitalWrite(pinOutput[i], stateOutput[i]);
  }
  
  for (char i = 0; i < 5; i++)
    saved_time_input[i] = 0;

  saved_led_time = millis();
  led = false;

  // Modbus settings
  mb.config( &Serial, 9600, PIN_direction_TX_RX );
  mb.setSlaveId(SlaveID);

  // Coins with address ID
  mb.addIreg(COIL_ADDRESS);
  mb.Ireg(COIL_ADDRESS, SlaveID);

  for (char i = 0; i < 5; i++) {
    // Coins with counts 
    mb.addIreg(COIL_COUNTS[i]);
    mb.Ireg(COIL_COUNTS[i], countOutput[i]);
    
    // Coins with current status
    mb.addIreg(COIL_STATUS[i]);
    mb.Ireg(COIL_STATUS[i], stateInput[i]);
    
    // Coins with threshold settings
    mb.addIreg(COIL_THRESHOLD[i]);
    mb.Ireg(COIL_THRESHOLD[i], threshold[i]);

    // Coins with invert settings
    mb.addIreg(COIL_INVERT[i]);
    mb.Ireg(COIL_INVERT[i], invertOutput[i]);
    
    // Coins with time output settings
    mb.addIreg(COIL_TIME[i]);
    mb.Ireg(COIL_TIME[i], timeOutput[i]);
  }
}

void loop() {
  // Call once inside loop() - all magic here
  mb.task();

  // Cute blinking of led
  if (millis() - saved_led_time > debounce) {
    if (led)  led = false;
    else      led = true;
    digitalWrite(ledPin, led);
    saved_led_time = millis();
  }

  // Processing Analog inputs 
  // This fields a resolution between readings of: 5 volts / 1024 units or, .0049 volts (4.9 mV) per unit.
  for(char i = 0; i < 5; i++) {
    if (millis() - saved_time_input[i] > debounce) {
      saved_time_input[i] = millis();
      stateInput[i] = analogRead(pinInput[i]);
      if ( stateInput[i] > threshold[i] ) {
        if (stateOutput[i] == LOW) {
          countOutput[i]++;
          mb.Ireg(COIL_COUNTS[i], countOutput[i]);
        }
        stateOutput[i] = HIGH;
        timeOutputTmp[i] = timeOutput[i];
      } else {
        if (timeOutputTmp[i]) {
          stateOutput[i] = HIGH;
          timeOutputTmp[i]--;
        } else {
          stateOutput[i] = LOW;
        }
      }
      mb.Ireg(COIL_STATUS[i], stateInput[i]);
      if (invertOutput[i])
        digitalWrite(pinOutput[i], !stateOutput[i]);
      else
        digitalWrite(pinOutput[i], stateOutput[i]);
    }
  }

  for(char i = 0; i < 5; i++) {
    if (mb.Ireg(COIL_THRESHOLD[i]) != threshold[i])
      threshold[i] = mb.Ireg(COIL_THRESHOLD[i]);
    if (mb.Ireg(COIL_INVERT[i]) != invertOutput[i])
      invertOutput[i] = mb.Ireg(COIL_INVERT[i]);
    if (mb.Ireg(COIL_TIME[i]) != timeOutput[i])
      timeOutput[i] = mb.Ireg(COIL_TIME[i]);
  }
}
