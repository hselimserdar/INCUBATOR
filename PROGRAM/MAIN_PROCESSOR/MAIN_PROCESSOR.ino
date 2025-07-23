//MAIN_PROCESSOR.ino
#include "globals.h"

String pcbsurum = "V4.4";
String yazilimsurum = "V4.0.1";

float temp;
byte humidity;
byte waterlevel;

byte timeformat;  //0:24-hour, 1:12-hour
byte dateformat;  //0:dd/mm/yy, 1:mm/dd/yy
byte daycount;    //Every 24hr elapsed from last timer reset
byte lang = 1;    //Language selection 0:English, 1:Turkish

bool heater = 1;          //0:off, 1:on
bool humidifier = 1;      //0:off, 1:on
bool humidityExtFan = 1;  //Moisture humidity extraction fan 0:off, 1:on
bool eggRotator = 0;      //Viol tilting/egg rotating motor 0:off,1:on
bool cabinFan = 1;
int processorReset = 0;  //Set 1 to reset processor
//auxProcessorHalt - Turns off the charging

//charging(bool start)
//WRITE THE CODES FOR ALL THE INPUTS AND OUTPUTS

// ---------------------------------------------------------------------------
// Task that runs serialComm() nonstop on Core 0 - THE PROBLEM IS WHEN THE ESP IS RUNNING ON ADAPTER, IT RUNS SLOWLY (MAYBE DUE TO NOISE ON THE POWER LINES)
// ---------------------------------------------------------------------------
static void serialCommTask(void *pvParameters) {
  (void)pvParameters;
  for (;;) {
    serialComm();
    vTaskDelay(pdMS_TO_TICKS(1));  // <- allow IDLE0 to run and reset its watchdog
  }
}

void setup() {
  initializeInput();
  initializeOutput();
  initializeNextion();
  serialCommSetup();
  initializeGraph();
  RTCinit();
  EEPROM_init();

    // Create exactly one task on Core 0:
  xTaskCreatePinnedToCore(
    serialCommTask,    // Task function
    "SerialComm0",     // Name (for debugging)
    4096,              // Stack size in bytes
    nullptr,           // No parameters
    1,                 // Priority (low to mid)
    nullptr,           // No handle needed
    0                  // Pin to core 0
  );
}

void loop() {
  nextionRun();
  inputControl();
  RTCcontroller();
  pageController();
  outputControl();
}
