#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <math.h>

// Variables for Battery Control
float ParallelBatteryVoltage = 0.0;  // Calculated battery voltage                               --SEND TO SHOW ON THE SCREEN
float SeriesBatteryVoltage = 0.0;    // Calculated battery voltage                               --SEND TO SHOW ON THE SCREEN
float BatteryCurrent = 0.0;          // Measured battery current                                 --SEND TO SHOW ON THE SCREEN
float SetBatteryVoltage = 14.4;      // Target battery voltage                                   --RECEIVE FROM THE ESP
float SetBatteryCurrent = 3.0;       // Target battery current                                   --RECEIVE FROM THE ESP
float LowVoltageThreshold = 12.8;    // Voltage to trigger charging                              --RECEIVE FROM THE ESP (MAYBE)
float MaxVoltageDifference = 0.5;    // Maximum voltage difference to allow parallel charging    --RECEIVE FROM THE ESP (MAYBE)

float Bat1Voltage = 0.0;  // Battery 1 voltage                                          --SEND TO SHOW ON THE SCREEN (DEBUG)
float Bat2Voltage = 0.0;  // Battery 2 voltage                                          --SEND TO SHOW ON THE SCREEN (DEBUG)
float PSUVoltage = 0.0;   // PSU voltage                                                --SEND TO SHOW ON THE SCREEN (DEBUG)

int dataReceived = 0;          // 0:Data has not received 1:Data has been received charging approved         --UPDATE FROM THE ESP
int chargeState = 0;           // Charging state: 0 = stopped, 1 = charging                                  --SEND TO SHOW ON THE SCREEN
int PSU_Available = 0;         // Power supply status: 0 = unavailable, 1 = available                        --SEND TO SHOW ON THE SCREEN
int Battery_Available = 0;     // Battery availability status: 0 = unavailable, 1 = available                --SEND TO SHOW ON THE SCREEN
int batteryPercentage = 0;     // Battery charging/discharging percentage                                    --SEND TO SHOW ON THE SCREEN
float boardTemperature = 0.0;  // Board temperature (Charging circuit)                                       --SEND TO SHOW ON THE SCREEN

int externalFAN = 1;         // External fan controlled with external ntc: 1 = ntc readings controls the fan output, 0 = ignore
float ntcTemperature = 0.0;  // External ntc calculated temperature in Celsius

const int chargeEN = 4;       // Pin 4 for Charge Relay control (Connects MOSFET to Battery) - HIGH: Battery connected to MOSFET's output
const int EXT_FAN_PIN = 5;    // Pin 5 (OC0B) for PWM to FAN
const int FAN_PIN = 6;        // Pin 6 (OC0A) for PWM to FAN
const int batterySW = 7;      // Pin 7 for Battery Switch (BATTERY/UPS Switch) - HIGH: On PSU
const int relayModuleSW = 8;  // Pin 8 for switching between 12V/24V - HIGH: 12V line (Charging), LOW: 24V line
const int MosfetPin = 9;      // Pin 9 for MOSFET control (OC1A)
#define NTC_PIN A1            // Analog input pin for external temperature measurement
#define PSUVoltage_PIN A2     // Analog input pin for voltage measurement
#define TEMP_SENSOR_PIN A3    // Analog input pin for on-board temperature measurement

bool PWMEnable = true;

// startCharging() -- RECEIVE FROM ESP
// stopCharging()  -- RECEIVE FROM ESP

void setup() {
  delay(200);
  initializeInput();
  initializeOutput();
  SerialCommSetup();
}

void loop() {
  // Control input
  inputControl();

  // Check if charging should start
  checkStartConditions();

  // Update PID control
  updatePIDControl();

  // Control output
  outputControl();

  // Calculate battery Percentage
  calculatePercentage();

  //Communicate with ESP32
  SerialComm();
}
