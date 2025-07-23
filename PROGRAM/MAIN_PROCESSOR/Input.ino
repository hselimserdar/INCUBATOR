//Input.ino
Adafruit_SHT31 sht31 = Adafruit_SHT31();

unsigned long inputTimer = 0;
int inputReadInterval = 500;  //500ms internal between input and sensor readings

void initializeInput() {
  sht31.begin(0x44);
}

void inputControl() {
  unsigned long tmr = millis();
  if ((tmr - inputTimer) >= inputReadInterval) {
    temp = sht31.readTemperature();
    humidity = sht31.readHumidity();
    inputTimer = tmr;
  }
}
