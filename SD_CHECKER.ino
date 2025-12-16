#include <SD.h>
#define SD_CS 53

void setup() {
  Serial.begin(9600);
  pinMode(SD_CS, OUTPUT);
  Serial.println("SD Test");

  if (SD.begin(SD_CS)) {
    Serial.println("SD INIT OK ✅");
  } else {
    Serial.println("SD INIT FAIL ❌");
  }
}

void loop() {}
