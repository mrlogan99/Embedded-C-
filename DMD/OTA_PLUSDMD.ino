/******************** LIBRARIES ********************/
#include <DMD32Plus.h>
#include "fonts/SystemFont5x7.h"
#include "fonts/Arial_black_16.h"

#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

/******************** DISPLAY CONFIG ********************/
#define DISPLAYS_ACROSS 4
#define DISPLAYS_DOWN   6

#define PANEL_W 32
#define PANEL_H 16

#define SCREEN_W (DISPLAYS_ACROSS * PANEL_W)   // 128
#define SCREEN_H (DISPLAYS_DOWN   * PANEL_H)   // 96

/******************** PIN CONFIG ********************/
#define OE_PIN     22
#define A_PIN      19
#define B_PIN      21
#define CLK_PIN    18
#define LAT_PIN    4
#define R_DATA_PIN 23

#define LED_BUILTIN 2

/******************** WIFI ********************/
const char* ssid     = "DMD";
const char* password = "12345678";

/******************** DMD OBJECT ********************/
DMD dmd(
  DISPLAYS_ACROSS,
  DISPLAYS_DOWN,
  OE_PIN,
  A_PIN,
  B_PIN,
  CLK_PIN,
  LAT_PIN,
  R_DATA_PIN
);

/******************** TIMER ********************/
hw_timer_t *timer = NULL;

/******************** DMD SCAN ISR ********************/
void IRAM_ATTR triggerScan() {
  dmd.scanDisplayBySPI();
}

/******************** SETUP ********************/
void setup() {
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  /******** WIFI ********/
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    delay(3000);
    ESP.restart();
  }

  /******** OTA ********/
  ArduinoOTA.setHostname("DMD-ESP32");

  ArduinoOTA.onStart([]() {
    timerAlarmDisable(timer);
    dmd.clearScreen(true);
  });

  ArduinoOTA.onEnd([]() {
    timerAlarmEnable(timer);
  });

  ArduinoOTA.begin();

  /******** TIMER INIT ********/
  timer = timerBegin(0, 60, true);
  timerAttachInterrupt(timer, &triggerScan, true);
  timerAlarmWrite(timer, 2500, true);
  timerAlarmEnable(timer);

  /******** DMD INIT ********/
  dmd.setBrightness(8);
  dmd.clearScreen(true);
}

/******************** ANIMATIONS ********************/

// ---------- SCROLL RIGHT → LEFT (CENTER HEIGHT)
void animScrollRL(const char* txt) {
  static int x = SCREEN_W;
  static unsigned long t = 0;

  if (millis() - t > 40) {
    t = millis();
    dmd.clearScreen(true);
    dmd.selectFont(SystemFont5x7);

    int y = (SCREEN_H - 7) / 2;
    dmd.drawString(x, y, txt, strlen(txt), GRAPHICS_NORMAL);

    x -= 2;
    if (x < -(strlen(txt) * 6)) x = SCREEN_W;
  }
}

// ---------- DROP TEXT (CENTER X)
void animDrop(const char* txt) {
  static int y = -16;
  static unsigned long t = 0;

  if (millis() - t > 60) {
    t = millis();
    dmd.clearScreen(true);
    dmd.selectFont(Arial_Black_16);

    int textW = strlen(txt) * 16;
    int x = (SCREEN_W - textW) / 2;

    dmd.drawString(x, y, txt, strlen(txt), GRAPHICS_NORMAL);

    y += 2;
    if (y > (SCREEN_H - 16)) y = -16;
  }
}

// ---------- BLINK TEXT (FULL CENTER)
void animBlink(const char* txt) {
  static bool on = false;
  static unsigned long t = 0;

  if (millis() - t > 400) {
    t = millis();
    on = !on;
    dmd.clearScreen(true);

    if (on) {
      dmd.selectFont(SystemFont5x7);

      int textW = strlen(txt) * 6;
      int x = (SCREEN_W - textW) / 2;
      int y = (SCREEN_H - 7) / 2;

      dmd.drawString(x, y, txt, strlen(txt), GRAPHICS_NORMAL);
    }
  }
}

// ---------- BOUNCE BOX (FULL 4×6 AREA)
void animBounceBox() {
  static int x = 0, y = 0, dx = 2, dy = 2;
  static unsigned long t = 0;

  int boxW = 30;
  int boxH = 15;

  if (millis() - t > 40) {
    t = millis();
    dmd.clearScreen(true);

    dmd.drawBox(x, y, x + boxW, y + boxH, GRAPHICS_NORMAL);

    x += dx;
    y += dy;

    if (x <= 0 || x >= (SCREEN_W - boxW)) dx = -dx;
    if (y <= 0 || y >= (SCREEN_H - boxH)) dy = -dy;
  }
}

/******************** LOOP ********************/
void loop() {
  ArduinoOTA.handle();

  static uint8_t mode = 0;
  static unsigned long change = 0;

  if (millis() - change > 7000) {
    change = millis();
    mode++;
    if (mode > 3) mode = 0;
    dmd.clearScreen(true);
  }

  switch (mode) {
    case 0: animScrollRL("LOGAN"); break;
    case 1: animDrop("OTA");       break;
    case 2: animBlink("HAPPY BIRTHDAY ADIL JI!");    break;
    // case 3: animBounceBox();       break;
  }
}
