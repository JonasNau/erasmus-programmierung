#include <ESP32Servo.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
// Blynk
#define BLYNK_PRINT Serial

/* Fill in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID           "TMPL4SfbZ8l6e"
#define BLYNK_TEMPLATE_NAME         "Quickstart Template"
#define BLYNK_AUTH_TOKEN            "sDkjltm2LocJH4JmEVUyhkRIOWLyazBy"


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Gregor Hotspot";
char pass[] = "Athosloko20";
// ===

int Programm_LED = 15;

int myServoPin = 14;
int target_pos = 0;
int current_pos = 0;
Servo myServo;

int Lichtsensor_Pin = 27;
bool Is_Day = true;

int Hygrometer = 34;
int Feuchtigkeit = 0;

int Gies_Delay = 0; 
int max_Gies = 65;
// 21 entspricht rund 1 min. Delay
// 65 = 3 min.
// 112 = 5 min. Delay zwischen zwei Pumpendurchgängen

int Temperatur_Pin = 19;
int Temperatur = 0;
OneWire oneWire(Temperatur_Pin);
DallasTemperature Sensors(&oneWire);

int Relais_Pin = 32;

LiquidCrystal_I2C lcd(0x27,16,2);

int Schalter = 5;

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
  pinMode(Hygrometer, INPUT);
  pinMode(Relais_Pin, OUTPUT);
  pinMode(Programm_LED, OUTPUT);
  pinMode(Schalter, INPUT);
  digitalWrite(Relais_Pin, HIGH);

  Serial.begin(115200);
  // Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
// ===
}

int Get_Temperatur_Luft()
{
  Sensors.requestTemperatures();
  delay(1000);
  return Sensors.getTempCByIndex(0);
}
int Get_Feuchtigkeit()
{
  Serial.println(analogRead(Hygrometer));
  return map(analogRead(Hygrometer),0,4095,100,0);
}
int Get_Dach_Position(int Temp)
{
  if(digitalRead(Schalter) == HIGH)
  {
    return 0;
  }
  return map(Temp,20,30,0,120);
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
  Serial.print("Gießdelay: ");
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
void Display_Values(int Feuchtigkeit, int Temp, int Delay, bool Day)
{
  lcd.clear();
  String Tag;
  String F = "";
  String T = "";
  if (Day == HIGH)
  {
    Tag = "Nacht";  
  }
  else
  {
    Tag = "Tag";  
  }
  lcd.setCursor(0,0);
  F = F + Feuchtigkeit + " %";
  lcd.print(F);
  lcd.setCursor(7,0);
  lcd.print("|");
  lcd.setCursor(14,0);
  lcd.print(Delay);
  //lcd.setCursor(14,0);
  //lcd.print("|%");
  lcd.setCursor(0,1);
  T = T + Temp + " C";
  lcd.print(T);
  lcd.setCursor(7,1);
  lcd.print("|");
  lcd.setCursor(11,1);
  lcd.print(Tag);
  //lcd.setCursor(14,1);
  //lcd.print("|C");
}

void loop() {
  digitalWrite(Programm_LED, HIGH);
  // Werte einlesen
  Feuchtigkeit = Get_Feuchtigkeit();
  Temperatur = Get_Temperatur_Luft();
  Is_Day = Get_Day();
  
  // Verarbeitung
  target_pos = Get_Dach_Position(Temperatur);
  Serial.print("Errechnete Dachöffnung: ");
  Serial.println(target_pos);

  // Ausgabe / Display u. Aktoren
  Set_Servo_Position(target_pos, 75);
  Display_Values(Feuchtigkeit,Temperatur,Gies_Delay,Is_Day);
  if(Feuchtigkeit < 25){Bewaesserung();}
  else{Gies_Delay = 0;}

  Blynk.virtualWrite(V1, Temperatur);
  Blynk.virtualWrite(V2, Feuchtigkeit);
  Blynk.virtualWrite(V3, Is_Day);
  Blynk.run();
  
  digitalWrite(Programm_LED, LOW);
  delay(Read_Delay);
}
