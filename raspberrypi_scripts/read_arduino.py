import serial
ser = serial.Serial('/dev/ttyACM0', 9600)

ser.write('150')

while 1 :
    ser.readline()
