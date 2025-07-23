void initializeOutput() {
  pinMode(MosfetPin, OUTPUT);
  initializePIDPWM();

  pinMode(FAN_PIN, OUTPUT);
  pinMode(EXT_FAN_PIN, OUTPUT);

  pinMode(batterySW, OUTPUT);
  pinMode(chargeEN, OUTPUT);
  pinMode(relayModuleSW, OUTPUT);
}

void outputControl() {
  // Static variables to store the previous state for detecting transitions.
  static int prev_PSU_Available = 1;  // Assume initially PSU is available.
  static int prev_chargeState = 0;    // Assume initially not charging.

  // --- Handle transition from PSU_Available 1 -> 0 ---
  if (prev_PSU_Available == 1 && PSU_Available == 0) {
    // Only perform the sequence if relayModuleSW was HIGH.
    if (digitalRead(relayModuleSW) == HIGH) {
      digitalWrite(chargeEN, LOW);  // Step 1: turn off charge enable.
      delay(1);
      digitalWrite(relayModuleSW, LOW);  // Step 2: turn off relay.
      delay(1);
    } else {
      digitalWrite(chargeEN, LOW);
      digitalWrite(relayModuleSW, LOW);
    }
    disablePWM();
    digitalWrite(batterySW, LOW);
  }

  // --- Handle transition from chargeState 1 -> 0 ---
  else if (prev_chargeState == 1 && chargeState == 0) {
    // Only perform the sequence if relayModuleSW was HIGH.
    if (digitalRead(relayModuleSW) == HIGH) {
      digitalWrite(chargeEN, LOW);  // Step 1: turn off charge enable.
      delay(1);
      digitalWrite(relayModuleSW, LOW);  // Step 2: turn off relay.
    } else {
      digitalWrite(chargeEN, LOW);
      digitalWrite(relayModuleSW, LOW);
    }
    disablePWM();
    digitalWrite(batterySW, HIGH);
  }

  // --- Handle transition from chargeState 0 -> 1 ---
  else if (prev_chargeState == 0 && chargeState == 1) {
    // Only perform the sequence if relayModuleSW was LOW.
    if (digitalRead(relayModuleSW) == LOW) {
      digitalWrite(relayModuleSW, HIGH);  // Step 1: turn on relay.
      delay(1);
      digitalWrite(chargeEN, HIGH);  // Step 2: turn on charge enable.
    } else {
      digitalWrite(relayModuleSW, HIGH);
      digitalWrite(chargeEN, HIGH);
    }
    digitalWrite(batterySW, HIGH);
    enablePWM();
  } else {
    // --- Normal Operation ---
    // batterySW should be HIGH when PSU is available.
    if (PSU_Available == 1) {
      digitalWrite(batterySW, HIGH);
    } else {
      digitalWrite(batterySW, LOW);
    }

    // For charging control:
    // When chargeState is 1 (and PSU is available), both chargeEN and relayModuleSW are HIGH.
    if ((chargeState == 1) && (PSU_Available == 1)) {
      digitalWrite(chargeEN, HIGH);
      digitalWrite(relayModuleSW, HIGH);
      enablePWM();
    } else {
      // Otherwise, ensure they are turned off.
      digitalWrite(chargeEN, LOW);
      digitalWrite(relayModuleSW, LOW);
      disablePWM();
    }
  }

  // Update the previous state variables for the next loop iteration.
  prev_PSU_Available = PSU_Available;
  prev_chargeState = chargeState;

  // Continue with fan control
  controlFAN();
}

// Enable PWM on the MOSFET output by configuring Timer1
void enablePWM() {
  if (!PWMEnable) {
    // Set the PWM pin as output.
    pinMode(MosfetPin, OUTPUT);

    // Configure Timer1 for Fast PWM mode (Mode 14) in non-inverting mode.
    TCCR1A = (1 << WGM11) | (1 << COM1A1);
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS10);  // No prescaling

    // Set the TOP value to define the PWM frequency.
    ICR1 = PWMResolution;

    // Initialize duty cycle to 0 (or any desired starting value).
    OCR1A = 0;
    PWMEnable = true;
  }
}

// Disable PWM on the MOSFET output by stopping the timer and forcing the pin LOW.
void disablePWM() {
  if (PWMEnable) {
    // Disable Timer1 PWM by clearing its control registers.
    TCCR1A = 0;
    TCCR1B = 0;

    // Ensure the output pin is set as digital output and drive it LOW.
    pinMode(MosfetPin, OUTPUT);
    digitalWrite(MosfetPin, LOW);
    PWMEnable = false;
  }
}
