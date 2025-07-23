//EEPROM.ino
unsigned long resetTimestamp = 0;  //Stores the last reset time
float targetVoltage = 14.4;        //target voltage.
float targetCurrent = 2.0;         //target current.
bool auxProcessorHalt = 0;         //Stop data transfer to aux processor. Eventually stops charge controller 0:Active, 1:Deactivated
float TempUpperLimit;
float TempLowerLimit;
int HumidityUpperLimit;
int HumidityLowerLimit;
int EggRotateDuration;
int EggRotateInterval;
int EggRotateStoppingDay;

// EEPROM size: 41 bytes
#define EEPROM_SIZE 41
#define EEPROM_ADDR_resetTimestamp 0         // 0 - 3 bytes
#define EEPROM_ADDR_targetVoltage 4          // 4 - 7 bytes
#define EEPROM_ADDR_targetCurrent 8          // 8 - 11 bytes
#define EEPROM_ADDR_auxProcessorHalt 12      // 12th byte
#define EEPROM_ADDR_TempUpperLimit 13        // 13 - 16 bytes
#define EEPROM_ADDR_TempLowerLimit 17        // 17 - 20 bytes
#define EEPROM_ADDR_HumidityUpperLimit 21    // 21 - 24 bytes
#define EEPROM_ADDR_HumidityLowerLimit 25    // 25 - 28 bytes
#define EEPROM_ADDR_EggRotateDuration 29     // 29 - 32 bytes
#define EEPROM_ADDR_EggRotateInterval 33     // 33 - 36 bytes
#define EEPROM_ADDR_EggRotateStoppingDay 37  // 37 - 40 bytes

void EEPROM_init() {
  EEPROM.begin(EEPROM_SIZE);                          // Initialize EEPROM with defined size
  resetTimestamp = readResetTime();                   // Read the stored reset time from EEPROM.
  targetVoltage = readTargetVoltage();                // Read the stored target charging voltage from EEPROM.
  targetCurrent = readTargetCurrent();                // Read the stored target charging current from EEPROM.
  auxProcessorHalt = readAuxProcessorHalt();          // Read the stored Aux Processor Halt value from EEPROM.
  TempUpperLimit = readTempUpperLimit();              // Read the stored TempUpperLimit value from EEPROM.
  TempLowerLimit = readTempLowerLimit();              // Read the stored TempLowerLimit value from EEPROM.
  HumidityUpperLimit = readHumidityUpperLimit();      // Read the stored HumidityUpperLimit value from EEPROM.
  HumidityLowerLimit = readHumidityLowerLimit();      // Read the stored HumidityLowerLimit value from EEPROM.
  EggRotateDuration = readEggRotateDuration();        // Read the stored EggRotateDuration value from EEPROM.
  EggRotateInterval = readEggRotateInterval();        // Read the stored EggRotateInterval value from EEPROM.
  EggRotateStoppingDay = readEggRotateStoppingDay();  // Read the stored EggRotateStoppingDay from EEPROM.
  checkDataIntegrity();
}

void settingsEEPROMSave() {
  // Write temperature settings
  writeTempUpperLimit(TempUpperLimit);
  writeTempLowerLimit(TempLowerLimit);

  // Write humidity settings
  writeHumidityUpperLimit(HumidityUpperLimit);
  writeHumidityLowerLimit(HumidityLowerLimit);

  // Write egg rotation settings
  writeEggRotateDuration(EggRotateDuration);
  writeEggRotateInterval(EggRotateInterval);
  writeEggRotateStoppingDay(EggRotateStoppingDay);
}

