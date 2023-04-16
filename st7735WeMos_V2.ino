/*
 * Dr.TRonik / YouTube / NİSAN 2023 / İzmir / Türkiye 
 * ESP8266, ST7735s, 128x160 1.8" TFT LCD, GYBMEP...
 * Derleme ve karta yükleme öncesi, tüm kütüphaneler arduino ide'sine yüklenmiş olmalıdır...
 * YouTube: https://youtu.be/gqybujI4pcU
 * Bağlantılar: https://github.com/ronaer/ST7735s/blob/main/gybmep_bb.jpg
 * CS_D2, RST_D0, DC_D1, SCL_D5, SDA_D7, VDD_5V, GND_GND, BLK_3.3V
*/

/********************************************************************
  GLOBALS___GLOBALS___GLOBALS___GLOBALS___GLOBALS___GLOBALS___
 ********************************************************************/
//ESP8266
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

//ST7735
#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7735.h>  // Hardware-specific library for ST7735
#include <SPI.h>              // SPI comminication
//ESP8266 için pin bağlantıları - Pin Connections for ESP8266
#define TFT_CS 4              // D2
#define TFT_RST 16            // D0
#define TFT_DC 5              // D1
//SCL D5
//SDA D7
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

//İnternet üzerinden zamanı alabilme - TimeClient
#include <NTPClient.h>
#include <Time.h>
#include <TimeLib.h>
#include "Timezone.h"

//BMEP
#include <Wire.h>
#include <Adafruit_BME280.h>  //https://github.com/adafruit/Adafruit_BME280_Library
#include <Adafruit_Sensor.h>

//#define SEALEVELPRESSURE_HPA (1014.0)  // Doğru yükseklik için yerel meteorolojik kaynaklardan doğru olarak güncellenmeli
Adafruit_BME280 bme;

//SSID ve Şifre Tanımlama - SSID PASS
#define STA_SSID "Dr.TRonik"
#define STA_PASSWORD "Dr.TRonik"

//Zaman intervalleri - Time Intervals
#define NTP_OFFSET 60 * 60             // In seconds
#define NTP_INTERVAL 60 * 1000         // In miliseconds, dk da bir güncelleme
#define NTP_ADDRESS "tr.pool.ntp.org"  // The nearest ntp pool
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_ADDRESS, NTP_OFFSET, NTP_INTERVAL);

//Değişkenler - Variables
time_t local, utc;
bool connected = false;
unsigned long last_second;
int temp, hum, pressure, altitude;
int saat, dakika, saniye, gun, ay, dayWeek, yil;
char gun_isimleri[8][10] = { " ", "PAZAR", "PAZARTESi", "SALI", "CARSAMBA", "PERSEMBE", "CUMA", "CUMARTESi" };

/********************************************************************
  SETUP___SETUP___SETUP___SETUP___SETUP___SETUP___SETUP___SETUP___
 ********************************************************************/
void setup() {
  Serial.begin(9600);
  Wire.begin(2, 0);
  last_second = millis();

  tft.initR(INITR_BLACKTAB);     // initialize a ST7735S chip, black tab
  tft.setTextWrap(false);        // Allow text to run off right edge
  tft.fillScreen(ST77XX_BLACK);  // Ekranı temizlemek VE canvası siyah yapabilmek için
  tft.setRotation(3);            // 0_0°, 1_90°, 2_180°, 3_270° rotasyon...

  timeClient.begin();  // Start the NTP UDP client

  Serial.print("Trying WiFi connection to ");
  Serial.println(STA_SSID);

  WiFi.setAutoReconnect(true);
  WiFi.begin(STA_SSID, STA_PASSWORD);

  //Sensör I²C adresimzi kütüphaneye bildirelim, klon modüllerde adres, kütüphanedeki adresten farklı olabilir...
  unsigned status;           
  status = bme.begin(0x76);
  //Sensör mödüllerinde adres, kütüphaneden farklı ise parantez içinde yer almalı...

  read_data();
}

/********************************************************************
  LOOP__LOOP__LOOP__LOOP__LOOP__LOOP__LOOP__LOOP__LOOP__LOOP__LOOP__
 ********************************************************************/
