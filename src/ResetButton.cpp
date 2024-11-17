#include "ResetButton.h"

ResetButton::ResetButton(uint8_t buttonPin, unsigned long debounceDelay)
    : _buttonPin(buttonPin), _debounceDelay(debounceDelay), _lastDebounceTime(0),
      _lastButtonState(HIGH), _buttonState(HIGH) {}

void ResetButton::begin() {
    pinMode(_buttonPin, INPUT_PULLUP);
}

void ResetButton::update() {
    bool reading = digitalRead(_buttonPin);

    // Check if the button state has changed
    if (reading != _lastButtonState) {
        _lastDebounceTime = millis(); // Reset debounce timer
    }

    // If enough time has passed, update the button state
    if ((millis() - _lastDebounceTime) > _debounceDelay) {
        if (reading != _buttonState) {
            _buttonState = reading;

            // If the button is pressed (active-low), notify observers
            if (_buttonState == LOW) {
                for (auto &callback : _callbacks) {
                    callback(); // Notify each registered callback
                }
            }
        }
    }

    _lastButtonState = reading; // Save the current state
}

void ResetButton::registerCallback(std::function<void()> callback) {
    _callbacks.push_back(callback); // Add callback to the list
}
