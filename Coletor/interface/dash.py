# ------------------------------------------------------------------------------
# Dashboard App
# ------------------------------------------------------------------------------

# Modules

import tkinter as tk
import numpy as np
from PIL import ImageTk, Image
import threading
from datetime import datetime
import os
import pandas
import matplotlib.pyplot as plt
plt.switch_backend('agg')

import converter

# ------------------------------------------------------------------------------

# Defines

data_header_size = 12

fig_name = './dash_fig.png'

string_default = 'n/a'

colors = [
    'k',
    'b',
    'r',
    'm',
    'g',
    'c',
    'C1',
    'C4',
    'C5',
    'C7'
]

dev_types = ['Touro', 'Vaca', 'DIV']

N_DEV_MAX = 10

log_skiprows = [0]*N_DEV_MAX

DEFAULT_TIMER_VALUE = 0.5

time_lims = [np.inf, -np.inf]

# -----------------------------------------------------------------------------
# Functions
# -----------------------------------------------------------------------------

def fig_create():
    plt.figure()
    fig = plt.gcf()
    fig.set_dpi(100)
    fig.set_size_inches(18, 6)
    threading.Timer(DEFAULT_TIMER_VALUE, update_data, [file_list]).start()

# -----------------------------------------------------------------------------

def update_data(file_list):

    # Data Read

    for file in file_list:

        file_id = int(file.split("_")[-1])
        log_file = open(file, "r")

        try:
            data = pandas.read_csv(
                                    log_file,
                                    sep=';',
                                    header=None,
                                    skiprows=log_skiprows[file_id],
                                    nrows=1
                                   )
            log_skiprows[file_id] += 1

        except:
            threading.Timer(DEFAULT_TIMER_VALUE, update_data, [file_list]).start()
            return

        data = data.values[0]

        animal_id = data[0]
        animal_type = data[1]
        animal_acquisition = data[2]<<8 | data[3]
        animal_transmition = data[4]
        animal_vbat = data[5]
        animal_time_stamp = data[6]<<24 | data[7]<<16 | data[8]<<8 | data[9]
        animal_counter_send = data[10]<<8 | data[11]

        globals()[f'id{animal_id}_type_label'].configure(text=dev_types[animal_type])
        # globals()[f'id{animal_id}_voltage_label'].configure(text='%.3f'%((6.6*animal_vbat)/255.0))
        globals()[f'id{animal_id}_voltage_label'].configure(text='%.3f'%((3.3*animal_vbat)/255.0))
        globals()[f'led{animal_id}'].configure(bg="green")
        globals()['acquisition_label'].configure(text=f'Acquisition Time = {animal_acquisition} ms')
        globals()['transmition_label'].configure(text=f'Transmition Time = {animal_transmition} s')

        pck_length = int(animal_transmition/(animal_acquisition/1000.0))
        time = [animal_time_stamp+(i*(animal_acquisition/1000.0)) for i in range(pck_length)]

        global time_lims
        if (min(time) < time_lims[0]): time_lims[0] = min(time)
        if (max(time) > time_lims[1]): time_lims[1] = max(time)

        # Acc - plot posição 1
        graph_data = data[data_header_size : data_header_size + pck_length]
        graph_data = [converter.acc_converter(data) for data in graph_data]
        plot_update(graph_data, animal_id, 1, time)

        # Gyro - plot posição 2
        graph_data = data[data_header_size+pck_length : data_header_size + 2*pck_length]
        graph_data = [converter.gyro_converter(data) for data in graph_data]
        plot_update(graph_data, animal_id, 2, time)

        # Temperatura - plot posição 3
        graph_data = data[data_header_size+2*pck_length : data_header_size+3*pck_length]
        graph_data = [converter.temp_converter(data) for data in graph_data]
        plot_update(graph_data, animal_id, 3, time)

        # RSSI estendido - plot posição 4
        graph_data = [data[-1]]*pck_length
        graph_data = [converter.rssi_converter(data) for data in graph_data]
        plot_update(graph_data, animal_id, 4, time)

        update_canvas()

    # Agendamento
    threading.Timer(DEFAULT_TIMER_VALUE, update_data, [file_list]).start()

# ------------------------------------------------------------------------------

