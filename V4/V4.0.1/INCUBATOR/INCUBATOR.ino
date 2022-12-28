/*H.Selim Serdar Kuluçka Makinesi Projesi Program Sürüm V4.0.1*/

#include <MapFloat.h>
#include <Wire.h>
#include "Adafruit_SHT31.h"
#include <Adafruit_MCP23X17.h>
#include "RTClib.h"

#define RXD2 16
#define TXD2 17

Adafruit_MCP23X17 mcp;
Adafruit_SHT31 sht31 = Adafruit_SHT31();
RTC_DS3231 rtc;

String pcbsurum = "V4.4";
String yazilimsurum = "V4.0.1";
float temp;
byte humidity;
byte waterlevel;
byte processorReset = 0; //Set 1 to reset processor
byte chargestate = 1; //0:None, 1:Charging, 2:Discharging
byte batstate = 1; //0:Battery do not exist, 1:Battery exist
byte powerstate; //0:DC, 1:AC
byte batpercentage = 15; //Battery charge percentage
byte timeformat; //0:24-hour, 1:12-hour
byte dateformat; //0:dd/mm/yy, 1:mm/dd/yy
byte daycount = 3; //Every 24hr elapsed from last timer reset
byte lang = 1; //Language selection 0:English, 1:Turkish
byte heater = 1; //0:off, 1:on
byte humidifier = 1; //0:off, 1:on
byte evacuationfan = 1; //Moisture evacuation fan 0:off, 1:on
byte violtiltmotor = 0; //Viol tilting/egg turning motor 0:off,1:on
byte pcbfan1 = 0; //0:off, 1:on
byte pcbfan2 = 0; //0:off, 1:on
byte chargingmosfetpwm = 80; //Charging mosfet duty cycle. Values between 1-255
byte cabinfanpower = 80; //Cabin fan duty cycle. Values between 80-255
byte evacuationfanpower = 80; //Exhaust fan duty cycle. Values between 80-255
byte pcbfan1power = 80; //PCB fan 1 duty cycle. Values between 80-255
byte pcbfan2power = 80; //PCB fan 2 duty cycle. Values between 80-255

void setup() {
  delay(1000);
  Serial2.begin(921600, SERIAL_8N1, RXD2, TXD2);
  sht31.begin(0x44);
  rtc.begin();
  mcp.begin_I2C();
  pinSetup();
  zeroouttempgraph();
  zeroouthumgraph();
  mcp.digitalWrite(5, LOW);
}

void loop() {
  mainpage();
  tempgraphdraw();
  humgraphdraw();
  inputControl();
  outputControl();
}

void endNextionCommand()
{
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
}
