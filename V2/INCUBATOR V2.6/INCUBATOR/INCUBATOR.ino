/*H.Selim Serdar Kuluçka Makinesi Projesi Program Sürüm V2.6*/

#include "Nextion.h"
#include <DHT.h>
#include <EEPROM.h>
#include <DS3231.h>
#include <Wire.h>
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
#define AKUGIRIS A6

DHT dht(DHTPIN, DHTTYPE); //n8-nem, t8-sıcaklık
DS3231 rtc(SDA, SCL);
Time  t;

NexPage page0 = NexPage(0, 0, "INTRO");
NexPage page1 = NexPage(1, 0, "MAIN");
NexPage page2 = NexPage(2, 0, "SETTINGS");
NexPage page3 = NexPage(3, 0, "SETTINGS2");
NexPage page4 = NexPage(4, 0, "SETTINGS3");
NexPage page5 = NexPage(5, 0, "SETTINGS4");
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
NexButton b30 = NexButton(5, 9, "b30");
NexButton b31 = NexButton(5, 10, "b31");
NexButton b32 = NexButton(5, 13, "b32");
NexDSButton bt1 = NexDSButton(5, 4, "bt1");
NexDSButton bt2 = NexDSButton(5, 6, "bt2");
NexDSButton bt3 = NexDSButton(5, 8, "bt3");

NexTouch *nex_listen_list[] =
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
  &bt1,
  &bt2,
  &bt3,
  NULL
};

