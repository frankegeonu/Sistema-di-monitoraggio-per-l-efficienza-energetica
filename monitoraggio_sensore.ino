
#include "DHT.h"
#include <Servo.h> //per il servo
#define DHTPIN 2     // Digital pin connected to the DHT sensor

// Uncomment whatever type you're using!
#define DHTTYPE DHT22   // DHT 22
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);
Servo mioServo; // Oggetto per gestire il servomotore

// Definizione dei PIN
const int LED_RED    = 4;
const int LED_YELLOW = 5;
const int LED_GREEN  = 6;
const int PIN_SERVO  = 9; //Servo

// Variabili di temporizzazione e stato
unsigned long lastTime = 0;
int pos = 0;
const long interval = 2000; //2 secondi di intervallo tra le varie letture
bool ventolaAttiva = false; // Stato attuale del "ventilatore"

void setup() {
  Serial.begin(9600);
  Serial.println(F("Sistema di Monitoraggio con Consenso Ventola"));

  // Configurazione LED
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  // Inizializzazione sensore e servo
  dht.begin();
  mioServo.attach(PIN_SERVO);
  mioServo.write(0); // Posizione iniziale (fermo)
}

void loop() {
  unsigned long currentTime = millis();
  String stato;

  // 1. Logica Sensori e Comunicazione (Ogni 2 secondi)
  if (currentTime - lastTime >= interval) {
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    float hic = dht.computeHeatIndex(t, h, false);

    if (!isnan(h) && !isnan(t)) {
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_YELLOW, LOW);
      digitalWrite(LED_GREEN, LOW);

      if (hic > 30) {
        stato = "PERICOLO: Temperatura critica! Ventola ATTIVA.";
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

      // Invio dati formattati per il programma Python
      Serial.print(("Humidity: "));
      Serial.print(h);
      Serial.print(("%  Temperature: "));
      Serial.print(t);
      Serial.print(("°C "));
      Serial.print(("  Heat index: "));
      Serial.print(hic);
      Serial.println(("°C "));
      Serial.println(stato);
    }
    lastTime = currentTime;
  }

  // 2. Controllo Servo (Automatico basato su soglia)
  float t_check = dht.readTemperature();

  if (t_check > 30) {
    // Il ventilatore gira automaticamente
    for (pos = 0; pos <= 180; pos += 10) {
      mioServo.write(pos);
      delay(15);
    }
    for (pos = 180; pos >= 0; pos -= 10) {
      mioServo.write(pos);
      delay(15);
    }
  } else {
    mioServo.write(0); // Resta fermo se la temperatura è sotto i 30°C
  }
}
