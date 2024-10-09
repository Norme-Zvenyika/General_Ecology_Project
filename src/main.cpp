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
#define FLOW_SENSOR_PIN 39
#define RESET_BUTTON_PIN 36
#define SERVICE_UUID           "cca082e8-8ed2-4a53-9b7c-ff8f5ffc978d"
#define CHARACTERISTIC_UUID_RX "ecc6cf21-ed9b-40be-ba42-75ea9ca44b12"
#define CHARACTERISTIC_UUID_TX "a0817b22-9d2e-40d1-bb9f-ec167871ab76"

// global constants
const float FILTER_CAPACITY_LITERS = 1.0;              // Filter capacity in liters
const float PULSES_PER_LITER_CONVERSION = 746;      // Pulses per liter conversion (1L = 1260 pulses)
const float FLOW_RATE_SCALING_FACTOR = 12.4;           // Flow rate scaling factor (21 Hz per L/min)
const String DEVICE_NAME = "GE Filter PID";

// Timing variable for non-blocking update
unsigned long lastUpdateTime = 0;
const unsigned long UPDATE_INTERVAL_MS = 1000;

// Create an instance of the PID class
PID pidSystem(RED_LED_PIN, YELLOW_LED_PIN, GREEN_LED_PIN, FLOW_SENSOR_PIN, RESET_BUTTON_PIN, 
              FILTER_CAPACITY_LITERS, PULSES_PER_LITER_CONVERSION, FLOW_RATE_SCALING_FACTOR, 
              SERVICE_UUID,CHARACTERISTIC_UUID_RX, CHARACTERISTIC_UUID_TX, DEVICE_NAME);

void setup() {
    Serial.begin(115200);
    pidSystem.begin();
}

void loop() {
    unsigned long currentMillis = millis();

    // Non-blocking timing approach to call pidSystem.update()
    if (currentMillis - lastUpdateTime >= UPDATE_INTERVAL_MS) {
        pidSystem.update();
        lastUpdateTime = currentMillis;
    }
}


