/*H.Selim Serdar Kuluçka Makinesi Projesi Program Sürüm V4BETA*/

#include <MapFloat.h>
#include <Wire.h>
#include "Adafruit_SHT31.h"
#include "RTClib.h"

#define RXD2 16
#define TXD2 17

Adafruit_SHT31 sht31 = Adafruit_SHT31();
RTC_DS3231 rtc;

float temp;
byte humidity;
byte waterlevel;
byte chargestate = 1; //0:None, 1:Charging, 2:Discharging
byte batstate = 1; //0:Battery do not exist, 1:Battery exist
byte powerstate = 0; //0:DC, 1:AC
byte batpercentage = 15; //Battery charge percentage
byte timeformat; //0:24-hour, 1:12-hour
byte dateformat; //0:dd/mm/yy, 1:mm/dd/yy
byte daycount = 3; //Every 24hr elapsed from last timer reset
byte lang = 1; //Language selection 0:English, 1:Turkish
byte heater = 0; //0:off, 1:on
byte humidifier = 1; //0:off, 1:on
byte evacuationfan = 1; //Moisture evacuation fan 0:off, 1:on
byte violtiltmotor = 0; //Viol tilting/egg turning motor 0:off,1:on
byte fanpower; //Exhaust fan duty cycle. Values between 30-100
unsigned long timePassedSensorRead;

void setup() {
  delay(500);
  Serial2.begin(921600, SERIAL_8N1, RXD2, TXD2);
  sht31.begin(0x44);
  rtc.begin();
  zeroouttempgraph();
  zeroouthumgraph();
}

void loop() {
  sensorRead(5);
  mainpage();
  tempgraphdraw();
  humgraphdraw();
}

void sensorRead(byte readingThreshold) {
  if ((millis() - timePassedSensorRead) >= (readingThreshold * 100)) {
    temp = sht31.readTemperature();
    humidity = sht31.readHumidity();
    timePassedSensorRead = millis();
  }
}

void endNextionCommand()
{
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
}
