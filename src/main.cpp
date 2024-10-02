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
const float PULSES_PER_LITER_CONVERSION = 771.4;      // Pulses per liter conversion (1L = 1260 pulses)
const float FLOW_RATE_SCALING_FACTOR = 12.857;           // Flow rate scaling factor (21 Hz per L/min)
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

// #include <Arduino.h>

// // Define pin number for flow sensor
// #define FLOW_SENSOR_PIN 39

// // Constants for calculation
// const float PULSES_PER_LITER = 771.4;    // Pulses per liter conversion (1L = 1260 pulses)
// const float FLOW_RATE_SCALING_FACTOR = 12.857;  // Flow rate scaling factor (21 Hz per L/min)
// const unsigned long CALC_INTERVAL_MS = 1000;  // Calculation interval in milliseconds (1 second)

// // Variables for flow measurement
// volatile unsigned long pulseCount = 0;    // Pulse count from the sensor
// float flowRate = 0.0;                     // Flow rate in L/min
// float totalLitersPassed = 0.0;            // Total volume passed in liters
// const float volumePerPulse = 1.0 / PULSES_PER_LITER; // Volume per pulse in liters
// unsigned long lastCalcTime = 0;           // The time of the last calculation
// unsigned long lastPrintTime = 0;          // The time of the last print

// void IRAM_ATTR pulseCounter() {
//     pulseCount++;
// }

// void setup() {
//     Serial.begin(115200);

//     // Set up the flow sensor pin as an input with an internal pull-up resistor
//     pinMode(FLOW_SENSOR_PIN, INPUT_PULLUP);

//     // Attach interrupt to the sensor pin, triggering on every falling edge
//     attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN), pulseCounter, FALLING);

//     // Set initial calculation and print time
//     lastCalcTime = millis();
//     lastPrintTime = millis();
// }

// void loop() {
//     unsigned long currentMillis = millis();

//     // Calculate flow rate and volume passed every CALC_INTERVAL_MS
//     if (currentMillis - lastCalcTime >= CALC_INTERVAL_MS) {
//         // Disable interrupts while accessing shared variables
//         noInterrupts();
//         unsigned long currentPulseCount = pulseCount;
//         pulseCount = 0;
//         interrupts();

//         // Calculate the flow rate in L/min
//         float frequency = currentPulseCount / (CALC_INTERVAL_MS / 1000.0); // Pulses per second (Hz)
//         flowRate = frequency / FLOW_RATE_SCALING_FACTOR; // Convert to L/min using scaling factor

//         // Calculate total volume passed
//         float volumePassed = currentPulseCount * volumePerPulse; // Volume in liters
//         totalLitersPassed += volumePassed;

//         // Update the last calculation time
//         lastCalcTime = currentMillis;
//     }

//     // Print values every few seconds (e.g., every 3 seconds)
//     if (currentMillis - lastPrintTime >= 3000) {
//         Serial.print("Flow rate: ");
//         Serial.print(flowRate);
//         Serial.print(" L/min\t");

//         Serial.print("Total volume: ");
//         Serial.print(totalLitersPassed);
//         Serial.println(" L");

//         // Update the last print time
//         lastPrintTime = currentMillis;
//     }
// }


