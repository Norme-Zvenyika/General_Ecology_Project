// ResetButton.h

#ifndef RESETBUTTON_H
#define RESETBUTTON_H

#include <Arduino.h>

class ResetButton
{
public:
    // Constructor: Pass the pin where the button is connected and optional debounce delay
    ResetButton(uint8_t buttonPin, unsigned long debounceDelay = 2);

    // Initialize the button pin
    void begin();

    // Check if the button is pressed (with debouncing)
    bool isPressed();

private:
    uint8_t _buttonPin;               // Pin where the button is connected
    unsigned long _debounceDelay;     // Debounce delay time
    unsigned long _lastDebounceTime;  // Last time the button state was toggled
    bool _lastButtonState;            // Previous state of the button
    bool _buttonState;                // Current state of the button
};

#endif // RESETBUTTON_H
