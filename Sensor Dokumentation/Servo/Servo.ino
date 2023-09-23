#include <ESP32Servo.h>

Servo myservo;  // Das Servo Objekt stellt den Servo dar

int pos = 0;    // Variable für die Servo Position

int servoPin = 14;
// Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33 

void setup() {
  // Das sind Library spezifische einstellungen die für die generierung von PWM Signalen wichtig sind
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50);    // standard 50 hz servo
  myservo.attach(servoPin, 500, 2400); // Der Servo wird konfiguriert
}

void loop() {
    // Der Servo fährt die ganze Zeit zwischen 0-90 Grad hin und her
  
    for (pos = 0; pos <= 90; pos += 1) 
    { 
      myservo.write(pos);    
      delay(15);   
    }  
            
    for (pos = 90; pos >= 0; pos -= 1) 
    { 
    myservo.write(pos);    
    delay(15);             
    }
}
