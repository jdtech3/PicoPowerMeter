import serial
import time
import matplotlib.pyplot as plt

ser = serial.Serial('COM3', 115200)
time.sleep(2)

while True:
    ser.reset_input_buffer()
    b = ser.readline()
    time.sleep(1)
    print(float(b.decode().rstrip()))

# Voltages = []

# for i in range(50):
#     b = ser.readline()
#     Voltages.append(float(b.decode().rstrip()))
#     time.sleep(0.1)           

# ser.close()

# plt.plot(Voltages)
# plt.xlabel('Time (seconds)')
# plt.ylabel('Voltage Reading (Volts)')
# plt.title('Voltage Reading vs Time')
# plt.show()