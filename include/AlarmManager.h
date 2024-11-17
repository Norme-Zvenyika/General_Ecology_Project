#ifndef ALARM_MANAGER_H
#define ALARM_MANAGER_H

#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>
#include <SPI.h>
#include <string>
#include "ResetButton.h"

class AlarmManager {
private:
    RTC_DS3231 rtc;
    ResetButton& resetButton;
    uint8_t interruptPin; // Pin for the SQW interrupt
    static volatile bool alarmTriggered; // Static variable to track alarm status
    DateTime lastResetTime; //last day of reset

    // Alarm timing configuration
    int alarmMonths;
    int alarmDays;
    int alarmHours;
    int alarmMinutes;

    // Private methods
    TimeSpan calculateAlarmOffset();
    String getDateTime(const DateTime &dt) const;
    void handleAlarm();

    String getLastResetDateTime();    // Get last reset time
    String getCurrentDateTime();     // Get current time

    // Static interrupt handler
    static void IRAM_ATTR onAlarm();

public:
    // Constructor
    AlarmManager(uint8_t interruptPin, ResetButton& button, int months, int days, int hours, int minutes);

    // Initialization and periodic updates
    void begin();
    void update();
    void resetAlarm();
};

#endif // ALARM_MANAGER_H
