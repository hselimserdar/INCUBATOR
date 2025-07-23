//globals.h
#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <RTClib.h>
#include <Wire.h>
#include "Adafruit_SHT31.h"
#include <MapFloat.h>
#include <EEPROM.h>
#include <Nextion.h>

// ------------------------------
// MAIN_PROCESSOR.ino Globals
// ------------------------------
extern String pcbsurum;
extern String yazilimsurum;

extern float temp;
extern byte humidity;
extern byte waterlevel;

extern byte timeformat;  // 0: 24-hour, 1: 12-hour
extern byte dateformat;  // 0: dd/mm/yy, 1: mm/dd/yy
extern byte daycount;
extern byte lang;  // 0: English, 1: Turkish

extern bool heater;
extern bool humidifier;
extern bool humidityExtFan;
extern bool eggRotator;
extern bool cabinFan;

extern int processorReset;

// ------------------------------
// EEPROM.ino Globals
// ------------------------------
extern float targetVoltage;
extern float targetCurrent;
extern unsigned long resetTimestamp;

// ------------------------------
// RTC.ino Globals
// ------------------------------
extern RTC_DS3231 rtc;

extern byte sc_hour;
extern byte sc_min;
extern int sc_year;
extern byte sc_month;
extern byte sc_day;

extern unsigned long currentTime;
extern DateTime now;
extern DateTime lastSaved;

extern bool RTCBatLow;

// ------------------------------
// Page_Controller.ino Globals
// ------------------------------
extern int page;

// ------------------------------
// Serial_Communication.ino Globals
// ------------------------------
extern int auxChipOnline;

extern float gParallelBatteryVoltage;
extern float gSeriesBatteryVoltage;
extern float gBatteryCurrent;
extern float gBat1Voltage;
extern float gBat2Voltage;
extern float gPSUVoltage;
extern int gChargeState;
extern int gPSU_Available;
extern int gBattery_Available;
extern int gBatteryPercentage;
extern float gBoardTemperature;

// ------------------------------
// Nextion_Essential.ino Globals
// ------------------------------

//MAIN PAGE
extern NexButton mainb0;

//SETTINGS PAGE
extern NexButton setb0;
extern NexButton setb1;
extern NexButton setb2;
extern NexButton setb3;
extern NexButton setb4;
extern NexButton setb5;
extern NexButton setb6;
extern NexButton setb7;
extern NexButton setb8;
extern NexButton setb9;
extern NexButton setb10;
extern NexButton setb11;
extern NexButton setb12;
extern NexButton setb13;
extern NexButton setb14;
extern NexButton setb15;
extern NexButton setb16;
extern NexButton setb17;
extern NexButton setb18;
extern NexButton setb19;

// ------------------------------
// Settings_Page.ino Globals
// ------------------------------

extern float tempTempUpperLimit;
extern float tempTempLowerLimit;
extern int tempHumidityUpperLimit;
extern int tempHumidityLowerLimit;
extern int tempEggRotateDuration;
extern int tempEggRotateInterval;
extern int tempEggRotateStoppingDay;

#endif  // GLOBALS_H