void loop() {

  static unsigned long timer = millis();

  //Handle Connection to Access Point
  if (WiFi.status() == WL_CONNECTED) {
    if (!connected) {
      connected = true;
      Serial.println("");
      Serial.print("Connected to ");
      Serial.println(STA_SSID);
      Serial.println("WiFi connected");
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
    }
  } else {
    if (connected) {
      connected = false;
      Serial.print("Disonnected from ");
      Serial.println(STA_SSID);
    }
  }

  //___millis başlangıcı___
  if (millis() - last_second > 1000) {
    last_second = millis();

    timeClient.update();
    unsigned long epochTime = timeClient.getEpochTime();

    // convert received time stamp to time_t object

    utc = epochTime;

    // Then convert the UTC UNIX timestamp to local time
    // Eastern Daylight Time (EDT); Türkiye: sabit +UTC+3 nedeni ile  +2saat = +120dk ayarlanmalı
    // Eastern Time Zone: UTC- change this as needed; Türkiye için ulusal saat değişikliklerine göre kasım ve mart aylarında güncelleme isteyebilir
    TimeChangeRule usEDT = { "EDT", Second, Sun, Mar, 2, +120 };
    TimeChangeRule usEST = { "EST", First, Sun, Nov, 2 };
    Timezone usEastern(usEDT, usEST);
    local = usEastern.toLocal(utc);

    saat = hour(local);  // hour(local); -->14:42  hourFormat12(local)--> 02:42;
    dakika = minute(local);
    saniye = second(local);
    gun = day(local);
    ay = month(local);
    yil = year(local);
    dayWeek = weekday(local);  //Haftanın günleri...

    //Gün, ay, yıl, haftanın günü verilerini tek bir veri olarak ekrana yazdırabilimek için verileri char takvim_ dizisine biçimlendirerek aktaralım...
    char takvim_[22];
    sprintf(takvim_, "%02d.%02d.%04d / %-9s", gun, ay, yil, gun_isimleri[dayWeek]);
    tft.setCursor(15, 0);
    tft.setTextColor(0xF81F, 0x0000);  // Veriler değiştiğinde ekranda üst üste binmesini engeller...
    tft.setTextSize(1);
    tft.print(takvim_);

    yaz();
  }  
  //___millis sonu___

  //Dakikada bir sensörden veri okumak için...
  if (millis() - timer > 1 * 60000) {
    timer = millis();
    read_data();
  }
}  

/********************************************************************
  VOIDs__VOIDs__VOIDs__VOIDs__VOIDs__VOIDs__VOIDs__VOIDs__VOIDs__VOIDs
********************************************************************/
void yaz() {
  char saat_[6];  //5 characters + null
  char sec[3];    //2 characters + null
  sprintf(saat_, "%02d:%02d", saat, dakika);
  sprintf(sec, "%02d", saniye);
  if ((saat >= 7) && (saat <= 22)) {
    tft.setTextColor(0xFFFF, 0x0000);  // tft.fillScreen(0); -->Causes flicker...
  } else {
    tft.setTextColor(0xF81F, 0x0000);
  }
  tft.setTextSize(5);
  tft.setCursor(0, 15);
  tft.print(saat_);

  tft.setTextSize(1);
  tft.setCursor(149, 15);
  tft.print(sec);
}

// Sensörden veri okuma ve okunan verinin tft ekrana yazdırılması...
void read_data() {
  temp = bme.readTemperature();
  hum = bme.readHumidity();
  pressure = bme.readPressure() / 100.0F;

  char temp_[3];
  sprintf(temp_, "%02d", temp);
  tft.setCursor(0, 65);
  if ((saat >= 7) && (saat <= 22)) {
    tft.setTextColor(0x001F, 0x0000);
  } else {
    tft.setTextColor(0xF81F, 0x0000);
  }
  tft.setTextSize(4);
  tft.print(temp_);

  tft.setCursor(60, 68);
  tft.setTextColor(ST77XX_GREEN);
  tft.setTextSize(2);
  tft.print("C");
  tft.drawCircle(53, 68, 3, ST77XX_GREEN);

  char hum_[3];
  sprintf(hum_, "%02d", hum);
  tft.setCursor(112, 65);
  if ((saat >= 7) && (saat <= 22)) {
    tft.setTextColor(0x001F, 0x0000);
  } else {
    tft.setTextColor(0xF81F, 0x0000);
  }

  tft.setTextSize(4);
  tft.print(hum_);

  tft.setCursor(93, 65);
  tft.setTextColor(ST77XX_GREEN);
  tft.setTextSize(2);
  tft.print("%");

  tft.setCursor(91, 85);
  tft.setTextColor(ST77XX_GREEN);
  tft.setTextSize(1);
  tft.print("NEM");

  char pressure_[3];
  sprintf(pressure_, "%02d", pressure);
  tft.setCursor(65, 108);
  if ((saat >= 7) && (saat <= 22)) {
    tft.setTextColor(0xF800, 0x0000);
  } else {
    tft.setTextColor(0xF81F, 0x0000);  
  }
  tft.setTextSize(3);
  tft.print(pressure_);
  tft.setCursor(20, 114);
  tft.setTextColor(ST77XX_GREEN);
  tft.setTextSize(2);
  tft.print("hPa:");
  // Serial.println(altitude);
}

/* ___İletişim:
e-posta: bilgi@ronaer.com
https://www.instagram.com/dr.tronik2023/   
YouTube: Dr.TRonik: https://www.youtube.com/@DrTRonik
PCBWay: https://www.pcbway.com/project/member/shareproject/?bmbno=A0E12018-0BBC-4C

The nearest ntp pools:
  TURKİYE: "tr.pool.ntp.org"
  Worldwide: pool.ntp.org
  Asia:  asia.pool.ntp.org
  Europe:  europe.pool.ntp.org
  North America: north-america.pool.ntp.org
  Oceania: oceania.pool.ntp.org
  South America: south-america.pool.ntp.org
  
Color definitions for tft:
 #define BLACK 0x0000
 #define BLUE 0x001F
 #define RED 0xF800
 #define GREEN 0x07E0
 #define CYAN 0x07FF
 #define MAGENTA 0xF81F
 #define YELLOW 0xFFE0
 #define WHITE 0xFFFF
*/
