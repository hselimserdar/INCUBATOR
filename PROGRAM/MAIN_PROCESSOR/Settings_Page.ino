//Settings_Page.ino
float tempTempUpperLimit;
float tempTempLowerLimit;
int tempHumidityUpperLimit;
int tempHumidityLowerLimit;
int tempEggRotateDuration;
int tempEggRotateInterval;
int tempEggRotateStoppingDay;

void settingsPAGEinit() {
  Serial2.print("t10.txt=");
  Serial2.print("\"");
  Serial2.print(tempTempUpperLimit);
  Serial2.print(" "
                "\xB0"
                "C");
  Serial2.print("\"");
  endNextionCommand();
  Serial2.print("t11.txt=");
  Serial2.print("\"");
  Serial2.print(tempTempLowerLimit);
  Serial2.print(" "
                "\xB0"
                "C");
  Serial2.print("\"");
  endNextionCommand();
  Serial2.print("t12.txt=");
  Serial2.print("\"");
  Serial2.print("%");
  Serial2.print(tempHumidityUpperLimit);
  Serial2.print("\"");
  endNextionCommand();
  Serial2.print("t13.txt=");
  Serial2.print("\"");
  Serial2.print("%");
  Serial2.print(tempHumidityLowerLimit);
  Serial2.print("\"");
  endNextionCommand();
  Serial2.print("t14.txt=");
  Serial2.print("\"");
  Serial2.print(tempEggRotateDuration);
  if (lang == 0) {
    Serial2.print(" sec");
  } else {
    Serial2.print(" sn");
  }
  Serial2.print("\"");
  endNextionCommand();
  Serial2.print("t15.txt=");
  Serial2.print("\"");
  Serial2.print(tempEggRotateInterval);
  if (lang == 0) {
    Serial2.print(" min");
  } else {
    Serial2.print(" dk");
  }
  Serial2.print("\"");
  endNextionCommand();
  Serial2.print("t16.txt=");
  Serial2.print("\"");
  Serial2.print(tempEggRotateStoppingDay);
  if (lang == 0) {
    int ordin;
    String ordinnumber;
    ordin = tempEggRotateStoppingDay % 10;
    if (tempEggRotateStoppingDay == 1)
      ordinnumber = "st";
    else if (tempEggRotateStoppingDay == 2)
      ordinnumber = "nd";
    else if (tempEggRotateStoppingDay == 3)
      ordinnumber = "rd";
    else if (tempEggRotateStoppingDay > 20 && ordin == 1)
      ordinnumber = "st";
    else if (tempEggRotateStoppingDay > 20 && ordin == 2)
      ordinnumber = "nd";
    else if (tempEggRotateStoppingDay > 20 && ordin == 3)
      ordinnumber = "rd";
    else
      ordinnumber = "th";
    Serial2.print(ordinnumber);
  } else if (lang == 1) {
    Serial2.print(". G\xFC");
    Serial2.print("n");
  }
  Serial2.print("\"");
  endNextionCommand();
  if (lang == 0) {
    Serial2.print("settings.txt=");
    Serial2.print("\"");
    Serial2.print("BASIC SETTINGS");
    Serial2.print("\"");
    endNextionCommand();
    Serial2.print("t0.txt=");
    Serial2.print("\"");
    Serial2.print("Temperature");
    Serial2.print("\"");
    endNextionCommand();
    Serial2.print("t1.txt=");
    Serial2.print("\"");
    Serial2.print("Upper Limit");
    Serial2.print("\"");
    endNextionCommand();
    Serial2.print("t2.txt=");
    Serial2.print("\"");
    Serial2.print("Lower Limit");
    Serial2.print("\"");
    endNextionCommand();
    Serial2.print("t3.txt=");
    Serial2.print("\"");
    Serial2.print("Lower Limit");
    Serial2.print("\"");
    endNextionCommand();
    Serial2.print("t4.txt=");
    Serial2.print("\"");
    Serial2.print("Upper Limit");
    Serial2.print("\"");
    endNextionCommand();
    Serial2.print("t5.txt=");
    Serial2.print("\"");
    Serial2.print("Humidity");
    Serial2.print("\"");
    endNextionCommand();
    Serial2.print("t6.txt=");
    Serial2.print("\"");
    Serial2.print("Duration");
    Serial2.print("\"");
    endNextionCommand();
    Serial2.print("t7.txt=");
    Serial2.print("\"");
    Serial2.print("Interval");
    Serial2.print("\"");
    endNextionCommand();
    Serial2.print("t8.txt=");
    Serial2.print("\"");
    Serial2.print("Stopping Day");
    Serial2.print("\"");
    endNextionCommand();
    Serial2.print("t9.txt=");
    Serial2.print("\"");
    Serial2.print("Egg Rotator Settings");
    Serial2.print("\"");
    endNextionCommand();
    Serial2.print("b16.txt=");
    Serial2.print("\"");
    Serial2.print("REBOOT");
    Serial2.print("\"");
    endNextionCommand();
    Serial2.print("b17.txt=");
    Serial2.print("\"");
    Serial2.print("OTHER SETTINGS");
    Serial2.print("\"");
    endNextionCommand();
    Serial2.print("b18.txt=");
    Serial2.print("\"");
    Serial2.print("DATE");
    Serial2.print("\"");
    endNextionCommand();
    Serial2.print("b19.txt=");
    Serial2.print("\"");
    Serial2.print("TIME");
    Serial2.print("\"");
    endNextionCommand();
  } else if (lang == 1) {
    Serial2.print("settings.txt=\"BAS");
    Serial2.print("\xDD");
    Serial2.print("T AYARLAR\"");
    endNextionCommand();  //BASİT AYARLAR
    Serial2.print("t0.txt=\"SICAKLIK\"");
    endNextionCommand();  //SICAKLIK
    Serial2.print("t1.txt=\"");
    Serial2.print("\xDC");
    Serial2.print("ST SINIR\"");
    endNextionCommand();  //ÜST SINIR
    Serial2.print("t2.txt=\"ALT SINIR\"");
    endNextionCommand();  //ALT SINIR
    Serial2.print("t3.txt=\"ALT SINIR\"");
    endNextionCommand();  //ALT SINIR
    Serial2.print("t4.txt=\"");
    Serial2.print("\xDC");
    Serial2.print("ST SINIR\"");
    endNextionCommand();  //ÜST SINIR
    Serial2.print("t5.txt=\"NEM\"");
    endNextionCommand();  //NEM
    Serial2.print("t6.txt=\"S");
    Serial2.print("\xDC");
    Serial2.print("RE\"");
    endNextionCommand();  //SÜRE
    Serial2.print("t7.txt=\"ARALIK\"");
    endNextionCommand();  //ARALIK
    Serial2.print("t8.txt=\"DURDURMA G");
    Serial2.print("\xDC");
    Serial2.print("N");
    Serial2.print("\xDC");
    Serial2.print("\"");
    endNextionCommand();  //DURDUMA GÜNÜ
    Serial2.print("t9.txt=\"YUMURTA ");
    Serial2.print("\xC7");
    Serial2.print("EV");
    Serial2.print("\xDD");
    Serial2.print("R");
    Serial2.print("\xDD");
    Serial2.print("C");
    Serial2.print("\xDD");
    Serial2.print(" AYARLARI\"");
    endNextionCommand();  //YUMURTA ÇEVİRİCİ AYARLARI
    Serial2.print("b16.txt=\"YEN");
    Serial2.print("\xDD");
    Serial2.print("DEN BA");
    Serial2.print("\xDE");
    Serial2.print("LAT\"");
    endNextionCommand();  //YENİDEN BAŞLAT
    Serial2.print("b17.txt=\"D");
    Serial2.print("\xDD");
    Serial2.print("\xD0");
    Serial2.print("ER AYARLAR\"");
    endNextionCommand();  //DİĞER AYARLAR
    Serial2.print("b18.txt=\"TAR");
    Serial2.print("\xDD");
    Serial2.print("H\"");
    endNextionCommand();  //TARİH
    Serial2.print("b19.txt=\"ZAMAN\"");
    endNextionCommand();  //ZAMAN
  }
}

