long timermain;
long mainrefreshtimer;
byte am_pm_activate; //0:AM, 1:PM
byte sc_hour;
byte sc_min;
int sc_year;
byte sc_month;
byte sc_day;
byte channel_info; //1000:Evacuation Fan, 0100:Heater, 0010:Egg Turner, 0001:Humidifier
int ordinal;
String ordinalnumber;
String pcbsurum = "V4.4";
String yazilimsurum = "V4BETA";

void mainpage(void) {
  timermain = millis() / 200;
  DateTime now = rtc.now();
  sc_year = now.year();
  sc_month = now.month();
  sc_day = now.day();
  sc_hour = now.hour();
  sc_min = now.minute();
  if ((timermain - mainrefreshtimer) >= 1) {
    if (batstate == 1) {
      if (chargestate == 0) {
        Serial2.print("va1.val=0");
        endNextionCommand();
      }
      else if (chargestate == 1) {
        Serial2.print("va1.val=1");
        endNextionCommand();
      }
      else if (chargestate == 2) {
        Serial2.print("va1.val=2");
        endNextionCommand();
      }
    }
    else {
      Serial2.print("va1.val=3");
      endNextionCommand();
    }

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
    if (lang == 1) {
      if (daycount == 0)
        Serial2.print(" Gün");
      else
        Serial2.print(". Gün");
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

    Serial2.print("batpt.txt=");
    Serial2.print("\"");
    Serial2.print("%");
    Serial2.print(batpercentage);
    Serial2.print("\"");
    endNextionCommand();

    Serial2.print("batp.val=");
    Serial2.print(batpercentage);
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
