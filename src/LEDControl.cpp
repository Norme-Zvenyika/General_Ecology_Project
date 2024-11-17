// LEDControl.cpp

#include "LEDControl.h"

// Constructor
LEDControl::LEDControl(uint8_t redPin, uint8_t yellowPin, uint8_t greenPin)
    : _redPin(redPin), _yellowPin(yellowPin), _greenPin(greenPin)
{
}

// Initialize the LED pins
void LEDControl::begin()
{
    pinMode(_redPin, OUTPUT);
    pinMode(_yellowPin, OUTPUT);
    pinMode(_greenPin, OUTPUT);
    allOff();
}

// Turn off all LEDs
void LEDControl::allOff()
{
    digitalWrite(_redPin, LOW);
    digitalWrite(_yellowPin, LOW);
    digitalWrite(_greenPin, LOW);
}

// Update the LED status based on used percentage
void LEDControl::update(float usedPercentage)
{
    // If alarm is active, ignore water filter control
    if (_alarmActive) return;

    if (usedPercentage <= 50.0)
    {
        _filterStatus = "Excellent";
        _setGreen();
    }
    else if (usedPercentage > 50.0 && usedPercentage <= 80.0)
    {
        _filterStatus = "Average";
        _setYellow();
    }
    else
    {
        // Filter capacity exceeded
        _filterStatus = "Poor";
        _setRed();
    }
}

// Set alarm state and control LEDs accordingly
void LEDControl::setAlarmState(bool active) {
    _alarmActive = active;
    if (_alarmActive) {
        allOff();
        _setRed(); // Alarm always sets the LED to red
        _filterStatus = "Poor";
    } else {
        allOff(); // Release LED control
    }
}

// Get the current filter status
String LEDControl::getFilterStatus() const {
    return _filterStatus;
}

// Private helper methods
void LEDControl::_setRed()
{
    digitalWrite(_greenPin, LOW);
    digitalWrite(_yellowPin, LOW);
    digitalWrite(_redPin, HIGH);
}

void LEDControl::_setYellow()
{
    digitalWrite(_greenPin, LOW);
    digitalWrite(_yellowPin, HIGH);
    digitalWrite(_redPin, LOW);
}

void LEDControl::_setGreen()
{
    digitalWrite(_greenPin, HIGH);
    digitalWrite(_yellowPin, LOW);
    digitalWrite(_redPin, LOW);
}
