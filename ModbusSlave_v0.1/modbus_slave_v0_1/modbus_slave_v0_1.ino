#include <SimpleModbusSlave.h>
enum {
  D10,
  D11,
  RS0,
  HOLDING_REGS_SIZE
};
unsigned int holdingRegs[HOLDING_REGS_SIZE];
unsigned int address = 2;
void setup(){
  modbus_configure(
    &Serial, 
    9600, 
    SERIAL_8N2, 
    address, 
    13, 
    HOLDING_REGS_SIZE, 
    holdingRegs
  );
//  modbus_update_comms(9600, address);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);}
void loop(){
  modbus_update();
  analogWrite(10, holdingRegs[D10]>>2);   // Записываем полученное значение в 10-й пин.
  analogWrite(11, holdingRegs[D11]>>2);   // Записываем полученное значение в 11-й пин.
  holdingRegs[RS0] = analogRead(A0)/4;    // Читаем значение АЦП и записываем его в регистр
  delay(1); // Пауза для подавления возможного "дребезга"
}
