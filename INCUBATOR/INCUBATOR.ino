/*H.Selim Serdar Kuluçka Makinesi Projesi Program Sürüm V3.1.5*/

#include "Nextion.h"
#include "SDL_Arduino_INA3221.h"
#include <DHT.h>
#include <EEPROM.h>
#include <DS3231.h>
#include <Wire.h>
#include <math.h>
#include <Adafruit_ADS1015.h>

Adafruit_ADS1115 ads;
#define DHTTYPE DHT22
#define CH1 2 //nem tahliye
#define CH2 3 //yumurta çevirici
#define CH3 4 //ısıtıcı
#define CH4 5 //nem makinesi
#define CH5 6 //12V ısıtıcı
#define CH6 7 //akü şarj rölesi
#define DHTPIN 8
#define AKUSARJAKIMI 9
#define FANHIZI 10
#define GERILIMPIN 22
#define BUZZERPIN 11
#define RESETPIN 12
#define seviye1 A2
#define seviye2 A3
#define seviye3 A4
#define seviye4 A5
#define termistor1pin A6
#define termistor2pin A7
#define termistor3pin A8
#define VCC_HATTI 1
#define AKU_SARJ 2
#define AKU_DESARJ 3

SDL_Arduino_INA3221 ina3221;
DHT dht(DHTPIN, DHTTYPE); //n8-nem, t8-sıcaklık
DS3231 rtc(SDA, SCL);
Time  t;

NexPage page0 = NexPage(0, 0, "INTRO");
NexPage page1 = NexPage(1, 0, "MAIN");
NexPage page2 = NexPage(2, 0, "SETTINGS");
NexPage page3 = NexPage(3, 0, "SETTINGS2");
NexPage page4 = NexPage(4, 0, "SETTINGS3");
NexPage page5 = NexPage(5, 0, "SETTINGS4");
NexPage page6 = NexPage(6, 0, "MAIN_EN");
NexPage page7 = NexPage(7, 0, "SETTINGS_EN");
NexPage page8 = NexPage(8, 0, "SETTINGS2_EN");
NexPage page9 = NexPage(9, 0, "SETTINGS3_EN");
NexPage page10 = NexPage(10, 0, "SETTINGS4_EN");
NexPage page11 = NexPage(11, 0, "SETTINGS5");
NexPage page12 = NexPage(12, 0, "SETTINGS5_EN");
NexPage page13 = NexPage(13, 0, "SETTINGS6");
NexPage page14 = NexPage(14, 0, "SETTINGS6_EN");
NexButton b1 = NexButton(2, 3, "b1");
NexButton b2 = NexButton(2, 6, "b2");
NexButton b3 = NexButton(2, 7, "b3");
NexButton b4 = NexButton(2, 8, "b4");
NexButton b5 = NexButton(2, 9, "b5");
NexButton b6 = NexButton(2, 10, "b6");
NexButton b7 = NexButton(2, 11, "b7");
NexButton b8 = NexButton(2, 12, "b8");
NexButton b9 = NexButton(2, 13, "b9");
NexButton b10 = NexButton(2, 4, "b10");
NexButton b11 = NexButton(2, 5, "b11");
NexButton b12 = NexButton(2, 14, "b12");
NexButton b13 = NexButton(2, 15, "b13");
NexButton b14 = NexButton(3, 7, "b14");
NexButton b15 = NexButton(3, 8, "b15");
NexButton b16 = NexButton(3, 10, "b16");
NexButton b17 = NexButton(3, 9, "b17");
NexButton b18 = NexButton(3, 12, "b18");
NexButton b19 = NexButton(3, 11, "b19");
NexButton b20 = NexButton(3, 6, "b20");
NexButton b21 = NexButton(3, 5, "b21");
NexButton b22 = NexButton(4, 4, "b22");
NexButton b23 = NexButton(4, 7, "b23");
NexButton b24 = NexButton(4, 8, "b24");
NexButton b25 = NexButton(4, 5, "b25");
NexButton b26 = NexButton(4, 6, "b26");
NexButton b27 = NexButton(2, 24, "b27");
NexButton b28 = NexButton(2, 25, "b28");
NexButton b29 = NexButton(2, 28, "b29");
NexButton b30 = NexButton(5, 6, "b30");
NexButton b31 = NexButton(5, 7, "b31");
NexButton b32 = NexButton(5, 10, "b32");
NexButton b33 = NexButton(11, 3, "b33");
NexButton b34 = NexButton(11, 4, "b34");
NexButton b35 = NexButton(5, 12, "b35");
NexButton b36 = NexButton(5, 13, "b36");
NexPicture pb1 = NexPicture(5, 17, "pb1");
NexPicture pb2 = NexPicture(5, 18, "pb2");
NexPicture pb3 = NexPicture(5, 19, "pb3");
NexPicture pb4 = NexPicture(5, 20, "pb4");
NexPicture pb5 = NexPicture(5, 21, "pb5");
NexPicture pb6 = NexPicture(5, 22, "pb6");
NexButton b1en = NexButton(7, 3, "b1en");
NexButton b2en = NexButton(7, 6, "b2en");
NexButton b3en = NexButton(7, 7, "b3en");
NexButton b4en = NexButton(7, 8, "b4en");
NexButton b5en = NexButton(7, 9, "b5en");
NexButton b6en = NexButton(7, 10, "b6en");
NexButton b7en = NexButton(7, 11, "b7en");
NexButton b8en = NexButton(7, 12, "b8en");
NexButton b9en = NexButton(7, 13, "b9en");
NexButton b10en = NexButton(7, 4, "b10en");
NexButton b11en = NexButton(7, 5, "b11en");
NexButton b12en = NexButton(7, 14, "b12en");
NexButton b13en = NexButton(7, 15, "b13en");
NexButton b14en = NexButton(8, 7, "b14en");
NexButton b15en = NexButton(8, 8, "b15en");
NexButton b16en = NexButton(8, 10, "b16en");
NexButton b17en = NexButton(8, 9, "b17en");
NexButton b18en = NexButton(8, 12, "b18en");
NexButton b19en = NexButton(8, 11, "b19en");
NexButton b20en = NexButton(8, 6, "b20en");
NexButton b21en = NexButton(8, 5, "b21en");
NexButton b22en = NexButton(9, 4, "b22en");
NexButton b23en = NexButton(9, 7, "b23en");
NexButton b24en = NexButton(9, 8, "b24en");
NexButton b25en = NexButton(9, 5, "b25en");
NexButton b26en = NexButton(9, 6, "b26en");
NexButton b27en = NexButton(7, 24, "b27en");
NexButton b28en = NexButton(7, 25, "b28en");
NexButton b30en = NexButton(10, 14, "b30en");
NexButton b31en = NexButton(10, 11, "b31en");
NexButton b32en = NexButton(10, 10, "b32en");
NexButton b33en = NexButton(12, 4, "b33en");
NexButton b34en = NexButton(12, 3, "b34en");
NexButton b35en = NexButton(10, 13, "b35en");
NexButton b36en = NexButton(10, 14, "b36en");
NexPicture pb1en = NexPicture(10, 17, "pb1en");
NexPicture pb2en = NexPicture(10, 18, "pb2en");
NexPicture pb3en = NexPicture(10, 19, "pb3en");
NexPicture pb4en = NexPicture(10, 20, "pb4en");
NexPicture pb5en = NexPicture(10, 21, "pb5en");
NexPicture pb6en = NexPicture(10, 22, "pb6en");

NexTouch * nex_listen_list[] =
{
  &b1,
  &b2,
  &b3,
  &b4,
  &b5,
  &b6,
  &b7,
  &b8,
  &b9,
  &b10,
  &b11,
  &b12,
  &b13,
  &b14,
  &b15,
  &b16,
  &b17,
  &b18,
  &b19,
  &b20,
  &b21,
  &b22,
  &b23,
  &b24,
  &b25,
  &b26,
  &b27,
  &b28,
  &b29,
  &b30,
  &b31,
  &b32,
  &b33,
  &b34,
  &b35,
  &b36,
  &pb1,
  &pb2,
  &pb3,
  &pb4,
  &pb5,
  &pb6,
  &b1en,
  &b2en,
  &b3en,
  &b4en,
  &b5en,
  &b6en,
  &b7en,
  &b8en,
  &b9en,
  &b10en,
  &b11en,
  &b12en,
  &b13en,
  &b14en,
  &b15en,
  &b16en,
  &b17en,
  &b18en,
  &b19en,
  &b20en,
  &b21en,
  &b22en,
  &b23en,
  &b24en,
  &b25en,
  &b26en,
  &b27en,
  &b28en,
  &b30en,
  &b31en,
  &b32en,
  &b33en,
  &b34en,
  &b35en,
  &b36en,
  &pb1en,
  &pb2en,
  &pb3en,
  &pb4en,
  &pb5en,
  &pb6en,
  NULL
};