def plot_update(data, animal_id, fig_index, time):

    plt.subplot(2, 2, fig_index)

    DPI = plt.gcf().get_dpi()
    plt.gcf().set_size_inches(float(canvas_width)/float(DPI), float(canvas_height)/float(DPI))

    # plot cmd
    plt.plot(time, data, colors[animal_id], linewidth=0.1)
    plt.plot(time, data, 'o', markersize=1, color=colors[animal_id])

    ax = plt.gca()

    # [config] y axis

    if (fig_index == 1):
        ax.set_yticks(np.linspace(0, 7, 8))
        ax.title.set_text('Acelerômetro')

    elif (fig_index == 2):
        ax.set_yticks(np.linspace(0, 2000, 11))
        ax.set_ylim([-50, 2050])
        ax.title.set_text('Giroscópio')

    elif (fig_index == 3):
        ax.set_yticks(np.linspace(-1, 100, 6))
        ax.title.set_text('Temperatura')

    elif (fig_index == 4):
        ax.set_yticks(np.linspace(-157, 255-157, 11))
        ax.title.set_text('RSSI')

    # [config] x axis
    global time_lims
    ax.set_xticks(time_lims)
    ax.set_xticklabels([datetime.fromtimestamp(t).strftime("%H:%M:%S") for t in time_lims])

    # [config] grid
    plt.grid(axis='x', color='gray', linestyle='--', linewidth=0.5)
    ax.set_axisbelow(True)

# ------------------------------------------------------------------------------

def update_canvas():

    global img_update

    # savefig
    plt.savefig(fig_name, bbox_inches='tight')

    # image update
    img = Image.open(fig_name)
    img = img.resize((canvas_width, canvas_height), Image.ANTIALIAS)
    img_update = ImageTk.PhotoImage(img)
    canvas_fig.itemconfig(picture, image=img_update)

# -----------------------------------------------------------------------------

def on_closing():
    root.destroy()

# ------------------------------------------------------------------------------
# Root window
# ------------------------------------------------------------------------------

# Create
root = tk.Tk()

# Size
screen_width = 1920
screen_height = 1080
root.geometry(f'{screen_width}x{screen_height}')

# Canvas Size
canvas_width = 1800
canvas_height = 700

# icon
img = tk.PhotoImage(file='icon.png')
root.tk.call('wm', 'iconphoto', root._w, img)

# Window title
root.title('Dashboard App')

# ------------------------------------------------------------------------------
# Widgets
# ------------------------------------------------------------------------------

# Title

title_frame = tk.Frame(root, bd = 1)
title_frame.pack(side=tk.TOP)

title_label = tk.Label(title_frame, text='Dashboard App', font=('Arial', 25))
title_label.grid(row=0, column=0, pady=5)

# ------------------------------------------------------------------------------
# Network Status
# ------------------------------------------------------------------------------

status_rede_frame = tk.Frame(root)
status_rede_frame.pack(side=tk.TOP, pady=10)

transmition_label = tk.Label(status_rede_frame, text='??', font=('Arial', 15), borderwidth=2, relief="groove", width=70, height=2)
transmition_label.grid(row=0, column=0, sticky='', pady=5, padx=10)

acquisition_label = tk.Label(status_rede_frame, text='??', font=('Arial', 15), borderwidth=2, relief="groove", width=70, height=2)
acquisition_label.grid(row=0, column=1, sticky='w', pady=5, padx=10)

# -----------------------------------------------------------------------------
# Status
# -----------------------------------------------------------------------------

status_frame = tk.Frame(root)
status_frame.pack(side=tk.TOP)

for i in range(N_DEV_MAX):

    # led
    globals()[f"led{i}"] = tk.Button(status_frame, text=f'{i}', state="disabled", width=1, bg='gray', disabledforeground='black')
    globals()[f"led{i}"].grid(row=0, column=i, padx=5)

    # type
    globals()[f"id{i}_type_label"] = tk.Label(status_frame,text=string_default)
    globals()[f"id{i}_type_label"].grid(row=1, column=i, padx=5)

    # voltage
    globals()[f"id{i}_voltage_label"] = tk.Label(status_frame,text=string_default)
    globals()[f"id{i}_voltage_label"].grid(row=2, column=i, padx=5)

# ------------------------------------------------------------------------------
# Graphics
# ------------------------------------------------------------------------------

graf_frame = tk.Frame(root)
graf_frame.pack(side=tk.TOP)

canvas_fig = tk.Canvas(graf_frame, width=canvas_width, height=canvas_height)
canvas_fig.pack(padx=20, pady=20, side=tk.LEFT)

picture = canvas_fig.create_image(canvas_width/2, canvas_height/2, anchor=tk.CENTER, image='')
canvas_fig.create_rectangle(1, 1, canvas_width, canvas_height, width=1)

# ------------------------------------------------------------------------------
# Log
# ------------------------------------------------------------------------------

# Diretório raiz dos logs
ROOT_LOG_DIR = './log/'
dir_list = sorted(os.listdir(ROOT_LOG_DIR))


# Log
LOG_DIR = ROOT_LOG_DIR+dir_list[-1]+'/'
file_list = os.listdir(LOG_DIR)
file_list = [LOG_DIR+file for file in file_list]

# -----------------------------------------------------------------------------
# Main Loop
# -----------------------------------------------------------------------------

threading.Timer(DEFAULT_TIMER_VALUE, fig_create).start()
root.protocol("WM_DELETE_WINDOW", on_closing)
root.mainloop()