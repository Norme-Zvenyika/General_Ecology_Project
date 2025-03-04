#include "PID.h"

// Constructor to initialize all components
PID::PID(uint8_t redLedPin, uint8_t yellowLedPin, uint8_t greenLedPin, uint8_t flowSensorPin, uint8_t resetButtonPin,
         float filterCapacityLiters, float pulsesPerLiterConversion, float flowRateScalingFactor,
         const String& serviceUUID, const String& characteristicUUID_RX, const String& characteristicUUID_TX, const String& deviceName,
         uint8_t clockInterruptPin, int months, int days, int hours, int minutes)
    : _ledControl(redLedPin, yellowLedPin, greenLedPin),
      _waterFlowSensor(flowSensorPin, pulsesPerLiterConversion, flowRateScalingFactor),
      _resetButton(resetButtonPin),
      _waterFilter(_ledControl, _resetButton, _waterFlowSensor, filterCapacityLiters),
      _ble(deviceName, serviceUUID, characteristicUUID_RX, characteristicUUID_TX),
      _alarmManager(clockInterruptPin, _resetButton, months, days, hours, minutes, _ledControl),
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
    _waterFilter.begin();
    _waterFlowSensor.begin();  // Initialize the water flow sensor
    _ble.begin();              // Initialize the BLE module
    _alarmManager.begin();              // Initialize the BLE module
}

// Update method to be called in the main loop to run everything
void PID::update()
{
    _resetButton.update();

    // Update alram manager
    _alarmManager.update();

    // Update the water filter system (this handles flow rate, volume, and reset button)
    _waterFilter.update();

    // Display data on BLE LCD and prepare it for Bluetooth transmission
    float flowRate = _waterFlowSensor.getFlowRate();
    float totalVolume = _waterFilter.getTotalVolume();
    String filterStatus = _ledControl.getFilterStatus();

    // Fetch current date and last reset date
    // String currentDate = _alarmManager.getCurrentDateTime();
    String lastResetDate = _alarmManager.getLastResetDateTime();

    // // Print filter status
    // Serial.print("Filter Status: ");
    // Serial.println(filterStatus);

    // // Print the dates for debugging
    // Serial.print("Current Date: ");
    // Serial.println(currentDate);
    // Serial.print("Last Reset Date: ");
    // Serial.println(lastResetDate);


    _ble.displayData(flowRate, totalVolume, filterStatus, lastResetDate);
}
