// main.cpp
#include "PID.h"
#include "BLE.h"
#include "WaterFlowSensor.h"
#include <Arduino.h>
#include <Wire.h>

// Define pin numbers
#define RED_LED_PIN 18
#define YELLOW_LED_PIN 17
#define GREEN_LED_PIN 16
#define FLOW_SENSOR_PIN 36
#define RESET_BUTTON_PIN 39

// global constants
const float FILTER_CAPACITY_LITERS = 1.0;              // Filter capacity in liters
const float PULSES_PER_LITER_CONVERSION = 1260.0;      // Pulses per liter conversion (1L = 1260 pulses)
const float FLOW_RATE_SCALING_FACTOR = 21.0;           // Flow rate scaling factor (21 Hz per L/min)
const String DEVICE_NAME = "GE Filter PID";

// Create an instance of the PID class
PID pidSystem(RED_LED_PIN, YELLOW_LED_PIN, GREEN_LED_PIN, FLOW_SENSOR_PIN, RESET_BUTTON_PIN, 
              FILTER_CAPACITY_LITERS, PULSES_PER_LITER_CONVERSION, FLOW_RATE_SCALING_FACTOR, DEVICE_NAME);

void setup() {
    Serial.begin(115200);
    pidSystem.begin();
}

void loop() {
    // Update and run the system
    pidSystem.update(); 
    delay(1000);
}