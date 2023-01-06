#ifndef SETTINGS_H
#define SETTINGS_H

// --- Choose one ---
#define BOOTSEL_BUTTON_MODE_BOOTSEL
// #define BOOTSEL_BUTTON_MODE_CLEAR_MEASUREMENTS

// --- Pins ---
#define PIN_VOUTDIV             26
#define PIN_VDROPAMP            27
#define PIN_GNDREF              28

// --- Number/speed of measurements to average ---
#define NUM_OF_MEASUREMENTS     1000    // ~1000 is best
#define MEASUREMENT_DELAY       1       // in ms

#endif
