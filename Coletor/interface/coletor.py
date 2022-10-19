import serial
import datetime
import serial.tools.list_ports
import signal
import sys
import pandas as pd
import numpy as np
from csv import writer 
import inferencia
import time


# ------------------------------------------------------------------------------

def data_debug(data): 
    debug_len = 10

    print(f'\n[{datetime.datetime.now().strftime("%H:%M:%S")}] Packet Received (Size = {len(data)})\n')

    print('-' * (debug_len*5 + 4))

    print('    |', end = '')

    for i in range(debug_len):
        print(f'{i:>4} ', end = '')

    print('\n' + '-' * (debug_len*5 + 4), end = '')

    for i, data in enumerate(data):

        if (i%debug_len == 0):
                print(f'\n{int(i/debug_len):>3} |', end = '')
        print(f'{data:>4} ', end = '')
        dados = f'{int(data):>4}'

    print('\n' + '-' * (debug_len*5 + 4) )

# ------------------------------------------------------------------------------

def sigint_handler(signum, frame):
    print('\nClosing serial...')
    ser.close()
    print('Done.')
    sys.exit(0)

# ------------------------------------------------------------------------------

def write_data_to_file(data, index, LOG_DIR_NAME):
    delimiter = ';'
    filename = LOG_DIR_NAME+'/ID_'+(str(index).zfill(2))
    data_csv = delimiter.join(list(map(str, data)))
    with open(filename, 'a') as file:
            file.write(data_csv+'\n')
    file.close()

# # create log dir

# write data to serial
def serialWrite(ser, write_data):

    try:
        send_data_raw = bytearray(write_data)
        ser.write(send_data_raw)
        return True
    except:
        return False


def refresh_serial():

    available_ports = []

    ports = list(serial.tools.list_ports.comports())

    for port in ports:
        if port.hwid != "n/a":
            available_ports.append(port.device)

    return available_ports

# ------------------------------------------------------------------------------

available_ports = refresh_serial()

while(True):

    # clear terminal
    print("\033c", end="")

    print('-'*40)
    print('- Serial Port')
    print('-'*40, end="\n\n")

    for i, port in enumerate(available_ports):
        print(f'[{i}] {port}')
    print('[r] refresh ports')
    print('[q] quit\n')

    key = input('Choose an option and press enter: ')

    if ( key == 'r' ):
        available_ports = refresh_serial()
        continue
    elif ( key == 'q' ):
        quit()
    elif ( key.isnumeric() ) and ( int(key) < len(available_ports) ):
        # clear terminal
        print("\033c", end="")
        print('-'*40)
        print(f'- Serial Port = {available_ports[int(key)]}')
        print('-'*40, end="\n\n")
        serial_port = available_ports[int(key)]
        break
    else:
        print('--> Opção inválida!')
        time.sleep(1)

# ------------------------------------------------------------------------------
# open serial port
ser = serial.Serial(serial_port, 115200, timeout=1)
ser.reset_input_buffer()
# Ctrl + C
int_list = []

signal.signal(signal.SIGINT, sigint_handler)
print('Waiting for data...')
while True:
    s = ser.read(220)  
    if (len(s)!=0):
        inicio = time.time()
        int_list_aux = [b for b in s]
        #print(int_list_aux[3])
        if (int_list_aux[3]==0): #comando recebido 
            #get ID, type and timestamp
            write_data = []
            animal_id = int_list_aux[0]
            animal_type = int_list_aux[1]
            timestamp = int_list_aux[6]<<24 | int_list_aux[7]<<16 | int_list_aux[8]<<8 | int_list_aux[9]
            animal_counter_send = int_list_aux[10]<<8 | int_list_aux[11]
            for i in range (10):
                del int_list_aux[0]
            int_list_aux = int_list_aux[:len(int_list_aux)-10]
            a = np.resize(int_list_aux,(50,4))
            int_list.append(a)
            df = pd.DataFrame(a)
            b = df.iloc[:1]
            #df.index = datetime.datetime.now().strftime("%Y.%m.%d.%H.%M.%S")
            fim = time.time()
            print("Tempo parser =", (fim - inicio))
            inicio1 = time.time()
            inferencia.update_data(b)
            fim = time.time()
            print("Tempo execução Total =", (fim - inicio))
            #write_data.append(0x77)
            # write config buffer
            #serialWrite(ser, write_data)
            #print("Config buffer")
        else:
            data_debug(s) 
# # -----------------------------------------------------------------------------
# # Modules
# # -----------------------------------------------------------------------------

# import time, threading
# import serial
# import serial.tools.list_ports
# import datetime
# import os
# import signal
# import sys

# SERIAL_READ_TIMEOUT = 2

# counter_fails_crc = 0

