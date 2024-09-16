// BLE.cpp

#include "BLE.h"

// Constructor
BLE::BLE() {}

// Initialize the BLE module (initialize the LCD for now)
void BLE::begin()
{
    // Scan for I2C devices before initializing the LCD
    if (scanI2CDevices())
    {
        _lcd.init();             // Initialize the LCD
        _lcd.backlight();        // Turn on the LCD backlight
        _lcd.print("LCD Found"); // Display a test message
    }
    else
    {
        Serial.println("No I2C LCD found.");
    }
}

// Display the flow rate and total water filtered on the LCD
void BLE::displayData(float flowRate, float totalWaterFiltered)
{
    _lcd.clear(); // Clear the LCD
    _lcd.setCursor(0, 0);
    _lcd.print("Flow: ");
    _lcd.print(flowRate, 2); // Display flow rate with 2 decimal places
    _lcd.print(" L/min");

    _lcd.setCursor(0, 1);
    _lcd.print("Total: ");
    _lcd.print(totalWaterFiltered, 2); // Display total water filtered with 2 decimal places
    _lcd.print(" L");
}

// Prepare data for future Bluetooth transmission
String BLE::prepareDataForBluetooth(float flowRate, float totalWaterFiltered)
{
    String data = "Flow Rate: " + String(flowRate, 2) + " L/min\n";
    data += "Total Water: " + String(totalWaterFiltered, 2) + " L";
    return data;
}

// **New function to scan I2C devices and assign the correct address**
bool BLE::scanI2CDevices()
{
    byte error, address;
    int nDevices = 0;

    // Loop through all possible I2C addresses (1 to 126)
    for (address = 1; address < 127; address++)
    {
        Wire.beginTransmission(address); // Start I2C transmission
        error = Wire.endTransmission();  // End the transmission and capture error code

        if (error == 0)
        {
            if (address < 16)
            {
                Serial.print("0");
            }
            Serial.println(address, HEX);
            _lcdAddress = address; // Set the detected address
            nDevices++;
        }
        else if (error == 4)
        {
            Serial.print("Unknown error at address 0x");
            if (address < 16)
            {
                Serial.print("0");
            }
            Serial.println(address, HEX);
        }
    }

    if (nDevices == 0)
    {
        Serial.println("No I2C devices found.");
        return false;
    }
    else
    {
        return true;
    }
}