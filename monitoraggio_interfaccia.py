import serial
import threading
import queue
import csv
import time
from datetime import datetime
import dearpygui.dearpygui as dpg

# --- CONFIGURAZIONE ---
SERIAL_PORT = 'COM3'  # Cambia con la tua porta (es. 'COM3' su Windows o '/dev/ttyACM0' su Linux)
BAUD_RATE = 9600
CSV_FILE = 'monitoraggio_energetico.csv'

# Coda per la comunicazione tra thread
data_queue = queue.Queue()

# Liste per lo storico del grafico
x_data = []
y_temp = []

# --- FUNZIONE LETTURA SERIALE (THREAD SEPARATO) ---
def serial_reader():
    try:
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
        print(f"Connesso a {SERIAL_PORT}")
        
        while True:
            if ser.in_waiting > 0:
                # Legge la riga inviata da Arduino
                line = ser.readline().decode('utf-8', errors='ignore').strip()
                if line:
                    # Inserisce il dato nella coda per la GUI
                    data_queue.put(line)
                    
                    # Logica di salvataggio CSV (Step 4)
                    try:
                        # Estrazione semplice: cerchiamo di isolare i numeri
                        # Nota: Dato che Arduino invia testo, dobbiamo pulirlo
                        parts = line.split()
                        # Esempio parsing: "Humidity: 50.0% Temperature: 25.0°C"
                        # Cerchiamo di trovare i valori numerici nel tuo formato specifico
                        h_val = parts[1].replace('%', '')
                        t_val = parts[3].replace('°C', '')
                        
                        with open(CSV_FILE, mode='a', newline='') as f:
                            writer = csv.writer(f)
                            writer.writerow([datetime.now().strftime("%Y-%m-%d %H:%M:%S"), t_val, h_val])
                    except:
                        pass # Salta righe di testo come "DHTxx test!"
    except Exception as e:
        print(f"Errore Seriale: {e}")

# --- INTERFACCIA GRAFICA (DEAR PYGUI) ---
def update_gui():
    while not data_queue.empty():
        raw_data = data_queue.get()
        
        # Aggiorna il testo dello stato nella GUI
        dpg.set_value("status_text", f"Ultimo messaggio: {raw_data}")
        
        # Prova ad aggiornare il grafico se la riga contiene dati numerici
        try:
            parts = raw_data.split()
            t_val = float(parts[3].replace('°C', ''))
            
            x_data.append(len(x_data))
            y_temp.append(t_val)
            
            # Mantieni solo gli ultimi 50 punti per il grafico
            if len(x_data) > 50:
                x_data.pop(0)
                y_temp.pop(0)
            
            dpg.set_value("temp_series", [list(range(len(y_temp))), y_temp])
            dpg.fit_axis_data("x_axis")
            dpg.fit_axis_data("y_axis")
        except:
            pass

dpg.create_context()
dpg.create_viewport(title='Sistema Monitoraggio Energetico', width=800, height=600)

with dpg.window(label="Dashboard Monitoraggio", width=780, height=580):
    dpg.add_text("Dati in tempo reale dal sensore DHT")
    dpg.add_text("In attesa di dati...", tag="status_text", color=[0, 255, 0])
    
    with dpg.plot(label="Andamento Temperatura", height=400, width=-1):
        dpg.add_plot_legend()
        dpg.add_plot_axis(dpg.mvXAxis, label="Campionamenti", tag="x_axis")
        dpg.add_plot_axis(dpg.mvYAxis, label="Celsius (°C)", tag="y_axis")
        dpg.add_line_series([], [], label="Temp", parent="y_axis", tag="temp_series")

# Avvio del thread seriale
thread = threading.Thread(target=serial_reader, daemon=True)
thread.start()

dpg.setup_dearpygui()
dpg.show_viewport()

while dpg.is_dearpygui_running():
    update_gui()
    dpg.render_dearpygui_frame()

dpg.destroy_context()
