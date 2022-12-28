byte sp[95];
float sTp[95];
byte tempdif = 0;
byte TempG;
byte tempgraph;
long timertemp;
long tempgdrawtimer;

void tempgraphdraw(void) {
  timertemp = millis() / 200;
  if ((timertemp - tempgdrawtimer) >= 5) {
    if (temp < 20.10) {
      tempgraph = mapFloat(temp, 15.00, 20.00, 1, 100);
      if (tempdif != 1) {
        calibrateTempGraph(1);
        tempdif = 1;
      }
    }
    else if (temp < 25.1) {
      tempgraph = mapFloat(temp, 20.00, 25.00, 1, 100);
      if (tempdif != 2) {
        calibrateTempGraph(2);
        tempdif = 2;
      }
    }
    else if (temp < 30.1) {
      tempgraph = mapFloat(temp, 25.00, 30.00, 1, 100);
      if (tempdif != 3) {
        calibrateTempGraph(3);
        tempdif = 3;
      }
    }
    else if (temp < 35.1) {
      tempgraph = mapFloat(temp, 30.00, 35.00, 1, 100);
      if (tempdif != 4) {
        calibrateTempGraph(4);
        tempdif = 4;
      }
    }
    else if (temp < 40.1) {
      tempgraph = mapFloat(temp, 35.00, 40.00, 1, 100);
      if (tempdif != 5) {
        calibrateTempGraph(5);
        tempdif = 5;
      }
    }
    else {
      tempgraph = mapFloat(temp, 40.00, 45.00, 1, 100);
      if (tempdif != 6) {
        calibrateTempGraph(6);
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
    byte a = i+1;
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

void calibrateTempGraph(byte a) {
  if (a == 1) {
    for (int i = 0; i < 95; i++) {
      TempG = mapFloat(temp, 15.00, 20.00, 1, 100);
      sp[i] = TempG;
    }
  }
  else if (a == 2) {
    for (int i = 0; i < 95; i++) {
      TempG = mapFloat(temp, 20.00, 25.00, 1, 100);
      sp[i] = TempG;
    }
  }
  else if (a == 3) {
    for (int i = 0; i < 95; i++) {
      TempG = mapFloat(temp, 25.00, 30.00, 1, 100);
      sp[i] = TempG;
    }
  }
  else if (a == 4) {
    for (int i = 0; i < 95; i++) {
      TempG = mapFloat(temp, 30.00, 35.00, 1, 100);
      sp[i] = TempG;
    }
  }
  else if (a == 5) {
    for (int i = 0; i < 95; i++) {
      TempG = mapFloat(temp, 35.00, 40.00, 1, 100);
      sp[i] = TempG;
    }
  }
  else if(a == 6){
    for (int i = 0; i < 95; i++) {
      TempG = mapFloat(temp, 40.00, 45.00, 1, 100);
      sp[i] = TempG;
    }
  }
}
