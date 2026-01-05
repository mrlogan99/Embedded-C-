

#include <ModbusMaster.h>

ModbusMaster node;

// hex to float // slave raw data dega isley yha nhi hex to float
float hexToFloat(uint16_t low, uint16_t high)
{
  union {
    uint16_t h[2]; // h 2 elem ka array - 
    float f;
  } u;

  u.h[0] = low;
  u.h[1] = high;
  return u.f;
}

void setup()
{
  Serial.begin(115200);
  Serial1.begin(9600, SERIAL_8N1, 16, 17);

  node.begin(1, Serial1);   // Slave id = 1

  Serial.println("Modbus Master Ready");
}

void loop()
{
  uint8_t result;

  // 1 FLOAT = 2 registers
  // 3 floats = 6 registers
  result = node.readHoldingRegisters(0, 6); // ye 0 se 6 regiter tak sabki value dedega

  if (result == node.ku8MBSuccess)
  {
    Serial.println("---- DATA RECEIVED ----");

    float r1 = hexToFloat(
      node.getResponseBuffer(0), //register 0 or 1 ko jod ke 1 float bnega 
      node.getResponseBuffer(1)
    );

    float r2 = hexToFloat(
      node.getResponseBuffer(2),
      node.getResponseBuffer(3)
    );

    float r3 = hexToFloat(
      node.getResponseBuffer(4),
      node.getResponseBuffer(5)
    );

    Serial.print("R1: "); Serial.println(r1);
    Serial.print("R2: "); Serial.println(r2);
    Serial.print("R3: "); Serial.println(r3);

    Serial.println("-----------------------");
  }
  elses
  {
    Serial.print("Read Failed, Error: ");
    Serial.println(result);   
  }

  delay(1000);
}
