//graphDrawer.ino
//HUMIDITY GRAPH VARIABLES
byte np[95];
uint8_t npIndex = 0;  // Points to where the next humidity value will be stored

//TEMP GRAPH VARIABLES
byte sp[95];
float sTp[95];
uint8_t tempIndex = 0;  // Points to the next insertion spot (and also identifies the oldest element)
byte tempdif = 0;
byte calTemp = 0;
byte TempG;
byte tempgraph;

void graphController() {
  static int timergraph = 0;
  timergraph = (timergraph + 1) % 10;
  if (timergraph == 2) {
    tempgraphcalc();
    humgraphcalc();
    if (page == 2) {
      tempgraphdraw();
      humgraphdraw();
    }
  }
}

void initializeGraph() {
  zeroouttempgraph();
  zeroouthumgraph();
}

//DRAWS THE HUMIDITY GRAPH
void humgraphdraw(void) {
  // Print out the data in logical order (oldest first).
  // The oldest value is at npIndex (because that's where the next value will be written)
  for (int i = 0; i < 95; i++) {
    byte a = i + 1;
    // Compute the physical index corresponding to the logical order.
    uint8_t idx = (npIndex + i) % 95;
    Serial2.print("np");
    Serial2.print(a);
    Serial2.print(".val=");
    Serial2.print(np[idx]);
    endNextionCommand();
  }
}

void humgraphcalc(void) {
  // Update every ~1 second (1000ms) as before
  // Instead of shifting, write the new humidity value to the current index.
  np[npIndex] = humidity;
  // Advance the index, wrapping around when reaching the end.
  npIndex = (npIndex + 1) % 95;
}

void zeroouthumgraph(void) {
  // Initialize the array to 1's.
  for (int i = 0; i < 95; i++) {
    np[i] = 1;
  }
  npIndex = 0;  // Reset the circular buffer index.
}

//DRAWS THE TEMPERATURE GRAPH
void tempgraphdraw(void) {
  // Print the data in logical (oldest-first) order.
  // The oldest element is at tempIndex (the next insertion spot)
  for (int i = 0; i < 95; i++) {
    byte a = i + 1;
    uint8_t idx = (tempIndex + i) % 95;
    Serial2.print("sp");
    Serial2.print(a);
    Serial2.print(".val=");
    Serial2.print(sp[idx]);
    endNextionCommand();
  }
}

void tempgraphcalc(void) {
  // Update every 1000 ms (i.e. 5 intervals of 200ms)
  // Determine the tempgraph value based on the current temperature 'temp'
  if (temp < 20.10) {
    if (temp < 15) {
      tempgraph = 1;
    } else if (temp > 20) {
      tempgraph = 100;
    } else {
      tempgraph = mapFloat(temp, 15.00, 20.00, 1, 100);
    }
    if (tempdif != 1) {
      calTemp = 1;
      calibrateTempGraph();
      tempdif = 1;
    }
  } else if (temp < 25.1) {
    if (temp < 20) {
      tempgraph = 1;
    } else if (temp > 25) {
      tempgraph = 100;
    } else {
      tempgraph = mapFloat(temp, 20.00, 25.00, 1, 100);
    }
    if (tempdif != 2) {
      calTemp = 2;
      calibrateTempGraph();
      tempdif = 2;
    }
  } else if (temp < 30.1) {
    if (temp < 25) {
      tempgraph = 1;
    } else if (temp > 30) {
      tempgraph = 100;
    } else {
      tempgraph = mapFloat(temp, 25.00, 30.00, 1, 100);
    }
    if (tempdif != 3) {
      calTemp = 3;
      calibrateTempGraph();
      tempdif = 3;
    }
  } else if (temp < 35.1) {
    if (temp < 30) {
      tempgraph = 1;
    } else if (temp > 35) {
      tempgraph = 100;
    } else {
      tempgraph = mapFloat(temp, 30.00, 35.00, 1, 100);
    }
    if (tempdif != 4) {
      calTemp = 4;
      calibrateTempGraph();
      tempdif = 4;
    }
  } else if (temp < 40.1) {
    if (temp < 35) {
      tempgraph = 1;
    } else if (temp > 40) {
      tempgraph = 100;
    } else {
      tempgraph = mapFloat(temp, 35.00, 40.00, 1, 100);
    }
    if (tempdif != 5) {
      calTemp = 5;
      calibrateTempGraph();
      tempdif = 5;
    }
  } else {
    if (temp < 40) {
      tempgraph = 1;
    } else if (temp > 45) {
      tempgraph = 100;
    } else {
      tempgraph = mapFloat(temp, 40.00, 45.00, 1, 100);
    }
    if (tempdif != 6) {
      calTemp = 6;
      calibrateTempGraph();
      tempdif = 6;
    }
  }

  // Instead of shifting the arrays, we simply insert the new readings at tempIndex
  sp[tempIndex] = tempgraph;
  sTp[tempIndex] = temp;
  tempIndex = (tempIndex + 1) % 95;  // Wrap around if necessary
}

void zeroouttempgraph(void) {
  for (int i = 0; i < 95; i++) {
    sp[i] = 1;
    sTp[i] = 15.0;
  }
  tempIndex = 0;
}

void calibrateTempGraph() {
  // Recalculate all points in 'sp' based on the corresponding 'sTp' values.
  // The calibration is done over the entire buffer.
  if (calTemp == 1) {
    for (int i = 0; i < 95; i++) {
      if (sTp[i] < 15) {
        sp[i] = 1;
      } else if (sTp[i] > 20) {
        sp[i] = 100;
      } else {
        sp[i] = mapFloat(sTp[i], 15.00, 20.00, 1, 100);
      }
    }
    calTemp = 0;
  } else if (calTemp == 2) {
    for (int i = 0; i < 95; i++) {
      if (sTp[i] < 20) {
        sp[i] = 1;
      } else if (sTp[i] > 25) {
        sp[i] = 100;
      } else {
        sp[i] = mapFloat(sTp[i], 20.00, 25.00, 1, 100);
      }
    }
    calTemp = 0;
  } else if (calTemp == 3) {
    for (int i = 0; i < 95; i++) {
      if (sTp[i] < 25) {
        sp[i] = 1;
      } else if (sTp[i] > 30) {
        sp[i] = 100;
      } else {
        sp[i] = mapFloat(sTp[i], 25.00, 30.00, 1, 100);
      }
    }
    calTemp = 0;
  } else if (calTemp == 4) {
    for (int i = 0; i < 95; i++) {
      if (sTp[i] < 30) {
        sp[i] = 1;
      } else if (sTp[i] > 35) {
        sp[i] = 100;
      } else {
        sp[i] = mapFloat(sTp[i], 30.00, 35.00, 1, 100);
      }
    }
    calTemp = 0;
  } else if (calTemp == 5) {
    for (int i = 0; i < 95; i++) {
      if (sTp[i] < 35) {
        sp[i] = 1;
      } else if (sTp[i] > 40) {
        sp[i] = 100;
      } else {
        sp[i] = mapFloat(sTp[i], 35.00, 40.00, 1, 100);
      }
    }
    calTemp = 0;
  } else if (calTemp == 6) {
    for (int i = 0; i < 95; i++) {
      if (sTp[i] < 40) {
        sp[i] = 1;
      } else if (sTp[i] > 45) {
        sp[i] = 100;
      } else {
        sp[i] = mapFloat(sTp[i], 40.00, 45.00, 1, 100);
      }
    }
    calTemp = 0;
  }
}
