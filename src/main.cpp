// main.cpp
#include "WaterFilter.h"
#include <Arduino.h>
#include <Wire.h>

// Define pin numbers
#define RED_LED_PIN 18
#define YELLOW_LED_PIN 17
#define GREEN_LED_PIN 16
#define FLOW_SENSOR_PIN 36
#define RESET_BUTTON_PIN 39

// Define global constants for filter capacity and pulses per liter
const float FILTER_CAPACITY = 1.0;     // Filter capacity in liters
const float PULSES_PER_LITER = 1260.0; // Pulses per liter

// Create an instance of WaterFilter using the global constants
WaterFilter waterFilter(RED_LED_PIN, YELLOW_LED_PIN, GREEN_LED_PIN, FLOW_SENSOR_PIN, RESET_BUTTON_PIN, FILTER_CAPACITY, PULSES_PER_LITER);

float flowRate = 1.23; // Example flow rate in L/min
float totalWaterFiltered = 10.5;

void setup()
{
  // // display to the LED
  // Wire.begin();

  // // Set up Serial communication with a baud rate of 115200
  // Serial.begin(115200);

  // // start the filter
  // waterFilter.begin();

  // display to the terminal
  // Serial.begin(9600);

  // uncomment the following for calibration
  Wire.begin();
  Serial.begin(115200);

  // Initialize the Water Filter system
  waterFilter.begin();

  // Simulate a 1-liter pour to calibrate
  Serial.println("Please pour 1 liter of water to calibrate...");
  delay(60000);               // Wait 10 seconds for you to pour water
  waterFilter.calibrate(0.3); // eneter the amount you want to pour
}

void loop()
{
  waterFilter.update();

  // static unsigned long lastPrintTime = 0;
  // unsigned long currentMillis = millis();
  // if (currentMillis - lastPrintTime >= 1000)
  // {
  //   lastPrintTime = currentMillis;
  //   Serial.print("Flow Rate: ");
  //   Serial.print(waterFilter.getFlowRate(), 3);
  //   Serial.print(" L/min\tTotal Volume: ");
  //   Serial.print(waterFilter.getTotalVolume(), 3);
  //   Serial.println(" L");
  // }
  delay(1000);
}