void settingsPAGEvarinit() {
  tempTempUpperLimit = TempUpperLimit;
  tempTempLowerLimit = TempLowerLimit;
  tempHumidityUpperLimit = HumidityUpperLimit;
  tempHumidityLowerLimit = HumidityLowerLimit;
  tempEggRotateDuration = EggRotateDuration;
  tempEggRotateInterval = EggRotateInterval;
  tempEggRotateStoppingDay = EggRotateStoppingDay;
  page = 2;
  Serial2.print("page 2");
  endNextionCommand();
}

void setb0Push(void *ptr) {
  tempTempUpperLimit = tempTempUpperLimit + 0.1;
  if (tempTempUpperLimit > 40) { tempTempUpperLimit = 40; }
  Serial2.print("t10.txt=");
  Serial2.print("\"");
  Serial2.print(tempTempUpperLimit);
  Serial2.print(" C");
  Serial2.print("\"");
  endNextionCommand();
}

void setb1Push(void *ptr) {
  tempTempUpperLimit = tempTempUpperLimit - 0.1;
  if (tempTempUpperLimit < (tempTempLowerLimit + 0.1)) { tempTempUpperLimit = (tempTempLowerLimit + 0.1); }
  Serial2.print("t10.txt=");
  Serial2.print("\"");
  Serial2.print(tempTempUpperLimit);
  Serial2.print(" C");
  Serial2.print("\"");
  endNextionCommand();
}

