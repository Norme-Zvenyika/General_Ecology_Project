// WaterFlowSensor.cpp

#include "WaterFlowSensor.h"

const float PULSES_PER_LITER_FACTOR = 21.0;

// Initialize static member
WaterFlowSensor *WaterFlowSensor::_instance = nullptr;

// Constructor
WaterFlowSensor::WaterFlowSensor(uint8_t sensorPin, float pulsesPerLiter)
    : _sensorPin(sensorPin),
      _pulsesPerLiter(pulsesPerLiter),
      _pulseCount(0),
      _lastPulseCount(0),
      _flowRate(0.0),
      _totalVolume(0.0),
      _lastCalcTime(0)
{
    _volumePerPulse = 1.0 / _pulsesPerLiter;
}

// Initialize the sensor
void WaterFlowSensor::begin()
{
    pinMode(_sensorPin, INPUT_PULLUP);
    _instance = this; // Set the static instance pointer to this instance
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

// Update method to be called regularly
void WaterFlowSensor::update()
{
    unsigned long currentMillis = millis();

    if (currentMillis - _lastCalcTime >= _calcInterval)
    {
        // Disable interrupts while accessing shared variables
        noInterrupts();
        unsigned long pulses = _pulseCount;
        _pulseCount = 0;
        interrupts();

        // Calculate flow rate in L/min
        float frequency = pulses / (_calcInterval / 1000.0); // Pulses per second (Hz)
        _flowRate = frequency / PULSES_PER_LITER_FACTOR;     // Q = F / PULSES_PER_LITER_FACTOR

        // Update total volume
        _totalVolume += pulses * _volumePerPulse;

        // Update last calculation time
        _lastCalcTime = currentMillis;
    }
}

// Get flow rate in L/min
float WaterFlowSensor::getFlowRate()
{
    return _flowRate;
}

// Get total volume in liters
float WaterFlowSensor::getTotalVolume()
{
    return _totalVolume;
}

// Get used percentage based on filter capacity
float WaterFlowSensor::getUsedPercentage(float filterCapacity)
{
    if (filterCapacity > 0)
    {
        return (_totalVolume / filterCapacity) * 100.0;
    }
    else
    {
        return 0.0;
    }
}

// Reset total volume
void WaterFlowSensor::resetTotalVolume()
{
    _totalVolume = 0.0;
}

// Calibration function
void WaterFlowSensor::calibrate(float knownVolume)
{
    // Disable interrupts while accessing shared variables
    noInterrupts();
    unsigned long pulses = _pulseCount;
    _pulseCount = 0;
    interrupts();

    if (knownVolume > 0.0)
    {
        // Calculate new pulses per liter
        float newPulsesPerLiter = pulses / knownVolume;

        // Update calibration constants
        _pulsesPerLiter = newPulsesPerLiter;
        _volumePerPulse = 1.0 / _pulsesPerLiter;

        Serial.print("Calibration complete. New pulses per liter: ");
        Serial.println(_pulsesPerLiter);
    }
    else
    {
        Serial.println("Calibration failed. Known volume must be greater than zero.");
    }
}

// Reset the sensor
void WaterFlowSensor::reset()
{
    // Disable interrupts while accessing shared variables
    noInterrupts();
    _pulseCount = 0;
    interrupts();

    _flowRate = 0.0;
    _totalVolume = 0.0;
    _lastCalcTime = millis();
}