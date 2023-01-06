#include <hardware/watchdog.h>
#include <PicoAnalogCorrection.h>
#include "SPI.h"
#include <TFT_eSPI.h>

// Default color definitions
#define DISABLE_ALL_LIBRARY_WARNINGS
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

int vcc=A0;
int gnd=A1;
float sensorPin1 = 32;  // The Vdrop_amp on pin 32 
float sensorPin2 = 31;  // The Vout_div on pin 31                 
int ledPin = 13;     // The LED is connected on pin 13
float sensorValue1;     // variable1 to store data
float sensorValue2;     // Variable2 to store data
float current;  // Current I
float* power = NULL; // measured power
float newest_value; // the value that's gonna be displayed on the lcd screen after disconnection of the device
float new_value;
float summation = 0; // sum of little trapezoids over time
const uint8_t ADC_RES = 12; // ADC bits
const float VREF = 3.3; // Analog reference voltage
TFT_eSPI tft = TFT_eSPI();
PicoAnalogCorrection pico(ADC_RES, VREF);

void setup() // runs once when the sketch starts
{
  // make the LED pin (pin 13) an output pin
  pinMode(ledPin, OUTPUT);
  pinMode(vcc,OUTPUT);
  pinMode(gnd,OUTPUT);
  analogReadResolution(ADC_RES);
  tft.init();
  tft.setRotation(1);
  tft.setTextSize(2);
  tft.fillScreen(TFT_BLACK);
  // Calibrate ADC using an average of 5000 measurements
  pico.calibrateAdc(gnd, vcc, 5000);
  
  Serial.begin(9600);
  

}

void loop() // runs repeatedly after setup() finishes
{
  sensorValue1 = pico.analogRead(sensorPin1);  // read pin 32   
  sensorValue2 = pico.analogRead(sensorPin2);
  current = sensorValue1/(80*0.02);
  power = (float*)malloc(10*sizeof(float));
  *(power+0) = current*sensorValue2*(115/15);
  Serial.println(power[0]); // output power to serial
  
  if(sensorValue1/(80) + sensorValue2*(115/15) < 24 ){          
    digitalWrite(ledPin, LOW); 
  }else{                               
    digitalWrite(ledPin, HIGH);
  }
  
  tft.setCursor(3, 3);
  char* buf = (char*)malloc(100*sizeof(char));
  sprintf(buf, "P: %.3f W", power[0]);
  tft.print(buf);
  if (power[0] > 0) {
    // time to display
    tft.setCursor(3, 63);
    summation += (new_value+power[0])*0.1/2;
    new_value = power[0];
    char* buf2 = (char*)malloc(100*sizeof(char));
    sprintf(buf2, "E: %.3f J", summation);
    tft.print(buf2);
    newest_value = summation;
    free(power);
    free(buf2);
    buf2 = NULL;
    power = NULL;
  }else{
    tft.setCursor(3, 63);
    char* buf3 = (char*)malloc(100*sizeof(char));
    sprintf(buf3, "E: %.3f J", newest_value);
    tft.print(buf3);
    free(buf3);
    buf3 = NULL;
   }
  free(buf);
  buf = NULL;
  delay(100);             // Pause 100 milliseconds
}

void software_reset()
{
    watchdog_enable(1, 1);
    while(1);
}
