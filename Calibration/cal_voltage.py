import pyvisa
import time
import serial
import json
import matplotlib.pyplot as plt

def vout(adc_reading):
    print(adc_reading)
    return adc_reading * (3.3 / 4096) / 0.130383

def setup():
    ser = serial.Serial('COM3', 115200)

    rm = pyvisa.ResourceManager()
    psu = rm.open_resource('USB0::0x05E6::0x2230::9104291::INSTR')
    psu.read_termination = '\n'
    psu.write_termination = '\n'

    print(psu.query('*IDN?'))
    psu.write('INST:NSEL 2')
    psu.write('VOLT:LIM 24.0')

    return ser, psu

def set_voltage(psu, v):
    query = f'APPL CH2, {v}, MAX'
    psu.write(query)

if __name__ == '__main__':
    ser, psu = setup()
    set_voltage(psu, 0)
    time.sleep(2)

    v = 0
    v_set = []
    v_read = []
    for _ in range(100):
        print(v)
        set_voltage(psu, v)
        time.sleep(1)
        ser.reset_input_buffer()
        b = ser.readline()
        v_read.append(vout(float(b.decode().rstrip())))
        v_set.append(v)
        v += 25 / 100

    ser.close()

    print(v_set)
    print(v_read)
    
    v_diff = [v_set[i] - v_read[i] for i in range(len(v_set))]
    plt.plot(v_diff)
    # plt.plot(v_set)
    # plt.plot(v_read)
    plt.xlabel('Time (seconds)')
    plt.ylabel('Voltage Reading (Volts)')
    plt.title('Voltage Reading vs Time')
    plt.show()
