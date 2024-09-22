// WaterFilter.h

#ifndef WATERFILTER_H
#define WATERFILTER_H

#include <Arduino.h>
#include "LEDControl.h"
#include "WaterFlowSensor.h"
#include "ResetButton.h"

class WaterFilter
{
public:
    // Constructor
    WaterFilter(
        LEDControl& ledControl,         
        ResetButton& resetButton,       
        WaterFlowSensor& waterFlowSensor, 
        const float filterCapacityLiters);

    // Initialize the water filter system
    void begin();

    // Fetch and accumulate volume from WaterFlowSensor
    void update(); 

    // Reset the filter usage (e.g., when the filter is replaced)
    void resetFilter();

    // Getters
    float getUsedPercentage();       // Returns used percentage of the filter
    float getFilterCapacity() const; // Returns filter capacity in liters

private:
    // Components
    LEDControl& _ledControl;           
    ResetButton& _resetButton;         
    WaterFlowSensor& _waterFlowSensor;

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
