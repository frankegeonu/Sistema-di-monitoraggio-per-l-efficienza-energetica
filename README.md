# Sistema di Monitoraggio per l'Efficienza Energetica

## Descrizione del Progetto
Questo progetto realizza un sistema completo per il monitoraggio dei parametri ambientali interni (temperatura e umidità), con l'obiettivo di ottimizzare i consumi energetici e promuovere comportamenti sostenibili. Il sistema segue il paradigma **produttore-consumatore**, dove un'unità Arduino acquisisce i dati e un'applicazione Python li analizza e visualizza.

## Architettura del Sistema

### 1. Hardware (Produttore)
* **Microcontrollore**: Arduino Uno.
* **Sensore**: DHT22 per la misura di temperatura e umidità durante la simulazione su wokwi e un DHT11 nel montaggio fisico.
* **Segnalazione**: 3 LED (Rosso, Giallo, Verde) per il feedback visivo immediato sullo stato del microclima.

### 2. Software (Consumatore)
* **Linguaggio**: Python.
* **Interfaccia Grafica**: Sviluppata con la libreria **Dear PyGui** per la visualizzazione in tempo reale.
* **Comunicazione**: Seriale via USB con formato a pacchetti sincronizzati.

---

## Struttura del Repository
* `/Arduino`: Contiene lo sketch `.ino` per il sensore DHT e la gestione dei LED.
* `/Python`: Contiene l'applicazione per l'interfaccia grafica e la gestione della seriale.
* `monitoraggio_energetico.csv`: File contenente lo storico delle misurazioni (timestamp, temperatura, umidità).
* `README.md`: Relazione tecnica del progetto.

---

## Requisiti e Installazione
1. Caricare il codice presente nella cartella `/Arduino` sulla scheda.
2. Installare le dipendenze Python:
   ```bash
   pip install pyserial dearpygui
