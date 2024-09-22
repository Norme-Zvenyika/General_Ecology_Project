// BLE.h

#ifndef BLE_H
#define BLE_H

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

class BLE
{
public:
    // Constructor that accepts a device name
    BLE(const String& deviceName);

    // Initialize the BLE module (in this case, initialize the LCD)
    void begin();

    // Display the flow rate and total water filtered on the LCD
    void displayData(float flowRate, float totalWaterFiltered);

    // Prepare data for future Bluetooth transmission
    String prepareDataForBluetooth(float flowRate, float totalWaterFiltered);

    // **New function to scan I2C devices**
    bool scanI2CDevices();

private:
    // I2C LCD setup (default address 0x27, 16 columns, 2 rows)
    LiquidCrystal_I2C _lcd = LiquidCrystal_I2C(0x27, 16, 2);

    // The address of the I2C device (can be set during scanning)
    uint8_t _lcdAddress = 0x27; // Default I2C address

    // Device name to display on the LCD
    String _deviceName;
};

#endif // BLE_H
