#include "AlarmManager.h"
#include <string>

// Initialize static member variable
volatile bool AlarmManager::alarmTriggered = false;

// Constructor
AlarmManager::AlarmManager(uint8_t interruptPin, ResetButton& button, int months, int days, int hours, int minutes, LEDControl& ledControl)
    : resetButton(button),
      ledControl(ledControl),
      alarmMonths(months),
      alarmDays(days),
      alarmHours(hours),
      alarmMinutes(minutes),
      interruptPin(interruptPin) {
}

// Static interrupt handler
void IRAM_ATTR AlarmManager::onAlarm() {
    alarmTriggered = true; // Set the alarm flag
}

// Initialize RTC and attach interrupt
void AlarmManager::begin() {
    // Initialize RTC
    if (!rtc.begin()) {
        Serial.println("RTC initialization failed. Check connections!");
        while (1) delay(10);
    }

    // Check if RTC lost power and set time if needed
    if (rtc.lostPower()) {
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        // Serial.println("RTC lost power. Time set to compile time.");
    }

    // Configure RTC
    rtc.disable32K();
    rtc.clearAlarm(1);
    rtc.clearAlarm(2);
    rtc.writeSqwPinMode(DS3231_OFF); // Set SQW pin to alarm mode
    rtc.disableAlarm(2);

    // Attach interrupt to the SQW pin
    pinMode(interruptPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(interruptPin), AlarmManager::onAlarm, FALLING);

    // Initialize reset button
    resetButton.begin();

    resetButton.registerCallback([this]() { resetAlarm(); });

    // Set initial alarm
    resetAlarm();
}

// Calculate alarm offset
TimeSpan AlarmManager::calculateAlarmOffset() {
    DateTime now = rtc.now();  // Get current time

    // Start with current date/time
    DateTime futureTime = now;

    // Add months (this will handle different month lengths)
    if (alarmMonths > 0) {
        futureTime = DateTime(
            futureTime.year(),
            futureTime.month() + alarmMonths,
            futureTime.day(),
            futureTime.hour(),
            futureTime.minute(),
            futureTime.second()
        );
    }

    // Add days
    if (alarmDays > 0) {
        futureTime = futureTime + TimeSpan(alarmDays, 0, 0, 0);
    }

    // Add hours
    if (alarmHours > 0) {
        futureTime = futureTime + TimeSpan(0, alarmHours, 0, 0);
    }

    // Add minutes
    if (alarmMinutes > 0) {
        futureTime = futureTime + TimeSpan(0, 0, alarmMinutes, 0);
    }

    // Calculate difference between future and current time
    TimeSpan difference = futureTime - now;
    return difference;
}

// Print date and time for debugging
String AlarmManager::getDateTime(const DateTime &dt) const {
    char buffer[20]; // Buffer to hold the formatted string
    snprintf(buffer, sizeof(buffer), "%04d/%02d/%02d %02d:%02d:%02d",
             dt.year(), dt.month(), dt.day(),
             dt.hour(), dt.minute(), dt.second());
    return String(buffer);
}

// Reset and set the alarm
void AlarmManager::resetAlarm() {
    alarmTriggered = false;
    rtc.clearAlarm(1);

    DateTime now = rtc.now();
    DateTime nextAlarm = now + calculateAlarmOffset();

    if (!rtc.setAlarm1(nextAlarm, DS3231_A1_Minute)) {
        // Serial.println("Error: Failed to set Alarm1!");
        return;
    }

    lastResetTime = now;
    ledControl.setAlarmState(false);

    // Serial.println("Alarm set successfully.");
    // Serial.print("Alarm scheduled for: ");
    // Serial.println(getDateTime(nextAlarm));
}

// Handle alarm when triggered
void AlarmManager::handleAlarm() {
    ledControl.setAlarmState(true);
}

String AlarmManager::getLastResetDateTime() {
   return getDateTime(lastResetTime);
}

String AlarmManager::getCurrentDateTime() {
    DateTime now = rtc.now(); // Get current time from RTC
    return getDateTime(now);
}

// Periodic updates for reset button
void AlarmManager::update() {
    DateTime now = rtc.now();

    //check if alarm is triggered
    if (alarmTriggered) {
        handleAlarm();
    }

    // Serial.print("Current time: ");
    // Serial.println(getDateTime(now));
}
