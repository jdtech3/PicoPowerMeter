#include <Arduino.h>
#include "SPI.h"
#include <TFT_eSPI.h>

// Default color definitions
#define TFT_BLACK       0x0000      /*   0,   0,   0 */
#define TFT_NAVY        0x000F      /*   0,   0, 128 */
#define TFT_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define TFT_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define TFT_MAROON      0x7800      /* 128,   0,   0 */
#define TFT_PURPLE      0x780F      /* 128,   0, 128 */
#define TFT_OLIVE       0x7BE0      /* 128, 128,   0 */
#define TFT_LIGHTGREY   0xD69A      /* 211, 211, 211 */
#define TFT_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define TFT_BLUE        0x001F      /*   0,   0, 255 */
#define TFT_GREEN       0x07E0      /*   0, 255,   0 */
#define TFT_CYAN        0x07FF      /*   0, 255, 255 */
#define TFT_RED         0xF800      /* 255,   0,   0 */
#define TFT_MAGENTA     0xF81F      /* 255,   0, 255 */
#define TFT_YELLOW      0xFFE0      /* 255, 255,   0 */
#define TFT_WHITE       0xFFFF      /* 255, 255, 255 */
#define TFT_ORANGE      0xFDA0      /* 255, 180,   0 */
#define TFT_GREENYELLOW 0xB7E0      /* 180, 255,   0 */
#define TFT_PINK        0xFE19      /* 255, 192, 203 */
#define TFT_BROWN       0x9A60      /* 150,  75,   0 */
#define TFT_GOLD        0xFEA0      /* 255, 215,   0 */
#define TFT_SILVER      0xC618      /* 192, 192, 192 */
#define TFT_SKYBLUE     0x867D      /* 135, 206, 235 */
#define TFT_VIOLET      0x915C      /* 180,  46, 226 */

#define PS_PIN      23
#define PIN_VOUTDIV 26
#define PIN_GNDREF  28

TFT_eSPI tft = TFT_eSPI();

double voltage = 0.0;
double current = 0.0;
// double power = 0.0;
double energy = 0.0;
double* power = NULL;
// PicoAnalogCorrection pico(12, 3.3);

void setup() {
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_VOUTDIV, INPUT);
  pinMode(PIN_GNDREF, INPUT);
  // pico.calibrateAdc(PIN_GNDREF, PIN_VOUTDIV, 5000);
  tft.init();
  tft.setRotation(1);
  tft.setTextSize(1);
  tft.fillScreen(TFT_BLACK);
  analogReadResolution(12);
  digitalWrite(PS_PIN, HIGH);
}

void loop() {
  double sum=0.0;
  for(int i = 0; i< 1000; i++){
    sum += (analogRead(PIN_VOUTDIV) - analogRead(PIN_GNDREF));
    delay(1);
  }
  sum /= 1000.0;

  voltage = (((sum) * (3.295 / 4096.0) / 0.130383) - 0.22) * 1.048365;
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
