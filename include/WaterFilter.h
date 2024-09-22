// WaterFilter.h

#ifndef WATERFILTER_H
#define WATERFILTER_H

#include <Arduino.h>
#include "LEDControl.h"
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
    void update(float totalVolume);  // Accept total volume from WaterFlowSensor

    // Reset the filter usage (e.g., when the filter is replaced)
    void resetFilter();

    // Getters
    float getUsedPercentage();       // Returns used percentage of the filter
    float getFilterCapacity() const; // Returns filter capacity in liters

private:
    // Components
    LEDControl _ledControl;

    // Reset button pin
    uint8_t _resetButtonPin;

    // Filter capacity (constant)
    const float _filterCapacity;

    // State variables
    float _usedPercentage;
    float _totalVolume;

    // Private methods
    void _updateLEDs();
    void _handleResetButton();
};

#endif // WATERFILTER_H
