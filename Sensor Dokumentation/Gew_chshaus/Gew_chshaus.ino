#include <ESP32Servo.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

int Programm_LED = 15;

int myServoPin = 14;
int target_pos = 0;
int current_pos = 0;
Servo myServo;

int Lichtsensor_Pin = 27;
bool Is_Day = true;

int Hygrometer_links_Pin = 26;
int Hygrometer_rechts_Pin = 25;
int Feuchtigkeit_links = 0;
int Feuchtigkeit_rechts = 0;
int Feuchtigkeit_absolut = 0;
int Gies_Delay = 0; 
int max_Gies = 65;
// 21 entspricht rund 1 min. Delay
// 65 = 3 min.
// 112 = 5 min. Delay zwischen zwei Pumpendurchgängen

int Temperatur_Pin = 19;
int Temperatur_Boden = 0;
int Temperatur_Luft = 0;
OneWire oneWire(Temperatur_Pin);
DallasTemperature Sensors(&oneWire);

int Relais_Pin = 32;

LiquidCrystal_I2C lcd(0x27,16,2);
String Zeile1;
String Zeile2;

int Read_Delay = 1000; // = 2000 weil die Temperatur sensoren auch noch 1000ms Zeit brauchen

void setup() {
// Servo Setup
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myServo.setPeriodHertz(50);// 50 Hertz
  myServo.attach(myServoPin, 500, 2400);
  myServo.write(current_pos);
// ===
// LCD
  lcd.init(); 
  lcd.backlight();
// ===
  
  pinMode(Lichtsensor_Pin, INPUT);
  pinMode(Hygrometer_links_Pin, INPUT);
  pinMode(Hygrometer_rechts_Pin, INPUT);
  pinMode(Relais_Pin, OUTPUT);
  pinMode(Programm_LED, OUTPUT);
  digitalWrite(Relais_Pin, HIGH);

  Serial.begin(9600);
}

void Get_Temperatures()
{
  Sensors.requestTemperatures();
  delay(1000);
}
int Get_Temperatur_Boden()
{
  return Sensors.getTempCByIndex(0);
}
int Get_Temperatur_Luft()
{
  return Sensors.getTempCByIndex(1);
}
int Get_Feuchtigkeit_rechts()
{
  return map(analogRead(Hygrometer_rechts_Pin),0,4095,100,0);
}
int Get_Feuchtigkeit_links()
{
  return map(analogRead(Hygrometer_links_Pin),0,4095,100,0);
}
int Get_Feuchtigkeit_absolut(int F_links, int F_rechts)
{
  int absolut = (F_links + F_rechts)/2;
  return absolut;
}
int Get_Dach_Position(int T_Boden, int T_Luft)
{
  int absolut = (T_Boden + T_Luft)/2;
  return map(absolut,20,30,0,120);
}
bool Get_Day()
{
  return (digitalRead(Lichtsensor_Pin) == HIGH);
}
void Set_Servo_Position(int Pos, int Delay_ms)
{
  do{
      if(current_pos > target_pos)
      {
        current_pos = current_pos -1;
      }
      else
      {
        current_pos = current_pos +1;
      }
      myServo.write(current_pos);
      delay(Delay_ms);
    }
  while(current_pos != target_pos);
}
void Bewaesserung()
{
  Serial.println(Gies_Delay);
  if(Gies_Delay == 0)
  {
    digitalWrite(Relais_Pin, LOW);
    delay(1000);
    digitalWrite(Relais_Pin, HIGH);
    Gies_Delay = max_Gies;
  }
  else
  {
    Gies_Delay = Gies_Delay -1;  
  }
}
void Display_Values(int F_links, int F_rechts, int T_Boden, int T_Luft)
{
  lcd.clear();
  Zeile1 = "";
  Zeile2 = "";
  Zeile1 = Zeile1 + F_links + " / " + F_rechts;
  Zeile2 = Zeile2 + T_Boden + " / " + T_Luft;
  lcd.setCursor(0,0);
  lcd.print(Zeile1);
  lcd.setCursor(14,0);
  lcd.print("|%");
  lcd.setCursor(0,1);
  lcd.print(Zeile2);
  lcd.setCursor(14,1);
  lcd.print("|C");
}

void loop() {
  digitalWrite(Programm_LED, HIGH);
  // Werte einlesen
  Feuchtigkeit_links = Get_Feuchtigkeit_links();
  Feuchtigkeit_rechts = Get_Feuchtigkeit_rechts();
  Get_Temperatures();
  Temperatur_Boden = Get_Temperatur_Boden();
  Temperatur_Luft = Get_Temperatur_Luft();
  Is_Day = Get_Day();

  // Verarbeitung
  Feuchtigkeit_absolut = Get_Feuchtigkeit_absolut(Feuchtigkeit_links, Feuchtigkeit_rechts);
  Serial.println(Feuchtigkeit_absolut);
  target_pos = Get_Dach_Position(Temperatur_Boden,Temperatur_Luft);
  Serial.println(target_pos);

  // Ausgabe / Display u. Aktoren
  Set_Servo_Position(target_pos, 75);
  Display_Values(Feuchtigkeit_links,Feuchtigkeit_rechts,Temperatur_Boden,Temperatur_Luft);
  if(Feuchtigkeit_absolut < 25){Bewaesserung();}

  digitalWrite(Programm_LED, LOW);
  delay(Read_Delay);
}