unsigned long zamanms = 0; //Kod başladıktan sonra geçen zaman (ms cinsinden) NOT:MAX 50 GÜN
unsigned long zaman = 0; //Kod başladıktan sonra geçen zaman (sn cinsinden)
float zamanprevgucgos = 0; //En son kaydedilen kod başladıktan sonra geçen zaman (sn cinsinden)
float zamanprevbuz = 0; //En son kaydedilen kod başladıktan sonra geçen zaman (sn cinsinden)
float akuvrefresh = 0; //En son kaydedilen kod başladıktan sonra geçen zaman (sn cinsinden)
float R1 = 30000.0;
float R2 = 10000.0;
float akuvolt = 0.0; //Akü voltajı
float vgiris = 0.0; //5V üzerinden akü voltajı
float analogakuvoltaji; //Ham olarak analog girişten okunan akü voltajı verisi
byte akuyuzdesi; //Akünün şarj yüzdesi
float yuzdehesap; //Akü yüzde hesaplarken kullanılacak işlem arası geçiş değişkeni
byte GUNSAYACI; //Kuluçka makinesinin resetlendiğinden itibaren kaç tam gün çalıştığını gösteren değişken
int gun; //rtc üzerinde gün ayarı yapmadan önceki önizleme olan değer
int ay; //rtc üzerinde ay ayarı yapmadan önceki önizleme olan değer
int yil; //rtc üzerinde yıl ayarı yapmadan önceki önizleme olan değer
int dakika; //rtc üzerinde dakika ayarı yapmadan önceki önizleme olan değer
int saat; //rtc üzerinde saat ayarı yapmadan önceki önizleme olan değer
byte sayacsaat; //gün sayacı için saat reğerans değişkeni
byte sayacdakika; //gün sayacı için dakika reğerans değişkeni
float akusarjv; //eeprom için akü şarj voltajı değeri
float akusarjvoltaji = 0.0; //akü şarj voltajı değeri
boolean c = 0; //saatin tek veya çift olduğunu kaydeden değişken
boolean LANG; //dil ayarı İngilizce için 0, Türkçe için 1
float a = 10; //eeprom için sıcaklık ondalık değeri bölüm birimi
byte nem; //sensörden ölçülen nem
float sicaklik; //sensörden ölçülen sıcaklık
byte smin; //eeprom için sıcaklık alt değeri
byte smax; //eeprom için sıcaklık üst değeri
int hmin; //eeprom için nem alt değeri
int hmax; //eeprom için nem üst değeri
int turntime; //eeprom için yumurta çevirme süresi
float saltdeger; //sıcaklık alt değeri
float sustdeger; //sıcaklık üst değeri
int naltdeger; //nem alt değeri
int nustdeger; //nem üst değeri
byte nalarm; //nem alarm değeri
byte kanal1 = 0; //CH1
byte kanal2 = 0; //CH2
byte kanal3 = 0; //CH3
byte kanal4 = 0; //CH4
byte kanal5 = 0; //CH5
int s1; //su seviyesi 1
int s2; //su seviyesi 2
int s3; //su seviyesi 3
int s4; //su seviyesi 4
int termistor1analog;
int termistor2analog;
int termistor3analog;
double Termistor1(int analogOkuma1) {

  double sicaklik1;
  sicaklik1 = log(((10240000 / analogOkuma1) - 10000));
  sicaklik1 = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * sicaklik1 * sicaklik1)) * sicaklik1);
  sicaklik1 = sicaklik1 - 273.15;
  return sicaklik1;
}
double Termistor2(int analogOkuma2) {

  double sicaklik2;
  sicaklik2 = log(((10240000 / analogOkuma2) - 10000));
  sicaklik2 = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * sicaklik2 * sicaklik2)) * sicaklik2);
  sicaklik2 = sicaklik2 - 273.15;
  return sicaklik2;
}
double Termistor3(int analogOkuma3) {

  double sicaklik3;
  sicaklik3 = log(((10240000 / analogOkuma3) - 10000));
  sicaklik3 = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * sicaklik3 * sicaklik3)) * sicaklik3);
  sicaklik3 = sicaklik3 - 273.15;
  return sicaklik3;
}
byte analogref = 200; //su seviyesi ölçümü için analog girişi voltaj referansı
boolean GUCTIPI; //AC, DC değerini saklayacak olan değişken; 1'de iken AC, 0'da iken DC güç
byte SARJOLUYOR = 0; //Akünün şarj durumunu saklayan değişken
byte SUSEVIYEBUZZERAKTIF = 0;
byte AKUVARYOK = 0;
byte AKUVARMI; //Akünün kullanılıp kullanılmayacağını saklayan değişken; 1 ise, AKÜ KULLANILIYOR
byte SUSEVIYEBUZZER; //Su seviyesi alarmının kullanılıp kullanılmayacağını saklayan değişken; 1 ise kullanılıyor
byte SUSEVIYEGOSTERGESI; //Su seviyesi göstergesinin kullanılıp kullanılmayacağını saklayan değişken; 1 ise kullanılıyor
int CEVIRMEDGUNSAYISI; //Maksimum yumurta çevirme gün sayısı
unsigned long EkranYenileme1 = 0;
unsigned long EkranYenileme2 = 0;
unsigned long EkranYenileme3 = 0;
unsigned long EkranYenileme4 = 0;
unsigned long EkranYenileme5 = 0;
unsigned long sensetiming = 0;
unsigned long pbswitchdelay = 0;
int pidtiming = 0;
int pidpwm = 255;
int hata = 0;
int MID;
char nextion_array[3] = {0xFF, 0xFF, 0xFF};
char buffer[100] = {0};
float shuntvoltage1 = 0;
float busvoltage1 = 0;
float current_mA1 = 0;
float loadvoltage1 = 0;
float shuntvoltage2 = 0;
float busvoltage2 = 0;
float current_mA2 = 0;
float loadvoltage2 = 0;
float shuntvoltage3 = 0;
float busvoltage3 = 0;
float current_mA3 = 0;
float loadvoltage3 = 0;
int ordinal;
String ordinalnumber;
String pcbsurum = "V3.2";
String yazilimsurum = "V3.1.5";
byte akusarjcurr;
float setcurrentA;
float setcurrentMA;
int millissaniye;
int millisdakika;
int millissaat;
int millisgun;
float VCCHATAMP;
float AKUSARJAMP;
float AKUDESARJAMP;

/**********************************************************************************/

