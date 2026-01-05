#include <ModbusMaster.h>

// Slave ID 1
ModbusMaster node;

void setup()
{
  Serial.begin(115200);
  Serial1.begin(9600, SERIAL_8N1, 32, 33);
  node.begin(1,Serial1);

  Serial.println("Modbus Master Ready");
}

void loop()
{
  static uint32_t i;
  uint8_t j, result;
  uint16_t data[3];  



  // Read ONLY 3 holding registers starting from address 0
  result = node.readHoldingRegisters(0, 3);

  if (result == node.ku8MBSuccess)
  {
    Serial.println("- DATA RECEIVED -");

    for (j = 0; j < 3; j++)
    {
      data[j] = node.getResponseBuffer(j);
      Serial.print("Register ");
      Serial.print(j);
      Serial.print(": ");
      Serial.println(data[j]);
    }

    Serial.println("----");
  }
  else
  {
    Serial.print("Read Failed, Error: ");
    Serial.println(result);
  }

  delay(1000);
}
