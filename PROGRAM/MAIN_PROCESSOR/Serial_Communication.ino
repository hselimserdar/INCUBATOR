//Serial_Communication.ino
#define UART1_RX_PIN 4
#define UART1_TX_PIN 5
#define RX_BUFFER_SIZE 256  // Fixed-size receive buffer

int auxChipOnline = 0;  //Aux processor is online (data received correctly) if equals 1

// Use the built-in Serial1 instance provided by the ESP32 core.
char rxBuffer[RX_BUFFER_SIZE];
uint8_t rxIndex = 0;

unsigned long lastSetCommandTime = 0;
unsigned long lastDataRecv = 0;
int SendSETInterval = 3000;
int AuxProcessorTimeout = 30000;

// Global variables to store parsed data from the ATmega328P.
float gParallelBatteryVoltage = 0.0;
float gSeriesBatteryVoltage = 0.0;
float gBatteryCurrent = 0.0;
float gBat1Voltage = 0.0;
float gBat2Voltage = 0.0;
float gPSUVoltage = 0.0;
int gChargeState = 0;
int gPSU_Available = 0;
int gBattery_Available = 0;
int gBatteryPercentage = 0;
float gBoardTemperature = 0.0;

//
// Helper: Compute an additive checksum (modulo 256) over a C string,
// stopping at the delimiter if provided. If delimiter == '\0', process whole string.
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
// Helper: Convert a two-character hex string to an unsigned 8-bit value.
//
uint8_t hexToUint8(const char *hexStr) {
  return (uint8_t)strtol(hexStr, NULL, 16);
}

//
// Function: parseATmegaDataMessage
// Description: Validates the checksum and parses the CSV data string received from the ATmega328P.
// Expected incoming format:
//   DATA,<ParallelBatteryVoltage>,<SeriesBatteryVoltage>,<BatteryCurrent>,<Bat1Voltage>,<Bat2Voltage>,
//   <PSUVoltage>,<chargeState>,<PSU_Available>,<Battery_Available>,<batteryPercentage>,<boardTemperature>*XX
//
void parseATmegaMessage(char *message) {
  Serial0.print("AUXCHIP RECEIVED: ");
  Serial0.println(message);
  // Look for the '*' delimiter.
  char *asteriskPos = strchr(message, '*');
  if (!asteriskPos) {
    Serial0.println("ERROR: No checksum delimiter found; ignore message.");
    return;
  }

  // Temporarily terminate the payload.
  *asteriskPos = '\0';

  // Compute checksum over the payload.
  uint8_t computedChecksum = computeChecksum(message);

  // The provided checksum is in the two characters immediately following the '*'.
  char providedChecksumStr[3] = { 0 };
  strncpy(providedChecksumStr, asteriskPos + 1, 2);
  uint8_t providedChecksum = hexToUint8(providedChecksumStr);

  // Verify the checksum.
  if (computedChecksum != providedChecksum) {
    // Checksum mismatch.
    Serial0.println("ERROR: Checksum Error, message discarded");
    return;
  }

  // Expect the payload to begin with "DATA,"
  if (strncmp(message, "DATA,", 5) != 0) {
    Serial0.println("ERROR: Message is not starting with DATA");
    return;
  }

  // Skip the "DATA," prefix.
  char *csv = message + 5;

  // Tokenize the CSV string. Expect 11 tokens.
  const int expectedTokens = 11;
  char *tokens[expectedTokens];
  int tokenCount = 0;

  char *token = strtok(csv, ",");
  while (token != NULL && tokenCount < expectedTokens) {
    tokens[tokenCount++] = token;
    token = strtok(NULL, ",");
  }

  if (tokenCount != expectedTokens) {
    Serial0.println("ERROR: Token count error.");
    return;
  }

  // Convert tokens to numbers and update globals.
  gParallelBatteryVoltage = atof(tokens[0]);
  gSeriesBatteryVoltage = atof(tokens[1]);
  gBatteryCurrent = atof(tokens[2]);
  gBat1Voltage = atof(tokens[3]);
  gBat2Voltage = atof(tokens[4]);
  gPSUVoltage = atof(tokens[5]);
  gChargeState = atoi(tokens[6]);
  gPSU_Available = atoi(tokens[7]);
  gBattery_Available = atoi(tokens[8]);
  gBatteryPercentage = atoi(tokens[9]);
  gBoardTemperature = atof(tokens[10]);

  auxChipOnline = 1;
  Serial0.println("AUXCHIP: ONLINE - Data received and parsed correctly");
  lastDataRecv = millis();
}

//
// Function: sendSetCommand
// Outgoing command format: "SET,<voltage>,<current>*XX\n"
//
void sendSetCommand(float targetVoltage, float targetCurrent) {
  if (!auxProcessorHalt) {
    char payload[64];  // Buffer for the payload
    snprintf(payload, sizeof(payload), "SET,%.2f,%.2f", targetVoltage, targetCurrent);
    uint8_t chksum = computeChecksum(payload);
    char checksumStr[3];
    snprintf(checksumStr, sizeof(checksumStr), "%02X", chksum);

    char finalCommand[80];
    snprintf(finalCommand, sizeof(finalCommand), "%s*%s\n", payload, checksumStr);
    Serial1.print(finalCommand);

    Serial0.print("AUXCHIP SENT: ");
    Serial0.println(finalCommand);
  }
  Serial0.println("AUXCHIP: HALTED! - will not receive data");
}

//
// Function: charging
// Outgoing command format: "CMD,START*XX\n" or "CMD,STOP*XX\n"
//
void charging(bool start) {
  if (!auxProcessorHalt) {
    const char *cmd = start ? "CMD,START" : "CMD,STOP";
    uint8_t chksum = computeChecksum(cmd);
    char checksumStr[3];
    snprintf(checksumStr, sizeof(checksumStr), "%02X", chksum);

    char finalCommand[32];
    snprintf(finalCommand, sizeof(finalCommand), "%s*%s\n", cmd, checksumStr);
    Serial1.print(finalCommand);

    Serial0.print("AUXCHIP SENT: ");
    Serial0.println(finalCommand);
  }
  Serial0.println("AUXCHIP: HALTED! - will not start charging");
}

//
// Function: serialCommSetup
// Description: Initializes UART1 using the specified RX/TX pins.
//
void serialCommSetup() {
  Serial1.begin(115200, SERIAL_8N1, UART1_RX_PIN, UART1_TX_PIN);
  Serial0.begin(115200);
}

//
// Function: serialComm
// Reads incoming data into a fixed-size buffer, processes complete messages,
// and periodically sends a SET command.
//
void serialComm() {
  // Read available data.
  while (Serial1.available()) {
    char inChar = Serial1.read();
    if (inChar == '\n') {
      // End of message; terminate buffer and process.
      rxBuffer[rxIndex] = '\0';
      parseATmegaMessage(rxBuffer);
      rxIndex = 0;  // Reset buffer index.
    } else {
      if (rxIndex < RX_BUFFER_SIZE - 1) {
        rxBuffer[rxIndex++] = inChar;
      } else {
        // Buffer overflow: discard the message.
        rxIndex = 0;
      }
    }
  }

  // Periodically send a SET command every 5 seconds.
  if (millis() - lastSetCommandTime > 5000) {
    lastSetCommandTime = millis();
    sendSetCommand(targetVoltage, targetCurrent);
  }

  if (millis() - lastDataRecv > AuxProcessorTimeout) {
    lastDataRecv = millis();
    auxChipOnline = 0;
    Serial0.println("AUXCHIP: OFFLINE");
  }
}
