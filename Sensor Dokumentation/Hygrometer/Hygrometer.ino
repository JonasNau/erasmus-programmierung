const int hygrometer = 34; //Datenpin vom Hygrometer 
int value;

void setup() {
  // Starten der Seriellen Verbindung
  Serial.begin(9600);
  // Der Datenpin wird als Input konfiguriert
  pinMode(hygrometer, INPUT);

}

void loop() {
  // Der Wert wird vom Pin ausgelesen 
  value = analogRead(hygrometer);   
  // Und formatiert im Seriellen Monitor ausgegeben 
  Serial.print("Soil humidity: ");
  Serial.print(value);
  Serial.println();
  delay(2000); 

}
