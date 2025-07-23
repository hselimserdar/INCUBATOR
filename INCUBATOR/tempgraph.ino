byte sp[95];
float sTp[95];
byte tempdif = 0;
byte calTemp = 0;
byte TempG;
byte tempgraph;
unsigned long timertemp;
unsigned long tempgdrawtimer;

void tempgraphdraw(void) {
  timertemp = millis() / 200;
  if ((timertemp - tempgdrawtimer) >= 5) {
    if (temp < 20.10) {
      if (temp < 15) {
        tempgraph = 1;
      }
      else if (temp > 20) {
        tempgraph = 100;
      }
      else {
        tempgraph = mapFloat(temp, 15.00, 20.00, 1, 100);
      }
      if (tempdif != 1) {
        calTemp = 1;
        calibrateTempGraph();
        tempdif = 1;
      }
    }
    else if (temp < 25.1) {
      if (temp < 20) {
        tempgraph = 1;
      }
      else if (temp > 25) {
        tempgraph = 100;
      }
      else {
        tempgraph = mapFloat(temp, 20.00, 25.00, 1, 100);
      }
      if (tempdif != 2) {
        calTemp = 2;
        calibrateTempGraph();
        tempdif = 2;
      }
    }
    else if (temp < 30.1) {
      if (temp < 25) {
        tempgraph = 1;
      }
      else if (temp > 30) {
        tempgraph = 100;
      }
      else {
        tempgraph = mapFloat(temp, 25.00, 30.00, 1, 100);
      }
      if (tempdif != 3) {
        calTemp = 3;
        calibrateTempGraph();
        tempdif = 3;
      }
    }
    else if (temp < 35.1) {
      if (temp < 30) {
        tempgraph = 1;
      }
      else if (temp > 35) {
        tempgraph = 100;
      }
      else {
        tempgraph = mapFloat(temp, 30.00, 35.00, 1, 100);
      }
      if (tempdif != 4) {
        calTemp = 4;
        calibrateTempGraph();
        tempdif = 4;
      }
    }
    else if (temp < 40.1) {
      if (temp < 35) {
        tempgraph = 1;
      }
      else if (temp > 40) {
        tempgraph = 100;
      }
      else {
        tempgraph = mapFloat(temp, 35.00, 40.00, 1, 100);
      }
      if (tempdif != 5) {
        calTemp = 5;
        calibrateTempGraph();
        tempdif = 5;
      }
    }
    else {
      if (temp < 40) {
        tempgraph = 1;
      }
      else if (temp > 45) {
        tempgraph = 100;
      }
      else {
        tempgraph = mapFloat(temp, 40.00, 45.00, 1, 100);
      }
      if (tempdif != 6) {
        calTemp = 6;
        calibrateTempGraph();
        tempdif = 6;
      }
    }

    sliptempgraph();
    sp[94] = tempgraph;
    sTp[94] = temp;
    for (int i = 0; i < 95; i++) {
      byte a = i + 1;
      Serial2.print("sp");
      Serial2.print(a);
      Serial2.print(".val=");
      Serial2.print(sp[i]);
      endNextionCommand();
    }
    tempgdrawtimer = timertemp;
  }
}

void sliptempgraph(void) {
  for (int i = 0; i < 94; i++) {
    byte a = i + 1;
    sp[i] = sp[a];
    sTp[i] = sTp[a];
  }
}

void zeroouttempgraph(void) {
  for (int i = 0; i < 95; i++) {
    sp[i] = 1;
    sTp[i] = 15.0;
  }
}

void calibrateTempGraph() {
  if (calTemp == 1) {
    for (int i = 0; i < 95; i++) {
      if (sTp[i] < 15) {
        sp[i] = 1;
      }
      else if (sTp[i] > 20) {
        sp[i] = 100;
      }
      else {
        sp[i] = mapFloat(sTp[i], 15.00, 20.00, 1, 100);
      }
    }
    calTemp = 0;
  }
  else if (calTemp == 2) {
    for (int i = 0; i < 95; i++) {
      if (sTp[i] < 20) {
        sp[i] = 1;
      }
      else if (sTp[i] > 25) {
        sp[i] = 100;
      }
      else {
        sp[i] = mapFloat(sTp[i], 20.00, 25.00, 1, 100);
      }
    }
    calTemp = 0;
  }
  else if (calTemp == 3) {
    for (int i = 0; i < 95; i++) {
      if (sTp[i] < 25) {
        sp[i] = 1;
      }
      else if (sTp[i] > 30) {
        sp[i] = 100;
      }
      else {
        sp[i] = mapFloat(sTp[i], 25.00, 30.00, 1, 100);
      }
    }
    calTemp = 0;
  }
  else if (calTemp == 4) {
    for (int i = 0; i < 95; i++) {
      if (sTp[i] < 30) {
        sp[i] = 1;
      }
      else if (sTp[i] > 35) {
        sp[i] = 100;
      }
      else {
        sp[i] = mapFloat(sTp[i], 30.00, 35.00, 1, 100);
      }
    }
    calTemp = 0;
  }
  else if (calTemp == 5) {
    for (int i = 0; i < 95; i++) {
      if (sTp[i] < 35) {
        sp[i] = 1;
      }
      else if (sTp[i] > 40) {
        sp[i] = 100;
      }
      else {
        sp[i] = mapFloat(sTp[i], 35.00, 40.00, 1, 100);
      }
    }
    calTemp = 0;
  }
  else if (calTemp == 6) {
    for (int i = 0; i < 95; i++) {
      if (sTp[i] < 40) {
        sp[i] = 1;
      }
      else if (sTp[i] > 45) {
        sp[i] = 100;
      }
      else {
        sp[i] = mapFloat(sTp[i], 40.00, 45.00, 1, 100);
      }
    }
    calTemp = 0;
  }
}
