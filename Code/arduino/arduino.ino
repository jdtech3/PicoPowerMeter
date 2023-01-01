#include <PicoAnalogCorrection.h>
#include <LiquidCrystal.h> 
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
LiquidCrystal lcd(1,2,3,4,5,6,7,8); // pins connected to lcd screen

PicoAnalogCorrection pico(ADC_RES, VREF);

void setup() // runs once when the sketch starts
{
  // make the LED pin (pin 13) an output pin
  pinMode(ledPin, OUTPUT);
  pinMode(vcc,OUTPUT);
  pinMode(gnd,OUTPUT);
  lcd.begin(16, 2);    // initializing the dimensions of LCD    
  lcd.setCursor(2,0);   //  giving the  start location for the data    
  lcd.print("Energy Consumption(J)");
  analogReadResolution(ADC_RES);

  // Calibrate ADC using an average of 5000 measurements
  pico.calibrateAdc(GND_PIN, VCC_PIN, 5000);
  
  Serial.begin(9600);

}

void loop() // runs repeatedly after setup() finishes
{
  sensorValue1 = pico.analogRead(sensorPin1, 10);  // read pin 32   
  sensorValue2 = pico.analogRead(sensorPin2, 10);
  current = sensorValue1/(80*0.02);
  power = (float*)malloc(10*sizeof(float));
  *(power+0) = current*sensorValue2*(115/15);
  Serial.println(power[0]) // output power to serial
  
  if (sensorValue1/(80) + sensorValue2*(115/15) < 24 ) {            // less than 24?
    digitalWrite(ledPin, LOW); }     // keep the LED off
  
  else {                               // greater than 24?
    digitalWrite(ledPin, HIGH); } // turn the LED on
 
  if (power > 0) {
    lcd.setCursor(1,1); // time to display
    summation += (new_value+power[0])*0.1/2;
    lcd.print("%.5f", summation);
    lcd.print((char)74);
    lcd.print("j");
    new_value = power[0];
    newest_value = summation;
  }else{
   lcd.setCursor(1,1);
   lcd.print("%.5f", newest_value);
   lcd.print((char)74);
   lcd.print("j");
   }
  free(power);
  power = NULL;
  delay(100);             // Pause 100 milliseconds
}
