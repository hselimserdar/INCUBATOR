unsigned long timermain;
unsigned long timermain2;
unsigned long mainrefreshtimer;
unsigned long mainrefreshtimer2;
byte am_pm_activate; //0:AM, 1:PM
byte sc_hour;
byte sc_min;
int sc_year;
byte sc_month;
byte sc_day;
byte channel_info; //1000:Evacuation Fan, 0100:Heater, 0010:Egg Turner, 0001:Humidifier
int ordinal;
String ordinalnumber;
byte flipBatterySign = 0;

// setting PWM properties
const int freq = 5000;
const int resolution = 8;
const int channel2 = 1;
const int channel3 = 2;
const int channel4 = 3;
const int channel5 = 4;

int rtcLineAnalog;
int fiveVLineAnalog;
int twelveVLineAnalog;
int fifteenVLineAnalog;
int twentyFourVLineAnalog;

//Make a day counter

void inputControl(void) { //Write all of the inputs
  rtcLineAnalog = analogRead(34);
  fiveVLineAnalog = analogRead(27);
  twelveVLineAnalog = analogRead(26);
  fifteenVLineAnalog = analogRead(35);
  twentyFourVLineAnalog = analogRead(33);
  powerstate = mcp.digitalRead(8);
}

void outputControl(void) { //Write all of the outputs
  mcp.digitalWrite(0, LOW);
  mcp.digitalWrite(2, LOW);
  mcp.digitalWrite(3, LOW);
  mcp.digitalWrite(4, LOW);
  mcp.digitalWrite(5, LOW);
  mcp.digitalWrite(6, LOW);
  if (heater == 1) {
    if (powerstate == 0) {
      digitalWrite(4, HIGH);
      mcp.digitalWrite(1, LOW);
    }
    else {
      mcp.digitalWrite(1, HIGH);
      digitalWrite(4, LOW);
    }
  }
  else {
    digitalWrite(4, LOW);
    mcp.digitalWrite(1, LOW);
  }
  if (pcbfan1 == 1) {
    ledcWrite(channel2, pcbfan1power);
  }
  else {
    ledcWrite(channel2, 0);
  }
  if (pcbfan2 == 1) {
    ledcWrite(channel3, pcbfan2power);
  }
  else {
    ledcWrite(channel3, 0);
  }
  ledcWrite(channel4, (255 - cabinfanpower));
  if (chargestate == 1) {
    ledcWrite(channel5, chargingmosfetpwm);
  }
  else {
    ledcWrite(channel5, 0);
  }
}

void pinSetup(void) {
  mcp.pinMode(0, OUTPUT); //Battery charge enable
  mcp.pinMode(1, OUTPUT); //AC Heater
  mcp.pinMode(2, OUTPUT); //Egg turner
  mcp.pinMode(3, OUTPUT); //Humidifier
  mcp.pinMode(4, OUTPUT); //Evacuation fan
  mcp.pinMode(5, OUTPUT); //Buzzer
  mcp.pinMode(6, OUTPUT); //15V Enable for battery charging
  mcp.pinMode(8, INPUT); //AC sense pin
  mcp.pinMode(9, INPUT); //Water level 1
  mcp.pinMode(10, INPUT); //Water level 2
  mcp.pinMode(11, INPUT); //Water level 3
  mcp.pinMode(12, INPUT); //Water level 4
  mcp.pinMode(13, INPUT); //Water level 5
  mcp.pinMode(14, INPUT); //Water level 6
  ledcSetup(channel2, freq, resolution); //PCB Fan 1
  ledcAttachPin(5, channel2);
  ledcSetup(channel3, freq, resolution); //PCB Fan 2
  ledcAttachPin(18, channel3);
  ledcSetup(channel4, freq, resolution); //Cabin Fan
  ledcAttachPin(19, channel4);
  ledcSetup(channel5, freq, resolution); //Battery harging mosfet control signal PWM
  ledcAttachPin(23, channel5);
  pinMode(4, OUTPUT); //24V Heater
  pinMode(25, OUTPUT); //System Reset
  pinMode(26, INPUT); //12V_line
  pinMode(27, INPUT); //5V_line
  pinMode(33, INPUT); //24V_line
  pinMode(34, INPUT); //RTCbat
  pinMode(35, INPUT); //15V_line
}

