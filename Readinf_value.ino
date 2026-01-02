#include <ModbusMaster.h>

ModbusMaster node;
// defining the slave 
const uint8_t totalSlaves = 4;
uint8_t currentSlave = 1;

// defining the time 
unsigned long lastReadTime = 0;

const unsigned long interval = 2000;

// defining the reg 

const uint8_t regCount = 4;

// par_val array 
float para_val[6];

// 0 PM2.5 | 1 PM10 | 2 TEMP | 3 HUM | 4 SOX | 5 NOX

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, 16, 17);
  Serial.println("started the Ep32 Modbus ");
}

void readSlave(uint8_t slaveID) {

  node.begin(slaveID, Serial2);
  uint8_t result = node.readHoldingRegisters(0, regCount);

  if (result != node.ku8MBSuccess) {
    Serial.print("Slave ");
    Serial.print(slaveID);
    Serial.println(" Read Failed");
    return;
  }

// reading slave 1 

  if (slaveID == 1) {
    para_val[4] = node.getResponseBuffer(3) / 100.0;
    Serial.print("SLAVE 1 ");
    Serial.print("SOX:");
    Serial.println(para_val[4]);
  }

  // reading slave 2  

  else if (slaveID == 2) {
    para_val[5] = node.getResponseBuffer(2) / 100.0;
    Serial.print("SLAVE 2 ");
    Serial.print("NOX:");
    Serial.println(para_val[5]);
  }

  // slave 3 

  else if (slaveID == 3) {
    para_val[2] = node.getResponseBuffer(0) / 10.0;
    para_val[3] = node.getResponseBuffer(1);

    Serial.print("SLAVE 3 ");
    Serial.print("TEMP: ");
    Serial.print(para_val[2]);
    Serial.print("HUM:");
    Serial.println(para_val[3]);
  }

  // slave 4 

  else if (slaveID == 4) {
    para_val[0] = node.getResponseBuffer(0);
    para_val[1] = node.getResponseBuffer(1);

    Serial.print("SLAVE 4 → ");
    Serial.print("PM2.5: ");
    Serial.print(para_val[0]);
    Serial.print("  PM10: ");
    Serial.println(para_val[1]);
  }
}

void loop() {

  if (millis() - lastReadTime >= interval) {
    lastReadTime = millis();

    readSlave(currentSlave);
    currentSlave++;

    if (currentSlave > totalSlaves) {
      currentSlave = 1;
      Serial.println("-----");
    }
  }
}
