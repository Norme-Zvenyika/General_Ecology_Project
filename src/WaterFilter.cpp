#include "WaterFilter.h"

// Constructor
WaterFilter::WaterFilter(LEDControl& ledControl, ResetButton& resetButton, WaterFlowSensor& waterFlowSensor, const float filterCapacityLiters)
    : _ledControl(ledControl),           // Initialize LEDControl reference
      _resetButton(resetButton),         // Initialize ResetButton reference
      _waterFlowSensor(waterFlowSensor),  // Initialize WaterFlowSensor reference
      _filterCapacity(filterCapacityLiters),
      _usedPercentage(0.0),
      _totalVolume(0.0)  // Initialize total volume to 0
{
}

// register water filter for button press
void WaterFilter::begin() {
    _resetButton.registerCallback([this]() { resetFilter(); }); // Register callback
}

// Update method to be called regularly in the main loop
void WaterFilter::update()
{
    // call the water flow sensor
    _waterFlowSensor.update();

    // Get the volume passed in the current interval from the WaterFlowSensor
    float volumePassed = _waterFlowSensor.getVolumePassed();

    // Accumulate the total volume filtered
    _totalVolume += volumePassed;

    // Calculate the used percentage
    _usedPercentage = (_totalVolume / _filterCapacity) * 100.0;

    // Directly update LEDs based on used percentage
    _ledControl.update(_usedPercentage);
}

// Reset the filter usage when the filter is replaced
void WaterFilter::resetFilter()
{
    _totalVolume = 0.0;  // Reset the total volume
    _usedPercentage = 0.0;  // Reset the percentage

    // Turn off all LEDs
    _ledControl.allOff();
}

// Get used percentage based on filter capacity
float WaterFilter::getUsedPercentage() const
{
    return _usedPercentage;
}

// Get the total volume filtered so far
float WaterFilter::getTotalVolume() const
{
    return _totalVolume;
}

// Get filter capacity
float WaterFilter::getFilterCapacity() const
{
    return _filterCapacity;
}
