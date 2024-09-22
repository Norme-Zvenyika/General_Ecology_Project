// WaterFlowSensor.h

#ifndef WATERFLOWSENSOR_H
#define WATERFLOWSENSOR_H

#include <Arduino.h>

class WaterFlowSensor
{
public:
    // Constructor
    WaterFlowSensor(uint8_t sensorPin, float pulsesPerLiter, float flowRateScalingFactor);

    // Initialize the sensor
    void begin();

    // Update method to be called regularly
    void update();

    // Getters
    float getFlowRate();        // Returns flow rate in L/min
    float getVolumePassed();    // Returns the volume of water filtered in liters during the measurement period

    // Calibration function
    void calibrate(float knownVolume);  // Recalibrate the pulsesPerLiter based on known volume

private:
    uint8_t _sensorPin;

    // Calibration constants
    float _pulsesPerLiter;         // Number of pulses required for 1 liter of water
    float _flowRateScalingFactor;  // Scaling factor for flow rate calculation
    float _volumePerPulse;         // Amount of water (liters) per pulse

    // Pulse counting variables
    volatile unsigned long _pulseCount;  // Number of pulses detected

    // Flow calculation variables
    float _flowRate;    // Flow rate in liters per minute
    float _volumePassed; // Volume filtered during the current interval in liters

    // Timing variables
    unsigned long _lastCalcTime;

    // Interrupt Service Routine (ISR)
    static void IRAM_ATTR pulseCounter();

    // Static pointer to the instance (for ISR)
    static WaterFlowSensor *_instance;
};

#endif // WATERFLOWSENSOR_H
