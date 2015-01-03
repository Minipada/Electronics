#ifndef LIBMOTOR_H
#define LIBMOTOR_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "mraa.h"
#include "mraa/gpio.h"

// On intel Edison, there are 6 pwms and 6 gpios
// You can't use them at the same time, read documentation
#define PWM_NUMBER_PIN 6
#define GPIO_NUMBER_PIN 6
#define PWM_ENABLE 1
#define PWM_DISABLE 0
#define DEFAULT_FREQUENCY 700
#define GPIO_MOTOR_1 2
#define GPIO_MOTOR_2 8
#define PWM_MOTOR_1 3
#define PWM_MOTOR_2 9

mraa_gpio_context enable_gpio(mraa_gpio_context gpio_motor, int gpio_pin);
mraa_pwm_context enable_pwm(mraa_pwm_context pwm_motor, int pwm_pin);
void sig_handler(int signo);

// Functions to enable gpios
mraa_gpio_context enable_gpio(mraa_gpio_context gpio_motor, int gpio_pin)
{
  // Initialiaze the pin
  gpio_motor = mraa_gpio_init(gpio_pin);
  mraa_result_t r = MRAA_SUCCESS;
  
  // Catch success or fail of the initialization
  r = mraa_gpio_dir(gpio_motor, MRAA_GPIO_OUT);

  if (r != MRAA_SUCCESS){
    mraa_result_print(r);
  }
  fprintf(stdout, "Pin %d set to output\n", gpio_pin);

  return gpio_motor;
}

// Functions to enable pwm pins
mraa_pwm_context enable_pwm(mraa_pwm_context pwm_motor, int pwm_pin)
{
  if (pwm_motor == NULL) 
  {
    fprintf(stderr, "Can't initialiaze pwm pin\n");
    exit(1);
  }

  // Initialize pwm pin
  pwm_motor = mraa_pwm_init(pwm_pin);

  //Set the PWM's pin frequency
  mraa_pwm_period_us(pwm_motor, DEFAULT_FREQUENCY);

  // Enable the pwm pin
  mraa_pwm_enable(pwm_motor, PWM_ENABLE);

  return pwm_motor;
}

#endif /* LIBMOTOR_H */