// LEDControl.h

#ifndef LEDCONTROL_H
#define LEDCONTROL_H

#include <Arduino.h>

class LEDControl
{
public:
    // Constructor
    LEDControl(uint8_t redPin, uint8_t yellowPin, uint8_t greenPin);

    // Initialize the LED pins
    void begin();

    // Update the LED status based on used percentage
    void update(float usedPercentage);

    // Turn off all LEDs
    void allOff();

private:
    uint8_t _redPin;
    uint8_t _yellowPin;
    uint8_t _greenPin;

    // Helper methods
    void _setRed();
    void _setYellow();
    void _setGreen();
};

#endif // LEDCONTROL_H
