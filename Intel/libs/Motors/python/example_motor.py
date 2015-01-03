'''
*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <minipada@gmail.com> wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.   David Bensoussan
 * ----------------------------------------------------------------------------
 */
'''

import mraa
import time
import signal
import sys

# On Intel Edison
pwm_pin_number_list  = [3,5,6,9,10,11]
gpio_pin_number_list = [2,4,7,8,12,13]

pwm_list  =     [mraa.Pwm(3),
                mraa.Pwm(5),
                mraa.Pwm(6),
                mraa.Pwm(9)]
                #mraa.Pwm(10),
                #mraa.Pwm(11)]

gpio_list =     [mraa.Gpio(2),
                mraa.Gpio(4),
                mraa.Gpio(7),
                mraa.Gpio(8),
                mraa.Gpio(12),
                mraa.Gpio(13)]

print("Gpios and PWM pins initialized")
running = 1

# Define state of PWM (enabled or disabled), has to be 0 or 1
def state_gpio(gpio_pin,state):
  if (state == 1):
    gpio_list[gpio_pin_number_list.index(gpio_pin)].write(state)
    return
  elif (state == 0):
    gpio_list[gpio_pin_number_list.index(gpio_pin)].write(state)
    return
  else :
    print("Bad state pin_value")
    return

# PWM value, has to be a float between 0 and 1
def state_pwm(pin_number, pin_value):
  pwm_list[pwm_pin_number_list.index(pin_number)].write(pin_value)
  return

# Enable the pwm pin
def enable_pwm(pin_number):
  pwm_list[pwm_pin_number_list.index(pin_number)].enable(True)
  return

# Disable the pwm pin
def disable_pwm(pin_number):
  pwm_list[pwm_pin_number_list.index(pin_number)].enable(False)
  return

# Define a gpio as an output or an input
def in_out(pin_number, state):
  state = state.upper()
  if (state == "OUT"):
    gpio_list[gpio_pin_number_list.index(pin_number)].dir(mraa.DIR_OUT)
    return
  elif (state == "IN"):
    gpio_list[gpio_pin_number_list.index(pin_number)].dir(mraa.DIR_IN)
    return

def signal_handler(signal, frame):
  print('You pressed Ctrl+C!')
  state_gpio(gpio_b, 0)
  state_pwm(pwm_b, 0)
  disable_pwm(pwm_b)
  sys.exit(0)

signal.signal(signal.SIGINT, signal_handler)

#Setup
#Output for B
pwm_b = 9
gpio_b = 8
slow = 0.5
fast = 0.8
sleep_timer = 1
forward = 1
backward = 0

in_out(gpio_b,"OUT")
enable_pwm(pwm_b)

while True:
  #Forward and fast
  print("B Forward fast")
  state_gpio(gpio_b,forward)
  state_pwm(pwm_b,fast)
  time.sleep(sleep_timer)

  #Backward and slow
  print("B Backward slow")
  state_gpio(gpio_b,backward)
  state_pwm(pwm_b,slow)
  time.sleep(sleep_timer)