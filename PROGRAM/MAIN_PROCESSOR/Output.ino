//Output.ino

#define resetPin 25

void initializeOutput() {
  pinMode(resetPin, OUTPUT);
  digitalWrite(resetPin, LOW);
}

void outputControl() {
  if (processorReset) {
    processorReset = 0;
    digitalWrite(resetPin, HIGH);
    delay(10);
  } else {
    digitalWrite(resetPin, LOW);
  }
}
