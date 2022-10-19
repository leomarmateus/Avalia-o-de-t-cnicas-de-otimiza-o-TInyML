# ------------------------------------------------------------------------------
# Config App - GUI
# ------------------------------------------------------------------------------

# Modules
import tkinter as tk
from tkinter import messagebox
import serial.tools.list_ports
import serial
import time
import os
import hashlib

# ------------------------------------------------------------------------------

# cfg start (Arduino -> PC)
CFG = [0x63, 0x66, 0x67]

DEV_MAX = 10

SERIAL_READ_TIMEOUT = 5

# ------------------------------------------------------------------------------

# def horizontalLine():
#     ts = os.get_terminal_size()
#     print('-'*ts.columns)

def checksum_calc(data):

    checksum = 0

    for value in data:
        checksum ^= value

    return(checksum)

# read data from serial
def serialRead(ser):
    read_data = []

    # waiting for data available
    # TODO: create timeout
    t0 = time.time()
    while True:
        if ser.in_waiting:
            break
        t1 = time.time() - t0
        if t1 > SERIAL_READ_TIMEOUT:
            return False

    while ser.in_waiting:
        read_data_raw = ser.read()
        read_data.append(int.from_bytes(read_data_raw, byteorder='big'))

    return read_data

# write data to serial
def serialWrite(ser, write_data):

    try:
        send_data_raw = bytearray(write_data)
        ser.write(send_data_raw)
        return True
    except:
        return False

def hexPrint(data):
    print([hex(x) for x in data])

# ------------------------------------------------------------------------------

# Callbacks

def refresh_serial():

    available_ports = []

    ports = list(serial.tools.list_ports.comports())

    for p in ports:
        if p[1] != "n/a":
            available_ports.append(p[0])

    return available_ports


def send_config():

    os.system('clear')

    write_data = []

    # Validate

    try:
        id_data = int(id_entry.get())
        if id_data < 0 or id_data > DEV_MAX-1:
            raise Exception
    except:
        messagebox.showerror("ERROR", "ID Inválido\n\n0 \u2264 ID \u2264 %d" % (DEV_MAX-1))
        return

    try:
        measurement_interval_data = int(measurement_interval_entry.get())
        if measurement_interval_data <= 0:
            raise Exception
    except:
        messagebox.showerror("ERROR", "Intervalo de Medição Inválido")
        return

    try:
        send_interval_data = int(send_interval_entry.get())
        if send_interval_data <= 0:
            raise Exception
    except:
        messagebox.showerror("ERROR", "Intervalo de Envio Inválido")
        return

    if (send_interval_data/(measurement_interval_data/1000)) > 50:
        messagebox.showerror("ERROR", "Send/Measurement > 50")
        return

    unix_timestamp = int(time.time())

    # Debug
    # print("ID = %d" % id_data)
    # print("Tipo = %d" % dev_types.index(dev_type_var.get()))
    # print("Measurement Interval = %d" % measurement_interval_data)
    # print("Send Interval = %d" % send_interval_data)
    # print("Data = %d" % unix_timestamp)

    write_data.append(id_data)

    write_data.append(dev_types.index(dev_type_var.get()))

    write_data.append((measurement_interval_data & 0xFF00)>>8)
    write_data.append((measurement_interval_data & 0x00FF))

    write_data.append(send_interval_data)

    write_data.append((unix_timestamp & 0xFF00000000)>>32)
    write_data.append((unix_timestamp & 0x00FF000000)>>24)
    write_data.append((unix_timestamp & 0x0000FF0000)>>16)
    write_data.append((unix_timestamp & 0x000000FF00)>>8)
    write_data.append((unix_timestamp & 0x00000000FF))

    write_data.append(checksum_calc(write_data))

    try:

        serial_port_data = str(serial_port_var.get())

        # open serial port
        ser = serial.Serial(serial_port_data, 115200)

        # waiting for cfg string
        read_data = serialRead(ser)
        if (read_data != CFG):
            raise Exception

        # write config buffer
        serialWrite(ser, write_data)
        print("Config buffer")
        hexPrint(write_data)

        # waiting for response
        print("Response")
        read_data = serialRead(ser)
        hexPrint(read_data)

        # close serial port
        ser.close()

        messagebox.showinfo("INFO", "Success!")

    except:

        messagebox.showerror("ERROR", "Fail!")

# ------------------------------------------------------------------------------

# Root window

# Create
root = tk.Tk()

# Size
screen_width = root.winfo_screenwidth()
screen_height = root.winfo_screenheight()

# Window title
root.title('Config App')

# Disable maximize button
root.resizable(width=False, height=False)

os.system('clear')

# ------------------------------------------------------------------------------

available_ports = []

# Default values
default_measurement_interval = tk.StringVar(root, value='250')
default_send_interval = tk.StringVar(root, value='10')

# Radio types
dev_types = ['Touro', 'Vaca', 'DIV']

# ------------------------------------------------------------------------------

# Widgets

config_frame = tk.Frame(root)
config_frame.pack(side=tk.TOP)

# Serial Port

serial_label = tk.Label(config_frame, text='Serial Port')
serial_label.grid(row=0, column=0, padx=5, pady=5)

available_ports = refresh_serial()

serial_port_var = tk.StringVar(root)
serial_port_var.set(available_ports[0])

serial_popup = tk.OptionMenu(config_frame, serial_port_var, *available_ports)
serial_popup.config(width=15)
serial_popup.grid(row=0, column=1, padx=5, pady=5)

# Device Type

dev_type_label = tk.Label(config_frame, text='Tipo')
dev_type_label.grid(row=1, column=0, padx=5, pady=5)

dev_type_var = tk.StringVar(root)
dev_type_var.set(dev_types[0])

dev_type_popup = tk.OptionMenu(config_frame, dev_type_var, *dev_types)
dev_type_popup.config(width=15)
dev_type_popup.grid(row=1, column=1, padx=5, pady=5)

# ID

id_label = tk.Label(config_frame, text='ID')
id_label.grid(row=2, column=0, padx=5, pady=5)

id_entry = tk.Entry(config_frame, justify='center', width=15)
id_entry.grid(row=2, column=1, padx=5, pady=5)

# Measurement Interval

measurement_interval_label = tk.Label(config_frame, text='Intervalo de Medição (ms)')
measurement_interval_label.grid(row=3, column=0, padx=5, pady=5)

measurement_interval_entry = tk.Entry(config_frame, justify='center', textvariable=default_measurement_interval, width=15)
measurement_interval_entry.grid(row=3, column=1, padx=5, pady=5)

# Send Interval

send_interval_label = tk.Label(config_frame, text='Intervalo de Envio (s)')
send_interval_label.grid(row=4, column=0, padx=5, pady=5)

send_interval_entry = tk.Entry(config_frame, justify='center', textvariable=default_send_interval, width=15)
send_interval_entry.grid(row=4, column=1, padx=5, pady=5)

# Update Button

buttons_frame = tk.Frame(root)
buttons_frame.pack(side=tk.TOP)

send_button = tk.Button(buttons_frame, text='Send Config', command=send_config, width=15)
send_button.grid(row=0, column=0, padx=5, pady=5)

# ------------------------------------------------------------------------------

# LED Example
# led0 = tk.Button(buttons_frame, text='', state="disabled", width=1, bg='gray')
# led0.grid(row=0, column=1, padx=5, pady=5)
# LED Set
# led0.configure(bg="red")

# ------------------------------------------------------------------------------

# Main Loop

root.mainloop()
