// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include "DHT.h"

#define DHTPIN 2     // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment whatever type you're using!
#define DHTTYPE DHT22   // DHT 22
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);

// Definizione dei LED
const int LED_RED    = 4;
const int LED_YELLOW = 5;
const int LED_GREEN  = 6;

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));

  // Imposta i LED come output
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  dht.begin();
}


unsigned long lastTime = 0;
const long interval = 2000; // Invia dati ogni 2 secondi

void loop() {
  unsigned long currentTime = millis();
  String stato;

  if (currentTime - lastTime >= interval) {
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    
    float hic = dht.computeHeatIndex(t, h, false);

    if (!isnan(h) && !isnan(t)) {
      // 1. Gestione LED (Logica esistente corretta)
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_YELLOW, LOW);
      digitalWrite(LED_GREEN, LOW);

      if(hic > 30){
        stato = "Temperatura troppo alta, raffredare il sistema!!!";
        digitalWrite(LED_RED, HIGH);
      }else if(hic > 25){
        stato = "Temperatura in aumento, vicino alla soglia di massima sopportazione";
        digitalWrite(LED_YELLOW, HIGH);
      }else if(hic > 15){
        stato = "Temperatura ambiente, stato accetabile";
        digitalWrite(LED_GREEN, HIGH);
      }else if(hic > 10){
        stato = "Temperatura è in diminuzione, vicino alla soglia di massima sopportazione";
        digitalWrite(LED_YELLOW, HIGH);
      }else{
        stato = "Temperatura troppo bassa, scaldare il sistema!!!";
        digitalWrite(LED_RED, HIGH);
      }

      Serial.print(F("Humidity: "));
      Serial.print(h);
      Serial.print(F("%  Temperature: "));
      Serial.print(t);
      Serial.print(F("°C "));
      Serial.print(F("  Heat index: "));
      Serial.print(hic);
      Serial.println(F("°C "));
      Serial.println(stato);
    }
    lastTime = currentTime;
  }
}
