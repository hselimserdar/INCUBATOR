//Page_Controller.ino
int page = 1;  //0: INTRO PAGE, 1: MAIN MENU, 2: SETTINGS

int MAINPAGEFPS = 4;  //Maın Page refresh rate per second
unsigned long mainPageTimer = 0;

float SETTINGSPAGEFPS = 0.5;  //Maın Page refresh rate per second
unsigned long SettingsPageTimer = 0;

unsigned long essentialsTimer100ms = 0;

void pageController() {
  unsigned long tmr = millis();

  if ((tmr - essentialsTimer100ms) >= 100) {
    graphController();
    essentialsTimer100ms = tmr;
  }

  if ((tmr - mainPageTimer) >= (1000 / MAINPAGEFPS)) {
    if (page == 1) {  //MAIN MENU
      mainPage();
    }
    mainPageTimer = tmr;
  }

  if ((tmr - SettingsPageTimer) >= (1000 / SETTINGSPAGEFPS)) {
    if (page == 2) {  //MAIN MENU
      settingsPAGEinit();
    }
    SettingsPageTimer = tmr;
  }
}
