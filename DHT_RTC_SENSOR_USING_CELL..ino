#include <Wire.h>
#include "RTClib.h"
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11

RTC_DS3231 rtc;
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Wire.begin();

  if (!rtc.begin()) {
    Serial.println("RTC NOT FOUND");
    while (1);
  }

  dht.begin();

  Serial.println("Enter Date & Time:");
  Serial.println("FORMAT: YYYY MM DD HH MM SS");
}

void loop() {

  // 🔹 MANUAL TIME SET FROM SERIAL MONITOR
  if (Serial.available()) {
    int year   = Serial.parseInt();
    int month  = Serial.parseInt();
    int day    = Serial.parseInt();
    int hour   = Serial.parseInt();
    int minute = Serial.parseInt();
    int second = Serial.parseInt();

    rtc.adjust(DateTime(year, month, day, hour, minute, second));

    Serial.println("✅ DATE & TIME SET SUCCESSFULLY");
    Serial.println("-----------------------------");

    while (Serial.available()) Serial.read(); // clear buffer
  }

  DateTime now = rtc.now();

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  Serial.print("Time: ");
  if (now.hour() < 10) Serial.print("0");
  Serial.print(now.hour());
  Serial.print(":");
  if (now.minute() < 10) Serial.print("0");
  Serial.print(now.minute());
  Serial.print(":");
  if (now.second() < 10) Serial.print("0");
  Serial.println(now.second());

  Serial.print("Date: ");
  if (now.day() < 10) Serial.print("0");
  Serial.print(now.day());
  Serial.print("-");
  if (now.month() < 10) Serial.print("0");
  Serial.print(now.month());
  Serial.print("-");
  Serial.println(now.year());

  Serial.print("Temp: ");
  Serial.print(temperature);
  Serial.println(" C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  Serial.println("-------------------");
  delay(2000);
}
