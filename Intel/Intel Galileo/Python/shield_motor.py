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


# On Intel Galileo
pwm_pin_number_list  = [3,5,6,9,10,11]
gpio_pin_number_list = [2,4,7,8,12,13]

pwm_list  =     [mraa.Pwm(3), 
                mraa.Pwm(5), 
                mraa.Pwm(6), 
                mraa.Pwm(9), 
                mraa.Pwm(10), 
                mraa.Pwm(11)]

gpio_list =     [mraa.Gpio(2),
                mraa.Gpio(4),
                mraa.Gpio(7),
                mraa.Gpio(8),
                mraa.Gpio(12),
                mraa.Gpio(13)]

''' To test
pwm_list = []
gpio_list = []
i = 0;

while i < len(pwm_pin_number_list):
        pwm_list[i] = mraa.Pwm(pwm_pin_number_list[i])
        i++;

i = 0;
while i < len(gpio_pin_number_list):
        gpio_pin[i] = mraa.Gpio(gpio_pin_number_list[i])
        i++

'''

'''
On Intel Edison, you have to choose what Pwm you use. 
So just edit the 2 arrays
'''

i = 0
print("Gpios and PWM pins initialized")

#Define state of PWM (enabled or disabled)
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

def state_pwm(pin_number, pin_value):
        pwm_list[pwm_pin_number_list.index(pin_number)].write(pin_value)
        return

def enable_pwm(pin_number):
  pwm_list[pwm_pin_number_list.index(pin_number)].enable(True)
  return

def disable_pwm(pin_number):
        pwm_list[pwm_pin_number_list.index(pin_number)].enable(False)
        return

def in_out(pin_number, state):
  if (state == "OUT"):
    gpio_list[gpio_pin_number_list.index(pin_number)].dir(mraa.DIR_OUT)
    return
  elif (state == "IN"):
    gpio_list[gpio_pin_number_list.index(pin_number)].dir(mraa.DIR_IN)
    return


#Setup
#Output for A
pwm_a = 3
pwm_b = 9
gpio_a = 2
gpio_b = 8
slow = 0.4
fast = 0.8
sleep_timer = 1
forward = 1
backward = 0

in_out(gpio_a,"OUT")
in_out(8,"OUT")
enable_pwm(3)
enable_pwm(9)

while True:
        # Forward and slow
        state_gpio(gpio_a, forward)
        state_pwm(pwm_a, slow)
  print("sleep")
        time.sleep(sleep_timer)

        #Forward and fast
        state_gpio(gpio_b,forward)
        state_pwm(pwm_b,fast)
  print("sleep")
        time.sleep(sleep_timer)

        #Backward and slow
        state_gpio(gpio_a, backward)
        state_pwm(pwm_a, fast)
  print("sleep")
        time.sleep(sleep_timer)

        #Backward and slow
        state_gpio(gpio_b,backward)
        state_pwm(pwm_b,slow)
  print("sleep")
  time.sleep(sleep_timer)