def two2dec(s):
    if s[0] == '1':
        return -1 * (int(''.join('1' if x == '0' else '0' for x in s), 2) + 1)
    else:
        return int(s, 2)

def acc_converter(acc_raw):
    str_raw = f'{int(acc_raw)<<8:0>16b}'
    value = two2dec(str_raw)
    # +-4g
    lsb_sens = 8192
    return value/lsb_sens

def gyro_converter(gyro_raw):
    str_raw = f'{int(gyro_raw)<<8:0>16b}'
    value = two2dec(str_raw)
    # +- 1000
    lsb_sens = 32.8
    return value/lsb_sens

def mag_converter(mag_raw):
    str_raw = f'{int(mag_raw)<<6:0>14b}'
    value = two2dec(str_raw)
    # +- 4800
    lsb_sens = 0.6
    return value/lsb_sens

def temp_converter(temp_raw):
    str_raw = f'{int(temp_raw):0>8b}'
    value = int(str_raw, 2)
    temperature = (value/255)*100
    return temperature

def rssi_converter(rssi_raw):
    return rssi_raw - 157