void setup()
{
  TCCR1B = TCCR1B & 0b11111000 | 0x02;
  pinMode(CH1, OUTPUT);
  pinMode(CH2, OUTPUT);
  pinMode(CH3, OUTPUT);
  pinMode(CH4, OUTPUT);
  pinMode(CH5, OUTPUT);
  pinMode(CH6, OUTPUT);
  pinMode(AKUSARJAKIMI, OUTPUT);
  pinMode(seviye1, INPUT);
  pinMode(seviye2, INPUT);
  pinMode(seviye3, INPUT);
  pinMode(seviye4, INPUT);
  pinMode(GERILIMPIN, INPUT);
  digitalWrite(RESETPIN, HIGH);
  pinMode(RESETPIN, OUTPUT);
  digitalWrite(RESETPIN, HIGH);
  nexSerial.begin(9600);
  Serial2.begin(9600);
  Serial2.print("baud=115200");
  Serial2.write(nextion_array, 3);
  Serial2.end();
  Serial2.begin(115200);
  page0.show();
  b1.attachPop(b1PopCallback, &b1);
  b2.attachPush(b2PushCallback, &b2);
  b3.attachPush(b3PushCallback, &b3);
  b4.attachPush(b4PushCallback, &b4);
  b5.attachPush(b5PushCallback, &b5);
  b6.attachPush(b6PushCallback, &b6);
  b7.attachPush(b7PushCallback, &b7);
  b8.attachPush(b8PushCallback, &b8);
  b9.attachPush(b9PushCallback, &b9);
  b10.attachPush(b10PushCallback, &b10);
  b11.attachPush(b11PushCallback, &b11);
  b12.attachPush(b12PushCallback, &b12);
  b13.attachPush(b13PushCallback, &b13);
  b14.attachPush(b14PushCallback, &b14);
  b15.attachPush(b15PushCallback, &b15);
  b16.attachPush(b16PushCallback, &b16);
  b17.attachPush(b17PushCallback, &b17);
  b18.attachPush(b18PushCallback, &b18);
  b19.attachPush(b19PushCallback, &b19);
  b20.attachPush(b20PushCallback, &b20);
  b21.attachPush(b21PushCallback, &b21);
  b22.attachPush(b22PushCallback, &b22);
  b23.attachPush(b23PushCallback, &b23);
  b24.attachPush(b24PushCallback, &b24);
  b25.attachPush(b25PushCallback, &b25);
  b26.attachPush(b26PushCallback, &b26);
  b27.attachPush(b27PushCallback, &b27);
  b28.attachPush(b28PushCallback, &b28);
  b30.attachPush(b30PushCallback, &b30);
  b31.attachPush(b31PushCallback, &b31);
  b32.attachPush(b32PushCallback, &b32);
  b33.attachPush(b33PushCallback, &b33);
  b34.attachPush(b34PushCallback, &b34);
  b35.attachPush(b35PushCallback, &b35);
  b36.attachPush(b36PushCallback, &b36);
  pb1.attachPush(pb1PushCallback, &pb1);
  pb2.attachPush(pb2PushCallback, &pb2);
  pb3.attachPush(pb3PushCallback, &pb3);
  pb4.attachPush(pb4PushCallback, &pb4);
  pb5.attachPush(pb5PushCallback, &pb5);
  pb6.attachPush(pb6PushCallback, &pb6);
  b1en.attachPop(b1enPopCallback, &b1en);
  b2en.attachPush(b2enPushCallback, &b2en);
  b3en.attachPush(b3enPushCallback, &b3en);
  b4en.attachPush(b4enPushCallback, &b4en);
  b5en.attachPush(b5enPushCallback, &b5en);
  b6en.attachPush(b6enPushCallback, &b6en);
  b7en.attachPush(b7enPushCallback, &b7en);
  b8en.attachPush(b8enPushCallback, &b8en);
  b9en.attachPush(b9enPushCallback, &b9en);
  b10en.attachPush(b10enPushCallback, &b10en);
  b11en.attachPush(b11enPushCallback, &b11en);
  b12en.attachPush(b12enPushCallback, &b12en);
  b13en.attachPush(b13enPushCallback, &b13en);
  b14en.attachPush(b14enPushCallback, &b14en);
  b15en.attachPush(b15enPushCallback, &b15en);
  b16en.attachPush(b16enPushCallback, &b16en);
  b17en.attachPush(b17enPushCallback, &b17en);
  b18en.attachPush(b18enPushCallback, &b18en);
  b19en.attachPush(b19enPushCallback, &b19en);
  b20en.attachPush(b20enPushCallback, &b20en);
  b21en.attachPush(b21enPushCallback, &b21en);
  b22en.attachPush(b22enPushCallback, &b22en);
  b23en.attachPush(b23enPushCallback, &b23en);
  b24en.attachPush(b24enPushCallback, &b24en);
  b25en.attachPush(b25enPushCallback, &b25en);
  b26en.attachPush(b26enPushCallback, &b26en);
  b27en.attachPush(b27enPushCallback, &b27en);
  b28en.attachPush(b28enPushCallback, &b28en);
  b30en.attachPush(b30enPushCallback, &b30en);
  b31en.attachPush(b31enPushCallback, &b31en);
  b32en.attachPush(b32enPushCallback, &b32en);
  b33en.attachPush(b33enPushCallback, &b33en);
  b34en.attachPush(b34enPushCallback, &b34en);
  b35en.attachPush(b35enPushCallback, &b35en);
  b36en.attachPush(b36enPushCallback, &b36en);
  pb1en.attachPush(pb1enPushCallback, &pb1en);
  pb2en.attachPush(pb2enPushCallback, &pb2en);
  pb3en.attachPush(pb3enPushCallback, &pb3en);
  pb4en.attachPush(pb4enPushCallback, &pb4en);
  pb5en.attachPush(pb5enPushCallback, &pb5en);
  pb6en.attachPush(pb6enPushCallback, &pb6en);
  ads.setGain(GAIN_ONE); // 1x gain   +/- 4.096V  1 bit = 0.125mV
  dht.begin();
  rtc.begin();
  ads.begin();
  ina3221.begin();
  MID = ina3221.getManufID();
  smin = EEPROM.read(0);
  smax = EEPROM.read(1);
  hmin = EEPROM.read(2);
  hmax = EEPROM.read(3);
  turntime = EEPROM.read(4);
  GUNSAYACI = EEPROM.read(5);
  sayacsaat = EEPROM.read(6);
  sayacdakika = EEPROM.read(7);
  AKUVARMI = EEPROM.read(8);
  SUSEVIYEBUZZER = EEPROM.read(9);
  SUSEVIYEGOSTERGESI = EEPROM.read(10);
  CEVIRMEDGUNSAYISI = EEPROM.read(11);
  akusarjv = EEPROM.read(12);
  LANG = EEPROM.read(13);
  akusarjcurr = EEPROM.read(14);
  t = rtc.getTime();
  yil = (t.year);
  ay = (t.mon);
  gun = (t.date);
  saat = (t.hour);
  dakika = (t.min);
  /////////////////////////////////////////////////////////////////////////////////
  page0.show();
  Serial2.print("page INTRO");
  Serial2.write(nextion_array, 3);
  if (LANG == 0) {
    Serial2.print("intro.txt=");
    Serial2.print("\"");
    Serial2.print("H.Selim Serdar Incubator Project");
    Serial2.print("\"");
    Serial2.write(nextion_array, 3);
  }
  else if (LANG == 1) {
    Serial2.print("intro.txt=");
    Serial2.print("\"");
    Serial2.print("H.Selim Serdar Kulucka Makinesi Projesi");
    Serial2.print("\"");
    Serial2.write(nextion_array, 3);

  }
  Serial2.print("p2.val=0");
  Serial2.write(nextion_array, 3);
  Serial2.print("p1.val=0");
  Serial2.write(nextion_array, 3);
  for (byte p = 0 ; p < 101 ; p++) {
    Serial2.print("p1.val=");
    Serial2.print(p);
    Serial2.write(nextion_array, 3);
    if (p == 0 || p == 40 || p == 80) {
      if (LANG == 1) {
        Serial2.print("load.txt=");
        Serial2.print("\"");
        Serial2.print("YUKLENIYOR");
        Serial2.print("\"");
        Serial2.write(nextion_array, 3);
      }
      else {
        Serial2.print("load.txt=");
        Serial2.print("\"");
        Serial2.print("LOADING");
        Serial2.print("\"");
        Serial2.write(nextion_array, 3);
      }
    }
    else if (p == 10 || p == 50 || p == 90) {
      if (LANG == 1) {
        Serial2.print("load.txt=");
        Serial2.print("\"");
        Serial2.print("YUKLENIYOR.");
        Serial2.print("\"");
        Serial2.write(nextion_array, 3);
      }
      else {
        Serial2.print("load.txt=");
        Serial2.print("\"");
        Serial2.print("LOADING.");
        Serial2.print("\"");
        Serial2.write(nextion_array, 3);
      }
    }
    else if (p == 20 || p == 60 || p == 100) {
      if (LANG == 1) {
        Serial2.print("load.txt=");
        Serial2.print("\"");
        Serial2.print("YUKLENIYOR..");
        Serial2.print("\"");
        Serial2.write(nextion_array, 3);
      }
      else {
        Serial2.print("load.txt=");
        Serial2.print("\"");
        Serial2.print("LOADING..");
        Serial2.print("\"");
        Serial2.write(nextion_array, 3);
      }
    }
    else if (p == 30 || p == 70) {
      if (LANG == 1) {
        Serial2.print("load.txt=");
        Serial2.print("\"");
        Serial2.print("YUKLENIYOR...");
        Serial2.print("\"");
        Serial2.write(nextion_array, 3);
      }
      else {
        Serial2.print("load.txt=");
        Serial2.print("\"");
        Serial2.print("LOADING...");
        Serial2.print("\"");
        Serial2.write(nextion_array, 3);
      }
    }
    delay(30);
  }
  for (byte p2 = 0 ; p2 < 101 ; p2++) {
    Serial2.print("p2.val=");
    Serial2.print(p2);
    Serial2.write(nextion_array, 3);
    if (p2 == 0 || p2 == 40 || p2 == 80) {
      if (LANG == 1) {
        Serial2.print("load.txt=");
        Serial2.print("\"");
        Serial2.print("YUKLENIYOR...");
        Serial2.print("\"");
        Serial2.write(nextion_array, 3);
      }
      else {
        Serial2.print("load.txt=");
        Serial2.print("\"");
        Serial2.print("LOADING...");
        Serial2.print("\"");
        Serial2.write(nextion_array, 3);
      }
    }
    else if (p2 == 10 || p2 == 50 || p2 == 90) {
      if (LANG == 1) {
        Serial2.print("load.txt=");
        Serial2.print("\"");
        Serial2.print("YUKLENIYOR");
        Serial2.print("\"");
        Serial2.write(nextion_array, 3);
      }
      else {
        Serial2.print("load.txt=");
        Serial2.print("\"");
        Serial2.print("LOADING");
        Serial2.print("\"");
        Serial2.write(nextion_array, 3);
      }
    }
    else if (p2 == 20 || p2 == 60 || p2 == 100) {
      if (LANG == 1) {
        Serial2.print("load.txt=");
        Serial2.print("\"");
        Serial2.print("YUKLENIYOR.");
        Serial2.print("\"");
        Serial2.write(nextion_array, 3);
      }
      else {
        Serial2.print("load.txt=");
        Serial2.print("\"");
        Serial2.print("LOADING.");
        Serial2.print("\"");
        Serial2.write(nextion_array, 3);
      }
    }
    else if (p2 == 30 || p2 == 70) {
      if (LANG == 1) {
        Serial2.print("load.txt=");
        Serial2.print("\"");
        Serial2.print("YUKLENIYOR..");
        Serial2.print("\"");
        Serial2.write(nextion_array, 3);
      }
      else {
        Serial2.print("load.txt=");
        Serial2.print("\"");
        Serial2.print("LOADING..");
        Serial2.print("\"");
        Serial2.write(nextion_array, 3);
      }
    }
    delay(30);
  }
  pinMode(BUZZERPIN, OUTPUT);
  digitalWrite(BUZZERPIN, HIGH);
  delay(80);
  digitalWrite(BUZZERPIN, LOW);
  delay(50);
  digitalWrite(BUZZERPIN, HIGH);
  delay(80);
  digitalWrite(BUZZERPIN, LOW);
  if (LANG == 0) {
    page6.show();
    Serial2.print("page MAIN_EN");
    Serial2.write(nextion_array, 3);
  }
  if (LANG == 1) {
    page1.show();
    Serial2.print("page MAIN");
    Serial2.write(nextion_array, 3);
  }
  ///////////////////////////////////////////////////////////////////////////////////
  delay(250);
}

