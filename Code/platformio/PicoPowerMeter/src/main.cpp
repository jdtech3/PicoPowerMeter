#include <Arduino.h>    // Arduino core
#include "SPI.h"        // LCD
#include <TFT_eSPI.h>

extern "C" {                  // Reboot into USB mode
  #include "pico/bootrom.h"   // C linkage guard required per https://forums.raspberrypi.com/viewtopic.php?t=302963
}

#include "utils.h"      // Bootsel detection
#include "settings.h"   // Settings

#define PS_PIN      23

TFT_eSPI tft = TFT_eSPI();

double voltage;
double current;
double power;
double energy;

void reset_measurements() {
  voltage = 0.0;
  current = 0.0;
  energy = 0.0;
  power = 0.0;
}

void setup() {
  // Pins
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_VOUTDIV, INPUT);
  pinMode(PIN_GNDREF, INPUT);

  // TFT
  tft.init();
  tft.setRotation(1);
  tft.setTextSize(1);
  tft.fillScreen(TFT_BLACK);

  // Measurements
  analogReadResolution(12);
  digitalWrite(PS_PIN, HIGH);
  reset_measurements();
}

void loop() {
  if (get_bootsel_button()) {
    digitalWrite(PIN_LED, HIGH);

    #ifdef BOOTSEL_BUTTON_MODE_BOOTSEL
      reset_usb_boot(0, 0);
    #elif BOOTSEL_BUTTON_MODE_CLEAR_MEASUREMENTS
      reset_measurements();
    #endif
  }

  double sum = 0.0;
  for (int i = 0; i < NUM_OF_MEASUREMENTS; i++) {
    sum += (analogRead(PIN_VOUTDIV) - analogRead(PIN_GNDREF));
    delay(MEASUREMENT_DELAY);
  }
  sum /= (double)NUM_OF_MEASUREMENTS;

  voltage = (((sum) * (3.295 / 4096.0) / 0.130383) - 0.22) * 1.048365;  // TODO: replace magic with settings
  // voltage *= 1.0577;
  // voltage = analogRead(PIN_ADC0);

  tft.setCursor(3, 3);
  char buf[16];
  sprintf(buf, "V: %.4f V", voltage);
  tft.print("                ");
  tft.setCursor(3, 3);
  tft.print(buf);
  // strcpy(buf, "             \0");
  // tft.print("V: 0.000 V");

  tft.setCursor(3, 23);
  // tft.print(pico.analogRead(PIN_GNDREF));
  // tft.print("I: 0.000 A");

  tft.setCursor(3, 43);
  // tft.print(pico.analogCRead(PIN_GNDREF, 10));
  // tft.print("P: 0.000 W");

  tft.setCursor(3, 63);
  // tft.print("E: 0.000 Wh");

  delay(10);
}
