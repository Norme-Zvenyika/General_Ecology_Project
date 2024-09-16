// WaterFlowSensor.h

#ifndef WATERFLOWSENSOR_H
#define WATERFLOWSENSOR_H

#include <Arduino.h>

class WaterFlowSensor
{
public:
    // Constructor
    WaterFlowSensor(uint8_t sensorPin, float pulsesPerLiter = 1260.0);

    // Initialize the sensor
    void begin();

    // Update method to be called regularly
    void update();

    // Getters
    float getFlowRate();                           // Returns flow rate in L/min
    float getTotalVolume();                        // Returns total volume in liters
    float getUsedPercentage(float filterCapacity); // Returns used percentage based on filter capacity

    // Reset total volume (e.g., when filter is replaced)
    void resetTotalVolume();

    // Reset the sensor
    void reset();

    // Calibration function (optional)
    void calibrate(float knownVolume);

private:
    uint8_t _sensorPin;

    // Calibration constants
    float _pulsesPerLiter;
    float _volumePerPulse;

    // Pulse counting variables
    volatile unsigned long _pulseCount;
    unsigned long _lastPulseCount;

    // Flow calculation variables
    float _flowRate;    // In liters per minute
    float _totalVolume; // In liters

    // Timing variables
    unsigned long _lastCalcTime;
    const unsigned long _calcInterval = 1000; // 1 second

    // Interrupt Service Routine (ISR)
    static void IRAM_ATTR pulseCounter();

    // Static pointer to the instance (for ISR)
    static WaterFlowSensor *_instance;
};

#endif // WATERFLOWSENSOR_H