void setb7Push(void *ptr) {
  tempTempLowerLimit = tempTempLowerLimit + 0.1;
  if (tempTempLowerLimit > (tempTempUpperLimit - 0.1)) {
    tempTempLowerLimit = (tempTempUpperLimit - 0.1);
  }
  Serial2.print("t11.txt=");
  Serial2.print("\"");
  Serial2.print(tempTempLowerLimit);
  Serial2.print(" C");
  Serial2.print("\"");
  endNextionCommand();
}

void setb6Push(void *ptr) {
  tempTempLowerLimit = tempTempLowerLimit - 0.1;
  if (tempTempLowerLimit < 30) { tempTempLowerLimit = 30; }
  Serial2.print("t11.txt=");
  Serial2.print("\"");
  Serial2.print(tempTempLowerLimit);
  Serial2.print(" C");
  Serial2.print("\"");
  endNextionCommand();
}

void setb3Push(void *ptr) {
  tempHumidityUpperLimit = tempHumidityUpperLimit + 5;
  if (tempHumidityUpperLimit > 95) { tempHumidityUpperLimit = 95; }
  Serial2.print("t12.txt=");
  Serial2.print("\"");
  Serial2.print("%");
  Serial2.print(tempHumidityUpperLimit);
  Serial2.print("\"");
  endNextionCommand();
}

void setb2Push(void *ptr) {
  tempHumidityUpperLimit = tempHumidityUpperLimit - 5;
  if (tempHumidityUpperLimit < (tempHumidityLowerLimit + 5)) { tempHumidityUpperLimit = (tempHumidityLowerLimit + 5); }
  Serial2.print("t12.txt=");
  Serial2.print("\"");
  Serial2.print("%");
  Serial2.print(tempHumidityUpperLimit);
  Serial2.print("\"");
  endNextionCommand();
}

void setb9Push(void *ptr) {
  tempHumidityLowerLimit = tempHumidityLowerLimit + 5;
  if (tempHumidityLowerLimit > (tempHumidityUpperLimit - 5)) { tempHumidityLowerLimit = (tempHumidityUpperLimit - 5); }
  Serial2.print("t13.txt=");
  Serial2.print("\"");
  Serial2.print("%");
  Serial2.print(tempHumidityLowerLimit);
  Serial2.print("\"");
  endNextionCommand();
}

void setb8Push(void *ptr) {
  tempHumidityLowerLimit = tempHumidityLowerLimit - 5;
  if (tempHumidityLowerLimit < 5) { tempHumidityLowerLimit = 5; }
  Serial2.print("t13.txt=");
  Serial2.print("\"");
  Serial2.print("%");
  Serial2.print(tempHumidityLowerLimit);
  Serial2.print("\"");
  endNextionCommand();
}

void setb5Push(void *ptr) {
  tempEggRotateDuration = tempEggRotateDuration + 1;
  if (tempEggRotateDuration > 99) { tempEggRotateDuration = 99; }
  Serial2.print("t14.txt=");
  Serial2.print("\"");
  Serial2.print(tempEggRotateDuration);
  if (lang == 0) {
    Serial2.print(" sec");
  } else {
    Serial2.print(" sn");
  }
  Serial2.print("\"");
  endNextionCommand();
}

void setb4Push(void *ptr) {
  tempEggRotateDuration = tempEggRotateDuration - 1;
  if (tempEggRotateDuration < 1) { tempEggRotateDuration = 1; }
  Serial2.print("t14.txt=");
  Serial2.print("\"");
  Serial2.print(tempEggRotateDuration);
  if (lang == 0) {
    Serial2.print(" sec");
  } else {
    Serial2.print(" sn");
  }
  Serial2.print("\"");
  endNextionCommand();
}

