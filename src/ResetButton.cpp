#include "ResetButton.h"

// Constructor: Initializes the button pin and debounce delay
ResetButton::ResetButton(uint8_t buttonPin, unsigned long debounceDelay)
    : _buttonPin(buttonPin), 
      _debounceDelay(debounceDelay), 
      _lastDebounceTime(0), 
      _lastButtonState(HIGH),  // Assuming active-low button (not pressed by default)
      _buttonState(HIGH)
{
}

// Initialize the button pin
void ResetButton::begin()
{
    pinMode(_buttonPin, INPUT_PULLUP);  // Set pin as input with pull-up resistor
}

// Check if the button is pressed with debounce handling
bool ResetButton::isPressed()
{
    bool reading = digitalRead(_buttonPin);  // Read the current state of the button

    // Check if the button state has changed
    if (reading != _lastButtonState)
    {
        _lastDebounceTime = millis();  // Reset the debounce timer
    }

    // If enough time has passed since the last state change, update the button state
    if ((millis() - _lastDebounceTime) > _debounceDelay)
    {
        // Only change the button state if it has changed for real
        if (reading != _buttonState)
        {
            _buttonState = reading;

            // Return true if the button is pressed (assuming active low)
            if (_buttonState == LOW)
            {
                return true;
            }
        }
    }

    _lastButtonState = reading;  // Save the current state
    return false;
}
