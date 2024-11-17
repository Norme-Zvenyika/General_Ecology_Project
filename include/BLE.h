// BLE.h

#ifndef BLE_H
#define BLE_H

#include "BLEDevice.h"
#include "BLEServer.h"
#include "BLEUtils.h"
#include "BLE2902.h"
#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

class BLE
{
public:
    // Constructor that accepts a device name
    BLE(const String& deviceName, const String& serviceUUID, const String& characteristicTXUUID, const String& characteristicRXUUID);

    // Initialize the BLE module and the LCD
    void begin();

    // Display the flow rate and total water filtered on the LCD and send it via Bluetooth
    void displayData(float flowRate, float totalWaterFiltered, const String& filterStatus, const String& lastResetDate);

    // Scan I2C devices for LCD
    bool scanI2CDevices();

private:

    // I2C LCD setup (default address 0x27, 16 columns, 2 rows)
    LiquidCrystal_I2C _lcd = LiquidCrystal_I2C(0x27, 16, 2);

    // BLE Characteristics and Server
    BLECharacteristic* pCharacteristic;
    BLEServer* pServer;

    // BLE UUIDs
    String _serviceUUID;
    String _characteristicTXUUID;
    String _characteristicRXUUID;

    // BLE Setup functions
    void setupBLE();
    void setupBLEServer();
    void setupBLEService();

    // Utility function to send notification
    void sendNotification(const char* message);

    // Device name for Bluetooth and LCD
    String _deviceName;
};

#endif // BLE_H