void checkDataIntegrity() {
  currentTime = rtc.now().unixtime();
  // If no valid time is stored (e.g., EEPROM contains 0xFFFFFFFF), initialize it.
  if (resetTimestamp == 0xFFFFFFFF) {
    resetTimestamp = currentTime;
    writeResetTime(resetTimestamp);
  }

  // Check if the stored reset time is in the future relative to the current time.
  // This can happen if EEPROM data is corrupted.
  if (currentTime < resetTimestamp) {
    resetTimestamp = currentTime;
    writeResetTime(resetTimestamp);
  }

  // If the computed days passed is out of the acceptable range [0, 100],
  // then assume EEPROM data is invalid/corrupted.
  if (daycount > 99) {
    resetTimestamp = currentTime;
    writeResetTime(resetTimestamp);
  }

  if ((targetVoltage < 12.0) || (targetVoltage > 16.0)) {
    targetVoltage = 14.4;
    writeTargetVoltage(targetVoltage);
  }

  if ((targetCurrent < 0.2) || (targetCurrent > 4.0)) {
    targetCurrent = 2.0;
    writeTargetCurrent(targetCurrent);
  }

  // Check TempUpperLimit (default 35.00, allowed range: 30.00 - 40.00, must be multiple of 0.1)
  if ((TempUpperLimit < 30.0) || (TempUpperLimit > 40.0) || (fabs(TempUpperLimit * 10 - round(TempUpperLimit * 10)) > 0.001)) {
    TempUpperLimit = 35.00;
    writeTempUpperLimit(TempUpperLimit);
  }

  // Check TempLowerLimit (default 33.00, allowed range: 30.00 - 40.00, must be multiple of 0.1)
  if ((TempLowerLimit < 30.0) || (TempLowerLimit > 40.0) || (fabs(TempLowerLimit * 10 - round(TempLowerLimit * 10)) > 0.001)) {
    TempLowerLimit = 33.00;
    writeTempLowerLimit(TempLowerLimit);
  }

  // Ensure TempLowerLimit is lower than TempUpperLimit.
  if (TempLowerLimit >= TempUpperLimit) {
    TempUpperLimit = 35.00;
    writeTempUpperLimit(TempUpperLimit);
    TempLowerLimit = 33.00;
    writeTempLowerLimit(TempLowerLimit);
  }

  // Check HumidityUpperLimit (default 70, allowed range: 5 - 95, must be multiple of 5)
  if ((HumidityUpperLimit < 5) || (HumidityUpperLimit > 95) || (HumidityUpperLimit % 5 != 0)) {
    HumidityUpperLimit = 70;
    writeHumidityUpperLimit(HumidityUpperLimit);
  }

  // Check HumidityLowerLimit (default 50, allowed range: 5 - 95, must be multiple of 5)
  if ((HumidityLowerLimit < 5) || (HumidityLowerLimit > 95) || (HumidityLowerLimit % 5 != 0)) {
    HumidityLowerLimit = 50;
    writeHumidityLowerLimit(HumidityLowerLimit);
  }

  // Ensure HumidityLowerLimit is lower than HumidityUpperLimit.
  if (HumidityLowerLimit >= HumidityUpperLimit) {
    HumidityUpperLimit = 70;
    writeHumidityUpperLimit(HumidityUpperLimit);
    HumidityLowerLimit = 50;
    writeHumidityLowerLimit(HumidityLowerLimit);
  }

  // Check EggRotateDuration (default 30, allowed range: 1 - 99)
  if ((EggRotateDuration < 1) || (EggRotateDuration > 99)) {
    EggRotateDuration = 30;
    writeEggRotateDuration(EggRotateDuration);
  }

  // Check EggRotateInterval (default 120, allowed range: 5 - 300, must be multiple of 5)
  if ((EggRotateInterval < 5) || (EggRotateInterval > 300) || (EggRotateInterval % 5 != 0)) {
    EggRotateInterval = 120;
    writeEggRotateInterval(EggRotateInterval);
  }

  // Check EggRotateStoppingDay (default 15, allowed range: 1 - 99)
  if ((EggRotateStoppingDay < 1) || (EggRotateStoppingDay > 99)) {
    EggRotateStoppingDay = 15;
    writeEggRotateStoppingDay(EggRotateStoppingDay);
  }
}

