#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "mraa.h"
#include "mraa/gpio.h"
#include <errno.h>
#include <signal.h>

#define PWM_NUMBER_PIN 6
#define GPIO_NUMBER_PIN 6
#define PWM_ENABLE 1
#define PWM_DISABLE 0
#define DEFAULT_FREQUENCY 700
#define GPIO_MOTOR_1 2
#define GPIO_MOTOR_2 8
#define PWM_MOTOR_1 3
#define PWM_MOTOR_2 9

int running = 0;
static int iopin = GPIO_MOTOR_2;

mraa_gpio_context enable_gpio(mraa_gpio_context gpio_motor, int gpio_pin);
mraa_pwm_context enable_pwm(mraa_pwm_context pwm_motor, int pwm_pin);

mraa_gpio_context enable_gpio(mraa_gpio_context gpio_motor, int gpio_pin)
{

  gpio_motor = mraa_gpio_init(gpio_pin);
  mraa_result_t r = MRAA_SUCCESS;
  
  r = mraa_gpio_dir(gpio_motor, MRAA_GPIO_OUT);
  fprintf(stdout, "Pin %d set to output\n", gpio_pin);

  if (r != MRAA_SUCCESS){
    mraa_result_print(r);
  }
  fprintf(stdout, "pin %d enabled\n", gpio_pin);

  return gpio_motor;
}

mraa_pwm_context enable_pwm(mraa_pwm_context pwm_motor, int pwm_pin)
{
  if (pwm_motor == NULL) 
  {
    fprintf(stderr, "Can't initialiaze pwm pin\n");
    exit(1);
  }

  pwm_motor = mraa_pwm_init(PWM_MOTOR_2);
  mraa_pwm_period_us(pwm_motor, DEFAULT_FREQUENCY);
  mraa_pwm_enable(pwm_motor, PWM_ENABLE);

  return pwm_motor;
}

void sig_handler(int signo)
{
    if (signo == SIGINT) {
        printf("closing IO%d nicely\n", iopin);
        running = -1;
    }
}

int main()
{
	mraa_init();
  mraa_gpio_context gpio_motor;
  mraa_pwm_context pwm_motor;
  signal(SIGINT, sig_handler);

  //  in_out(GPIO_MOTOR_2, pinout, "OUT");
  gpio_motor = enable_gpio(gpio_motor, GPIO_MOTOR_2);

  //enable_pwm(PWM_MOTOR_2, pinout);
  pwm_motor = enable_pwm(pwm_motor, PWM_MOTOR_2);
  
  int direction = 1;
  float speed = 0;

  mraa_gpio_write(gpio_motor, direction);

  while(running == 0)
    {
      speed = speed + 0.2;
      printf("speed = %f\n", speed);

      if (speed >= 1)
      {
        speed = 0;
        direction = !direction;
        printf("dir = %d\n", direction);
        
        //state_gpio(GPIO_MOTOR_2, pinout, direction);
        mraa_gpio_write(gpio_motor, direction);
      }
    
      //state_pwm(PWM_MOTOR_2, speed, pinout);
      mraa_pwm_write(pwm_motor, speed);

      sleep(1);
    }
    // After catching the signal
    mraa_pwm_write(pwm_motor,0);
    mraa_gpio_close(gpio_motor);
    printf("end\n");

	return 0;
}