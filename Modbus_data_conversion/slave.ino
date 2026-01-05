# slave 
#include <ModbusRTU.h>

ModbusRTU mb;

// float values 
float resistor1 = 100.5;
float resistor2 = 220.75;
float resistor3 = 330.25;

// register 16 bit ka hota he modbus me  ab float me .me values he to 2 registers lgenge 
// 2 element array 161.23  161 -1 23 -2 

uint16_t regR1[2];
uint16_t regR2[2];
uint16_t regR3[2];

// function float to hex 
//modbus float smjhta ni he to float to hex krenge 

//union ek hi memeory ko alag alag datatype se dekhna
//u.f → data ko float samajh ke dekho
//u.h[0], u.h[1] → wahi data 16-16 bit ke tukdo me dekho 

void floatToHex(float value, uint16_t *reg) {
  union {
    float f; // 32 bit 
    uint16_t h[2];  // 32 bit h array for 2 elem 
  } u; // u denotes union  // uf //uh same value access krenge 

  u.f = value; //  cpu convert float to binary IEEE-754 format 100.5 → 0x42C9000
  reg[0] = u.h[0];   // Low word // union ki wahi memeoery 16 bit ke firsts half ko read kregi and 1 register me save krdegi 
  reg[1] = u.h[1];   // High word // union ki sam wahi memory again next 16 bit ke second half ko read kreg 2 register me save kredegi .
} 

// hex to float 
float hexToFloat(uint16_t low, uint16_t high) { // pehele first and then second part
  union {
    uint16_t h[2];
    float f;
  } u;

  u.h[0] = low; // first 16bit - register
  u.h[1] = high; // second 16 bit -register
  return u.f; // convert hex to float 
}

void setup() {
  Serial.begin(115200);

 // rx tx 32 33 
  Serial2.begin(9600, SERIAL_8N1, 32, 33);

  mb.begin(&Serial2);
  mb.slave(1);

  // calling  float to hex 
  floatToHex(resistor1, regR1);
  floatToHex(resistor2, regR2);
  floatToHex(resistor3, regR3);

  // Add Holding Registers (FLOAT = 2 REGISTERS)
  mb.addHreg(0, regR1[0]); // register 0 or regsiter ki value 
  mb.addHreg(1, regR1[1]); //holding regisster 1 or register ki value 

  mb.addHreg(2, regR2[0]);
  mb.addHreg(3, regR2[1]);

  mb.addHreg(4, regR3[0]);
  mb.addHreg(5, regR3[1]);

  Serial.println("Modbus Ready");
}

void loop() {
  mb.task(); // handel modbus master rekuest 
 
  //registers and merge to float
  float r1 = hexToFloat(mb.Hreg(0), mb.Hreg(1)); // merge krdenge 
  float r2 = hexToFloat(mb.Hreg(2), mb.Hreg(3));
  float r3 = hexToFloat(mb.Hreg(4), mb.Hreg(5));

  Serial.print("R1: "); Serial.print(r1);
  Serial.print("  R2: "); Serial.print(r2);
  Serial.print("  R3: "); Serial.println(r3);

  delay(1000);
}

m