void loop()
{
  int16_t adc0;
  adc0 = ads.readADC_SingleEnded(0);
  nexLoop(nex_listen_list);
  analogakuvoltaji = adc0;
  GUCTIPI = digitalRead(GERILIMPIN);
  vgiris = (analogakuvoltaji * 0.000125);
  zamanms = millis();
  zaman = zamanms / 1000;
  saltdeger = (200 + smin) / a;
  sustdeger = (200 + smax) / a;
  naltdeger = hmin;
  nustdeger = hmax;
  nalarm = (nustdeger + 5);
  t = rtc.getTime();
  c = t.hour % 2;
  ordinal = GUNSAYACI % 10;
  s1 = analogRead(seviye1);
  s2 = analogRead(seviye2);
  s3 = analogRead(seviye3);
  s4 = analogRead(seviye4);
  millissaniye = zaman % 60;
  millisdakika = (zaman / 60) % 60;
  millissaat = (zaman / 3600) % 24;
  millisgun = (zaman / 3600) / 24;
  akusarjvoltaji = 12 + (akusarjv / 50);
  hata = current_mA2 - setcurrentMA;
  setcurrentMA = 500 + (akusarjcurr * 10);
  setcurrentA = setcurrentMA / 1000;

  termistor1analog = analogRead(termistor1pin);
  termistor2analog = analogRead(termistor2pin);
  termistor3analog = analogRead(termistor3pin);

  double termistor1 = Termistor1(termistor1analog);
  double termistor2 = Termistor2(termistor2analog);
  double termistor3 = Termistor3(termistor3analog);

  if ((zamanms - sensetiming) > 1500) {
    busvoltage1 = ina3221.getBusVoltage_V(VCC_HATTI);
    shuntvoltage1 = ina3221.getShuntVoltage_mV(VCC_HATTI);
    current_mA1 = ina3221.getCurrent_mA(VCC_HATTI);
    loadvoltage1 = busvoltage1 + (shuntvoltage1 / 1000);
    if (current_mA1 < 0) {
      current_mA1 = 0;
    }
    VCCHATAMP = current_mA1 / 1000;

    busvoltage2 = ina3221.getBusVoltage_V(AKU_SARJ);
    shuntvoltage2 = ina3221.getShuntVoltage_mV(AKU_SARJ);
    current_mA2 = ina3221.getCurrent_mA(AKU_SARJ);
    loadvoltage2 = busvoltage2 + (shuntvoltage2 / 1000);
    if (current_mA2 < 0) {
      current_mA2 = 0;
    }
    AKUSARJAMP = current_mA2 / 1000;

    busvoltage3 = ina3221.getBusVoltage_V(AKU_DESARJ);
    shuntvoltage3 = ina3221.getShuntVoltage_mV(AKU_DESARJ);
    current_mA3 = ina3221.getCurrent_mA(AKU_DESARJ);
    loadvoltage3 = busvoltage3 + (shuntvoltage3 / 1000);
    if (current_mA3 < 0) {
      current_mA3 = 0;
    }
    AKUDESARJAMP = current_mA3 / 1000;
    sensetiming = zamanms;
  }

  if (SARJOLUYOR == 1) {
    if (zamanms - pidtiming >= 100) {
      if (hata > 100) {
        if (pidpwm < 256) {
          pidpwm = pidpwm + 1;
        }
      }
      if (hata < -100) {
        if (pidpwm < 256) {
          pidpwm = pidpwm - 1;
        }
        pidtiming = zamanms;
      }
      analogWrite(AKUSARJAKIMI, pidpwm);
    }
  }

  if ((zaman - akuvrefresh) >= 1) {
    akuvolt = (vgiris / (R2 / (R1 + R2)));
    if (akuvolt < 2) {
      akuvolt = 0;
    }
    if (akuvolt < 10.5) {
      akuyuzdesi = 0;
    }
    else if (akuvolt >= 10.5 && akuvolt < 11.51) {
      yuzdehesap = (akuvolt - 10.5) * 100;
      akuyuzdesi = map(yuzdehesap, 0, 101, 0, 10);
    }
    else if (akuvolt >= 11.51 && akuvolt < 11.66) {
      yuzdehesap = (akuvolt - 11.51) * 100;
      akuyuzdesi = map(yuzdehesap, 0, 15, 10, 20);
    }
    else if (akuvolt >= 11.66 && akuvolt < 11.81) {
      yuzdehesap = (akuvolt - 11.66) * 100;
      akuyuzdesi = map(yuzdehesap, 0, 15, 20, 30);
    }
    else if (akuvolt >= 11.81 && akuvolt < 11.95) {
      yuzdehesap = (akuvolt - 11.81) * 100;
      akuyuzdesi = map(yuzdehesap, 0, 14, 30, 40);
    }
    else if (akuvolt >= 11.95 && akuvolt < 12.05) {
      yuzdehesap = (akuvolt - 11.95) * 100;
      akuyuzdesi = map(yuzdehesap, 0, 10, 40, 50);
    }
    else if (akuvolt >= 12.05 && akuvolt < 12.15) {
      yuzdehesap = (akuvolt - 12.05) * 100;
      akuyuzdesi = map(yuzdehesap, 0, 10, 50, 60);
    }
    else if (akuvolt >= 12.15 && akuvolt < 12.30) {
      yuzdehesap = (akuvolt - 12.15) * 100;
      akuyuzdesi = map(yuzdehesap, 0, 15, 60, 70);
    }
    else if (akuvolt >= 12.30 && akuvolt < 12.50) {
      yuzdehesap = (akuvolt - 12.30) * 100;
      akuyuzdesi = map(yuzdehesap, 0, 20, 70, 80);
    }
    else if (akuvolt >= 12.50 && akuvolt < 12.75) {
      yuzdehesap = (akuvolt - 12.50) * 100;
      akuyuzdesi = map(yuzdehesap, 0, 25, 80, 90);
    }
    else if (akuvolt >= 12.75 && akuvolt < 13.00) {
      yuzdehesap = (akuvolt - 12.75) * 100;
      akuyuzdesi = map(yuzdehesap, 0, 25, 90, 100);
    }
    else if (akuvolt > 13.00) {
      akuyuzdesi = 100;
    }
    akuvrefresh = zaman;
  }

  if (akuvolt > akusarjvoltaji && GUCTIPI == 1 && AKUVARMI == 1) {
    digitalWrite (CH6, HIGH);
    SARJOLUYOR = 0;
    kanal5 = 0;
  }

  if (akuvolt < 12.5 && akuvolt > 5 && AKUVARMI == 1 && GUCTIPI == 1) {
    digitalWrite (CH6, LOW);
    SARJOLUYOR = 1;
    kanal5 = 1;
  }

  if (GUCTIPI == 0 && akuyuzdesi >= 5 && AKUVARMI == 1 && kanal3 == 1) {
    digitalWrite(CH5, LOW);
  }
  else {
    digitalWrite(CH5, HIGH);
  }

  if (GUCTIPI == 0) {
    SARJOLUYOR = 0;
    kanal5 = 0;
    digitalWrite(CH6, HIGH);
  }

  if ((zaman - zamanprevbuz) >= 10 < 10.5 && GUCTIPI == 0 && AKUVARMI == 1) {
    digitalWrite(BUZZERPIN, LOW);
  }

  if ((zaman - zamanprevbuz) >= 10.5) {
    zamanprevbuz = zaman;
    if (GUCTIPI == 0 && AKUVARMI == 1) {
      digitalWrite(BUZZERPIN, HIGH);
    }
  }

  if ((zaman - zamanprevgucgos) >= 0 < 0.5 && GUCTIPI == 1 && SARJOLUYOR == 1 && AKUVARMI == 1) {
    Serial2.print("vis ac,0");
    Serial2.write(nextion_array, 3);
  }

  if ((zaman - zamanprevgucgos) >= 0.5) {
    zamanprevgucgos = zaman;
    if (GUCTIPI == 1 && SARJOLUYOR == 1 && AKUVARMI == 1) {
      Serial2.print("vis ac,1");
      Serial2.write(nextion_array, 3);
    }
  }

  if (akusarjv > 150) {
    akusarjv = 50;
    EEPROM.write(12, akusarjv);
  }

  else if (akusarjv < 50) {
    akusarjv = 150;
    EEPROM.write(12, akusarjv);
  }

  if (smin > 200) {
    smin = 100;
    EEPROM.write(0, smin);
  }

  else if (smin < 100) {
    smin = 200;
    EEPROM.write(0, smin);
  }

  if (smax > 200) {
    smax = 100;
    EEPROM.write(1, smax);
  }

  else if (smax < 100) {
    smax = 200;
    EEPROM.write(1, smax);
  }

  if (hmin > 100) {
    hmin = 0;
    EEPROM.write(2, hmin);
  }

  else if (hmin < 0) {
    hmin = 100;
    EEPROM.write(2, hmin);
  }

  if (hmax > 100) {
    hmax = 0;
    EEPROM.write(3, hmax);
  }

  else if (hmax < 0) {
    hmax = 100;
    EEPROM.write(3, hmax);
  }

  if (turntime > 59) {
    turntime = 0;
    EEPROM.write(4, turntime);
  }

  else if (turntime < 0) {
    turntime = 59;
    EEPROM.write(4, turntime);
  }

  if (gun > 31) {
    gun = 1;
  }

  else if (gun < 1) {
    gun = 31;
  }

  if (ay > 12) {
    ay = 1;
  }

  else if (ay < 1) {
    ay = 12;
  }

  if (yil > 2030) {
    yil = 2018;
  }

  else if (yil < 2018) {
    yil = 2030;
  }

  if (dakika > 59) {
    dakika = 0;
  }

  else if (dakika < 0) {
    dakika = 59;
  }

  if (saat > 23) {
    saat = 0;
  }
  else if (saat < 0) {
    saat = 23;
  }

  if (CEVIRMEDGUNSAYISI > 30) {
    CEVIRMEDGUNSAYISI = 0;
  }
  else if (CEVIRMEDGUNSAYISI < 0) {
    CEVIRMEDGUNSAYISI = 30;
  }

  if (akusarjcurr > 200) {
    akusarjcurr = 0;
  }
  else if (akusarjcurr < 0) {
    akusarjcurr = 200;
  }

  ///////////////////////////////////////////////////  Nextion Data Send Over

  if ((zamanms - pbswitchdelay) > 20) {
    if (SUSEVIYEGOSTERGESI == 0) {
      Serial2.print("vis pb2,1");
      Serial2.write(nextion_array, 3);
      Serial2.print("vis pb1,0");
      Serial2.write(nextion_array, 3);
    }
    else {
      Serial2.print("vis pb2,0");
      Serial2.write(nextion_array, 3);
      Serial2.print("vis pb1,1");
      Serial2.write(nextion_array, 3);
    }

    if (SUSEVIYEBUZZER == 0) {
      Serial2.print("vis pb4,1");
      Serial2.write(nextion_array, 3);
      Serial2.print("vis pb3,0");
      Serial2.write(nextion_array, 3);
    }
    else {
      Serial2.print("vis pb4,0");
      Serial2.write(nextion_array, 3);
      Serial2.print("vis pb3,1");
      Serial2.write(nextion_array, 3);
    }

    if (AKUVARMI == 0) {
      Serial2.print("vis pb6,1");
      Serial2.write(nextion_array, 3);
      Serial2.print("vis pb5,0");
      Serial2.write(nextion_array, 3);
    }
    else {
      Serial2.print("vis pb6,0");
      Serial2.write(nextion_array, 3);
      Serial2.print("vis pb5,1");
      Serial2.write(nextion_array, 3);
    }

    if (SUSEVIYEGOSTERGESI == 0) {
      Serial2.print("vis pb2en,1");
      Serial2.write(nextion_array, 3);
      Serial2.print("vis pb1en,0");
      Serial2.write(nextion_array, 3);
    }
    else {
      Serial2.print("vis pb2en,0");
      Serial2.write(nextion_array, 3);
      Serial2.print("vis pb1en,1");
      Serial2.write(nextion_array, 3);
    }

    if (SUSEVIYEBUZZER == 0) {
      Serial2.print("vis pb4en,1");
      Serial2.write(nextion_array, 3);
      Serial2.print("vis pb3en,0");
      Serial2.write(nextion_array, 3);
    }
    else {
      Serial2.print("vis pb4en,0");
      Serial2.write(nextion_array, 3);
      Serial2.print("vis pb3en,1");
      Serial2.write(nextion_array, 3);
    }

    if (AKUVARMI == 0) {
      Serial2.print("vis pb6en,1");
      Serial2.write(nextion_array, 3);
      Serial2.print("vis pb5en,0");
      Serial2.write(nextion_array, 3);
    }
    else {
      Serial2.print("vis pb6en,0");
      Serial2.write(nextion_array, 3);
      Serial2.print("vis pb5en,1");
      Serial2.write(nextion_array, 3);
    }
    pbswitchdelay = zamanms;
  }

  if (zamanms - EkranYenileme1 > 300) {
    nem = dht.readHumidity();
    Serial2.print("n8.txt=");
    Serial2.print("\"");
    Serial2.print(nem);
    Serial2.print("\"");
    Serial2.write(nextion_array, 3);
    sicaklik = dht.readTemperature();
    Serial2.print("t8.txt=");
    Serial2.print("\"");
    Serial2.print(sicaklik);
    Serial2.print("\"");
    Serial2.write(nextion_array, 3);
    Serial2.print("yazilimsur.txt=");
    Serial2.print("\"");
    Serial2.print(yazilimsurum);
    Serial2.print("\"");
    Serial2.write(nextion_array, 3);
    Serial2.print("pcbsur.txt=");
    Serial2.print("\"");
    Serial2.print(pcbsurum);
    Serial2.print("\"");
    Serial2.write(nextion_array, 3);
    Serial2.print("cputemp.txt=");
    Serial2.print("\"");
    Serial2.print(termistor1);
    Serial2.print("°C");   //°C
    Serial2.print("\"");
    Serial2.write(nextion_array, 3);
    Serial2.print("psutemp.txt=");
    Serial2.print("\"");
    Serial2.print(termistor3);
    Serial2.print("°C");
    Serial2.print("\"");
    Serial2.write(nextion_array, 3);
    Serial2.print("regtemp.txt=");
    Serial2.print("\"");
    Serial2.print(termistor2);
    Serial2.print("°C");
    Serial2.print("\"");
    Serial2.write(nextion_array, 3);
    Serial2.print("psucur.txt=");
    Serial2.print("\"");
    Serial2.print(VCCHATAMP);
    Serial2.print("A");
    Serial2.print("\"");
    Serial2.write(nextion_array, 3);
    Serial2.print("batchargecur.txt=");
    Serial2.print("\"");
    Serial2.print(AKUSARJAMP);
    Serial2.print("A");
    Serial2.print("\"");
    Serial2.write(nextion_array, 3);
    Serial2.print("batdechargecur.txt=");
    Serial2.print("\"");
    Serial2.print(AKUDESARJAMP);
    Serial2.print("A");
    Serial2.print("\"");
    Serial2.write(nextion_array, 3);
    EkranYenileme1 = zamanms;
  }
  if (zamanms - EkranYenileme2 > 50) {
    Serial2.print("t1.txt=");
    Serial2.print("\"");
    Serial2.print(sustdeger);
    Serial2.print("\"");
    Serial2.write(nextion_array, 3);
    Serial2.print("t2.txt=");
    Serial2.print("\"");
    Serial2.print(saltdeger);
    Serial2.print("\"");
    Serial2.write(nextion_array, 3);
    Serial2.print("t3.txt=");
    Serial2.print("\"");
    Serial2.print(nustdeger);
    Serial2.print("\"");
    Serial2.write(nextion_array, 3);
    Serial2.print("t4.txt=");
    Serial2.print("\"");
    Serial2.print(naltdeger);
    Serial2.print("\"");
    Serial2.write(nextion_array, 3);
    EkranYenileme2 = zamanms;
  }
  if (zamanms - EkranYenileme3 > 100) {
    Serial2.print("t10.txt=");
    Serial2.print("\"");
    Serial2.print(rtc.getDateStr());
    Serial2.print("\"");
    Serial2.write(nextion_array, 3);
    Serial2.print("t11.txt=");
    Serial2.print("\"");
    Serial2.print(rtc.getTimeStr());
    Serial2.print("\"");
    Serial2.write(nextion_array, 3);
    Serial2.print("t5.txt=");
    Serial2.print("\"");
    Serial2.print(turntime);
    Serial2.print("\"");
    Serial2.write(nextion_array, 3);
    Serial2.print("ta1.val=");
    Serial2.print(gun);
    Serial2.write(nextion_array, 3);
    Serial2.print("ta2.val=");
    Serial2.print(ay);
    Serial2.write(nextion_array, 3);
    Serial2.print("ta3.val=");
    Serial2.print(yil);
    Serial2.write(nextion_array, 3);
    Serial2.print("t13.txt=");
    Serial2.print("\"");
    Serial2.print(GUNSAYACI);
    Serial2.print(".Gun");
    Serial2.print("\"");
    Serial2.write(nextion_array, 3);

    if (GUNSAYACI == 0) {
      ordinalnumber = " Day";
    }
    else if (GUNSAYACI == 1) {
      ordinalnumber = "st Day";
    }
    else if (GUNSAYACI == 2) {
      ordinalnumber = "nd Day";
    }
    else if (GUNSAYACI == 3) {
      ordinalnumber = "rd Day";
    }
    else if (GUNSAYACI > 20 && ordinal == 1) {
      ordinalnumber = "st Day";
    }
    else if (GUNSAYACI > 20 && ordinal == 2) {
      ordinalnumber = "nd Day";
    }
    else if (GUNSAYACI > 20 && ordinal == 3) {
      ordinalnumber = "rd Day";
    }
    else {
      ordinalnumber = "th Day";
    }
    Serial2.print("t13en.txt=");
    Serial2.print("\"");
    Serial2.print(GUNSAYACI);
    Serial2.print(ordinalnumber);
    Serial2.print("\"");
    Serial2.write(nextion_array, 3);
    Serial2.print("t14.txt=");
    Serial2.print("\"");
    Serial2.print(akuvolt);
    Serial2.print("V");
    Serial2.print("\"");
    Serial2.write(nextion_array, 3);
    Serial2.print("t15a.val=");
    Serial2.print(saat);
    Serial2.write(nextion_array, 3);
    Serial2.print("t15b.val=");
    Serial2.print(dakika);
    Serial2.write(nextion_array, 3);
    Serial2.print("tyuzde.txt=");
    Serial2.print("\"");
    Serial2.print(akuyuzdesi);
    Serial2.print("\"");
    Serial2.write(nextion_array, 3);
    Serial2.print("akusarjv.txt=");
    Serial2.print("\"");
    Serial2.print(akusarjvoltaji);
    Serial2.print("V");
    Serial2.print("\"");
    Serial2.write(nextion_array, 3);
    Serial2.print("akusarjamp.txt=");
    Serial2.print("\"");
    Serial2.print(setcurrentA);
    Serial2.print("A");
    Serial2.print("\"");
    Serial2.write(nextion_array, 3);
    Serial2.print("cgun.txt=");
    Serial2.print("\"");
    Serial2.print(CEVIRMEDGUNSAYISI);
    Serial2.print("\"");
    Serial2.write(nextion_array, 3);
    Serial2.print("ontime.txt=");
    Serial2.print("\"");
    if (millisgun < 10) {
      Serial2.print("0");
    }
    Serial2.print(millisgun);
    Serial2.print(":");
    if (millissaat < 10) {
      Serial2.print("0");
    }
    Serial2.print(millissaat);
    Serial2.print(":");
    if (millisdakika < 10) {
      Serial2.print("0");
    }
    Serial2.print(millisdakika);
    Serial2.print(":");
    if (millissaniye < 10) {
      Serial2.print("0");
    }
    Serial2.print(millissaniye);
    Serial2.print("\"");
    Serial2.write(nextion_array, 3);
    EkranYenileme3 = zamanms;
  }
  if (zamanms - EkranYenileme4 > 100) {
    if (GUCTIPI == 1 && AKUVARMI == 1) {
      Serial2.print("vis dc,0");
      Serial2.write(nextion_array, 3);
      if (SARJOLUYOR == 0) {
        Serial2.print("vis ac,1");
        Serial2.write(nextion_array, 3);
      }
    }
    else if (GUCTIPI == 0 && AKUVARMI == 1) {
      Serial2.print("vis dc,1");
      Serial2.write(nextion_array, 3);
      if (SARJOLUYOR == 0) {
        Serial2.print("vis ac,0");
        Serial2.write(nextion_array, 3);
      }
    }

    if (SUSEVIYEGOSTERGESI == 0) {
      Serial2.print("vis p9,0");
      Serial2.write(nextion_array, 3);
      Serial2.print("vis pic5e,0");
      Serial2.write(nextion_array, 3);
    }
    else {
      Serial2.print("vis p9,1");
      Serial2.write(nextion_array, 3);
      Serial2.print("vis pic5e,1");
      Serial2.write(nextion_array, 3);
    }

    if (akuyuzdesi <= 10) {
      Serial2.print("t14.pco=49152");
      Serial2.write(nextion_array, 3);
      Serial2.print("tyuzde.pco=49152");
      Serial2.write(nextion_array, 3);
    }
    else {
      Serial2.print("t14.pco=65535");
      Serial2.write(nextion_array, 3);
      Serial2.print("tyuzde.pco=65535");
      Serial2.write(nextion_array, 3);
    }

    if (AKUVARMI == 0) {
      Serial2.print("vis obj6e,0");
      Serial2.write(nextion_array, 3);
      Serial2.print("vis taku,0");
      Serial2.write(nextion_array, 3);
      Serial2.print("vis takuen,0");
      Serial2.write(nextion_array, 3);
      Serial2.print("vis p4,0");
      Serial2.write(nextion_array, 3);
      Serial2.print("vis dc,0");
      Serial2.write(nextion_array, 3);
      Serial2.print("vis ac,0");
      Serial2.write(nextion_array, 3);
      Serial2.print("vis t14,0");
      Serial2.write(nextion_array, 3);
      Serial2.print("vis tyuzde,0");
      Serial2.write(nextion_array, 3);
      Serial2.print("vis bat,0");
      Serial2.write(nextion_array, 3);
      AKUVARYOK = 1;
    }
    else {
      Serial2.print("vis obj6e,1");
      Serial2.write(nextion_array, 3);
      Serial2.print("vis taku,1");
      Serial2.write(nextion_array, 3);
      Serial2.print("vis p4,1");
      Serial2.write(nextion_array, 3);
      Serial2.print("vis t14,1");
      Serial2.write(nextion_array, 3);
      Serial2.print("vis tyuzde,1");
      Serial2.write(nextion_array, 3);
      Serial2.print("vis bat,1");
      Serial2.write(nextion_array, 3);

      if (AKUVARYOK == 1) {
        Serial2.print("vis dc,1");
        Serial2.write(nextion_array, 3);
        Serial2.print("vis ac,1");
        Serial2.write(nextion_array, 3);
      }
      AKUVARYOK = 0;
    }
    EkranYenileme4 = zamanms;
  }

  if (sicaklik <= saltdeger) {
    digitalWrite(CH3, LOW);
    kanal3 = 1;
  }
  else if (sicaklik >= sustdeger) {
    digitalWrite(CH3, HIGH);
    kanal3 = 0;
  }

  if (nem <= naltdeger) {
    digitalWrite(CH4, LOW);
    kanal4 = 1;
  }
  else if (nem >= nustdeger) {
    digitalWrite(CH4, HIGH);
    kanal4 = 0;
  }

  if (nem <= nalarm) {
    digitalWrite(CH1, HIGH);
    kanal1 = 0;
  }
  else if (nem >= nustdeger) {
    digitalWrite(CH1, LOW);
    kanal1 = 1;
  }

  if (c == 1 && t.min == 40 && t.sec == 00 && turntime > 0 && GUNSAYACI < CEVIRMEDGUNSAYISI) {
    digitalWrite(CH2, LOW);
    kanal2 = 1;
  }
  else if (c == 1 && t.min == 40 && t.sec == turntime) {
    digitalWrite(CH2, HIGH);
    kanal2 = 0;
  }

  if (t.hour == sayacsaat && t.min == sayacdakika && t.sec == 00) {
    GUNSAYACI = GUNSAYACI + 1;
    EEPROM.write(5, GUNSAYACI);
    delay(1500);
  }

  if (zamanms - EkranYenileme5 > 500) {
    if (kanal1 == 1) {
      ///////////////////TÜRKÇE///////////////////
      Serial2.print("t17.txt=");
      Serial2.print("\"");
      Serial2.print("ACIK");
      Serial2.print("\"");
      Serial2.write(nextion_array, 3);
      Serial2.print("t17.pco=2016");
      Serial2.write(nextion_array, 3);
    }
    else {
      Serial2.print("t17.txt=");
      Serial2.print("\"");
      Serial2.print("KAPALI");
      Serial2.print("\"");
      Serial2.write(nextion_array, 3);
      Serial2.print("t17.pco=63488");
      Serial2.write(nextion_array, 3);
    }

    if (kanal2 == 1) {
      Serial2.print("t18.txt=");
      Serial2.print("\"");
      Serial2.print("ACIK");
      Serial2.print("\"");
      Serial2.write(nextion_array, 3);
      Serial2.print("t18.pco=2016");
      Serial2.write(nextion_array, 3);
    }
    else {
      if (GUNSAYACI < CEVIRMEDGUNSAYISI) {
        Serial2.print("t18.txt=");
        Serial2.print("\"");
        Serial2.print("KAPALI");
        Serial2.print("\"");
        Serial2.write(nextion_array, 3);
        Serial2.print("t18.pco=63488");
        Serial2.write(nextion_array, 3);
      }
      if (GUNSAYACI >= CEVIRMEDGUNSAYISI) {
        Serial2.print("t18.txt=");
        Serial2.print("\"");
        Serial2.print("D.DISI");
        Serial2.print("\"");
        Serial2.write(nextion_array, 3);
        Serial2.print("t18.pco=64520");
        Serial2.write(nextion_array, 3);
      }
    }

    if (kanal3 == 1 && GUCTIPI == 1) {
      Serial2.print("t19.txt=");
      Serial2.print("\"");
      Serial2.print("ACIK");
      Serial2.print("\"");
      Serial2.write(nextion_array, 3);
      Serial2.print("t19.pco=2016");
      Serial2.write(nextion_array, 3);
    }
    else if (kanal3 == 0 && GUCTIPI == 1) {
      Serial2.print("t19.txt=");
      Serial2.print("\"");
      Serial2.print("KAPALI");
      Serial2.print("\"");
      Serial2.write(nextion_array, 3);
      Serial2.print("t19.pco=63488");
      Serial2.write(nextion_array, 3);
    }
    else if (kanal3 == 1 && GUCTIPI == 0 && akuyuzdesi >= 5) {
      Serial2.print("t19.txt=");
      Serial2.print("\"");
      Serial2.print("ACIK");
      Serial2.print("\"");
      Serial2.write(nextion_array, 3);
      Serial2.print("t19.pco=2016");
      Serial2.write(nextion_array, 3);
    }
    else if (kanal3 == 0 && GUCTIPI == 0 && akuyuzdesi >= 5) {
      Serial2.print("t19.txt=");
      Serial2.print("\"");
      Serial2.print("KAPALI");
      Serial2.print("\"");
      Serial2.write(nextion_array, 3);
      Serial2.print("t19.pco=63488");
      Serial2.write(nextion_array, 3);
    }
    else {
      Serial2.print("t19.txt=");
      Serial2.print("\"");
      Serial2.print("D.DISI");
      Serial2.print("\"");
      Serial2.write(nextion_array, 3);
      Serial2.print("t19.pco=64520");
      Serial2.write(nextion_array, 3);
    }

    if (kanal4 == 1) {
      Serial2.print("t20.txt=");
      Serial2.print("\"");
      Serial2.print("ACIK");
      Serial2.print("\"");
      Serial2.write(nextion_array, 3);
      Serial2.print("t20.pco=2016");
      Serial2.write(nextion_array, 3);
    }
    else {
      Serial2.print("t20.txt=");
      Serial2.print("\"");
      Serial2.print("KAPALI");
      Serial2.print("\"");
      Serial2.write(nextion_array, 3);
      Serial2.print("t20.pco=63488");
      Serial2.write(nextion_array, 3);
    }

    if (kanal5 == 1) {
      Serial2.print("taku.txt=");
      Serial2.print("\"");
      Serial2.print("ACIK");
      Serial2.print("\"");
      Serial2.write(nextion_array, 3);
      Serial2.print("taku.pco=2016");
      Serial2.write(nextion_array, 3);
    }
    else {
      Serial2.print("taku.txt=");
      Serial2.print("\"");
      Serial2.print("KAPALI");
      Serial2.print("\"");
      Serial2.write(nextion_array, 3);
      Serial2.print("taku.pco=63488");
      Serial2.write(nextion_array, 3);
    }
    ///////////////////İNGİLİZCE///////////////////
    if (kanal1 == 1) {
      Serial2.print("t17en.txt=");
      Serial2.print("\"");
      Serial2.print("ON");
      Serial2.print("\"");
      Serial2.write(nextion_array, 3);
      Serial2.print("t17en.pco=2016");
      Serial2.write(nextion_array, 3);
    }
    else {
      Serial2.print("t17en.txt=");
      Serial2.print("\"");
      Serial2.print("OFF");
      Serial2.print("\"");
      Serial2.write(nextion_array, 3);
      Serial2.print("t17en.pco=63488");
      Serial2.write(nextion_array, 3);
    }

    if (kanal2 == 1) {
      Serial2.print("t18en.txt=");
      Serial2.print("\"");
      Serial2.print("ON");
      Serial2.print("\"");
      Serial2.write(nextion_array, 3);
      Serial2.print("t18en.pco=2016");
      Serial2.write(nextion_array, 3);
    }
    else {
      if (GUNSAYACI < CEVIRMEDGUNSAYISI) {
        Serial2.print("t18en.txt=");
        Serial2.print("\"");
        Serial2.print("OFF");
        Serial2.print("\"");
        Serial2.write(nextion_array, 3);
        Serial2.print("t18en.pco=63488");
        Serial2.write(nextion_array, 3);
      }
      if (GUNSAYACI >= CEVIRMEDGUNSAYISI) {
        Serial2.print("t18en.txt=");
        Serial2.print("\"");
        Serial2.print("DISABLED");
        Serial2.print("\"");
        Serial2.write(nextion_array, 3);
        Serial2.print("t18en.pco=64520");
        Serial2.write(nextion_array, 3);
      }
    }

    if (kanal3 == 1 && GUCTIPI == 1) {
      Serial2.print("t19en.txt=");
      Serial2.print("\"");
      Serial2.print("ON");
      Serial2.print("\"");
      Serial2.write(nextion_array, 3);
      Serial2.print("t19en.pco=2016");
      Serial2.write(nextion_array, 3);
    }
    else if (kanal3 == 0 && GUCTIPI == 1) {
      Serial2.print("t19en.txt=");
      Serial2.print("\"");
      Serial2.print("OFF");
      Serial2.print("\"");
      Serial2.write(nextion_array, 3);
      Serial2.print("t19en.pco=63488");
      Serial2.write(nextion_array, 3);
    }
    else if (kanal3 == 1 && GUCTIPI == 0 && akuyuzdesi >= 5) {
      Serial2.print("t19en.txt=");
      Serial2.print("\"");
      Serial2.print("ON");
      Serial2.print("\"");
      Serial2.write(nextion_array, 3);
      Serial2.print("t19en.pco=2016");
      Serial2.write(nextion_array, 3);
    }
    else if (kanal3 == 0 && GUCTIPI == 0 && akuyuzdesi >= 5) {
      Serial2.print("t19en.txt=");
      Serial2.print("\"");
      Serial2.print("OFF");
      Serial2.print("\"");
      Serial2.write(nextion_array, 3);
      Serial2.print("t19en.pco=63488");
      Serial2.write(nextion_array, 3);
    }
    else {
      Serial2.print("t19en.txt=");
      Serial2.print("\"");
      Serial2.print("DISABLED");
      Serial2.print("\"");
      Serial2.write(nextion_array, 3);
      Serial2.print("t19en.pco=64520");
      Serial2.write(nextion_array, 3);
    }

    if (kanal4 == 1) {
      Serial2.print("t20en.txt=");
      Serial2.print("\"");
      Serial2.print("ON");
      Serial2.print("\"");
      Serial2.write(nextion_array, 3);
      Serial2.print("t20en.pco=2016");
      Serial2.write(nextion_array, 3);
    }
    else {
      Serial2.print("t20en.txt=");
      Serial2.print("\"");
      Serial2.print("OFF");
      Serial2.print("\"");
      Serial2.write(nextion_array, 3);
      Serial2.print("t20en.pco=63488");
      Serial2.write(nextion_array, 3);
    }

    if (kanal5 == 1) {
      Serial2.print("takuen.txt=");
      Serial2.print("\"");
      Serial2.print("ON");
      Serial2.print("\"");
      Serial2.write(nextion_array, 3);
      Serial2.print("takuen.pco=2016");
      Serial2.write(nextion_array, 3);
    }
    else {
      Serial2.print("takuen.txt=");
      Serial2.print("\"");
      Serial2.print("OFF");
      Serial2.print("\"");
      Serial2.write(nextion_array, 3);
      Serial2.print("takuen.pco=63488");
      Serial2.write(nextion_array, 3);
    }
    EkranYenileme5 = zamanms;
  }

  if (s4 > analogref && s3 > analogref && s2 > analogref && s1 > analogref) { //yüzde 100 su seviyesi
    Serial2.print("p9.val=100");
    Serial2.write(nextion_array, 3);
    if (SUSEVIYEBUZZERAKTIF == 1) {
      digitalWrite(BUZZERPIN, LOW);
      SUSEVIYEBUZZERAKTIF = 0;
    }
  }
  else if (s4 < analogref && s3 > analogref && s2 > analogref && s1 > analogref) { //yüzde 75 su seviyesi
    Serial2.print("p9.val=75");
    Serial2.write(nextion_array, 3);
    if (SUSEVIYEBUZZERAKTIF == 1) {
      digitalWrite(BUZZERPIN, LOW);
      SUSEVIYEBUZZERAKTIF = 0;
    }
  }
  else if (s4 < analogref && s3 < analogref && s2 > analogref && s1 > analogref) { //yüzde 50 su seviyesi
    Serial2.print("p9.val=50");
    Serial2.write(nextion_array, 3);
    if (SUSEVIYEBUZZERAKTIF == 1) {
      digitalWrite(BUZZERPIN, LOW);
      SUSEVIYEBUZZERAKTIF = 0;
    }
  }
  else if (s4 < analogref && s3 < analogref && s2 < analogref && s1 > analogref) { // yüzde 25 su seviyesi
    Serial2.print("p9.val=25");
    Serial2.write(nextion_array, 3);
    if (SUSEVIYEBUZZERAKTIF == 1) {
      digitalWrite(BUZZERPIN, LOW);
      SUSEVIYEBUZZERAKTIF = 0;
    }
  }
  else if (s4 < analogref && s3 < analogref && s2 < analogref && s1 < analogref) { //yüzde 0 su seviyesi
    Serial2.print("p9.val=0");
    Serial2.write(nextion_array, 3);
    if (SUSEVIYEBUZZER == 1) {
      digitalWrite(BUZZERPIN, HIGH);
      SUSEVIYEBUZZERAKTIF = 1;
    }
  }
  if (SUSEVIYEBUZZERAKTIF == 1 && SUSEVIYEBUZZER == 0) {
    digitalWrite(BUZZERPIN, LOW);
  }
}


