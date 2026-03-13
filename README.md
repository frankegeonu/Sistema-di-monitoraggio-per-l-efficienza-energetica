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

## Versioni del Progetto

### Versione 1 — File unico

La prima versione del progetto è contenuta in un singolo file Python: `monitoraggio_interfaccia.py`.

In questo script convivono tutte le responsabilità del sistema:

- La lettura dei dati dalla porta seriale viene gestita da un thread in background (`serial_reader`), che decodifica le righe ricevute da Arduino, calcola il trend di temperatura e scrive ogni rilevazione nel file CSV.
- L'interfaccia grafica viene costruita e aggiornata nella funzione `update_gui`, chiamata a ogni frame del loop principale di Dear PyGui.
- Il loop principale tiene insieme lettura, aggiornamento GUI e rendering in un unico ciclo continuo.

Questa struttura è semplice da avviare — basta eseguire un solo file — ma mescola la logica di acquisizione dati con quella di presentazione, rendendo più difficile modificare o testare le due parti separatamente.

---

### Versione 2 — Separazione lettura e interfaccia

La seconda versione divide il progetto in due script indipendenti che comunicano tramite il file CSV condiviso `monitoraggio_energetico.csv`.

#### `lettore_dati.py`

Si occupa esclusivamente di acquisire i dati:

- Apre la connessione seriale con Arduino e rimane in ascolto.
- Per ogni riga ricevuta, estrae umidità e temperatura tramite regex, calcola il trend rispetto alla lettura precedente e scrive una nuova riga nel CSV con data, orario e valori formattati.
- In caso di errore sulla porta seriale, attende 5 secondi e ritenta automaticamente la connessione.

#### `monitoraggio_interfaccia.py`

Si occupa esclusivamente di mostrare i dati:

- Legge il CSV ad intervalli regolari (ogni 1,5 secondi) tenendo traccia di quante righe ha già processato, così da leggere solo le nuove ad ogni ciclo.
- Aggiorna i widget della dashboard: temperatura istantanea, media di sessione, umidità, trend con colore dinamico e grafico dell'andamento termico.
- Funziona indipendentemente da `lettore_seriale.py`: può essere avviato anche su una macchina diversa, purché abbia accesso al CSV.


## Requisiti e Installazione
1. Caricare lo sketch `/Arduino` sulla scheda.
2. Installare le librerie: `pip install dearpygui pyserial`.
3. Avviare lo script: `python monitoraggio_interfaccia.py`.


## Team di Progetto
Questo lavoro è stato realizzato da:

* Egeonu Chibueike Frank

