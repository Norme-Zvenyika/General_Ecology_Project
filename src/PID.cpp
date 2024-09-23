#include "PID.h"

// Constructor to initialize all components
PID::PID(uint8_t redLedPin, uint8_t yellowLedPin, uint8_t greenLedPin, uint8_t flowSensorPin, uint8_t resetButtonPin, 
         float filterCapacityLiters, float pulsesPerLiterConversion, float flowRateScalingFactor, 
         const String& serviceUUID, const String& characteristicUUID_RX, const String& characteristicUUID_TX, const String& deviceName)
    : _ledControl(redLedPin, yellowLedPin, greenLedPin),
      _waterFlowSensor(flowSensorPin, pulsesPerLiterConversion, flowRateScalingFactor),
      _resetButton(resetButtonPin),
      _waterFilter(_ledControl, _resetButton, _waterFlowSensor, filterCapacityLiters),
      _ble(deviceName, serviceUUID, characteristicUUID_RX, characteristicUUID_TX),
      _filterCapacityLiters(filterCapacityLiters),
      _pulsesPerLiterConversion(pulsesPerLiterConversion),
      _flowRateScalingFactor(flowRateScalingFactor)
{
    // Initialization logic if needed
}

// Initialize all components
void PID::begin()
{
    _ledControl.begin();       // Initialize the LED system
    _resetButton.begin();      // Initialize the reset button
    _waterFlowSensor.begin();  // Initialize the water flow sensor
    _ble.begin();              // Initialize the BLE module
}

// Update method to be called in the main loop to run everything
void PID::update()
{
    // Update the water filter system (this handles flow rate, volume, and reset button)
    _waterFilter.update();

    // Display data on BLE LCD and prepare it for Bluetooth transmission
    float flowRate = _waterFlowSensor.getFlowRate();
    float totalVolume = _waterFilter.getTotalVolume();
    
    _ble.displayData(flowRate, totalVolume);  // Display on the LCD
    // String dataForBluetooth = _ble.prepareDataForBluetooth(flowRate, totalVolume);  // Prepare data for Bluetooth transmission
}

void PID::calibrateFlowSensor(float knownVolume)
{
    _waterFlowSensor.calibrate(knownVolume);
}
