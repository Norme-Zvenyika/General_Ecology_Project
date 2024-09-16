// WaterFilter.cpp

#include "WaterFilter.h"

// Constructor
WaterFilter::WaterFilter(uint8_t redLedPin, uint8_t yellowLedPin, uint8_t greenLedPin, uint8_t flowSensorPin, uint8_t resetButtonPin, const float filterCapacityLiters, const float pulsesPerLiter)
    : _ledControl(redLedPin, yellowLedPin, greenLedPin),
      _waterFlowSensor(flowSensorPin, pulsesPerLiter),
      _resetButtonPin(resetButtonPin),
      _filterCapacity(filterCapacityLiters), // Initialize filter capacity as constant
      _pulsesPerLiter(pulsesPerLiter),       // Initialize pulses per liter as constant
      _usedPercentage(0.0)
{
}

// Initialize the water filter system
void WaterFilter::begin()
{
    // Initialize components
    _ledControl.begin();
    _waterFlowSensor.begin();

    //  initialize the bluetooth module
    _bleModule.begin();
    // Initialize reset button pin
    pinMode(_resetButtonPin, INPUT_PULLUP);

    // Initialize other variables if necessary
    _usedPercentage = 0.0;
}

// Update method to be called regularly in the main loop
void WaterFilter::update()
{
    // Update the water flow sensor
    _waterFlowSensor.update();

    // Calculate the used percentage
    _usedPercentage = getUsedPercentage();

    // Update the LEDs based on used percentage
    _updateLEDs();

    // Handle reset button press
    _handleResetButton();

    // display the message
    _bleModule.displayData(getFlowRate(), getTotalVolume());
}

// Reset the filter usage (e.g., when the filter is replaced)
void WaterFilter::resetFilter()
{
    // Re-initialize the system as in begin()
    _waterFlowSensor.reset();
    _ledControl.allOff(); // Turn off all LEDs
    _usedPercentage = 0.0;
}

// Calibration method
void WaterFilter::calibrate(float knownVolume)
{
    _waterFlowSensor.calibrate(knownVolume);
}

// Getters

float WaterFilter::getFlowRate()
{
    return _waterFlowSensor.getFlowRate();
}

float WaterFilter::getTotalVolume()
{
    return _waterFlowSensor.getTotalVolume();
}

float WaterFilter::getUsedPercentage()
{
    float totalVolume = _waterFlowSensor.getTotalVolume();
    if (_filterCapacity > 0)
    {
        return (totalVolume / _filterCapacity) * 100.0;
    }
    else
    {
        return 0.0;
    }
}

float WaterFilter::getFilterCapacity() const
{
    return _filterCapacity;
}

// **Getter for pulses per liter**
float WaterFilter::getPulsesPerLiter() const
{
    return _pulsesPerLiter;
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
