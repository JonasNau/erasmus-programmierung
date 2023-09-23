#include <ESP32Servo.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

int LED = 12;

int myServoPin = 14;
int pos = 0;
Servo myServo;

int Lichtsensor = 27;

int Hygrometer1 = 26;
int Hygrometer2 = 25;

int Temperatur = 19;
OneWire oneWire(Temperatur);
DallasTemperature Sensors(&oneWire);

LiquidCrystal_I2C lcd(0x27,16,2);
String Zeile1;
String Zeile2;

void setup() {
// Servo Setup
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myServo.setPeriodHertz(50);// 50 Hertz
  myServo.attach(myServoPin, 500, 2400);
  myServo.write(pos);
// ===
// LCD
lcd.init(); 
lcd.backlight();
// ===
  
  pinMode(LED, OUTPUT);
  pinMode(Lichtsensor, INPUT);
  pinMode(Hygrometer1, INPUT);
  pinMode(Hygrometer2, INPUT);

  Serial.begin(9600);
}

void loop() {
  // LED
  digitalWrite(LED, HIGH);
  delay(1000);
  digitalWrite(LED, LOW);
  delay(1000);
  // ===
  // Servo
  for (pos = 0; pos <= 90; pos += 1) 
  { 
    myServo.write(pos);    
    delay(15);   
  }  
            
  for (pos = 90; pos >= 0; pos -= 1) 
  { 
    myServo.write(pos);    
    delay(15);             
  }
  // ===
  // Lichtsensor
  if(digitalRead(Lichtsensor) == HIGH){
    Serial.println("Es ist Dunkel");
  }
  else{
    Serial.println("Es ist Hell");
  }
  // ===
  // Hygrometer
  Serial.print("AnalogRead Feuchtigkeitsensor 1: ");
  Serial.println(analogRead(Hygrometer1));
  Serial.print("AnalogRead Feuchtigkeitsensor 2: ");
  Serial.println(analogRead(Hygrometer2));
  // LCD Hygrometer 
  Zeile1 = "";
  Zeile1 = Zeile1 + map(analogRead(Hygrometer1),0,4095,100,0);
  Zeile1 = Zeile1 + " / ";
  Zeile1 = Zeile1 + map(analogRead(Hygrometer2),0,4095,100,0);
  lcd.setCursor(0,0);
  lcd.print(Zeile1);
  lcd.setCursor(14,0);
  lcd.print("|%");
  // ===
  // ===
  // Temperatur
  Sensors.requestTemperatures();
  delay(1000);
  Serial.print("Temperatursensor 1: ");
  Serial.println(Sensors.getTempCByIndex(0));
  Serial.print("Temperatursensor 2: ");
  Serial.println(Sensors.getTempCByIndex(1));
  // LCD Temperatur
  Zeile2 = "";
  Zeile2 = Zeile2 + Sensors.getTempCByIndex(0);
  Zeile2 = Zeile2 + " / ";
  Zeile2 = Zeile2 + Sensors.getTempCByIndex(1);
  lcd.setCursor(0,1);
  lcd.print(Zeile2);
  lcd.setCursor(14,1);
  lcd.print("|C");
  // ===
  // ===
}
