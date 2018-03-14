#include <EEPROM.h>

struct MyArray {
  String string;
  byte lenght;
};

int address = 5;
byte value;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  MyArray arrayValue;
  EEPROM.get(address, arrayValue);

  Serial.print(address);
  Serial.print("\t");
  Serial.print(arrayValue.string);
  Serial.print("\t");
  Serial.print(arrayValue.lenght);
  Serial.println();
}

void loop() {
  while (Serial.available()) {
    MyArray arrayValue;
    String input, output;
    char in;
    while (true) {
//      valueChar = Serial.read();
      if(Serial.read()=='\n'){
        arrayValue.string = input;
        arrayValue.lenght = sizeof(input);
        EEPROM.put(address, arrayValue);
        break;
      } else {
        in = Serial.read();
        input = String(in);
      }
      
      
//      if (valueChar == '\n') {
//          arrayValue.lastId = sizeof(arrayValue.string);
//          EEPROM.put(address, arrayValue);
//          Serial.println(arrayValue.string);
//        break;
//      }
//      arrayValue.string[sizeof(arrayValue.string)] = (char)valueChar;
    }
  }
}
