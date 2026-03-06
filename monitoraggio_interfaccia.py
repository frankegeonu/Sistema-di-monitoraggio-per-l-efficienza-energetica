import serial
import threading
import queue
import csv
import re
import os
from datetime import datetime
import dearpygui.dearpygui as dpg


SERIAL_PORT = 'COM11' 
BAUD_RATE = 9600
CSV_FILE = 'monitoraggio_energetico.csv'

data_queue = queue.Queue()
x_data = []
y_temp = []
last_temp = None
somma_temperature = 0.0
conteggio_letture = 0


if not os.path.exists(CSV_FILE):
    with open(CSV_FILE, mode='w', newline='', encoding='utf-8') as f:
        writer = csv.writer(f)
        writer.writerow(["Data", "Orario", "Umidità", "Temperatura", "Trend"])


def serial_reader():
    global last_temp
    try:
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
        while True:
            if ser.in_waiting > 0:
                line = ser.readline().decode('utf-8', errors='ignore').strip()
                if line:
                    data_queue.put(line)
                    valori = re.findall(r"[-+]?\d*\.\d+|\d+", line)
                    if len(valori) >= 2:
                        h_val = valori[0]
                        t_val = float(valori[1])
                        trend = "Stabile"
                        if last_temp is not None:
                            if t_val > last_temp: trend = "In aumento"
                            elif t_val < last_temp: trend = "In diminuzione"
                        last_temp = t_val
                        now = datetime.now()
                        with open(CSV_FILE, mode='a', newline='', encoding='utf-8') as f:
                            writer = csv.writer(f)
                            writer.writerow([now.strftime("%d/%m/%Y"), now.strftime("%H:%M:%S"), f"{h_val}%", f"{t_val}°C", trend])
    except Exception as e:
        print(f"Errore Seriale: {e}")


def update_gui():
    global somma_temperature, conteggio_letture
    while not data_queue.empty():
        raw_data = data_queue.get()
        try:
            valori = re.findall(r"[-+]?\d*\.\d+|\d+", raw_data)
            if len(valori) >= 2:
                h_f = float(valori[0])
                t_f = float(valori[1])

                somma_temperature += t_f
                conteggio_letture += 1
                media = somma_temperature / conteggio_letture

                dpg.set_value("cur_temp", f"{t_f} °C")
                dpg.set_value("cur_hum", f"{h_f} %")
                dpg.set_value("avg_temp", f"{media:.2f} °C") 

                x_data.append(len(x_data))
                y_temp.append(t_f)
                if len(x_data) > 100:
                    x_data.pop(0)
                    y_temp.pop(0)
                dpg.set_value("temp_series", [list(range(len(y_temp))), y_temp])
                dpg.fit_axis_data("x_axis")
                dpg.fit_axis_data("y_axis")
            dpg.set_value("status_text", f"Log: {raw_data}")
        except:
            pass


dpg.create_context()
dpg.create_viewport(title='Sistema Monitoraggio Energetico', width=1000, height=600)

with dpg.window(label="Dashboard di Monitoraggio", width=980, height=560):
    with dpg.group(horizontal=True):

        with dpg.child_window(width=650, height=-1, border=True):
            dpg.add_text("ANDAMENTO TERMICO", color=[100, 150, 255])
            with dpg.plot(height=-1, width=-1):
                dpg.add_plot_axis(dpg.mvXAxis, label="Tempo", tag="x_axis")
                dpg.add_plot_axis(dpg.mvYAxis, label="Celsius", tag="y_axis")
                dpg.add_line_series([], [], label="Temp", parent="y_axis", tag="temp_series")


        with dpg.child_window(width=-1, height=-1, border=True):
            dpg.add_text("RIEPILOGO SESSIONE", color=[255, 200, 0])
            dpg.add_separator()
            dpg.add_spacer(height=10)
            dpg.add_text("Temperatura Istantanea:")
            dpg.add_text("-- °C", tag="cur_temp", color=[255, 100, 100])
            dpg.add_spacer(height=15)
            dpg.add_text("Media Sessione:")
            dpg.add_text("-- °C", tag="avg_temp", color=[255, 255, 255])
            dpg.add_spacer(height=15)
            dpg.add_text("Umidità:")
            dpg.add_text("-- %", tag="cur_hum", color=[100, 255, 100])
            dpg.add_spacer(height=30)
            dpg.add_separator()
            dpg.add_text("Messaggi Arduino:")
            dpg.add_text("In attesa...", tag="status_text", wrap=250, color=[150, 150, 150])

thread = threading.Thread(target=serial_reader, daemon=True)
thread.start()

dpg.setup_dearpygui()
dpg.show_viewport()

while dpg.is_dearpygui_running():
    update_gui()
    dpg.render_dearpygui_frame()

dpg.destroy_context()
