//RTC.ino
RTC_DS3231 rtc;

byte sc_hour;
byte sc_min;
int sc_year;
byte sc_month;
byte sc_day;

unsigned long currentTime;
DateTime now;
DateTime lastSaved;

bool RTCBatLow = 0;

unsigned long RTCTimer = 0;
int RTCInterval = 500;  //500ms internal between RTC readings

void RTCinit() {
  rtc.begin();
  RTCBatLow = rtc.lostPower();
}

void RTCcontroller() {
  unsigned long tmr = millis();
  if ((tmr - RTCTimer) >= RTCInterval) {
    now = rtc.now();
    sc_year = now.year();
    sc_month = now.month();
    sc_day = now.day();
    sc_hour = now.hour();
    sc_min = now.minute();
    dayCounterCalc();
    RTCTimer = tmr;
  }
}

void resetDayCounter() {
  // Update the reset time.
  resetTimestamp = currentTime;
  writeResetTime(resetTimestamp);
  daycount = 0;  // Reset daycount to 0 immediately after the reset.
}

void dayCounterCalc() {
  currentTime = now.unixtime();

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

  unsigned long diff = currentTime - resetTimestamp;
  daycount = diff / 86400;  // 86400 seconds per day
}
