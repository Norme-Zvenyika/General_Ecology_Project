// WaterFilter.cpp

#include "WaterFilter.h"

// Constructor
WaterFilter::WaterFilter(uint8_t redLedPin, uint8_t yellowLedPin, uint8_t greenLedPin, uint8_t resetButtonPin, const float filterCapacityLiters)
    : _ledControl(redLedPin, yellowLedPin, greenLedPin),
      _resetButtonPin(resetButtonPin),
      _filterCapacity(filterCapacityLiters), // Initialize filter capacity as constant
      _usedPercentage(0.0)
{
}

// Initialize the water filter system
void WaterFilter::begin()
{
    // Initialize components
    _ledControl.begin();

    // Initialize the Bluetooth module
    _bleModule.begin();

    // Initialize reset button pin
    pinMode(_resetButtonPin, INPUT_PULLUP);

    // Initialize other variables if necessary
    _usedPercentage = 0.0;
}

// Update method to be called regularly in the main loop
void WaterFilter::update(float flowRate, float totalVolume)
{
    // Calculate the used percentage
    _usedPercentage = (totalVolume / _filterCapacity) * 100.0;

    // Update the LEDs based on used percentage
    _updateLEDs();

    // Handle reset button press
    _handleResetButton();

    // Display the message via BLE
    _bleModule.displayData(flowRate, totalVolume);
}

// Reset the filter usage (e.g., when the filter is replaced)
void WaterFilter::resetFilter()
{
    // Re-initialize the system as in begin()
    _ledControl.allOff(); // Turn off all LEDs
    _usedPercentage = 0.0;
}

// Get used percentage based on total volume and filter capacity
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
