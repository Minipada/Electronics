import mraa
import time

x = mraa.Pwm(3)
x.enable(True)

y = mraa.Gpio(2)
y.dir(mraa.DIR_OUT)
y.write(1)

while True:
	x.write(200)
	time.sleep(2)
	x.write(100)
	time.sleep(2)
	y.write(0)
	time.sleep(2)
	x.write(50)
	time.sleep(2)
