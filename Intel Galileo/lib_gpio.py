# /*
# * ----------------------------------------------------------------------------
# * "THE BEER-WARE LICENSE" (Revision 42):
# * <minipada@gmail.com> wrote this file.  As long as you retain this notice you
# * can do whatever you want with this stuff. If we meet some day, and you think
# * this stuff is worth it, you can buy me a beer in return.   David Bensoussan
# * ----------------------------------------------------------------------------
# */


import mraa
import time

#Pins list on Arduino Uno 3
pwm_pin_number_list  = [3,5,6,9,10,11]
gpio_pin_number_list = [2,4,7,8,12,13]

#mraa pwms and gpios types
'''
mraa_pwm_list  = [mraa.Pwm(3), mraa.Pwm(5), mraa.Pwm(6), mraa.Pwm(9), mraa.Pwm(10), m
mraa_gpio_list = [mraa.Gpio(2),mraa.Gpio(4),mraa.Gpio(7),mraa.Gpio(8),mraa.Gpio(12),m
'''

mraa_pwm_list  = []
mraa_gpio_list = []

i = 0
while (i < len(pwm_pin_number_list)):
  mraa_pwm_list[i]  = mraa.Pwm(pwm_pin_number_list[i])
  mraa_gpio_list[i] = mraa.Gpio(gpio_pin_number_list[i])
  i = i + 1


print("Gpios and PWM pins initialized")

#Define state of PWM (enabled or disabled)
def state_gpio(gpio_pin,state):
  if (state == 1 or state == 0):
    mraa_gpio_list[gpio_pin_number_list.index(gpio_pin)].write(state)
    return
  else:
    print("Bad state value. Please use 0 or 1 as value")
    return

#Define pwm value at pin_value for pin_number
def pwm_value(pin_number, pin_value):
  #Pwm's period has to be defined before each "write" call.
  #It depends on the motor you use, but 700 often works
  mraa_pwm_list[pwm_pin_number_list.index(pin_number)].perriod_us(700)
  mraa_pwm_list[pwm_pin_number_list.index(pin_number)].write(pin_value)
  return

#Enable pwm pin for pin_number
def enable_pwm(pin_number):
  mraa_pwm_list[pwm_pin_number_list.index(pin_number)].enable(True)
  return

#Disable pwm pin for pin_number
def disable_pwm(pin_number):
  mraa_pwm_list[pwm_pin_number_list.index(pin_number)].enable(False)
  return

#Define a gpio pin (not a pwm) as an input or an output
def in_out(pin_number, state):
  if (state == "OUT"):
    mraa_gpio_list[gpio_pin_number_list.index(pin_number)].dir(mraa.DIR_OUT)
    return
  elif (state == "IN"):
    mraa_gpio_list[gpio_pin_number_list.index(pin_number)].dir(mraa.DIR_IN)
    return


#Setup
#Motor A
in_out(2,"OUT")
enable_pwm(3)

motor_speed = 0
direction = 0

#Example: Rotate the motor at "motor speed" in direction set by "direction" (0 or 1)
# ^= 1 corresponds to XOR gate, change 0 to 1 and 1 to 0
while True:
  state_gpio(2, direction)
  pwm_value(3, motor_speed)

  time.sleep(1)
  motor_speed = motor_speed + 0.1
  direction ^= 1

  state_gpio(2, direction)
  pwm_value(3, motor_speed)

  time.sleep(1)
  direction ^= 1  

  if motor_speed >= 1:
    motor_speed = 0