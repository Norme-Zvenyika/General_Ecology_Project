// WaterFlowSensor.cpp

#include "WaterFlowSensor.h"

// Define constants
const float VOLUME_FOR_ONE_LITER = 1.0;                // Volume equivalent for 1 liter
const unsigned long CALC_INTERVAL_MS = 1000;           // Calculation interval in milliseconds (1 second)
const float MILLISECONDS_PER_SECOND = 1000.0;          // Milliseconds per second

// Initialize static member
WaterFlowSensor *WaterFlowSensor::_instance = nullptr;

// Constructor
WaterFlowSensor::WaterFlowSensor(uint8_t sensorPin, float pulsesPerLiter, float flowRateScalingFactor)
    : _sensorPin(sensorPin),
      _pulsesPerLiter(pulsesPerLiter),
      _flowRateScalingFactor(flowRateScalingFactor),
      _pulseCount(0),
      _flowRate(0.0),
      _volumePassed(0.0),
      _lastCalcTime(0)
{
    // Calculate volume per pulse based on the pulses per liter
    _volumePerPulse = VOLUME_FOR_ONE_LITER / _pulsesPerLiter;
}

// Initialize the sensor
void WaterFlowSensor::begin()
{
    pinMode(_sensorPin, INPUT_PULLUP);
    _instance = this;
    attachInterrupt(digitalPinToInterrupt(_sensorPin), WaterFlowSensor::pulseCounter, RISING);
    _lastCalcTime = millis();
}

// Interrupt Service Routine (ISR)
void IRAM_ATTR WaterFlowSensor::pulseCounter()
{
    if (_instance != nullptr)
    {
        _instance->_pulseCount++;
    }
}

// Update method to calculate flow rate and filtered volume over the measurement period
void WaterFlowSensor::update()
{
    unsigned long currentMillis = millis();
    if (currentMillis - _lastCalcTime >= CALC_INTERVAL_MS)
    {
        // Disable interrupts while accessing shared variables
        noInterrupts();
        unsigned long pulses = _pulseCount;
        _pulseCount = 0;
        interrupts();

        // Calculate frequency in Hz (pulses per second)
        float frequency = pulses / (CALC_INTERVAL_MS / MILLISECONDS_PER_SECOND);  // Pulses per second (Hz)

        // Calculate flow rate in L/min using the scaling factor
        _flowRate = (frequency / _flowRateScalingFactor); // Convert to liters per minute

        // Calculate the volume filtered during this interval
        _volumePassed = pulses * _volumePerPulse;

        // Update the last calculation time
        _lastCalcTime = currentMillis;
    }
}

// Get flow rate in L/min
float WaterFlowSensor::getFlowRate()
{
    return _flowRate;
}

// Get the volume of water filtered in liters during the measurement period
float WaterFlowSensor::getVolumePassed()
{
    return _volumePassed;
}

// Calibration function to adjust pulses per liter based on a known volume
void WaterFlowSensor::calibrate(float knownVolume)
{
    // Disable interrupts while accessing shared variables
    noInterrupts();
    unsigned long pulses = _pulseCount;
    _pulseCount = 0;
    interrupts();

    if (knownVolume > 0.0)
    {
        // Calculate new pulses per liter based on known volume
        float newPulsesPerLiter = pulses / knownVolume;

        // Update calibration constants
        _pulsesPerLiter = newPulsesPerLiter;
        _volumePerPulse = VOLUME_FOR_ONE_LITER / _pulsesPerLiter;  // Correct recalculation of volume per pulse

        Serial.print("Calibration complete. New pulses per liter: ");
        Serial.println(_pulsesPerLiter);
    }
    else
    {
        Serial.println("Calibration failed. Known volume must be greater than zero.");
    }
}
