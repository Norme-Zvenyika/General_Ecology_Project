// WaterFilter.cpp

#include "WaterFilter.h"

// Constructor
WaterFilter::WaterFilter(uint8_t redLedPin, uint8_t yellowLedPin, uint8_t greenLedPin, uint8_t resetButtonPin, const float filterCapacityLiters)
    : _ledControl(redLedPin, yellowLedPin, greenLedPin),
      _resetButtonPin(resetButtonPin),
      _filterCapacity(filterCapacityLiters),
      _usedPercentage(0.0),
      _totalVolume(0.0)
{
}

// Initialize the water filter system
void WaterFilter::begin()
{
    // Initialize components
    _ledControl.begin();

    // Initialize reset button pin
    pinMode(_resetButtonPin, INPUT_PULLUP);
}

// Update method to be called regularly in the main loop
void WaterFilter::update(float totalVolume)
{
    // Update total volume from WaterFlowSensor
    _totalVolume = totalVolume;

    // Calculate the used percentage
    _usedPercentage = (_totalVolume / _filterCapacity) * 100.0;

    // Update LEDs based on used percentage
    _updateLEDs();

    // Handle reset button press
    _handleResetButton();
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
float WaterFilter::getUsedPercentage()
{
    return _usedPercentage;
}

float WaterFilter::getFilterCapacity() const
{
    return _filterCapacity;
}

// Private methods

void WaterFilter::_updateLEDs()
{
    _ledControl.update(_usedPercentage);
}

void WaterFilter::_handleResetButton()
{
    static bool lastButtonState = HIGH;
    bool currentButtonState = digitalRead(_resetButtonPin);

    // Check for button press (assuming active low)
    if (lastButtonState == HIGH && currentButtonState == LOW)
    {
        // Debounce delay
        delay(50);
        currentButtonState = digitalRead(_resetButtonPin);
        if (currentButtonState == LOW)
        {
            // Reset the filter usage
            resetFilter();
        }
    }

    lastButtonState = currentButtonState;
}
