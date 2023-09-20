// Pin des Lichtsensors
int Lichtsensor = 18;

void setup() {
  // STarten der Seriellen Kommunikation 
  Serial.begin(9600);
  // Pin als Input konfigurieren
  pinMode(Lichtsensor, INPUT);
  
}

void loop() {
  // Der Wert wird gelesen
  int lightState = digitalRead(Lichtsensor);

  // Wenn der Pin High ist dann ist er verdeckt d.h. es ist dunkel sonst ist er frei d.h. es ist hell
  if (lightState == HIGH){
    Serial.println("It is dark");
  }
  else{
    Serial.println("It is light");
  }
}
