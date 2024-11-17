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
    WaterFilter(LEDControl& ledControl, ResetButton& resetButton, WaterFlowSensor& waterFlowSensor, const float filterCapacityLiters);

    // Update method to be called regularly in the main loop
    void update();

    // method to initialize the water filter
    void begin();

    // Reset the filter usage (e.g., when the filter is replaced)
    void resetFilter();

    // Getters
    float getUsedPercentage() const;  // Returns used percentage of the filter
    float getFilterCapacity() const;  // Returns filter capacity in liters
    float getTotalVolume() const;     // Returns total volume filtered so far
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
};

#endif // WATERFILTER_H
