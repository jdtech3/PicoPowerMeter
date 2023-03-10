#include "SPI.h"        // LCD
#include "pico/time.h" //for time span measurement
#include <TFT_eSPI.h> // also LCD

extern "C" {                  // Reboot into USB mode
  #include "pico/bootrom.h"   // C linkage guard required per https://forums.raspberrypi.com/viewtopic.php?t=302963
}

#include "utils.h"      // Bootsel detection
#include "settings.h"   // Settings

#define PS_PIN      23  // Pico power supply mode pin

#define TEXT_START_COL_OFFSET   3   // TFT text settings
#define TEXT_START_ROW_OFFSET   3
#define TEXT_ROW_SPACING        15
#define TEXT_FONT_SIZE          1

#define VOUT_CONVERSION(x) ((( x * (3.295 / 4096.0) / 0.130383) - 0.22) * 1.048365) //output correction for voltage
#define ADC_correction (3.295/4096.0)

TFT_eSPI tft = TFT_eSPI();

double voltage;
double current;
double power;
absolute_time_t last_measurement_time; //time stamp
double temporary_pow = 0.0; //temporarily store a power value
double energy;
double time;

void reset_measurements() {
  voltage = 0.0;
  current = 0.0;
  energy = 0.0;
  power = 0.0;
}

void println_to_tft(char* str, double val) {
  // Set cursor and clear
  tft.setCursor(TEXT_START_COL_OFFSET, TEXT_START_ROW_OFFSET + tft.getCursorY() + TEXT_ROW_SPACING);
  tft.print("                ");
  tft.setCursor(TEXT_START_COL_OFFSET, tft.getCursorY());

  // Format and print
  char buf[16];
  sprintf(buf, str, val);
  tft.print(buf);
}

void calibration(double *a){
  if(*a<=3.048){
    *a += 0.05013386496*(*a) + (-0.231968332926672);;
  }else if(*a <= 9.356){
    *a += 0.03801254021*(*a) + (-0.1511130402);
  }else if(*a <= 15.658){
    *a += 0.03641967881*(*a) + (-0.07988468282);
  }else if(*a <= 22.047){
     *a += 0.0387455535*(*a) + (-0.05433042421);
    }else{
    *a += 0.03681099704*(*a) + (0.05546201889);
    }
}

void setup() {
  // Pins
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_VOUTDIV, INPUT);
  pinMode(PIN_VDROPAMP, INPUT);
  pinMode(PIN_GNDREF, INPUT);

  // TFT
  tft.init();
  tft.setRotation(1);
  tft.setTextSize(TEXT_FONT_SIZE);
  tft.fillScreen(TFT_BLACK);

  // Measurements
  analogReadResolution(12);
  digitalWrite(PS_PIN, HIGH);
  reset_measurements();
  last_measurement_time = get_absolute_time();// the very first time stamp
}

void loop() {
  // BOOTSEL button detection
  if (get_bootsel_button()) {
    digitalWrite(PIN_LED, HIGH);

    #ifdef BOOTSEL_BUTTON_MODE_BOOTSEL
      reset_usb_boot(0, 0);
    #elif BOOTSEL_BUTTON_MODE_CLEAR_MEASUREMENTS
      reset_measurements();
    #endif
  }

  tft.setCursor(0, -TEXT_ROW_SPACING);  // reset cursor

  // Get and display Vout (calibrated)
  double sum = 0.0;
  for (int i = 0; i < NUM_OF_MEASUREMENTS; i++) {
    sum += (analogRead(PIN_VOUTDIV) - analogRead(PIN_GNDREF));
    delay(MEASUREMENT_DELAY);
  }
  sum /= NUM_OF_MEASUREMENTS;
  //calibration for Vout
  calibration(&voltage);
  voltage = VOUT_CONVERSION(sum);
  println_to_tft("V: %.3f V", voltage);

  digitalWrite(PIN_LED, (voltage > 24.0) ? HIGH : LOW);   // overvoltage warning


  // Get and display I (calibrated)
  for (;;) {
    current = analogRead(PIN_VDROPAMP)*ADC_correction / 80;
    delay(0.1);
    double test = analogRead(PIN_VDROPAMP)*ADC_correction / 80;
    if (abs((current - test) / current) < 0.001) break;
  }

  // Display I
  println_to_tft("I: %.3f A", current);
  
  // Calculate and display P
  power = current * voltage;
  absolute_time_t new_time = get_absolute_time();// the new time stamp
  println_to_tft("P: %.3f W", power);
  // Calculate delta time for energy
  time = absolute_time_diff_us(last_measurement_time, new_time)/(1000000*3600);
  // Calculate and display E
  energy += (power+temporary_pow)*time*0.5;
  println_to_tft("E: %.3f Wh", energy);
  temporary_pow = power;
  last_measurement_time = new_time;// pass on the time stamp
  delay(10);  // slow down loop
}