/**********************************************************************************/ //TR İÇİN DOKUNMATİK PANEL GERİBİLDİRİMİ

void b1PopCallback(void *ptr) //REBOOT butonu
{
  page0.show();
  digitalWrite(RESETPIN, LOW);
}

void b2PushCallback(void *ptr) //sıcaklık üst sınır deger ekleme
{
  smax = smax + 1;
  if (smax > 200) {
    smax = 100;
  }
  EEPROM.write(1, smax);
}

void b3PushCallback(void *ptr) //sıcaklık üst sınır deger çıkartma
{
  smax = smax - 1;
  if (smax < 100) {
    smax = 200;
  }
  EEPROM.write(1, smax);
}

void b4PushCallback(void *ptr) //sıcaklık alt sınır deger ekleme
{
  smin = smin + 1;
  if (smin > 200) {
    smin = 100;
  }
  EEPROM.write(0, smin);
}

void b5PushCallback(void *ptr) //sıcaklık alt sınır deger çıkartma
{
  smin = smin - 1;
  if (smin < 100) {
    smin = 200;
  }
  EEPROM.write(0, smin);
}

void b6PushCallback(void *ptr) //nem üst sınır deger ekleme
{
  hmax = hmax + 1;
  if (hmax > 100) {
    hmax = 0;
  }
  EEPROM.write(3, hmax);
}

