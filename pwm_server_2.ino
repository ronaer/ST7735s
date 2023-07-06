/*
 * Dr.TRonik / YouTube / TEMMUZ 2023 / İzmir / Türkiye 
 * ESP8266, Local PWM WebServer...
 * Derleme ve karta yükleme öncesi, tüm kütüphaneler ile kart platformları arduino ide'sine yüklenmiş olmalıdır...
 * YouTube: https://youtu.be/aMAQGEW1EFQ
 * Bağlantılar: https://github.com/ronaer/ST7735s/blob/main/pwm_server_2.jpg
 * credit:https://www.youtube.com/@RuiSantosdotme
 * 
*/

/********************************************************************
  GLOBALS___GLOBALS___GLOBALS___GLOBALS___GLOBALS___GLOBALS___
 ********************************************************************/
// ESP8266
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>        //https://github.com/me-no-dev/ESPAsyncTCP
#include <ESPAsyncWebServer.h>  //https://github.com/me-no-dev/ESPAsyncWebServer
#include "PageIndex.h"          // Herhangi bir txt editöründe yazılıp PageIndex.h olarak aynı çalışma klasöründe olmalı...

//Tanımlamalar...
#define ssid "Dr.TRonik"
#define password "Dr.TRonik"

AsyncWebServer server(80);

#define ledPWM 4  //GPIO 4
#define inPut "value"
String sliderValue = "0";


/********************************************************************
  SETUP___SETUP___SETUP___SETUP___SETUP___SETUP___SETUP___SETUP___
 ********************************************************************/
void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);

  analogWrite(ledPWM, sliderValue.toInt());

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", mainPage, ilkVeri);
  });

  // Send a GET request to <ESP_IP>/slider?value=<inputMessage>
  server.on("/slider", HTTP_GET, [](AsyncWebServerRequest *request) {
    String inputMessage;
    // GET input1 value on <ESP_IP>/slider?value=<inputMessage>
    if (request->hasParam(inPut)) {
      inputMessage = request->getParam(inPut)->value();
      sliderValue = inputMessage;
      analogWrite(ledPWM, sliderValue.toInt());
    } else {
      inputMessage = "No message sent";
    }
    Serial.println(inputMessage);
    request->send(200, "text/plain", "OK");
  });

  // Start server
  server.begin();
}

/********************************************************************
  LOOP__LOOP__LOOP__LOOP__LOOP__LOOP__LOOP__LOOP__LOOP__LOOP__LOOP__
********************************************************************/
void loop() {
}

/********************************************************************
  VOIDs__VOIDs__VOIDs__VOIDs__VOIDs__VOIDs__VOIDs__VOIDs__VOIDs__VOIDs
********************************************************************/
/* Replaces placeholder with button section in your web page
"const string&" is a reference to a constant string. 
The "const" keyword indicates that the string is read-only and cannot be modified, 
while the "&" symbol indicates that it is a reference to an existing string object, 
rather than a copy of the object itself. This is used to pass strings as function parameters 
without incurring the overhead of copying the entire string. */

String ilkVeri(const String &var) {
  
  if (var == "SLIDERVALUE") {
    return sliderValue;
  }
  return String();
  
}

/* ___İletişim:
e-posta: bilgi@ronaer.com
https://www.instagram.com/dr.tronik2023/   
YouTube: Dr.TRonik: https://www.youtube.com/@DrTRonik
PCBWay: https://www.pcbway.com/project/member/shareproject/?bmbno=A0E12018-0BBC-4C
*/