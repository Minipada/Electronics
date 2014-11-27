import serial

ser = serial.Serial('/dev/ttyS0',38400)

while True:
	line = ser.readline()
	print(line)

ser.close()
