// --- Global Variables for Fan 1 ---
bool fanWasOff = true;            // Tracks if fan 1 was previously off
bool blipActive = false;          // Indicates if fan 1 blip is active
bool fanPWR = false;              // Fan 1 power state: true = on, false = off
unsigned long blipStartTime = 0;  // Timestamp for fan 1 blip

// --- Global Variables for Optional Fan ---
bool fan2WasOff = true;           // Tracks if fan 2 was previously off
bool blipActive2 = false;         // Indicates if fan 2 blip is active
bool fan2PWR = false;             // Fan 2 power state: true = on, false = off
unsigned long blipStartTime2 = 0; // Timestamp for fan 2 blip

void controlFAN() {
  // --- Fan 1 Control (Pin 6) ---
  // Determine desired power state for Fan 1 using boardTemperature.
  if (boardTemperature >= 40.0) {
    fanPWR = true;
  } else if (boardTemperature < 35.0) {
    fanPWR = false;
    fanWasOff = true;
  }

  if (fanPWR) {
    // Map temperature to duty cycle: 20% at 50°C, 100% at 80°C.
    int pwmValue = map(boardTemperature, 40, 80, 51, 255);
    pwmValue = constrain(pwmValue, 51, 255);

    // If the fan was off and we now need to start, perform a blip.
    if (fanWasOff && pwmValue > 0 && !blipActive) {
      blipActive = true;
      blipStartTime = millis();
      analogWrite(FAN_PIN, 255);  // Full speed blip.
    }

    // End the blip after 200ms.
    if (blipActive && (millis() - blipStartTime >= 200)) {
      blipActive = false;
      fanWasOff = false;
    }

    // Apply normal PWM if not in blip.
    if (!blipActive) {
      analogWrite(FAN_PIN, pwmValue);
    }
  } else {
    // If not powered, ensure PWM is zero.
    analogWrite(FAN_PIN, 0);
  }

  // --- Fan 2 Control (Pin 5) ---
  if (externalFAN == 1) {
    if (ntcTemperature >= 45.0) {
      fan2PWR = true;
    } else if (ntcTemperature < 40.0) {
      fan2PWR = false;
      fan2WasOff = true;
    }

    if (fan2PWR) {
      // Map ntcTemperature to duty cycle (20% at 45°C, 100% at 80°C)
      int pwmValue2 = map(ntcTemperature, 45, 80, 51, 255);
      pwmValue2 = constrain(pwmValue2, 51, 255);

      // Handle the fan blip when transitioning from off to on.
      if (fan2WasOff && pwmValue2 > 0 && !blipActive2) {
        blipActive2 = true;
        blipStartTime2 = millis();
        analogWrite(EXT_FAN_PIN, 255);  // Full speed blip for Fan 2.
      }

      // End the blip after 200ms.
      if (blipActive2 && (millis() - blipStartTime2 >= 200)) {
        blipActive2 = false;
        fan2WasOff = false;
      }

      // Apply PWM if the blip is not active.
      if (!blipActive2) {
        analogWrite(EXT_FAN_PIN, pwmValue2);
      }
    } else {
      analogWrite(EXT_FAN_PIN, 0);
    }
  }
}
