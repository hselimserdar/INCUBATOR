//Nextion_Essential.ino
#define RXD2 16
#define TXD2 17

// Array of pointers to all objects for nexLoop
NexTouch *nex_listen_list[] = {
  &mainb0, &setb0, &setb1, &setb2, &setb3, &setb4, &setb5, &setb6, &setb7,
  &setb8, &setb9, &setb10, &setb11, &setb12, &setb13, &setb14, &setb15,
  &setb16, &setb17, &setb18, &setb19,
  NULL
};

//MAIN PAGE
NexButton mainb0 = NexButton(1, 201, "b0");

//SETTINGS PAGE
NexButton setb0 = NexButton(2, 1, "b0");
NexButton setb1 = NexButton(2, 2, "b1");
NexButton setb2 = NexButton(2, 3, "b2");
NexButton setb3 = NexButton(2, 4, "b3");
NexButton setb4 = NexButton(2, 5, "b4");
NexButton setb5 = NexButton(2, 6, "b5");
NexButton setb6 = NexButton(2, 7, "b6");
NexButton setb7 = NexButton(2, 8, "b7");
NexButton setb8 = NexButton(2, 9, "b8");
NexButton setb9 = NexButton(2, 10, "b9");
NexButton setb10 = NexButton(2, 11, "b10");
NexButton setb11 = NexButton(2, 12, "b11");
NexButton setb12 = NexButton(2, 13, "b12");
NexButton setb13 = NexButton(2, 14, "b13");
NexButton setb14 = NexButton(2, 26, "b14");
NexButton setb15 = NexButton(2, 27, "b15");
NexButton setb16 = NexButton(2, 28, "b16");
NexButton setb17 = NexButton(2, 29, "b17");
NexButton setb18 = NexButton(2, 30, "b18");
NexButton setb19 = NexButton(2, 31, "b19");

void initializeNextion() {
  nexInit();
    Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);

  // Attach push and pop callbacks for each button.
  //MAIN PAGE
  mainb0.attachPop(mainb0Pop, &mainb0);

  //SETTINGS PAGE
  setb0.attachPush(setb0Push, &setb0);
  setb1.attachPush(setb1Push, &setb1);
  setb2.attachPush(setb2Push, &setb2);
  setb3.attachPush(setb3Push, &setb3);
  setb4.attachPush(setb4Push, &setb4);
  setb5.attachPush(setb5Push, &setb5);
  setb6.attachPush(setb6Push, &setb6);
  setb7.attachPush(setb7Push, &setb7);
  setb8.attachPush(setb8Push, &setb8);
  setb9.attachPush(setb9Push, &setb9);
  setb10.attachPush(setb10Push, &setb10);
  setb11.attachPush(setb11Push, &setb11);
  setb12.attachPush(setb12Push, &setb12);
  setb13.attachPush(setb13Push, &setb13);
  setb14.attachPop(setb14Pop, &setb14);
  setb15.attachPop(setb15Pop, &setb15);
  setb16.attachPop(setb16Pop, &setb16);
  setb17.attachPop(setb17Pop, &setb17);
  setb18.attachPop(setb18Pop, &setb18);
  setb19.attachPop(setb19Pop, &setb19);
}

void endNextionCommand() {
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
}

void nextionRun() {
  nexLoop(nex_listen_list);
}
