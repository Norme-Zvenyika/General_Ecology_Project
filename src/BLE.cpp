// BLE.cpp

#include "BLE.h"

// Constructor
BLE::BLE(const String& deviceName, const String& serviceUUID, const String& characteristicTXUUID, const String& characteristicRXUUID)
    : _deviceName(deviceName), _serviceUUID(serviceUUID), _characteristicTXUUID(characteristicTXUUID), _characteristicRXUUID(characteristicRXUUID)
{
    // Empty constructor body
}

// Initialize the BLE module and LCD
void BLE::begin()
{
    // Start I2C communication and initialize LCD
    Wire.begin();

    // Scan I2C devices to check if the LCD is connected
    if (!scanI2CDevices()) {
        // If LCD is not found, maybe handle error on the LCD
        _lcd.begin(16, 2);  // Initialize the LCD even if no address is detected
        _lcd.backlight();
        _lcd.clear();
        _lcd.setCursor(0, 0);
        _lcd.print("LCD Error");
    } else {
        _lcd.begin(16, 2);  // Initialize the LCD
        _lcd.backlight();    // Turn on the backlight
        _lcd.clear();        // Clear the display

        // Display welcome message
        _lcd.setCursor(0, 0);      
        _lcd.print("Welcome!");
        _lcd.setCursor(0, 1);     
        _lcd.print(_deviceName);   

        delay(4000);  // Display for 4 seconds
    }

    // Initialize the BLE module
    setupBLE();
}

// Initialize BLE
void BLE::setupBLE()
{
    BLEDevice::init(_deviceName.c_str());  // Initialize BLE with device name
    setupBLEServer();                      // Set up BLE server
    setupBLEService();                     // Set up BLE service and characteristics
}

// === BLE Server Callbacks ===
class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) override {
    // Optional: Handle what happens when a client connects
  }

  void onDisconnect(BLEServer* pServer) override {
    // Optional: Handle what happens when a client disconnects
    
    // Restart advertising after disconnect
    BLEDevice::startAdvertising();
  }
};

// === BLE Characteristic Callbacks ===
class MyCharacteristicCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* pCharacteristic) override {
    // Optional: Handle received data from the client if needed
  }
};

// Set up BLE Server
void BLE::setupBLEServer()
{
    pServer = BLEDevice::createServer();  // Create BLE server
    pServer->setCallbacks(new MyServerCallbacks());  // Set server callbacks
}

// Set up BLE Service and Characteristics
void BLE::setupBLEService()
{
    BLEService* pService = pServer->createService(_serviceUUID.c_str());  // Create BLE service

    // Create characteristic for sending notifications
    pCharacteristic = pService->createCharacteristic(
       _characteristicTXUUID.c_str(), BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_READ
    );
    pCharacteristic->addDescriptor(new BLE2902());

    // Create characteristic for receiving data
    BLECharacteristic* pCharacteristicRX = pService->createCharacteristic(
        _characteristicRXUUID.c_str(), BLECharacteristic::PROPERTY_WRITE
    );
    pCharacteristicRX->setCallbacks(new MyCharacteristicCallbacks());

    pService->start();                   // Start BLE service
    BLEDevice::getAdvertising()->start();  // Start advertising
}

// Send the notification message over BLE
void BLE::sendNotification(const char* message)
{
    pCharacteristic->setValue(message);  // Set characteristic value
    pCharacteristic->notify();           // Send the notification
}

// Display the flow rate and total water filtered on the LCD and send it via Bluetooth
void BLE::displayData(float flowRate, float totalWaterFiltered)
{
    _lcd.clear();
    _lcd.setCursor(0, 0);
    _lcd.print("Flow: ");
    _lcd.print(flowRate, 2);  // Display flow rate with 2 decimal places
    _lcd.print(" L/min");

    _lcd.setCursor(0, 1);
    _lcd.print("Total: ");
    _lcd.print(totalWaterFiltered, 2);  // Display total water filtered with 2 decimal places
    _lcd.print(" L");

    // Prepare and send the data via Bluetooth
    String data = "Volume: " + String(totalWaterFiltered, 2) + " L";
    sendNotification(data.c_str());
}

// **New function to scan I2C devices and assign the correct address**
bool BLE::scanI2CDevices()
{
    byte error, address;
    int nDevices = 0;

    for (address = 1; address < 127; address++) {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();

        if (error == 0) {
            nDevices++;
        }
    }

    return nDevices > 0;
}