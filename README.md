# Sistema di Monitoraggio per l'Efficienza Energetica

## Descrizione del Progetto
Questo progetto realizza un sistema completo per il monitoraggio dei parametri ambientali interni (temperatura e umidità), con l'obiettivo di ottimizzare i consumi energetici e promuovere comportamenti sostenibili. Il sistema segue il paradigma **produttore-consumatore**, dove un'unità Arduino acquisisce i dati e un'applicazione Python li analizza e visualizza.

## Architettura del Sistema

### 1. Hardware (Produttore)
* **Microcontrollore**: Arduino Uno.
* **Sensore**: DHT22 per la misura di temperatura e umidità durante la simulazione su Wokwi e un DHT11 nel montaggio fisico.
* **Attuatori**: 
  * **3 LED** (Rosso, Giallo, Verde) per il feedback visivo immediato sullo stato del microclima.
  * **Servomotore**: Simula le pale di un ventilatore che si attiva automaticamente in caso di emergenza termica.

### 2. Software (Consumatore)
* **Linguaggio**: Python.
* **Interfaccia Grafica**: Sviluppata con la libreria **Dear PyGui** per la visualizzazione in tempo reale e dei grafici storici.
* **Comunicazione**: Seriale via USB con formato a pacchetti sincronizzati.

---

## Funzionamento e Logica di Controllo
Il sistema monitora costantemente l'indice di calore (*Heat Index*). In base ai valori rilevati, il sistema reagisce come segue:
* **T > 30°C**: Stato critico. LED Rosso acceso e **attivazione automatica del servomotore** (ventilatore) per il raffreddamento.
* **25°C < T <= 30°C**: Temperatura in aumento. LED Giallo acceso.
* **15°C < T <= 25°C**: Stato ottimale. LED Verde acceso.
* **T <= 15°C**: Temperatura troppo bassa. LED Rosso acceso.

---

## Struttura del Repository
* `/Arduino`: Contiene lo sketch `.ino` per la gestione di sensori, LED e servo.
* `/Python`: Contiene l'applicazione `monitoraggio_interfaccia.py`.
* `monitoraggio_energetico.csv`: File di database contenente lo storico delle misurazioni.
* `README.md`: Relazione tecnica del progetto.

---

## Requisiti e Installazione
1. Caricare il codice presente nella cartella `/Arduino` sulla scheda.
2. Installare le dipendenze Python:
   ```bash
   pip install pyserial dearpygui