unsigned long zamanms = 0; //Kod başladıktan sonra geçen zaman (ms cinsinden) NOT:MAX 50 GÜN
unsigned long zaman = 0; //Kod başladıktan sonra geçen zaman (sn cinsinden)
unsigned long zamanprevgucgos = 0; //En son kaydedilen kod başladıktan sonra geçen zaman (sn cinsinden)
unsigned long zamanprevbuz = 0; //En son kaydedilen kod başladıktan sonra geçen zaman (sn cinsinden)
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
byte analogref = 200; //su seviyesi ölçümü için analog girişi voltaj referansı
boolean GUCTIPI; //AC, DC değerini saklayacak olan değişken; 1'de iken AC, 0'da iken DC güç
byte SARJOLUYOR = 0; //Akünün şarj durumunu saklayan değişken
byte SUSEVIYEBUZZERAKTIF = 0;
byte AKUVARYOK = 0;
byte AKUVARMI; //Akünün kullanılıp kullanılmayacağını saklayan değişken; 1 ise, AKÜ KULLANILIYOR
byte SUSEVIYEBUZZER; //Su seviyesi alarmının kullanılıp kullanılmayacağını saklayan değişken; 1 ise kullanılıyor
byte SUSEVIYEGOSTERGESI; //Su seviyesi göstergesinin kullanılıp kullanılmayacağını saklayan değişken; 1 ise kullanılıyor
int CEVIRMEDGUNSAYISI; //Maksimum yumurta çevirme gün sayısı
char nextion_array[3] = {0xFF, 0xFF, 0xFF};
char buffer[100] = {0};

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
  pinMode(AKUGIRIS, INPUT);
  pinMode(GERILIMPIN, INPUT);
  digitalWrite(RESETPIN, HIGH);
  pinMode(RESETPIN, OUTPUT);
  digitalWrite(RESETPIN, HIGH);
  Serial.begin(9600);
  Serial.print("baud=115200");
  Serial.write(nextion_array, 3);
  Serial.end();
  Serial.begin(115200);
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
  b29.attachPush(b29PushCallback, &b29);
  b30.attachPush(b30PushCallback, &b30);
  b31.attachPush(b31PushCallback, &b31);
  b32.attachPush(b32PushCallback, &b32);
  bt1.attachPop(bt1PopCallback, &bt1);
  bt2.attachPop(bt2PopCallback, &bt2);
  bt3.attachPop(bt3PopCallback, &bt3);
  ads.setGain(GAIN_ONE); // 1x gain   +/- 4.096V  1 bit = 0.125mV
  dht.begin();
  rtc.begin();
  ads.begin();
  page0.show();
  Serial.print("page INTRO");
  Serial.write(nextion_array, 3);
  Serial.print("p1.val=0");
  Serial.write(nextion_array, 3);
  Serial.print("p2.val=0");
  Serial.write(nextion_array, 3);
  for (byte p = 0 ; p < 101 ; p++) {
    Serial.print("p1.val=");
    Serial.print(p);
    Serial.write(nextion_array, 3);
    delay(30);
  }
  for (byte p2 = 0 ; p2 < 101 ; p2++) {
    Serial.print("p2.val=");
    Serial.print(p2);
    Serial.write(nextion_array, 3);
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
  page1.show();
  Serial.print("page MAIN");
  Serial.write(nextion_array, 3);
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
  t = rtc.getTime();
  yil = (t.year);
  ay = (t.mon);
  gun = (t.date);
  saat = (t.hour);
  dakika = (t.min);
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
  akuvolt = (vgiris / (R2 / (R1 + R2)));
  zamanms = millis();
  zaman = zamanms / 1000;
  saltdeger = (200 + smin) / a;
  sustdeger = (200 + smax) / a;
  naltdeger = hmin;
  nustdeger = hmax;
  nalarm = (nustdeger + 5);
  t = rtc.getTime();
  c = t.hour % 2;
  s1 = analogRead(seviye1);
  s2 = analogRead(seviye2);
  s3 = analogRead(seviye3);
  s4 = analogRead(seviye4);
  akusarjvoltaji = 12 + (akusarjv / 50);

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

  if (akuvolt > akusarjvoltaji && GUCTIPI == 1 && AKUVARMI == 1) {
    digitalWrite (CH6, HIGH);
    analogWrite(AKUSARJAKIMI, 0);
    SARJOLUYOR = 0;
    kanal5 = 0;
  }

  if (akuvolt < 12.5 && akuvolt > 5 && AKUVARMI == 1 && GUCTIPI == 1) {
    digitalWrite (CH6, LOW);
    analogWrite(AKUSARJAKIMI, 100);
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
    Serial.print("vis ac,0");
    Serial.write(nextion_array, 3);
  }

  if ((zaman - zamanprevgucgos) >= 0.5) {
    zamanprevgucgos = zaman;
    if (GUCTIPI == 1 && SARJOLUYOR == 1 && AKUVARMI == 1) {
      Serial.print("vis ac,1");
      Serial.write(nextion_array, 3);
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

  nem = dht.readHumidity();
  Serial.print("n8.txt=");
  Serial.print("\"");
  Serial.print(nem);
  Serial.print("\"");
  Serial.write(nextion_array, 3);

  sicaklik = dht.readTemperature();
  Serial.print("t8.txt=");
  Serial.print("\"");
  Serial.print(sicaklik);
  Serial.print("\"");
  Serial.write(nextion_array, 3);

  Serial.print("t1.txt=");
  Serial.print("\"");
  Serial.print(sustdeger);
  Serial.print("\"");
  Serial.write(nextion_array, 3);

  Serial.print("t2.txt=");
  Serial.print("\"");
  Serial.print(saltdeger);
  Serial.print("\"");
  Serial.write(nextion_array, 3);

  Serial.print("t3.txt=");
  Serial.print("\"");
  Serial.print(nustdeger);
  Serial.print("\"");
  Serial.write(nextion_array, 3);

  Serial.print("t4.txt=");
  Serial.print("\"");
  Serial.print(naltdeger);
  Serial.print("\"");
  Serial.write(nextion_array, 3);

  Serial.print("t10.txt=");
  Serial.print("\"");
  Serial.print(rtc.getDateStr());
  Serial.print("\"");
  Serial.write(nextion_array, 3);

  Serial.print("t11.txt=");
  Serial.print("\"");
  Serial.print(rtc.getTimeStr());
  Serial.print("\"");
  Serial.write(nextion_array, 3);

  Serial.print("t5.txt=");
  Serial.print("\"");
  Serial.print(turntime);
  Serial.print("\"");
  Serial.write(nextion_array, 3);

  Serial.print("ta1.val=");
  Serial.print(gun);
  Serial.write(nextion_array, 3);
  Serial.print("ta2.val=");
  Serial.print(ay);
  Serial.write(nextion_array, 3);
  Serial.print("ta3.val=");
  Serial.print(yil);
  Serial.write(nextion_array, 3);

  Serial.print("t13.txt=");
  Serial.print("\"");
  Serial.print(GUNSAYACI);
  Serial.print(".Gun");
  Serial.print("\"");
  Serial.write(nextion_array, 3);

  Serial.print("t14.txt=");
  Serial.print("\"");
  Serial.print(akuvolt);
  Serial.print("V");
  Serial.print("\"");
  Serial.write(nextion_array, 3);

  Serial.print("t15a.val=");
  Serial.print(saat);
  Serial.write(nextion_array, 3);
  Serial.print("t15b.val=");
  Serial.print(dakika);
  Serial.write(nextion_array, 3);

  Serial.print("tyuzde.txt=");
  Serial.print("\"");
  Serial.print(akuyuzdesi);
  Serial.print("\"");
  Serial.write(nextion_array, 3);

  Serial.print("akusarjv.txt=");
  Serial.print("\"");
  Serial.print(akusarjvoltaji);
  Serial.print("\"");
  Serial.write(nextion_array, 3);

  Serial.print("cgun.txt=");
  Serial.print("\"");
  Serial.print(CEVIRMEDGUNSAYISI);
  Serial.print("\"");
  Serial.write(nextion_array, 3);

  if (GUCTIPI == 1 && AKUVARMI == 1) {
    Serial.print("vis dc,0");
    Serial.write(nextion_array, 3);
    if (SARJOLUYOR == 0) {
      Serial.print("vis ac,1");
      Serial.write(nextion_array, 3);
    }
  }
  else if (GUCTIPI == 0 && AKUVARMI == 1) {
    Serial.print("vis dc,1");
    Serial.write(nextion_array, 3);
    if (SARJOLUYOR == 0) {
      Serial.print("vis ac,0");
      Serial.write(nextion_array, 3);
    }
  }

  if (SUSEVIYEGOSTERGESI == 0) {
    Serial.print("vis p9,0");
    Serial.write(nextion_array, 3);
    Serial.print("vis pic5e,0");
    Serial.write(nextion_array, 3);
  }
  else {
    Serial.print("vis p9,1");
    Serial.write(nextion_array, 3);
    Serial.print("vis pic5e,1");
    Serial.write(nextion_array, 3);
  }

  if (AKUVARMI == 0) {
    Serial.print("vis obj6e,0");
    Serial.write(nextion_array, 3);
    Serial.print("vis taku,0");
    Serial.write(nextion_array, 3);
    Serial.print("vis p4,0");
    Serial.write(nextion_array, 3);
    Serial.print("vis dc,0");
    Serial.write(nextion_array, 3);
    Serial.print("vis ac,0");
    Serial.write(nextion_array, 3);
    Serial.print("vis t14,0");
    Serial.write(nextion_array, 3);
    Serial.print("vis tyuzde,0");
    Serial.write(nextion_array, 3);
    Serial.print("vis bat,0");
    Serial.write(nextion_array, 3);
    AKUVARYOK = 1;
  }
  else {
    Serial.print("vis obj6e,1");
    Serial.write(nextion_array, 3);
    Serial.print("vis taku,1");
    Serial.write(nextion_array, 3);
    Serial.print("vis p4,1");
    Serial.write(nextion_array, 3);
    Serial.print("vis t14,1");
    Serial.write(nextion_array, 3);
    Serial.print("vis tyuzde,1");
    Serial.write(nextion_array, 3);
    Serial.print("vis bat,1");
    Serial.write(nextion_array, 3);
    if (AKUVARYOK == 1) {
      Serial.write(nextion_array, 3);
      Serial.print("vis dc,1");
      Serial.write(nextion_array, 3);
      Serial.print("vis ac,1");
    }
    AKUVARYOK = 0;
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

  if (akuyuzdesi <= 10) {
    Serial.print("t14.pco=49152");
    Serial.write(nextion_array, 3);
    Serial.print("tyuzde.pco=49152");
    Serial.write(nextion_array, 3);
  }
  else {
    Serial.print("t14.pco=65535");
    Serial.write(nextion_array, 3);
    Serial.print("tyuzde.pco=65535");
    Serial.write(nextion_array, 3);
  }

  if (kanal1 == 1) {
    Serial.print("t17.txt=");
    Serial.print("\"");
    Serial.print("ACIK");
    Serial.print("\"");
    Serial.write(nextion_array, 3);
    Serial.print("t17.pco=2016");
    Serial.write(nextion_array, 3);
  }
  else {
    Serial.print("t17.txt=");
    Serial.print("\"");
    Serial.print("KAPALI");
    Serial.print("\"");
    Serial.write(nextion_array, 3);
    Serial.print("t17.pco=63488");
    Serial.write(nextion_array, 3);
  }

  if (kanal2 == 1) {
    Serial.print("t18.txt=");
    Serial.print("\"");
    Serial.print("ACIK");
    Serial.print("\"");
    Serial.write(nextion_array, 3);
    Serial.print("t18.pco=2016");
    Serial.write(nextion_array, 3);
  }
  else {
    if (GUNSAYACI < CEVIRMEDGUNSAYISI) {
      Serial.print("t18.txt=");
      Serial.print("\"");
      Serial.print("KAPALI");
      Serial.print("\"");
      Serial.write(nextion_array, 3);
      Serial.print("t18.pco=63488");
      Serial.write(nextion_array, 3);
    }
    if (GUNSAYACI == CEVIRMEDGUNSAYISI) {
      Serial.print("t18.txt=");
      Serial.print("\"");
      Serial.print("D.DISI");
      Serial.print("\"");
      Serial.write(nextion_array, 3);
      Serial.print("t18.pco=64520");
      Serial.write(nextion_array, 3);
    }
  }

  if (kanal3 == 1 && GUCTIPI == 1) {
    Serial.print("t19.txt=");
    Serial.print("\"");
    Serial.print("ACIK");
    Serial.print("\"");
    Serial.write(nextion_array, 3);
    Serial.print("t19.pco=2016");
    Serial.write(nextion_array, 3);
  }
  else if (kanal3 == 0 && GUCTIPI == 1) {
    Serial.print("t19.txt=");
    Serial.print("\"");
    Serial.print("KAPALI");
    Serial.print("\"");
    Serial.write(nextion_array, 3);
    Serial.print("t19.pco=63488");
    Serial.write(nextion_array, 3);
  }
  else if (kanal3 == 1 && GUCTIPI == 0 && akuyuzdesi >= 5) {
    Serial.print("t19.txt=");
    Serial.print("\"");
    Serial.print("ACIK");
    Serial.print("\"");
    Serial.write(nextion_array, 3);
    Serial.print("t19.pco=2016");
    Serial.write(nextion_array, 3);
  }
  else if (kanal3 == 0 && GUCTIPI == 0 && akuyuzdesi >= 5) {
    Serial.print("t19.txt=");
    Serial.print("\"");
    Serial.print("KAPALI");
    Serial.print("\"");
    Serial.write(nextion_array, 3);
    Serial.print("t19.pco=63488");
    Serial.write(nextion_array, 3);
  }
  else {
    Serial.print("t19.txt=");
    Serial.print("\"");
    Serial.print("D.DISI");
    Serial.print("\"");
    Serial.write(nextion_array, 3);
    Serial.print("t19.pco=64520");
    Serial.write(nextion_array, 3);
  }

  if (kanal4 == 1) {
    Serial.print("t20.txt=");
    Serial.print("\"");
    Serial.print("ACIK");
    Serial.print("\"");
    Serial.write(nextion_array, 3);
    Serial.print("t20.pco=2016");
    Serial.write(nextion_array, 3);
  }
  else {
    Serial.print("t20.txt=");
    Serial.print("\"");
    Serial.print("KAPALI");
    Serial.print("\"");
    Serial.write(nextion_array, 3);
    Serial.print("t20.pco=63488");
    Serial.write(nextion_array, 3);
  }

  if (kanal5 == 1) {
    Serial.print("taku.txt=");
    Serial.print("\"");
    Serial.print("ACIK");
    Serial.print("\"");
    Serial.write(nextion_array, 3);
    Serial.print("taku.pco=2016");
    Serial.write(nextion_array, 3);
  }
  else {
    Serial.print("taku.txt=");
    Serial.print("\"");
    Serial.print("KAPALI");
    Serial.print("\"");
    Serial.write(nextion_array, 3);
    Serial.print("taku.pco=63488");
    Serial.write(nextion_array, 3);
  }

  if (s4 > analogref && s3 > analogref && s2 > analogref && s1 > analogref) { //yüzde 100 su seviyesi
    Serial.print("p9.val=100");
    Serial.write(nextion_array, 3);
    if (SUSEVIYEBUZZERAKTIF == 1) {
      digitalWrite(BUZZERPIN, LOW);
      SUSEVIYEBUZZERAKTIF = 0;
    }
  }
  else if (s4 < analogref && s3 > analogref && s2 > analogref && s1 > analogref) { //yüzde 75 su seviyesi
    Serial.print("p9.val=75");
    Serial.write(nextion_array, 3);
    if (SUSEVIYEBUZZERAKTIF == 1) {
      digitalWrite(BUZZERPIN, LOW);
      SUSEVIYEBUZZERAKTIF = 0;
    }
  }
  else if (s4 < analogref && s3 < analogref && s2 > analogref && s1 > analogref) { //yüzde 50 su seviyesi
    Serial.print("p9.val=50");
    Serial.write(nextion_array, 3);
    if (SUSEVIYEBUZZERAKTIF == 1) {
      digitalWrite(BUZZERPIN, LOW);
      SUSEVIYEBUZZERAKTIF = 0;
    }
  }
  else if (s4 < analogref && s3 < analogref && s2 < analogref && s1 > analogref) { // yüzde 25 su seviyesi
    Serial.print("p9.val=25");
    Serial.write(nextion_array, 3);
    if (SUSEVIYEBUZZERAKTIF == 1) {
      digitalWrite(BUZZERPIN, LOW);
      SUSEVIYEBUZZERAKTIF = 0;
    }
  }
  else if (s4 < analogref && s3 < analogref && s2 < analogref && s1 < analogref) { //yüzde 0 su seviyesi
    Serial.print("p9.val=0");
    Serial.write(nextion_array, 3);
    if (SUSEVIYEBUZZER == 1) {
      digitalWrite(BUZZERPIN, HIGH);
      SUSEVIYEBUZZERAKTIF = 1;
    }
  }
  if (SUSEVIYEBUZZERAKTIF == 1 && SUSEVIYEBUZZER == 0) {
    digitalWrite(BUZZERPIN, LOW);
  }
}


/**********************************************************************************/

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

void b29PushCallback(void *ptr)
{

  if (SUSEVIYEGOSTERGESI == 1) {
    Serial.print("bt1.val=1");
    Serial.write(nextion_array, 3);
  }
  else {
    Serial.print("bt1.val=0");
    Serial.write(nextion_array, 3);
  }

  if (SUSEVIYEBUZZER == 1) {
    Serial.print("bt2.val=1");
    Serial.write(nextion_array, 3);
  }
  else {
    Serial.print("bt2.val=0");
    Serial.write(nextion_array, 3);
  }

  if (AKUVARMI == 1) {
    Serial.print("bt3.val=1");
    Serial.write(nextion_array, 3);
  }
  else {
    Serial.print("bt3.val=0");
    Serial.write(nextion_array, 3);
  }
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
    analogWrite(AKUSARJAKIMI, 100);
    SARJOLUYOR = 1;
    kanal5 = 1;
    page1.show();
  }
}

void bt1PopCallback(void *ptr)
{
  uint32_t dual_state;
  NexDSButton *btn = (NexDSButton *)ptr;
  dbSerialPrintln("bt1PopCallback");
  dbSerialPrint("ptr=");
  dbSerialPrintln((uint32_t)ptr);
  memset(buffer, 0, sizeof(buffer));

  bt1.getValue(&dual_state);
  if (dual_state) {
    SUSEVIYEGOSTERGESI = 1;
    EEPROM.write(10, SUSEVIYEGOSTERGESI);
  }
  else {
    SUSEVIYEGOSTERGESI = 0;
    EEPROM.write(10, SUSEVIYEGOSTERGESI);
  }
}

void bt2PopCallback(void *ptr)
{
  uint32_t dual_state;
  NexDSButton *btn = (NexDSButton *)ptr;
  dbSerialPrintln("bt2PopCallback");
  dbSerialPrint("ptr=");
  dbSerialPrintln((uint32_t)ptr);
  memset(buffer, 0, sizeof(buffer));

  bt2.getValue(&dual_state);
  if (dual_state) {
    SUSEVIYEBUZZER = 1;
    EEPROM.write(9, SUSEVIYEBUZZER);
  }
  else {
    SUSEVIYEBUZZER = 0;
    EEPROM.write(9, SUSEVIYEBUZZER);
  }
}

void bt3PopCallback(void *ptr)
{
  uint32_t dual_state;
  NexDSButton *btn = (NexDSButton *)ptr;
  dbSerialPrintln("bt3PopCallback");
  dbSerialPrint("ptr=");
  dbSerialPrintln((uint32_t)ptr);
  memset(buffer, 0, sizeof(buffer));

  bt3.getValue(&dual_state);
  if (dual_state) {
    AKUVARMI = 1;
    EEPROM.write(8, AKUVARMI);
  }
  else {
    AKUVARMI = 0;
    EEPROM.write(8, AKUVARMI);
  }
}

/**********************************************************************************/
