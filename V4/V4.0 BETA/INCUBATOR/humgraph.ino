byte np[95];
long timerhum;
long humgdrawtimer;

void humgraphdraw(void) {
  timerhum = millis() / 200;
  if ((timerhum - humgdrawtimer) >= 5) {
    sliphumgraph();
    np[94] = humidity;
    for (int i = 0; i < 95; i++) {
      byte a = i + 1;
      Serial2.print("np");
      Serial2.print(a);
      Serial2.print(".val=");
      Serial2.print(np[i]);
      endNextionCommand();
    }
    humgdrawtimer = timerhum;
  }
}

void sliphumgraph(void) {
  for (int i = 0; i < 94; i++)
    np[i] = np[i + 1];
}

void zeroouthumgraph(void) {
  for (int i = 0; i < 95; i++)
    np[i] = 1;
}
