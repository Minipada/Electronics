import mraa
import time

pwms = [mraa.Pwm(3),mraa.Pwm(9)]
dirs = [mraa.Gpio(2), mraa.Gpio(8)]

pwms[0].period_us(700)


pwms[0].enable(True)
pwms[1].enable(True)

dirs[0].dir(mraa.DIR_OUT)
dirs[1].dir(mraa.DIR_OUT)

while True:
        print("dirs 0 = 1, pwms 0 = 50")
        dirs[0].write(1)
        pwms[0].write(50)

        print("dirs 1 = 0, pwms 0 = 50")
        dirs[1].write(1)
        pwms[1].write(50)
        time.sleep(2)

        print("dirs 0 = 0, pwms 0 = 200")
        dirs[0].write(0)
        pwms[0].write(200)

        print("dirs 1 = 1, pwms 0 = 200")
        dirs[1].write(0)
        pwms[1].write(200)
        time.sleep(2)
