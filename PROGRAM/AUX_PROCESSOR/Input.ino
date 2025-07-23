Adafruit_ADS1115 ads;

const float PSUVoltage_divider_factor = 6.0;       // Resistor network multiplier (100k - 20k)
const float ADS1115_voltage_divider_factor = 6.0;  // Since the divider is 1/6
const float ads_gain_factor = 6.144 / 32768.0;     // ADS1115 with 2/3 gain, LSB value
const float current_sensitivity = 0.200;           // CC6900SO-10A: 200mV per Amp

const float PSU_UPPER_LIMIT = 21.0;  // PSU engage voltage
const float PSU_LOWER_LIMIT = 20.0;  // PSU disengage voltage

unsigned long lastReadTime = 0;
const int READINTERVAL = 100;  // Analog Read Interval

//NTC READING PARAMETERS
const float R_fixed = 10000.0;  // Fixed resistor value in ohms
const float R0 = 10000.0;       // Nominal resistance of the NTC at 25°C (ohms)
const float T0 = 298.15;        // Nominal temperature in Kelvin (25°C)
const float B = 3950.0;         // Beta parameter

#define Vtemp_OFFSET 0.4    // Offset voltage at 0°C (400mV)
#define SENSITIVITY 0.0195  // Sensitivity (19.5mV/°C)

void initializeInput() {
  Wire.begin();
  ads.begin(0x4A);              // I2C address for ADS1115 when ADDR tied to SDA
  ads.setGain(GAIN_TWOTHIRDS);  // ±6.144V range (For 5V input from divider)
}

void inputControl() {
  unsigned long currentTime = millis();
  if ((currentTime - lastReadTime) >= READINTERVAL) {
    readBatteryVoltages();
    readBatteryCurrent();
    readTemperature();
    readNTC();

    PSUVoltage = readPSUVoltage();
    if (PSUVoltage < PSU_LOWER_LIMIT) {
      chargeState = 0;
      PSU_Available = 0;
      stopCharging();
    } else if (PSUVoltage >= PSU_UPPER_LIMIT) {
      PSU_Available = 1;
    }

    if (ParallelBatteryVoltage > 6.0) {
      Battery_Available = 1;
    } else {
      Battery_Available = 0;
    }

    lastReadTime = currentTime;
  }
}

bool isADS1115Connected() {
  Wire.beginTransmission(0x4A);
  return (Wire.endTransmission() == 0);
}

void readNTC() {
  if (externalFAN == 1) {
    // Read the ADC value (0-1023)
    int ntcValue = analogRead(NTC_PIN);

    // Convert ADC reading to voltage (assuming a 10-bit ADC)
    float ntcVoltage = ntcValue * (5.0 / 1023.0);

    // To avoid division by zero in case of a very low voltage reading:
    if (ntcVoltage <= 0) {
      ntcVoltage = 0.0001;
    }

    // Calculate the thermistor resistance using the voltage divider formula:
    // V_out = Vcc * (R_fixed / (R_ntc + R_fixed))  -->  R_ntc = R_fixed * (Vcc/V_out - 1)
    float R_ntc = R_fixed * (5.0 / ntcVoltage - 1.0);

    // Use the Beta parameter equation to calculate temperature in Kelvin:
    // 1/T = 1/T0 + (1/B)*ln(R_ntc/R0)
    float ntcTemperatureK = 1.0 / ((1.0 / T0) + (1.0 / B) * log(R_ntc / R0));

    // Convert Kelvin to Celsius
    ntcTemperature = ntcTemperatureK - 273.15;
  }
}

void readTemperature() {
  int adcTempValue = analogRead(TEMP_SENSOR_PIN);                 // Read ADC value (0-1023)
  float tempvoltage = (adcTempValue * 5.0) / 1023.0;              // Convert to voltage
  boardTemperature = (tempvoltage - Vtemp_OFFSET) / SENSITIVITY;  // Calculate temperature
}

float readPSUVoltage() {
  int adcPSUValue = analogRead(PSUVoltage_PIN);                                     // Read ADC value from pin A0
  float f_PSUVoltage = ((adcPSUValue / 1023.0) * 5.0) * PSUVoltage_divider_factor;  // Convert ADC value to actual voltage
  return f_PSUVoltage;
}

void readBatteryVoltages() {
  int16_t raw_voltage2 = 0;
  int16_t raw_voltage1 = 0;
  if (isADS1115Connected()) {
    raw_voltage2 = ads.readADC_SingleEnded(0);  // AIN0
    raw_voltage1 = ads.readADC_SingleEnded(1);  // AIN1
  }
  // Convert raw ADC values to voltages
  float measured_voltage1 = raw_voltage1 * ads_gain_factor;
  float measured_voltage2 = raw_voltage2 * ads_gain_factor;
  // Scale voltages based on the 1/6 divider
  if (chargeState == 0) {
    Bat1Voltage = measured_voltage1 * ADS1115_voltage_divider_factor;
    Bat2Voltage = (measured_voltage2 * ADS1115_voltage_divider_factor) - Bat1Voltage;
  } else {
    Bat1Voltage = measured_voltage1 * ADS1115_voltage_divider_factor;
    Bat2Voltage = measured_voltage2 * ADS1115_voltage_divider_factor;
  }

  SeriesBatteryVoltage = Bat1Voltage + Bat2Voltage;
  ParallelBatteryVoltage = (Bat1Voltage + Bat2Voltage) / 2;
}

void readBatteryCurrent() {
  BatteryCurrent = 0;
  if (isADS1115Connected()) {
    int16_t raw_current = ads.readADC_SingleEnded(2);  // AIN2
                                                       // Convert raw ADC value to voltage
    float measured_current_voltage = raw_current * ads_gain_factor;
    // Convert current sensor output voltage to actual current
    BatteryCurrent = fabs((measured_current_voltage - 2.5) / current_sensitivity);
  }
}
