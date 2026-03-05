# Sistema di Monitoraggio per l'Efficienza Energetica

## Descrizione del Progetto
Questo progetto realizza un sistema completo per il monitoraggio dei parametri ambientali interni (temperatura e umidità), con l'obiettivo di ottimizzare i consumi energetici e promuovere comportamenti sostenibili. Il sistema segue il paradigma **produttore-consumatore**, dove un'unità Arduino acquisisce i dati e un'applicazione Python li analizza e visualizza.

## Architettura del Sistema

### 1. Hardware (Produttore)
* **Microcontrollore**: Arduino Uno.
* **Sensore**: DHT22 per la misura di temperatura e umidità.
* **Segnalazione**: 3 LED (Rosso, Giallo, Verde) per il feedback visivo immediato sullo stato del microclima.

### 2. Software (Consumatore)
* **Linguaggio**: Python 3.
* **Interfaccia Grafica**: Sviluppata con la libreria **Dear PyGui** per la visualizzazione in tempo reale.
* **Comunicazione**: Seriale via USB con formato a pacchetti sincronizzati.

---

## Strategie di Sincronizzazione e Scelte Progettuali

Come richiesto dalle specifiche tecniche, il progetto adotta le seguenti strategie:

* **Delay-less Coding (Arduino)**: La logica di campionamento è gestita tramite la funzione `millis()`. Questo evita blocchi del processore, garantendo che l'unità rimanga reattiva per la comunicazione seriale.
* **Threading (Python)**: L'acquisizione dei dati avviene su un thread separato rispetto a quello della GUI. Questo impedisce il "freezing" dell'interfaccia durante l'attesa dei dati dalla porta seriale.
* **Buffer e Queue**: Viene utilizzata una coda (`queue.Queue`) per il trasferimento sicuro dei dati tra il thread di lettura seriale e il thread di visualizzazione, garantendo l'integrità delle informazioni.
* **Gestione della Memoria**: I dati vengono salvati periodicamente in un file CSV per l'analisi storica, agendo come un buffer persistente dei dati rilevati.

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
