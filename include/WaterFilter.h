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
        uint8_t resetButtonPin,
        const float filterCapacityLiters);

    // Initialize the water filter system
    void begin();

    // Update method to be called regularly in the main loop
    void update(float flowRate, float totalVolume); // Accept flow rate and total volume from WaterFlowSensor

    // Reset the filter usage (e.g., when the filter is replaced)
    void resetFilter();

    // Getters
    float getUsedPercentage(); // Returns used percentage of the filter
    float getFilterCapacity() const;

private:
    // Components
    LEDControl _ledControl;
    BLE _bleModule;

    // Reset button pin
    uint8_t _resetButtonPin;

    // Filter capacity (constant)
    const float _filterCapacity;

    // State variables
    float _usedPercentage;

    // Private methods
    void _updateLEDs();
    void _handleResetButton();
};

#endif // WATERFILTER_H