void setb10Push(void *ptr) {
  tempEggRotateInterval = tempEggRotateInterval + 5;
  if (tempEggRotateInterval > 300) { tempEggRotateInterval = 300; }
  Serial2.print("t15.txt=");
  Serial2.print("\"");
  Serial2.print(tempEggRotateInterval);
  if (lang == 0) {
    Serial2.print(" min");
  } else {
    Serial2.print(" dk");
  }
  Serial2.print("\"");
  endNextionCommand();
}

void setb11Push(void *ptr) {
  tempEggRotateInterval = tempEggRotateInterval - 5;
  if (tempEggRotateInterval < 5) { tempEggRotateInterval = 5; }
  Serial2.print("t15.txt=");
  Serial2.print("\"");
  Serial2.print(tempEggRotateInterval);
  if (lang == 0) {
    Serial2.print(" min");
  } else {
    Serial2.print(" dk");
  }
  Serial2.print("\"");
  endNextionCommand();
}

void setb13Push(void *ptr) {
  tempEggRotateStoppingDay = tempEggRotateStoppingDay + 1;
  if (tempEggRotateStoppingDay > 50) { tempEggRotateStoppingDay = 50; }
  Serial2.print("t16.txt=");
  Serial2.print("\"");
  Serial2.print(tempEggRotateStoppingDay);
  if (lang == 0) {
    String ordinnumber;
    int ordin = tempEggRotateStoppingDay % 10;
    if (tempEggRotateStoppingDay == 1)
      ordinnumber = "st";
    else if (tempEggRotateStoppingDay == 2)
      ordinnumber = "nd";
    else if (tempEggRotateStoppingDay == 3)
      ordinnumber = "rd";
    else if (tempEggRotateStoppingDay > 20 && ordin == 1)
      ordinnumber = "st";
    else if (tempEggRotateStoppingDay > 20 && ordin == 2)
      ordinnumber = "nd";
    else if (tempEggRotateStoppingDay > 20 && ordin == 3)
      ordinnumber = "rd";
    else
      ordinnumber = "th";
    Serial2.print(ordinnumber);
  } else if (lang == 1) {
    Serial2.print(". G\xFC");
    Serial2.print("n");
  }
  Serial2.print("\"");
  endNextionCommand();
}

void setb12Push(void *ptr) {
  tempEggRotateStoppingDay = tempEggRotateStoppingDay - 1;
  if (tempEggRotateStoppingDay < 1) { tempEggRotateStoppingDay = 1; }
  Serial2.print("t16.txt=");
  Serial2.print("\"");
  Serial2.print(tempEggRotateStoppingDay);
  if (lang == 0) {
    String ordinnumber;
    int ordin = tempEggRotateStoppingDay % 10;
    if (tempEggRotateStoppingDay == 1)
      ordinnumber = "st";
    else if (tempEggRotateStoppingDay == 2)
      ordinnumber = "nd";
    else if (tempEggRotateStoppingDay == 3)
      ordinnumber = "rd";
    else if (tempEggRotateStoppingDay > 20 && ordin == 1)
      ordinnumber = "st";
    else if (tempEggRotateStoppingDay > 20 && ordin == 2)
      ordinnumber = "nd";
    else if (tempEggRotateStoppingDay > 20 && ordin == 3)
      ordinnumber = "rd";
    else
      ordinnumber = "th";
    Serial2.print(ordinnumber);
  } else if (lang == 1) {
    Serial2.print(". G\xFC");
    Serial2.print("n");
  }
  Serial2.print("\"");
  endNextionCommand();
}

void setb14Pop(void *ptr) {
  page = 1;
  Serial2.print("page 1");
  endNextionCommand();
}

void setb15Pop(void *ptr) {
  TempUpperLimit = tempTempUpperLimit;
  TempLowerLimit = tempTempLowerLimit;
  HumidityUpperLimit = tempHumidityUpperLimit;
  HumidityLowerLimit = tempHumidityLowerLimit;
  EggRotateDuration = tempEggRotateDuration;
  EggRotateInterval = tempEggRotateInterval;
  EggRotateStoppingDay = tempEggRotateStoppingDay;
  settingsEEPROMSave();
  page = 1;
  Serial2.print("page 1");
  endNextionCommand();
}

void setb16Pop(void *ptr) {
  processorReset = 1;
}

void setb17Pop(void *ptr) {
  othersettingsPAGEvarinit();
  othersettingsPAGEinit();
}

void setb18Pop(void *ptr) {
  datePAGEvarinit();
  datePAGEinit();
}
void setb19Pop(void *ptr) {
  timePAGEvarinit();
  timePAGEinit();
}
