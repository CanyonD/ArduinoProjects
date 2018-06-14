// RS-485
const uint8_t     PIN_direction_TX_RX = 10;

// Used Pins
const int pinInput[]  = {A0, A1, 2, 3, 4};
const int pinOutput[] = {5, 6, 7, 8, 9};
const int ledPin = 13;

// States
int stateInput[5];
int stateOutput[] = {LOW,LOW,LOW,LOW,LOW};

// Counts
long int countOutput[] = {0,0,0,0,0};

// Timers
long saved_time_Digital[] = {0,0,0,0,0};

const long debounce = 500;
const int threshold = 550;

void setup() {
  // RS-485
  pinMode(PIN_direction_TX_RX, OUTPUT);
  digitalWrite(PIN_direction_TX_RX, LOW);
  
  // Set Led Pin
  pinMode(ledPin, OUTPUT);

  // Inputs
//  pinMode(pinInput[0], INPUT);    // ????
//  pinMode(pinInput[1], INPUT);    // ????
  pinMode(pinInput[2], INPUT);
  pinMode(pinInput[3], INPUT);
  pinMode(pinInput[4], INPUT);

  // Outputs
  pinMode(pinOutput[0], OUTPUT);
  pinMode(pinOutput[1], OUTPUT);
  pinMode(pinOutput[2], OUTPUT);
  pinMode(pinOutput[3], OUTPUT);
  pinMode(pinOutput[4], OUTPUT);
}

void loop() {
  // Led Pin
  digitalWrite(ledPin, HIGH);

  // Processing Analog inputs 
  // This fields a resolution between readings of: 5 volts / 1024 units or, .0049 volts (4.9 mV) per unit.
  for(char i = 0; i < 2; i++) {
    stateInput[i] = analogRead(pinInput[i]);
    if (stateInput[i] > threshold)
      stateOutput[i] = HIGH;
    else
      stateOutput[i] = LOW;

    digitalWrite(pinOutput[i], stateOutput[i]);
  }
  
  // Processing Digital inputs 
  for(char i = 2; i < 5; i++) {
    stateInput[i] = digitalRead(pinInput[i]);
    if ( stateInput[i] == HIGH && (millis() - saved_time_Digital[i] > debounce) ) {
      if (stateOutput[i] == HIGH) {
        stateOutput[i] = LOW;
      } else {
        stateOutput[i] = HIGH;
        countOutput[i]++;
      }
        saved_time_Digital[i] = millis();
      
        digitalWrite(pinOutput[i], stateOutput[i]);
    }
  }

  // Led Pin
  digitalWrite(ledPin, LOW);
}
