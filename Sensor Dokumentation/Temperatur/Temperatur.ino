// Es werden die benötigten Librarys hinzugefügt
#include <DallasTemperature.h>
#include <OneWire.h>

// Hier wird der DatenPin definiert wo später die Temperatur abgelesen werden kann
int ONE_WIRE_BUS = 21;

// Ein OneWire Objekt wird erstellt was die Kommunikation auf dem Datenbus managed
OneWire oneWire(ONE_WIRE_BUS);

// Das DallasTemperature Objekt organisiert die Kommunikation mit den Temperatursensoren
DallasTemperature sensors(&oneWire);

void setup(void) 
{
 // Es wird eine Serielle Kommunikation mit dem PC aufgebaut
 Serial.begin(9600); 
 Serial.println("Dallas Temperature IC Control Library Demo"); 
 // Hier werden die Sensoren gestartet
 sensors.begin(); 
} 
void loop(void) 
{
 // requestTemperatures() fordert alle Sensoren auf die Temperatur zu messen es wird ihnen Zeit gegeben um die Daten zu erfassen
 Serial.print(" Requesting temperatures..."); 
 sensors.requestTemperatures(); // Send the command to get temperature readings
 delay(1000); // Wait 1 second for temperature grab to stabilize
 Serial.println("done"); 
 Serial.print("Temperature (1) is: ");

 // ByIndex() deswegen weil man mehrere Sensoren 
 // 0 = erste 
 // 1 = zweite

 // getTempCByIndex == C°
 // getTempFByIndex == F°
 Serial.print(sensors.getTempCByIndex(0)); 
 Serial.print("° C  (");
 
 Serial.print(sensors.getTempFByIndex(0));
 Serial.print("° F) -");

 Serial.print("Temperature (1) is: ");

 Serial.print(sensors.getTempCByIndex(1)); 
 Serial.print("° C  (");
 
 Serial.print(sensors.getTempFByIndex(1));
 Serial.print("° F) -");
 
 delay(1000); 
} 
