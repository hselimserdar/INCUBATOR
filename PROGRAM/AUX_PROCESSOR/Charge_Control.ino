// Battery Charge Logic
float EndChargeCurrent = 0.1;                         // Current threshold to stop charging (e.g., 0.1A)
const float MinSafeVoltage = 6.0;                     // Minimum safe voltage for charging
const float MaxSafeVoltage = 15.0;                    // Maximum safe voltage for charging
unsigned long lastChargeTime = 0;                     // Tracks the last time charging was initiated
const unsigned long chargeCooldown = 20 * 60 * 1000;  // 20 minutes in milliseconds

// PID Parameters for Voltage Control
float Kp_voltage = 1.0;
float Ki_voltage = 0.1;
float Kd_voltage = 0.01;

// PID Parameters for Current Control
float Kp_current = 1.0;
float Ki_current = 0.1;
float Kd_current = 0.01;

// PID State Variables
float voltageError = 0.0;
float voltageIntegral = 0.0;
float voltagePreviousError = 0.0;

float currentError = 0.0;
float currentIntegral = 0.0;
float currentPreviousError = 0.0;

// PWM Parameters
const int PWMFrequency = 100000;  // 100 kHz
const int PWMResolution = 159;    // Timer1 TOP value for 100 kHz
int dutyCycle = 0;                // PWM duty cycle (0 to 159)

// Timing Variables for Non-Blocking PID Control
unsigned long lastUpdateTime = 0;         // Time of last PID update
const unsigned long updateInterval = 10;  // Update interval in milliseconds (10 ms)

// Soft-Start Variables
unsigned long softStartTime = 0;               // Time when soft start began
const unsigned long softStartDuration = 3000;  // Soft start duration in milliseconds (3 seconds)
bool softStartActive = false;


// Function to initialize the PWM setup
void initializePIDPWM() {
  // Configure Timer1 for Fast PWM mode (Mode 14)
  TCCR1A = (1 << WGM11) | (1 << COM1A1);               // Fast PWM, non-inverting mode
  TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS10);  // Prescaler = 1

  // Set the PWM frequency to 100 kHz
  ICR1 = PWMResolution;  // TOP value for 100 kHz

  // Initialize the duty cycle to 0
  OCR1A = 0;
  PWMEnable = true;
}

// Function to handle PID control
void updatePIDControl() {
  // Only update PID if charging is active
  if ((chargeState == 1) && (PWMEnable == true)) {
    unsigned long currentTime = millis();

    if (currentTime - lastUpdateTime >= updateInterval) {
      lastUpdateTime = currentTime;

      // Calculate errors for both voltage and current
      voltageError = SetBatteryVoltage - ParallelBatteryVoltage;
      currentError = SetBatteryCurrent - BatteryCurrent;

      // Voltage PID terms
      voltageIntegral += voltageError * (updateInterval / 1000.0);
      float voltageDerivative = (voltageError - voltagePreviousError) / (updateInterval / 1000.0);
      float voltageOutput = Kp_voltage * voltageError + Ki_voltage * voltageIntegral + Kd_voltage * voltageDerivative;

      // Current PID terms
      currentIntegral += currentError * (updateInterval / 1000.0);
      float currentDerivative = (currentError - currentPreviousError) / (updateInterval / 1000.0);
      float currentOutput = Kp_current * currentError + Ki_current * currentIntegral + Kd_current * currentDerivative;

      // Determine control output: Use voltage PID unless current hits the limit
      if (BatteryCurrent >= SetBatteryCurrent) {
        dutyCycle = constrain((int)(currentOutput * PWMResolution / SetBatteryCurrent), 0, PWMResolution);
      } else {
        dutyCycle = constrain((int)(voltageOutput * PWMResolution / SetBatteryVoltage), 0, PWMResolution);
      }

      // Apply soft-start if active
      if (softStartActive) {
        unsigned long elapsedSoftStartTime = millis() - softStartTime;
        int maxDutyCycle = map(elapsedSoftStartTime, 0, softStartDuration, 0, PWMResolution);
        if (elapsedSoftStartTime >= softStartDuration) {
          softStartActive = false;  // End soft-start
        }
        dutyCycle = min(dutyCycle, maxDutyCycle);
      }

      // Update the duty cycle
      OCR1A = dutyCycle;

      // Update previous error terms
      voltagePreviousError = voltageError;
      currentPreviousError = currentError;

      // Stop charging if end condition is met
      if (ParallelBatteryVoltage >= SetBatteryVoltage && BatteryCurrent <= EndChargeCurrent) {
        stopCharging();
      }
    }
  }
}

// Function to start charging with soft-start
void startCharging() {
  if ((PSU_Available == 1) && (Battery_Available == 1) && (dataReceived == 1) && (Bat1Voltage >= MinSafeVoltage) && (Bat1Voltage <= MaxSafeVoltage) && (Bat2Voltage >= MinSafeVoltage) && (Bat2Voltage <= MaxSafeVoltage) && canChargeInParallel()) {
    chargeState = 1;
    softStartTime = millis();
    softStartActive = true;
    dutyCycle = 0;
    lastChargeTime = millis();
  }
}

// Function to stop charging
void stopCharging() {
  chargeState = 0;
  dutyCycle = 0;
  OCR1A = dutyCycle;  // Turn off PWM

  // RESET PWM STATE VARIABLES
  voltageError = 0.0;
  voltageIntegral = 0.0;
  voltagePreviousError = 0.0;

  currentError = 0.0;
  currentIntegral = 0.0;
  currentPreviousError = 0.0;
}

// Function to check if parallel charging is allowed
bool canChargeInParallel() {
  if (fabs(Bat1Voltage - Bat2Voltage) <= MaxVoltageDifference) {
    return true;
  } else {
    return false;
  }
}

// Function to decide when to start charging
void checkStartConditions() {
  unsigned long currentTime = millis();
  if ((Battery_Available == 0) || (dataReceived == 0)) {
    chargeState = 0;
    softStartActive = true;
    dutyCycle = 0;
  } else if (PSU_Available == 1) {
    if (((currentTime - lastChargeTime) >= chargeCooldown) && (Bat1Voltage < LowVoltageThreshold) && (Bat2Voltage < LowVoltageThreshold)) {
      startCharging();
    }
  }
}