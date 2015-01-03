#include <stdio.h>
#include <string.h>
#include "mraa.h"
#include "mraa/gpio.h"
#include <signal.h>
#include "libmotor.h"

// Catch a ctrl-c signal
int running = 0;
static int iopin = GPIO_MOTOR_2;

// Signal handler
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
  
  // direction is a boolean
  int direction = 1;
  float speed = 0;

  // Write gpio pin value
  mraa_gpio_write(gpio_motor, direction);

  // Running is set to 1 when Ctrl-C is hit
  while(running == 0)
    {
      speed = speed + 0.2;
      printf("speed = %f\n", speed);

      if (speed >= 1)
      {
        speed = 0;
        direction = !direction;
        printf("dir = %d\n", direction);
        
        mraa_gpio_write(gpio_motor, direction);
      }
    
      mraa_pwm_write(pwm_motor, speed);
      sleep(1);
    }
    
    // After catching the signal
    mraa_pwm_write(pwm_motor,0);
    mraa_gpio_close(gpio_motor);
    printf("end\n");

	return 0;
}