// Function to write a 4-byte timestamp to EEPROM
void writeResetTime(unsigned long timeValue) {
  for (int i = 0; i < 4; i++) {
    EEPROM.write(EEPROM_ADDR_resetTimestamp + i, (timeValue >> (8 * i)) & 0xFF);
  }
  EEPROM.commit();
}

// Function to write a float (4 bytes) to EEPROM for targetVoltage.
void writeTargetVoltage(float voltage) {
  int stored = (int)(voltage * 10 + 0.5);  // Multiply by 10 and round
  union {
    int stored;
    byte bytes[4];
  } data;
  data.stored = stored;
  for (int i = 0; i < 4; i++) {
    EEPROM.write(EEPROM_ADDR_targetVoltage + i, data.bytes[i]);
  }
  EEPROM.commit();
}

// Function to write a float (4 bytes) to EEPROM for targetCurrent.
void writeTargetCurrent(float current) {
  int stored = (int)(current * 10 + 0.5);  // Multiply by 10 and round
  union {
    int stored;
    byte bytes[4];
  } data;
  data.stored = stored;
  for (int i = 0; i < 4; i++) {
    EEPROM.write(EEPROM_ADDR_targetCurrent + i, data.bytes[i]);
  }
  EEPROM.commit();
}

// Function to write the boolean value to EEPROM for auxProcessorHalt.
void writeAuxProcessorHalt(bool state) {
  // Write 1 for true and 0 for false.
  EEPROM.write(EEPROM_ADDR_auxProcessorHalt, state ? 1 : 0);
  EEPROM.commit();
}

// TempUpperLimit (float)
void writeTempUpperLimit(float temp) {
  int stored = (int)(temp * 10 + 0.5);  // Multiply by 10 and round
  union {
    int stored;
    byte bytes[4];
  } data;
  data.stored = stored;
  for (int i = 0; i < 4; i++) {
    EEPROM.write(EEPROM_ADDR_TempUpperLimit + i, data.bytes[i]);
  }
  EEPROM.commit();
}

// TempLowerLimit (float)
void writeTempLowerLimit(float temp) {
  int stored = (int)(temp * 10 + 0.5);  // Multiply by 10 and round
  union {
    int stored;
    byte bytes[4];
  } data;
  data.stored = stored;
  for (int i = 0; i < 4; i++) {
    EEPROM.write(EEPROM_ADDR_TempLowerLimit + i, data.bytes[i]);
  }
  EEPROM.commit();
}

// HumidityUpperLimit (int)
void writeHumidityUpperLimit(int humidity) {
  union {
    int value;
    byte bytes[4];
  } data;
  data.value = humidity;
  for (int i = 0; i < 4; i++) {
    EEPROM.write(EEPROM_ADDR_HumidityUpperLimit + i, data.bytes[i]);
  }
  EEPROM.commit();
}

// HumidityLowerLimit (int)
void writeHumidityLowerLimit(int humidity) {
  union {
    int value;
    byte bytes[4];
  } data;
  data.value = humidity;
  for (int i = 0; i < 4; i++) {
    EEPROM.write(EEPROM_ADDR_HumidityLowerLimit + i, data.bytes[i]);
  }
  EEPROM.commit();
}

// EggRotateDuration (int)
void writeEggRotateDuration(int duration) {
  union {
    int value;
    byte bytes[4];
  } data;
  data.value = duration;
  for (int i = 0; i < 4; i++) {
    EEPROM.write(EEPROM_ADDR_EggRotateDuration + i, data.bytes[i]);
  }
  EEPROM.commit();
}

// EggRotateInterval (int)
void writeEggRotateInterval(int interval) {
  union {
    int value;
    byte bytes[4];
  } data;
  data.value = interval;
  for (int i = 0; i < 4; i++) {
    EEPROM.write(EEPROM_ADDR_EggRotateInterval + i, data.bytes[i]);
  }
  EEPROM.commit();
}

