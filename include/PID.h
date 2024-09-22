// PID.h

#ifndef PID_H
#define PID_H

#include <Arduino.h>
#include "LEDControl.h"
#include "WaterFlowSensor.h"
#include "ResetButton.h"
#include "WaterFilter.h"
#include "BLE.h"

class PID
{
public:
    // Constructor that accepts the necessary constants and pins
    PID(uint8_t redLedPin, uint8_t yellowLedPin, uint8_t greenLedPin, uint8_t flowSensorPin, uint8_t resetButtonPin, 
        float filterCapacityLiters, float pulsesPerLiterConversion, float flowRateScalingFactor);

    // Initialize all components (LEDs, flow sensor, reset button, BLE, WaterFilter)
    void begin();

    // Run all components (update WaterFilter, handle display, etc.)
    void update();

private:
    // Components
    LEDControl _ledControl;
    WaterFlowSensor _waterFlowSensor;
    ResetButton _resetButton;
    WaterFilter _waterFilter;
    BLE _ble;

    // Global constants
    float _filterCapacityLiters;
    float _pulsesPerLiterConversion;
    float _flowRateScalingFactor;
};

#endif // PID_H
