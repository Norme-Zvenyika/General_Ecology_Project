// WaterFilter.h

#ifndef WATERFILTER_H
#define WATERFILTER_H

#include <Arduino.h>
#include "LEDControl.h"
#include "BLE.h"
#include "WaterFlowSensor.h"

class WaterFilter
{
public:
    // Constructor
    WaterFilter(
        uint8_t redLedPin,
        uint8_t yellowLedPin,
        uint8_t greenLedPin,
        uint8_t flowSensorPin,
        uint8_t resetButtonPin,
        const float filterCapacityLiters,
        const float pulsesPerLiter);

    // Initialize the water filter system
    void begin();

    // Update method to be called regularly in the main loop
    void update();

    // Reset the filter usage (e.g., when the filter is replaced)
    void resetFilter();

    // Calibration method
    void calibrate(float knownVolume);

    // Getters
    float getFlowRate();       // Returns flow rate in L/min
    float getTotalVolume();    // Returns total volume in liters
    float getUsedPercentage(); // Returns used percentage of the filter
    float getFilterCapacity() const;
    float getPulsesPerLiter() const;

private:
    // Components
    LEDControl _ledControl;
    WaterFlowSensor _waterFlowSensor;
    BLE _bleModule;

    // Reset button pin
    uint8_t _resetButtonPin;

    // Filter capacity and pulses per liter (constant)
    const float _filterCapacity;
    const float _pulsesPerLiter;

    // State variables
    float _usedPercentage;

    // Private methods
    void _updateLEDs();
    void _handleResetButton();
};

#endif // WATERFILTER_H