void b7PushCallback(void *ptr) //nem üst sınır deger çıkartma
{
  hmax = hmax - 1;
  if (hmax < 0) {
    hmax = 100;
  }
  EEPROM.write(3, hmax);
}

void b8PushCallback(void *ptr) //nem alt sınır deger ekleme
{
  hmin = hmin + 1;
  if (hmin > 100) {
    hmin = 0;
  }
  EEPROM.write(2, hmin);
}

void b9PushCallback(void *ptr) //nem alt sınır deger çıkartma
{
  hmin = hmin - 1;
  if (hmin < 0) {
    hmin = 100;
  }
  EEPROM.write(2, hmin);
}

void b10PushCallback(void *ptr)
{
  saat = (t.hour);
  dakika = (t.min);
}

void b11PushCallback(void *ptr)
{
  yil = (t.year);
  ay = (t.mon);
  gun = (t.date);
}

void b12PushCallback(void *ptr)
{
  turntime = turntime + 1;
  if (turntime > 59) {
    turntime = 0;
  }
  EEPROM.write(4, turntime);
}

void b13PushCallback(void *ptr)
{
  turntime = turntime - 1;
  if (turntime < 0) {
    turntime = 59;
  }
  EEPROM.write(4, turntime);
}

void b14PushCallback(void *ptr)
{
  gun = gun + 1;
  if (gun > 31) {
    gun = 1;
  }
}