# # -----------------------------------------------------------------------------
# # Functions
# # -----------------------------------------------------------------------------

# def write_data_to_file(data, index, LOG_DIR_NAME):
#     delimiter = ';'
#     filename = LOG_DIR_NAME+'/ID_'+(str(index).zfill(2))
#     data_csv = delimiter.join(list(map(str, data)))
#     with open(filename, 'a') as file:
#             file.write(data_csv+'\n')
#     file.close()

# # -----------------------------------------------------------------------------

# def checksum_calc(data):

#     checksum = 0

#     for value in data:
#         checksum ^= value

#     return(checksum)

# # -----------------------------------------------------------------------------

# def data_debug(data):

#     debug_len = 10

#     print(f'\n[{datetime.datetime.now().strftime("%H:%M:%S")}] Packet Received (Size = {len(data)})\n')

#     print('-' * (debug_len*5 + 4))

#     print('    |', end = '')

#     for i in range(debug_len):
#         print(f'{i:>4} ', end = '')

#     print('\n' + '-' * (debug_len*5 + 4), end = '')

#     for i, data in enumerate(data):

#         if (i%debug_len == 0):
#                 print(f'\n{int(i/debug_len):>3} |', end = '')

#         print(f'{data:>4} ', end = '')

#     print('\n' + '-' * (debug_len*5 + 4) )

# # -----------------------------------------------------------------------------

# def read_serial(ser, LOG_DIR_NAME):

#     global counter_fails_crc
#     # global time_last

#     read_data = []

#     # waiting for data available
#     n = int.from_bytes(ser.read(1), byteorder='big')
#     t0 = time.time()

#     while True:
#         if ser.in_waiting == n+2:
#             break
#         t1 = time.time() - t0
#         if t1 > SERIAL_READ_TIMEOUT:
#             print("Timeout!")
#             ser.reset_input_buffer()
#             threading.Timer(.1, read_serial(ser, LOG_DIR_NAME)).start()
#             return False

#     for index_read in range (n+2):   # tamanho(dados, sem CRC e RSSI) + dados + CRC + RSSI
#         read_data_raw = ser.read(1)
#         read_data.append(int.from_bytes(read_data_raw, byteorder='big'))

#     # data debug
#     data_debug(read_data)

#     crc = read_data[-2] # CRC
#     del(read_data[-2]) # retirar CRC dos Dados

#     if(crc == checksum_calc(read_data[:-1])): # checksum sem RSSI
#         write_data_to_file(read_data, read_data[0], LOG_DIR_NAME)
#         return read_data
#     else:
#         counter_fails_crc += 1
#         print(f'CRC Pacote    = {crc}')
#         print(f'CRC Calculado = {checksum_calc(read_data[:-1])}')
#         return False

# # -----------------------------------------------------------------------------

# def sigint_handler(signum, frame):
#     print('\nClosing serial...')
#     ser.close()
#     print('Done.')
#     sys.exit(0)

# # -----------------------------------------------------------------------------

# def refresh_serial():

#     available_ports = []

#     ports = list(serial.tools.list_ports.comports())

#     for port in ports:
#         if port.hwid != "n/a":
#             available_ports.append(port.device)

#     return available_ports

# # -----------------------------------------------------------------------------

# available_ports = refresh_serial()

# while(True):

#     # clear terminal
#     print("\033c", end="")

#     print('-'*40)
#     print('- Serial Port')
#     print('-'*40, end="\n\n")

#     for i, port in enumerate(available_ports):
#         print(f'[{i}] {port}')
#     print('[r] refresh ports')
#     print('[q] quit\n')

#     key = input('Choose an option and press enter: ')

#     if ( key == 'r' ):
#         available_ports = refresh_serial()
#         continue
#     elif ( key == 'q' ):
#         quit()
#     elif ( key.isnumeric() ) and ( int(key) < len(available_ports) ):
#         # clear terminal
#         print("\033c", end="")
#         print('-'*40)
#         print(f'- Serial Port = {available_ports[int(key)]}')
#         print('-'*40, end="\n\n")
#         serial_port = available_ports[int(key)]
#         break
#     else:
#         print('--> Opção inválida!')
#         time.sleep(1)

# # open serial port
# ser = serial.Serial(serial_port, 115200)
# ser.reset_input_buffer()

# # string date
# DATE = (datetime.datetime.now()).strftime("%Y_%m_%d_%H_%M")

# # create log dir
# LOG_DIR_NAME = './log/'+DATE
# if not os.path.exists(LOG_DIR_NAME):
#     os.makedirs(LOG_DIR_NAME)

# # Ctrl + C
# signal.signal(signal.SIGINT, sigint_handler)

# # Loop
# print('Waiting for data...')
# while(True):
#     read_serial(ser, LOG_DIR_NAME)