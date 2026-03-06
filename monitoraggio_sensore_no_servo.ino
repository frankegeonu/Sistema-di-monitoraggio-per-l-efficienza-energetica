#include "DHT.h"

#define DHTPIN 2     
#define DHTTYPE DHT22 // Usa DHT11 per il montaggio fisico se necessario

DHT dht(DHTPIN, DHTTYPE);

// Definizione dei PIN per i LED
const int LED_RED    = 4;
const int LED_YELLOW = 5;
const int LED_GREEN  = 6;

// Variabili di temporizzazione
unsigned long lastTime = 0;
const long interval = 2000; // Intervallo di 2 secondi tra le letture

void setup() {
  Serial.begin(9600);
  
  // Configurazione LED
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  dht.begin();
  Serial.println(F("Sistema di Monitoraggio Energetico Avviato"));
}

void loop() {
  unsigned long currentTime = millis(); // Gestione tempo senza delay
  String stato;

  if (currentTime - lastTime >= interval) {
    lastTime = currentTime;

    float h = dht.readHumidity();
    float t = dht.readTemperature();
    float hic = dht.computeHeatIndex(t, h, false);

    // Verifica se la lettura è andata a buon fine
    if (!isnan(h) && !isnan(t)) {
      
      // Reset LED
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_YELLOW, LOW);
      digitalWrite(LED_GREEN, LOW);

      // Logica delle soglie (Step 3 della consegna)
      if (hic > 30) {
        stato = "PERICOLO: Temperatura critica!";
        digitalWrite(LED_RED, HIGH);
      } 
      else if (hic > 25) {
        stato = "Attenzione: Temperatura in aumento.";
        digitalWrite(LED_YELLOW, HIGH);
      } 
      else if (hic > 15) {
        stato = "Temperatura ottimale.";
        digitalWrite(LED_GREEN, HIGH);
      } 
      else {
        stato = "Temperatura troppo bassa.";
        digitalWrite(LED_RED, HIGH);
      }

      // Output formattato per il parsing Python (Consumatore)
      Serial.print(F("Humidity: "));
      Serial.print(h);
      Serial.print(F("%  Temperature: "));
      Serial.print(t);
      Serial.print(F("°C   Heat index: "));
      Serial.print(hic);
      Serial.println(F("°C "));
      Serial.println(stato);
    }
  }
}