void b15PushCallback(void *ptr)
{
  gun = gun - 1;
  if (gun < 1) {
    gun = 31;
  }
}

void b16PushCallback(void *ptr)
{
  ay = ay + 1;
  if (ay > 12) {
    ay = 1;
  }
}

void b17PushCallback(void *ptr)
{
  ay = ay - 1;
  if (ay < 1) {
    ay = 12;
  }
}

void b18PushCallback(void *ptr)
{
  yil = yil + 1;
  if (yil > 2030) {
    yil = 2018;
  }
}

void b19PushCallback(void *ptr)
{
  yil = yil - 1;
  if (yil < 2018) {
    yil = 2030;
  }
}

void b20PushCallback(void *ptr)
{
  rtc.setDate(gun, ay, yil);
  delay(100);
  page1.show();
}

void b21PushCallback(void *ptr)
{
  GUNSAYACI = 0;
  sayacsaat = t.hour;
  sayacdakika = t.min;
  EEPROM.write(5, GUNSAYACI);
  EEPROM.write(6, sayacsaat);
  EEPROM.write(7, sayacdakika);
  delay(100);
  page1.show();
}

void b22PushCallback(void *ptr)
{
  rtc.setTime(saat, dakika, 0);
  delay(100);
  page1.show();
}

void b23PushCallback(void *ptr)
{
  saat = saat + 1;
  if (saat > 23) {
    saat = 0;
  }
}

void b24PushCallback(void *ptr)
{
  saat = saat - 1;
  if (saat < 0) {
    saat = 23;
  }
}

void b25PushCallback(void *ptr)
{
  dakika = dakika + 1;
  if (dakika > 59) {
    dakika = 0;
  }
}

void b26PushCallback(void *ptr)
{
  dakika = dakika - 1;
  if (dakika < 0) {
    dakika = 59;
  }
}

void b27PushCallback(void *ptr)
{
  CEVIRMEDGUNSAYISI = CEVIRMEDGUNSAYISI + 1;
  if (CEVIRMEDGUNSAYISI > 30) {
    CEVIRMEDGUNSAYISI = 0;
  }
  EEPROM.write(11, CEVIRMEDGUNSAYISI);
}

void b28PushCallback(void *ptr)
{
  CEVIRMEDGUNSAYISI = CEVIRMEDGUNSAYISI - 1;
  if (CEVIRMEDGUNSAYISI < 0) {
    CEVIRMEDGUNSAYISI = 30;
  }
  EEPROM.write(11, CEVIRMEDGUNSAYISI);
}

void b30PushCallback(void *ptr)
{
  akusarjv = akusarjv + 1;
  if (akusarjv > 150) {
    akusarjv = 50;
  }
  EEPROM.write(12, akusarjv);
}

void b31PushCallback(void *ptr)
{
  akusarjv = akusarjv - 1;
  if (akusarjv < 50) {
    akusarjv = 150;
  }
  EEPROM.write(12, akusarjv);
}

void b32PushCallback(void *ptr)
{
  if (akuvolt > 5 && AKUVARMI == 1 && GUCTIPI == 1) {
    digitalWrite (CH6, LOW);
    SARJOLUYOR = 1;
    kanal5 = 1;
    page1.show();
  }
}

void b33PushCallback(void *ptr)
{
  LANG = 1;
  EEPROM.write(13, 1);
  page0.show();
  digitalWrite(RESETPIN, LOW);
}

void b34PushCallback(void *ptr)
{
  LANG = 0;
  EEPROM.write(13, 0);
  page0.show();
  digitalWrite(RESETPIN, LOW);
}

void b35PushCallback(void *ptr)
{
  akusarjcurr = akusarjcurr + 5;
  if (akusarjcurr > 200) {
    akusarjcurr = 0;
  }
  EEPROM.write(14, akusarjcurr);
}

void b36PushCallback(void *ptr)
{
  akusarjcurr = akusarjcurr - 5;
  if (akusarjcurr < 0) {
    akusarjcurr = 200;
  }
  EEPROM.write(14, akusarjcurr);
}

void pb1PushCallback(void *ptr)
{
  SUSEVIYEGOSTERGESI = 0;
  EEPROM.write(10, SUSEVIYEGOSTERGESI);
  Serial2.print("vis pb2,1");
  Serial2.write(nextion_array, 3);
  Serial2.print("vis pb1,0");
  Serial2.write(nextion_array, 3);
}

void pb2PushCallback(void *ptr)
{
  SUSEVIYEGOSTERGESI = 1;
  EEPROM.write(10, SUSEVIYEGOSTERGESI);
  Serial2.print("vis pb2,0");
  Serial2.write(nextion_array, 3);
  Serial2.print("vis pb1,1");
  Serial2.write(nextion_array, 3);
}