// EggRotateStoppingDay (int)
void writeEggRotateStoppingDay(int stoppingDay) {
  union {
    int value;
    byte bytes[4];
  } data;
  data.value = stoppingDay;
  for (int i = 0; i < 4; i++) {
    EEPROM.write(EEPROM_ADDR_EggRotateStoppingDay + i, data.bytes[i]);
  }
  EEPROM.commit();
}

// Function to read a 4-byte timestamp from EEPROM
unsigned long readResetTime() {
  unsigned long value = 0;
  for (int i = 0; i < 4; i++) {
    value |= ((unsigned long)EEPROM.read(EEPROM_ADDR_resetTimestamp + i)) << (8 * i);
  }
  return value;
}

// Function to read a float (4 bytes) from EEPROM for targetVoltage.
float readTargetVoltage() {
  union {
    int stored;
    byte bytes[4];
  } data;
  for (int i = 0; i < 4; i++) {
    data.bytes[i] = EEPROM.read(EEPROM_ADDR_targetVoltage + i);
  }
  return data.stored / 10.0;
}

// Function to read a float (4 bytes) from EEPROM for targetCurrent.
float readTargetCurrent() {
  union {
    int stored;
    byte bytes[4];
  } data;
  for (int i = 0; i < 4; i++) {
    data.bytes[i] = EEPROM.read(EEPROM_ADDR_targetCurrent + i);
  }
  return data.stored / 10.0;
}

bool readAuxProcessorHalt() {
  // Read the byte at the designated address and interpret 0 as false and any non-zero as true.
  return (EEPROM.read(EEPROM_ADDR_auxProcessorHalt) != 0);
}

// TempUpperLimit (float)
float readTempUpperLimit() {
  union {
    int stored;
    byte bytes[4];
  } data;
  for (int i = 0; i < 4; i++) {
    data.bytes[i] = EEPROM.read(EEPROM_ADDR_TempUpperLimit + i);
  }
  return data.stored / 10.0;
}

// TempLowerLimit (float)
float readTempLowerLimit() {
  union {
    int stored;
    byte bytes[4];
  } data;
  for (int i = 0; i < 4; i++) {
    data.bytes[i] = EEPROM.read(EEPROM_ADDR_TempLowerLimit + i);
  }
  return data.stored / 10.0;
}

// HumidityUpperLimit (int)
int readHumidityUpperLimit() {
  union {
    int value;
    byte bytes[4];
  } data;
  for (int i = 0; i < 4; i++) {
    data.bytes[i] = EEPROM.read(EEPROM_ADDR_HumidityUpperLimit + i);
  }
  return data.value;
}

// HumidityLowerLimit (int)
int readHumidityLowerLimit() {
  union {
    int value;
    byte bytes[4];
  } data;
  for (int i = 0; i < 4; i++) {
    data.bytes[i] = EEPROM.read(EEPROM_ADDR_HumidityLowerLimit + i);
  }
  return data.value;
}

// EggRotateDuration (int)
int readEggRotateDuration() {
  union {
    int value;
    byte bytes[4];
  } data;
  for (int i = 0; i < 4; i++) {
    data.bytes[i] = EEPROM.read(EEPROM_ADDR_EggRotateDuration + i);
  }
  return data.value;
}

// EggRotateInterval (int)
int readEggRotateInterval() {
  union {
    int value;
    byte bytes[4];
  } data;
  for (int i = 0; i < 4; i++) {
    data.bytes[i] = EEPROM.read(EEPROM_ADDR_EggRotateInterval + i);
  }
  return data.value;
}

// EggRotateStoppingDay (int)
int readEggRotateStoppingDay() {
  union {
    int value;
    byte bytes[4];
  } data;
  for (int i = 0; i < 4; i++) {
    data.bytes[i] = EEPROM.read(EEPROM_ADDR_EggRotateStoppingDay + i);
  }
  return data.value;
}