void mainpage(void) {
  timermain = millis() / 200;
  timermain2 = millis() / 100;
  DateTime now = rtc.now();
  sc_year = now.year();
  sc_month = now.month();
  sc_day = now.day();
  sc_hour = now.hour();
  sc_min = now.minute();
  if ((timermain2 - mainrefreshtimer2) >= 10) {
    if (batstate == 1) {
      if (chargestate == 0) {
        Serial2.print("vis batn,0");
        endNextionCommand();
        Serial2.print("vis batc,0");
        endNextionCommand();
        Serial2.print("vis batp,1");
        endNextionCommand();
        Serial2.print("vis batpt,1");
        endNextionCommand();
        Serial2.print("vis bat,1");
        endNextionCommand();
        batteryPercentageSerial();
      }
      else if (chargestate == 1) {
        Serial2.print("vis batn,0");
        endNextionCommand();
        Serial2.print("vis batp,1");
        endNextionCommand();
        if (flipBatterySign != 0) {
          Serial2.print("vis batc,0");
          endNextionCommand();
          flipBatterySign = 0;
        }
        else {
          Serial2.print("vis batc,1");
          endNextionCommand();
          flipBatterySign = 1;
        }
        Serial2.print("vis batpt,1");
        endNextionCommand();
        Serial2.print("vis bat,1");
        endNextionCommand();
        batteryPercentageSerial();
      }
      else if (chargestate == 2) {
        Serial2.print("vis batc,0");
        endNextionCommand();
        Serial2.print("vis batn,0");
        endNextionCommand();
        Serial2.print("vis batpt,1");
        endNextionCommand();
        if (flipBatterySign != 0) {
          Serial2.print("vis batp,0");
          endNextionCommand();
          Serial2.print("vis bat,1");
          endNextionCommand();
          flipBatterySign = 0;
        }
        else {
          Serial2.print("vis batp,1");
          endNextionCommand();
          Serial2.print("vis bat,1");
          endNextionCommand();
          flipBatterySign = 1;
        }
        batteryPercentageSerial();
      }
    }
    else {
      Serial2.print("vis batc,0");
      endNextionCommand();
      Serial2.print("vis batp,0");
      endNextionCommand();
      Serial2.print("vis batpt,0");
      endNextionCommand();
      Serial2.print("vis batn,1");
      endNextionCommand();
      Serial2.print("vis bat,1");
      endNextionCommand();
    }

    mainrefreshtimer2 = timermain2;
  }
  if ((timermain - mainrefreshtimer) >= 2) {
    temp = sht31.readTemperature();
    humidity = sht31.readHumidity();
    if (timeformat == 1) {
      if (sc_hour >= 12) {
        am_pm_activate = 1;
        sc_hour = sc_hour - 12;
        if (sc_hour == 0)
          sc_hour = 12; // Day, 12 PM
      }
      else {
        am_pm_activate = 0;
        if (sc_hour == 0)
          sc_hour = 12; // Night, 12 AM
      }
      Serial2.print("time.txt=");
      Serial2.print("\"");
      if (sc_hour <= 9) {
        Serial2.print("0");
        Serial2.print(sc_hour);
        Serial2.print(":");
      }
      else {
        Serial2.print(sc_hour);
        Serial2.print(":");
      }
      if (sc_min <= 9) {
        Serial2.print("0");
        Serial2.print(sc_min);
      }
      else
        Serial2.print(sc_min);

      if (am_pm_activate == 1) //AM-PM
        Serial2.print(" PM");
      else
        Serial2.print(" AM");

      Serial2.print("\"");
      endNextionCommand();
    }
    else {
      Serial2.print("time.txt=");
      Serial2.print("\"");
      if (sc_hour <= 9) {
        Serial2.print("0");
        Serial2.print(sc_hour);
        Serial2.print(":");
      }
      else {
        Serial2.print(sc_hour);
        Serial2.print(":");
      }
      if (sc_min <= 9) {
        Serial2.print("0");
        Serial2.print(sc_min);
      }
      else
        Serial2.print(sc_min);

      Serial2.print("\"");
      endNextionCommand();
    }

    if (dateformat == 1) {
      Serial2.print("date.txt=");
      Serial2.print("\"");
      if (sc_month <= 9) {
        Serial2.print("0");
        Serial2.print(sc_month);
      }
      else
        Serial2.print(sc_month);

      Serial2.print(".");
      if (sc_day <= 9) {
        Serial2.print("0");
        Serial2.print(sc_day);
      }
      else
        Serial2.print(sc_day);

      Serial2.print(".");
      Serial2.print(sc_year);
    }
    else {
      Serial2.print("date.txt=");
      Serial2.print("\"");

      if (sc_day <= 9) {
        Serial2.print("0");
        Serial2.print(sc_day);
      }
      else
        Serial2.print(sc_day);

      Serial2.print(".");

      if (sc_month <= 9) {
        Serial2.print("0");
        Serial2.print(sc_month);
      }
      else
        Serial2.print(sc_month);

      Serial2.print(".");
      Serial2.print(sc_year);
    }
    Serial2.print(" (");
    Serial2.print(daycount);
    if (lang == 0) {
      ordinal = daycount % 10;
      if (daycount == 0)
        ordinalnumber = " Day";
      else if (daycount == 1)
        ordinalnumber = "st Day";
      else if (daycount == 2)
        ordinalnumber = "nd Day";
      else if (daycount == 3)
        ordinalnumber = "rd Day";
      else if (daycount > 20 && ordinal == 1)
        ordinalnumber = "st Day";
      else if (daycount > 20 && ordinal == 2)
        ordinalnumber = "nd Day";
      else if (daycount > 20 && ordinal == 3)
        ordinalnumber = "rd Day";
      else
        ordinalnumber = "th Day";
      Serial2.print(ordinalnumber);
    }
    else if (lang == 1) {
      if (daycount == 0) {
        Serial2.print(" G\xFC""n");
      }
      else {
        Serial2.print(". G\xFC""n");
      }
      Serial2.print(")");
      Serial2.print("\"");
      endNextionCommand();

      Serial2.print("temp.txt=");
      Serial2.print("\"");
      Serial2.print(temp);
      Serial2.print(" ");
      Serial2.print("\xB0""C");
      Serial2.print("\"");
      endNextionCommand();

      Serial2.print("humid.txt=");
      Serial2.print("\"");
      Serial2.print(humidity);
      Serial2.print(" %");
      Serial2.print("\"");
      endNextionCommand();

      Serial2.print("wlvl.val=");
      Serial2.print(waterlevel);
      endNextionCommand();

      channel_info = 0;
      if (evacuationfan == 1)
        channel_info = channel_info + 8;
      if (heater == 1)
        channel_info = channel_info + 4;
      if (violtiltmotor == 1)
        channel_info = channel_info + 2;
      if (humidifier == 1)
        channel_info = channel_info + 1;

      Serial2.print("va0.val=");
      Serial2.print(channel_info);
      endNextionCommand();
      mainrefreshtimer = timermain;
    }
  }
}

void batteryPercentageSerial(void) {
  Serial2.print("batpt.txt=");
  Serial2.print("\"");
  Serial2.print("%");
  Serial2.print(batpercentage);
  Serial2.print("\"");
  endNextionCommand();

  Serial2.print("batp.val=");
  Serial2.print(batpercentage);
  endNextionCommand();
}
