=====================================

# Master Code 
  #include <ModbusMaster.h>

ModbusMaster node;


// converting hex to float 

float hexToFloat(uint16_t low, uint16_t high) {
  union {
    uint16_t h[2];
    float f;
  } u;
  u.h[0] = low;
  u.h[1] = high; 
  return u.f;
}

void setup() {
  Serial.begin(115200);

 
  Serial1.begin(9600, SERIAL_8N1, 32, 33); //  uart communication 

  node.begin(1, Serial1);   // Slave ID = 1
  Serial.println("Modbus Master Ready");
}

void loop() {
  uint8_t result = node.readHoldingRegisters(0, 8);  // 0-8 register data 

  if (result == node.ku8MBSuccess) {
    Serial.println("---- DATA RECEIVED ----");

    Serial.print("PM2.5: ");
    Serial.println(hexToFloat(
      node.getResponseBuffer(0), // pehel o register ki value 
      node.getResponseBuffer(1))); // then 1 regiter ki value in dono ko merge krke float same condition applied for all..
  
    Serial.print("PM10 : ");
    Serial.println(hexToFloat(
      node.getResponseBuffer(2),
      node.getResponseBuffer(3)));

    Serial.print("TEMP : ");
    Serial.println(hexToFloat(
      node.getResponseBuffer(4),
      node.getResponseBuffer(5)));

    Serial.print("HUM  : ");
    Serial.println(hexToFloat(
      node.getResponseBuffer(6),
      node.getResponseBuffer(7)));

    Serial.println("-----------------------");
  } else {
    Serial.print("Read Failed: ");
    Serial.println(result);
  }

  delay(1000);
}