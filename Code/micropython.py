import serial
import time
ser = serial.Serial('COM4', 9600)
time.sleep(2)
data = []
b = ser.readline()         # read a byte string
string_n = b.decode()  # decode byte string into Unicode  
string = string_n.rstrip() # remove \n and \r
flt = float(string)        # convert string to float
data.append(flt)
summation = 0
time.sleep(0.1)
next = True
while (next):
    if flt == 0:
        next = False
    else:
      flt = float(ser.readline().decode().rstrip())
      data.append(flt)
      summation += (data[0]+data[1])*0.1/2  #take the area of the trapezoid formed between two points
      del data[0]
      byte_command = encode(summation)
      ser.writelines(byte_command)
      time.sleep(0.1)

ser.close()
