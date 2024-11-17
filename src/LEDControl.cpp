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
    if (usedPercentage <= 50.0)
    {
        _setGreen();
    }
    else if (usedPercentage > 50.0 && usedPercentage <= 80.0)
    {
        _setYellow();
    }
    else if (usedPercentage > 80.0 && usedPercentage <= 100.0)
    {
        _setRed();
    }
    else
    {
        // Filter capacity exceeded
        _setRed();
    }
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
