void calculatePercentage() {
  if (chargeState == 1) {
    batteryPercentage = calculateChargingPercentage(ParallelBatteryVoltage, SetBatteryVoltage, BatteryCurrent, SetBatteryCurrent);
  } else {
    batteryPercentage = calculateDischargePercentage(SeriesBatteryVoltage);
  }
}

// Function to estimate discharge percentage using a non-linear discharge curve
float calculateDischargePercentage(float batteryVoltage) {
  // Define the voltage levels and corresponding SoC percentages
  const int numPoints = 11;
  float voltages[numPoints] = { 12.90, 12.80, 12.70, 12.60, 12.50, 12.40, 12.30, 12.20, 12.10, 12.00, 11.00 };
  float percentages[numPoints] = { 100, 90, 80, 70, 60, 50, 40, 30, 20, 10, 0 };

  // Clamp battery voltage within the range
  if (batteryVoltage >= voltages[0]) {
    return 100.0;  // Fully charged
  } else if (batteryVoltage <= voltages[numPoints - 1]) {
    return 0.0;  // Fully discharged
  }

  // Find the appropriate segment in the discharge curve
  for (int i = 0; i < numPoints - 1; i++) {
    if (batteryVoltage <= voltages[i] && batteryVoltage > voltages[i + 1]) {
      // Perform linear interpolation within the segment
      float slope = (percentages[i + 1] - percentages[i]) / (voltages[i + 1] - voltages[i]);
      return percentages[i] + slope * (batteryVoltage - voltages[i]);
    }
  }

  // Default case (should not reach here if clamped properly)
  return 0.0;
}

// Function to estimate the charging percentage
float calculateChargingPercentage(float batteryVoltage, float setVoltage, float batteryCurrent, float cutoffCurrent) {
  // Voltage contribution to charging percentage
  float voltagePercentage = 0.0;
  if (batteryVoltage >= 12.0 && batteryVoltage <= setVoltage) {
    voltagePercentage = ((batteryVoltage - 12.0) / (setVoltage - 12.0)) * 100.0;
  } else if (batteryVoltage > setVoltage) {
    voltagePercentage = 100.0;  // Clamp to 100% if voltage exceeds set voltage
  }

  // Current contribution to charging percentage
  float currentPercentage = 0.0;
  if (batteryCurrent > cutoffCurrent) {
    currentPercentage = (1.0 - (batteryCurrent / SetBatteryCurrent)) * 100.0;
  } else {
    currentPercentage = 100.0;  // Clamp to 100% if current is below cutoff
  }

  // Combine voltage and current contributions
  float chargingPercentage = (voltagePercentage * 0.7) + (currentPercentage * 0.3);

  // Ensure percentage is within valid range
  if (chargingPercentage > 100.0) {
    chargingPercentage = 100.0;
  } else if (chargingPercentage < 0.0) {
    chargingPercentage = 0.0;
  }

  return chargingPercentage;
}
