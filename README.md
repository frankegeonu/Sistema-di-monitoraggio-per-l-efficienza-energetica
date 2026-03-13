# Sistema di Monitoraggio per l'Efficienza Energetica

## Descrizione del Progetto
Il progetto consiste in un sistema di monitoraggio ambientale basato sul paradigma **produttore-consumatore**. Un'unità Arduino (produttore) rileva temperatura e umidità, mentre un'applicazione Python (consumatore) elabora i dati, gestisce un database CSV e fornisce un'interfaccia grafica avanzata per l'analisi energetica.

## Architettura del Sistema

### 1. Hardware e Simulazione
* **Microcontrollore**: Arduino Uno.
* **Sensore**: DHT22 (in simulazione Wokwi) e DHT11 (nel montaggio fisico).
* **Feedback Visivo**: 3 LED (Verde, Giallo, Rosso) per segnalare lo stato del microclima in tempo reale.
* **Nota sul Servomotore**: 
  * **In simulazione**: È stato implementato un servomotore che agisce come ventilatore automatico sopra i 30°C.
  * **Parte fisica**: Il servomotore non è presente nel montaggio reale per mancanza del componente specifico (il motore DC a disposizione richiederebbe un driver di potenza dedicato). La logica di controllo rimane comunque attiva nel codice e segnalata tramite il LED Rosso.

### 2. Software (Interfaccia e Grafica)
* **Linguaggio**: Python.
* **GUI**: Sviluppata con **Dear PyGui**.
* **Comunicazione**: Seriale a 9600 baud.


## Analisi del Grafico e della Dashboard
L'interfaccia grafica è stata progettata per offrire un'analisi dinamica e leggibile dei dati:

### Caratteristiche del Grafico
* **Auto-Scaling Dinamico**: Il grafico utilizza la funzione `fit_axis_data` per adattare costantemente gli assi X e Y. Questo evita che la curva venga "tagliata" o risulti illeggibile quando la temperatura subisce variazioni (aumento o diminuzione), garantendo la visibilità dell'intero set di dati.
* **Buffer Circolare**: Il grafico mostra gli ultimi 100 campionamenti con effetto di scorrimento continuo.
* **Sidebar di Riepilogo**: Una colonna dedicata visualizza i dati in tempo reale separati dal grafico:
  * **Temperatura Istantanea**: Ultima lettura rilevata.
  * **Media Sessione**: Calcolata dinamicamente su tutte le letture dall'avvio.
  * **Umidità**: Valore percentuale attuale.


## Sincronizzazione e Gestione Dati
* **Multithreading**: La lettura seriale avviene su un thread separato per non bloccare la fluidità della GUI.
* **Persistenza CSV**: I dati vengono salvati nel file `monitoraggio_energetico.csv` nel seguente formato:
  `{ Data, Orario, Umidità, Temperatura, Trend }`
* **Analisi del Trend**: Il software confronta la temperatura attuale con la precedente per determinare se il valore è "In aumento", "In diminuzione" o "Stabile".


## Requisiti e Installazione
1. Caricare lo sketch `/Arduino` sulla scheda.
2. Installare le librerie: `pip install dearpygui pyserial`.
3. Avviare lo script: `python monitoraggio_interfaccia.py`.


## Team di Progetto
Questo lavoro è stato realizzato da:

* Egeonu Chibueike Frank

