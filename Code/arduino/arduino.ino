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
#define PIN_VDROPAMP 27
#define Vout(x) (((x*(3.295/4096.0)/0.130383) - 0.22) * 1.048365)

TFT_eSPI tft = TFT_eSPI();

double voltage = 0.0;
double current = 0.0;
double energy = 0.0;

void setup() {
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_VOUTDIV, INPUT);
  pinMode(PIN_GNDREF, INPUT);
  tft.init();
  tft.setRotation(1);
  tft.setTextSize(1);
  tft.fillScreen(TFT_BLACK);
  analogReadResolution(12);
  digitalWrite(PS_PIN, HIGH);
}

void loop() {
  // Value of Vout (calibrated)
  double sum=0.0;
  for(int i = 0; i< 1000; i++){
    sum += (analogRead(PIN_VOUTDIV) - analogRead(PIN_GNDREF));
    delay(1);
  }
  sum /= 1000.0;
  voltage = Vout(sum);
  digitalWrite(PIN_LED, (voltage > 24)? HIGH : LOW);
  print_f("V: %.4f V", voltage, 3);
  // Value of I (calibrated)
  for(;;){
    current = analogRead(PIN_VDROPAMP)/80;
    delay(0.1);
    double test = analogRead(PIN_VDROPAMP)/80;
    if(test-current < 0.001 && current-test < 0.001)
      break;
  }
  print_f("I: %.4f A", current, 23);
  // value of power
  print_f("P: %.4f Wh", current*voltage, 43);
  // value of energy
  enegry += power;
  printf("E: %.4f J", energy, 63);
  delay(10);
}

void print_f(char* ptr, double a, int b){
  tft.setCursot(3, b);
  char buf[16];
  sprintf(buf, ptr, a);
  tft.print("                 ");
  tft.setCursor(3, b);
  tft.print(buf);
}
