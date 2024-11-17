#ifndef RESETBUTTON_H
#define RESETBUTTON_H

#include <Arduino.h>
#include <vector>
#include <functional>

class ResetButton {
public:
    // Constructor: Initialize button pin and debounce delay
    ResetButton(uint8_t buttonPin, unsigned long debounceDelay = 2);

    void begin();                     // Initialize the button pin
    void update();                    // Update button state (call in loop)
    void registerCallback(std::function<void()> callback); // Add observer

private:
    uint8_t _buttonPin;
    unsigned long _debounceDelay;
    unsigned long _lastDebounceTime;
    bool _lastButtonState;
    bool _buttonState;

    std::vector<std::function<void()>> _callbacks; // List of observers
};

#endif // RESETBUTTON_H
