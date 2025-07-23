//Main_Page.ino
void mainb0Pop(void *ptr) {
  settingsPAGEvarinit();
  settingsPAGEinit();
}

void mainPage(void) {  //100ms run interval
  int channel_info;    //1000:Evacuation Fan, 0100:Heater, 0010:Egg Turner, 0001:Humidifier
  int ordinal;
  String ordinalnumber;
  static int flipBatterySign = 0;
  static int timermain1 = 0;
  static int timermain2 = 0;
  int am_pm_state;  //0:AM, 1:PM

  timermain1 = (timermain1 + 1) % 20;
  timermain2 = (timermain2 + 1) % 2;

  if (timermain1 == 1) {
    if (gBattery_Available == 1) {
      if (gChargeState == 0) {
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
      } else if (gChargeState == 1) {
        Serial2.print("vis batn,0");
        endNextionCommand();
        Serial2.print("vis batp,1");
        endNextionCommand();
        if (flipBatterySign != 0) {
          Serial2.print("vis batc,0");
          endNextionCommand();
          flipBatterySign = 0;
        } else {
          Serial2.print("vis batc,1");
          endNextionCommand();
          flipBatterySign = 1;
        }
        Serial2.print("vis batpt,1");
        endNextionCommand();
        Serial2.print("vis bat,1");
        endNextionCommand();
      } else if (gPSU_Available == 0) {
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
        } else {
          Serial2.print("vis batp,1");
          endNextionCommand();
          Serial2.print("vis bat,1");
          endNextionCommand();
          flipBatterySign = 1;
        }
      }
      Serial2.print("batpt.txt=");
      Serial2.print("\"");
      Serial2.print("%");
      Serial2.print(gBatteryPercentage);
      Serial2.print("\"");
      endNextionCommand();

      Serial2.print("batp.val=");
      Serial2.print(gBatteryPercentage);
      endNextionCommand();
    } else {
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
  }
  if (timermain2 == 0) {
    if (timeformat == 1) {
      if (sc_hour >= 12) {
        am_pm_state = 1;
        sc_hour = sc_hour - 12;
        if (sc_hour == 0)
          sc_hour = 12;  // Day, 12 PM
      } else {
        am_pm_state = 0;
        if (sc_hour == 0)
          sc_hour = 12;  // Night, 12 AM
      }
      Serial2.print("time.txt=");
      Serial2.print("\"");
      if (sc_hour <= 9) {
        Serial2.print("0");
        Serial2.print(sc_hour);
        Serial2.print(":");
      } else {
        Serial2.print(sc_hour);
        Serial2.print(":");
      }
      if (sc_min <= 9) {
        Serial2.print("0");
        Serial2.print(sc_min);
      } else
        Serial2.print(sc_min);

      if (am_pm_state == 1)  //AM-PM
        Serial2.print(" PM");
      else
        Serial2.print(" AM");

      Serial2.print("\"");
      endNextionCommand();
    } else {
      Serial2.print("time.txt=");
      Serial2.print("\"");
      if (sc_hour <= 9) {
        Serial2.print("0");
        Serial2.print(sc_hour);
        Serial2.print(":");
      } else {
        Serial2.print(sc_hour);
        Serial2.print(":");
      }
      if (sc_min <= 9) {
        Serial2.print("0");
        Serial2.print(sc_min);
      } else
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
      } else
        Serial2.print(sc_month);

      Serial2.print(".");
      if (sc_day <= 9) {
        Serial2.print("0");
        Serial2.print(sc_day);
      } else
        Serial2.print(sc_day);

      Serial2.print(".");
      Serial2.print(sc_year);
    } else {
      Serial2.print("date.txt=");
      Serial2.print("\"");

      if (sc_day <= 9) {
        Serial2.print("0");
        Serial2.print(sc_day);
      } else
        Serial2.print(sc_day);

      Serial2.print(".");

      if (sc_month <= 9) {
        Serial2.print("0");
        Serial2.print(sc_month);
      } else
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
    } else if (lang == 1) {
      if (daycount == 0) {
        Serial2.print(" G\xFC"
                      "n");
      } else {
        Serial2.print(". G\xFC"
                      "n");
      }
    }
    Serial2.print(")");
    Serial2.print("\"");
    endNextionCommand();

    Serial2.print("temp.txt=");
    Serial2.print("\"");
    Serial2.print(temp);
    Serial2.print(" ");
    Serial2.print("\xB0"
                  "C");
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
    if (humidityExtFan == 1)
      channel_info = channel_info + 8;
    if (heater == 1)
      channel_info = channel_info + 4;
    if (eggRotator == 1)
      channel_info = channel_info + 2;
    if (humidifier == 1)
      channel_info = channel_info + 1;

    Serial2.print("va0.val=");
    Serial2.print(channel_info);
    endNextionCommand();
  }
}
