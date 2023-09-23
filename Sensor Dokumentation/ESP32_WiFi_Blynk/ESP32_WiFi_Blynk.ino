/*************************************************************
  Blynk is a platform with iOS and Android apps to control
  ESP32, Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build mobile and web interfaces for any
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: https://www.blynk.io
    Sketch generator:           https://examples.blynk.cc
    Blynk community:            https://community.blynk.cc
    Follow us:                  https://www.fb.com/blynkapp
                                https://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example runs directly on ESP32 chip.

  NOTE: This requires ESP32 support package:
    https://github.com/espressif/arduino-esp32

  Please be sure to select the right ESP32 module
  in the Tools -> Board menu!

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

/* Fill in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID           "TMP_1"
#define BLYNK_TEMPLATE_NAME         "ESP32"
#define BLYNK_AUTH_TOKEN            "AUBglV3fTSYPN3iZl-_UPd6ibzzMR2NM"


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Erasmus_Haus";
char pass[] = "Erasmus123";

void setup()
{
  // Debug console
  Serial.begin(9600);
  
  Blynk.config(BLYNK_AUTH_TOKEN, "192.168.2.1", 8080);
  Blynk.connect();
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  pinMode(15, OUTPUT);
  digitalWrite(15, HIGH);
}

void loop()
{
  Blynk.run();
}
