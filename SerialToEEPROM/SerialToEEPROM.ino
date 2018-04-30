#include <EEPROM.h>

int address = 5;
byte value;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  EEPROM.get(address, value);

  Serial.print(address);
  Serial.print("\t");
  Serial.print(value);
  Serial.println();
}

void loop() {
  while (Serial.available()) {
    value = Serial.read();
    EEPROM.put(address, value);
  }
}
