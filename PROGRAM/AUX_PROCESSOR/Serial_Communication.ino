#include <stdlib.h>
#include <string.h>

#define RX_BUFFER_SIZE 256  // Fixed-size receive buffer

// Fixed-size buffer for incoming serial data.
char rxBuffer[RX_BUFFER_SIZE];
uint8_t rxIndex = 0;

unsigned long lastSendTime = 0;
unsigned long lastDataRecv = 0;
int MainProcessorTimeout = 30000;

//
// Helper: Compute an additive checksum (modulo 256) over a C string.
// If delimiter is provided (non-zero), stops summing at that character.
//
uint8_t computeChecksum(const char *data, char delimiter = '\0') {
  uint8_t sum = 0;
  for (int i = 0; data[i] != '\0'; i++) {
    if (delimiter != '\0' && data[i] == delimiter) break;
    sum += data[i];
  }
  return sum;
}

//
// Helper: Convert a two-character hex string to an 8-bit unsigned integer.
//
uint8_t hexToUint8(const char *hexStr) {
  return (uint8_t)strtol(hexStr, NULL, 16);
}

//
// Function: parseCommand
// Description: Validates the checksum of an incoming command and processes it.
// Expected formats:
//   "SET,<voltage>,<current>*XX"  or  "CMD,START*XX"  or  "CMD,STOP*XX"
//
void parseCommand(char *message) {
  // Find the '*' delimiter.
  char *asteriskPos = strchr(message, '*');
  if (!asteriskPos) {
    // No checksum delimiter found; ignore.
    return;
  }

  // Terminate the payload.
  *asteriskPos = '\0';

  // Compute checksum over the payload.
  uint8_t computedChecksum = computeChecksum(message);

  // Get the provided checksum (two hex digits).
  char providedChecksumStr[3] = { 0 };
  strncpy(providedChecksumStr, asteriskPos + 1, 2);
  uint8_t providedChecksum = hexToUint8(providedChecksumStr);

  // Verify checksum.
  if (computedChecksum != providedChecksum) {
    // Checksum mismatch; ignore command.
    return;
  }

  // Process the command.
  if (strncmp(message, "SET,", 4) == 0) {
    // Format: "SET,<voltage>,<current>"
    char *p = message + 4;
    char *commaPos = strchr(p, ',');
    if (!commaPos) return;
    *commaPos = '\0';
    float voltage = atof(p);
    float current = atof(commaPos + 1);
    SetBatteryVoltage = voltage;
    SetBatteryCurrent = current;
    dataReceived = 1;
    lastDataRecv = millis();
  } else if (strncmp(message, "CMD,", 4) == 0) {
    // Format: "CMD,START" or "CMD,STOP"
    char *cmd = message + 4;
    if (strcmp(cmd, "START") == 0) {
      // Start charging.
      startCharging();
    } else if (strcmp(cmd, "STOP") == 0) {
      // Stop charging.
      stopCharging();
    }
  }
}

//
// Function: sendData
// Description: Sends sensor data with a checksum.
// Format:
//   "DATA,<ParallelBatteryVoltage>,<SeriesBatteryVoltage>,<BatteryCurrent>,<Bat1Voltage>,<Bat2Voltage>,<PSUVoltage>,<chargeState>,<PSU_Available>,<Battery_Available>,<batteryPercentage>,<boardTemperature>*XX\n"
//
void sendData() {
  char payload[128];
  // Temporary buffers for float-to-string conversions.
  char strParallel[10];
  char strSeries[10];
  char strCurrent[10];
  char strBat1[10];
  char strBat2[10];
  char strPSU[10];
  char strBoardTemp[10];

  // Convert floats to strings with 2 decimal places.
  dtostrf(ParallelBatteryVoltage, 0, 2, strParallel);
  dtostrf(SeriesBatteryVoltage, 0, 2, strSeries);
  dtostrf(BatteryCurrent, 0, 2, strCurrent);
  dtostrf(Bat1Voltage, 0, 2, strBat1);
  dtostrf(Bat2Voltage, 0, 2, strBat2);
  dtostrf(PSUVoltage, 0, 2, strPSU);
  dtostrf(boardTemperature, 0, 2, strBoardTemp);

  // Build the payload using the converted string values.
  snprintf(payload, sizeof(payload),
           "DATA,%s,%s,%s,%s,%s,%s,%d,%d,%d,%d,%s",
           strParallel,
           strSeries,
           strCurrent,
           strBat1,
           strBat2,
           strPSU,
           chargeState,
           PSU_Available,
           Battery_Available,
           batteryPercentage,
           strBoardTemp);

  // Compute the checksum and build the final message.
  uint8_t chksum = computeChecksum(payload);
  char checksumStr[3];
  snprintf(checksumStr, sizeof(checksumStr), "%02X", chksum);

  char finalMessage[140];
  snprintf(finalMessage, sizeof(finalMessage), "%s*%s\n", payload, checksumStr);
  Serial.print(finalMessage);
}

//
// Serial Communication Setup
//
void SerialCommSetup() {
  Serial.begin(115200);
  // Reserve buffer memory if desired.
}

//
// Serial Communication Loop
//
void SerialComm() {
  // --- Read incoming serial data into the fixed-size buffer ---
  while (Serial.available()) {
    char inChar = Serial.read();
    if (inChar == '\n') {
      rxBuffer[rxIndex] = '\0';  // Null-terminate.
      parseCommand(rxBuffer);
      rxIndex = 0;  // Reset buffer index.
    } else {
      if (rxIndex < RX_BUFFER_SIZE - 1) {
        rxBuffer[rxIndex++] = inChar;
      } else {
        // Buffer overflow; discard message.
        rxIndex = 0;
      }
    }
  }


  // --- Send sensor data every 3 seconds ---
  if (millis() - lastSendTime >= 3000) {
    lastSendTime = millis();
    sendData();
  }

  if (millis() - lastDataRecv > MainProcessorTimeout) {
    lastDataRecv = millis();
    dataReceived = 0;
    stopCharging();
  }
}