void pb3PushCallback(void *ptr)
{
  SUSEVIYEBUZZER = 0;
  EEPROM.write(9, SUSEVIYEBUZZER);
  Serial2.print("vis pb4,1");
  Serial2.write(nextion_array, 3);
  Serial2.print("vis pb3,0");
  Serial2.write(nextion_array, 3);
}

void pb4PushCallback(void *ptr)
{
  SUSEVIYEBUZZER = 1;
  EEPROM.write(9, SUSEVIYEBUZZER);
  Serial2.print("vis pb4,0");
  Serial2.write(nextion_array, 3);
  Serial2.print("vis pb3,1");
  Serial2.write(nextion_array, 3);
}

void pb5PushCallback(void *ptr)
{
  AKUVARMI = 0;
  EEPROM.write(8, AKUVARMI);
  Serial2.print("vis pb6,1");
  Serial2.write(nextion_array, 3);
  Serial2.print("vis pb5,0");
  Serial2.write(nextion_array, 3);
}

void pb6PushCallback(void *ptr)
{
  AKUVARMI = 1;
  EEPROM.write(8, AKUVARMI);
  Serial2.print("vis pb6,0");
  Serial2.write(nextion_array, 3);
  Serial2.print("vis pb5,1");
  Serial2.write(nextion_array, 3);
}

/**********************************************************************************/ //EN İÇİN DOKUNMATİK PANEL GERİBİLDİRİMİ

void b1enPopCallback(void *ptr) //REBOOT butonu
{
  page0.show();
  digitalWrite(RESETPIN, LOW);
}

void b2enPushCallback(void *ptr) //sıcaklık üst sınır deger ekleme
{
  smax = smax + 1;
  if (smax > 200) {
    smax = 100;
  }
  EEPROM.write(1, smax);
}

void b3enPushCallback(void *ptr) //sıcaklık üst sınır deger çıkartma
{
  smax = smax - 1;
  if (smax < 100) {
    smax = 200;
  }
  EEPROM.write(1, smax);
}

void b4enPushCallback(void *ptr) //sıcaklık alt sınır deger ekleme
{
  smin = smin + 1;
  if (smin > 200) {
    smin = 100;
  }
  EEPROM.write(0, smin);
}

void b5enPushCallback(void *ptr) //sıcaklık alt sınır deger çıkartma
{
  smin = smin - 1;
  if (smin < 100) {
    smin = 200;
  }
  EEPROM.write(0, smin);
}

void b6enPushCallback(void *ptr) //nem üst sınır deger ekleme
{
  hmax = hmax + 1;
  if (hmax > 100) {
    hmax = 0;
  }
  EEPROM.write(3, hmax);
}

void b7enPushCallback(void *ptr) //nem üst sınır deger çıkartma
{
  hmax = hmax - 1;
  if (hmax < 0) {
    hmax = 100;
  }
  EEPROM.write(3, hmax);
}

void b8enPushCallback(void *ptr) //nem alt sınır deger ekleme
{
  hmin = hmin + 1;
  if (hmin > 100) {
    hmin = 0;
  }
  EEPROM.write(2, hmin);
}

void b9enPushCallback(void *ptr) //nem alt sınır deger çıkartma
{
  hmin = hmin - 1;
  if (hmin < 0) {
    hmin = 100;
  }
  EEPROM.write(2, hmin);
}

void b10enPushCallback(void *ptr)
{
  saat = (t.hour);
  dakika = (t.min);
}

void b11enPushCallback(void *ptr)
{
  yil = (t.year);
  ay = (t.mon);
  gun = (t.date);
}

void b12enPushCallback(void *ptr)
{
  turntime = turntime + 1;
  if (turntime > 59) {
    turntime = 0;
  }
  EEPROM.write(4, turntime);
}

void b13enPushCallback(void *ptr)
{
  turntime = turntime - 1;
  if (turntime < 0) {
    turntime = 59;
  }
  EEPROM.write(4, turntime);
}

void b14enPushCallback(void *ptr)
{
  gun = gun + 1;
  if (gun > 31) {
    gun = 1;
  }
}

void b15enPushCallback(void *ptr)
{
  gun = gun - 1;
  if (gun < 1) {
    gun = 31;
  }
}

void b16enPushCallback(void *ptr)
{
  ay = ay + 1;
  if (ay > 12) {
    ay = 1;
  }
}

void b17enPushCallback(void *ptr)
{
  ay = ay - 1;
  if (ay < 1) {
    ay = 12;
  }
}

void b18enPushCallback(void *ptr)
{
  yil = yil + 1;
  if (yil > 2030) {
    yil = 2018;
  }
}

void b19enPushCallback(void *ptr)
{
  yil = yil - 1;
  if (yil < 2018) {
    yil = 2030;
  }
}

void b20enPushCallback(void *ptr)
{
  rtc.setDate(gun, ay, yil);
  delay(100);
  page6.show();
}

void b21enPushCallback(void *ptr)
{
  GUNSAYACI = 0;
  sayacsaat = t.hour;
  sayacdakika = t.min;
  EEPROM.write(5, GUNSAYACI);
  EEPROM.write(6, sayacsaat);
  EEPROM.write(7, sayacdakika);
  delay(100);
  page6.show();
}

void b22enPushCallback(void *ptr)
{
  rtc.setTime(saat, dakika, 0);
  delay(100);
  page6.show();
}

void b23enPushCallback(void *ptr)
{
  saat = saat + 1;
  if (saat > 23) {
    saat = 0;
  }
}

void b24enPushCallback(void *ptr)
{
  saat = saat - 1;
  if (saat < 0) {
    saat = 23;
  }
}

void b25enPushCallback(void *ptr)
{
  dakika = dakika + 1;
  if (dakika > 59) {
    dakika = 0;
  }
}

void b26enPushCallback(void *ptr)
{
  dakika = dakika - 1;
  if (dakika < 0) {
    dakika = 59;
  }
}

void b27enPushCallback(void *ptr)
{
  CEVIRMEDGUNSAYISI = CEVIRMEDGUNSAYISI + 1;
  if (CEVIRMEDGUNSAYISI > 30) {
    CEVIRMEDGUNSAYISI = 0;
  }
  EEPROM.write(11, CEVIRMEDGUNSAYISI);
}

void b28enPushCallback(void *ptr)
{
  CEVIRMEDGUNSAYISI = CEVIRMEDGUNSAYISI - 1;
  if (CEVIRMEDGUNSAYISI < 0) {
    CEVIRMEDGUNSAYISI = 30;
  }
  EEPROM.write(11, CEVIRMEDGUNSAYISI);
}

void b30enPushCallback(void *ptr)
{
  akusarjv = akusarjv + 1;
  if (akusarjv > 150) {
    akusarjv = 50;
  }
  EEPROM.write(12, akusarjv);
}

void b31enPushCallback(void *ptr)
{
  akusarjv = akusarjv - 1;
  if (akusarjv < 50) {
    akusarjv = 150;
  }
  EEPROM.write(12, akusarjv);
}

void b32enPushCallback(void *ptr)
{
  if (akuvolt > 5 && AKUVARMI == 1 && GUCTIPI == 1) {
    digitalWrite (CH6, LOW);
    SARJOLUYOR = 1;
    kanal5 = 1;
    page6.show();
  }
}

void b33enPushCallback(void *ptr)
{
  LANG = 1;
  EEPROM.write(13, 1);
  page0.show();
  digitalWrite(RESETPIN, LOW);
}

void b34enPushCallback(void *ptr)
{
  LANG = 0;
  EEPROM.write(13, 0);
  page0.show();
  digitalWrite(RESETPIN, LOW);
}

void b35enPushCallback(void *ptr)
{
  akusarjcurr = akusarjcurr + 5;
  if (akusarjcurr > 200) {
    akusarjcurr = 0;
  }
  EEPROM.write(14, akusarjcurr);
}

void b36enPushCallback(void *ptr)
{
  akusarjcurr = akusarjcurr - 5;
  if (akusarjcurr < 0) {
    akusarjcurr = 200;
  }
  EEPROM.write(14, akusarjcurr);
}

void pb1enPushCallback(void *ptr)
{
  SUSEVIYEGOSTERGESI = 0;
  EEPROM.write(10, SUSEVIYEGOSTERGESI);
  Serial2.print("vis pb2en,1");
  Serial2.write(nextion_array, 3);
  Serial2.print("vis pb1en,0");
  Serial2.write(nextion_array, 3);
}

void pb2enPushCallback(void *ptr)
{
  SUSEVIYEGOSTERGESI = 1;
  EEPROM.write(10, SUSEVIYEGOSTERGESI);
  Serial2.print("vis pb2en,0");
  Serial2.write(nextion_array, 3);
  Serial2.print("vis pb1en,1");
  Serial2.write(nextion_array, 3);
}

void pb3enPushCallback(void *ptr)
{
  SUSEVIYEBUZZER = 0;
  EEPROM.write(9, SUSEVIYEBUZZER);
  Serial2.print("vis pb4en,1");
  Serial2.write(nextion_array, 3);
  Serial2.print("vis pb3en,0");
  Serial2.write(nextion_array, 3);
}

void pb4enPushCallback(void *ptr)
{
  SUSEVIYEBUZZER = 1;
  EEPROM.write(9, SUSEVIYEBUZZER);
  Serial2.print("vis pb4en,0");
  Serial2.write(nextion_array, 3);
  Serial2.print("vis pb3en,1");
  Serial2.write(nextion_array, 3);
}

void pb5enPushCallback(void *ptr)
{
  AKUVARMI = 0;
  EEPROM.write(8, AKUVARMI);
  Serial2.print("vis pb6en,1");
  Serial2.write(nextion_array, 3);
  Serial2.print("vis pb5en,0");
  Serial2.write(nextion_array, 3);
}

void pb6enPushCallback(void *ptr)
{
  AKUVARMI = 1;
  EEPROM.write(8, AKUVARMI);
  Serial2.print("vis pb6en,0");
  Serial2.write(nextion_array, 3);
  Serial2.print("vis pb5en,1");
  Serial2.write(nextion_array, 3);
}

/**********************************************************************************/
