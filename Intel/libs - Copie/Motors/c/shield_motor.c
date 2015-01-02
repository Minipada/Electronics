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

typedef struct structpins
{
  int pwmnb[PWM_NUMBER_PIN];
  int pwmfreq[PWM_NUMBER_PIN];
  int gpionb[PWM_NUMBER_PIN];
  int gpiost[PWM_NUMBER_PIN];
} enabledpins;

int running = 0;


void state_gpio(int gpio_pin, enabledpins pinout, int state);
void state_pwm(int pin_number, float pin_value,enabledpins pinout);
void enable_pwm(int pin_number, enabledpins pinout);
void disable_pwm(int pin_number,enabledpins pinout);
void in_out(int iopin, enabledpins pinout, char *inout);
int check_good_pin(int pin_number);
int check_if_enabled(int pin_number, enabledpins pinout, char* pin_type);
void add_gpio_in_struct_pin_to_struct(int pin_number, int frequency, enabledpins pinout);
int check_pwm_pin_value(float pin_value);
void init_mraa_pwms_gpios_enabled(enabledpins pinout);
void add_gpio_in_struct(int iopin, int state , enabledpins pinout);

void add_gpio_in_struct(int iopin, int state , enabledpins pinout)
{
  //No need to check if iopin is valid
  //check has been done earlier
  int i;

  for (i = 0; i < GPIO_NUMBER_PIN; i++)
  {
    if (pinout.gpionb[i] == 0)
    {
      pinout.gpionb[i] = iopin;
      pinout.gpiost[i] = state;
    }
  }
}


void in_out(int iopin, enabledpins pinout, char *inout)
{

  int check_flag = -1;

  //Check if the pin has already been initialized in the structure
  check_flag = check_if_enabled(iopin, pinout, "gpio");
  
  //If yes, just leave here
  if (check_flag != 0)
  {
    return ;
  }
  //If no, just continue

  mraa_gpio_context gpio;
  gpio = mraa_gpio_init(iopin);
  
  if (gpio == NULL) 
  {
    fprintf(stderr, "Are you sure that pin %d you requested is valid on your platform?", iopin);
    exit(1);
  }
  
  printf("Pin %d initialized\n", iopin);
  
  mraa_result_t r = MRAA_SUCCESS;

  if ((strcmp(inout, "OUT") == 0) || (strcmp(inout, "out") == 0))
  {
    r = mraa_gpio_dir(gpio, MRAA_GPIO_OUT);
    
    fprintf(stdout, "Pin %d set to output\n", iopin);

    add_gpio_in_struct(iopin, MRAA_GPIO_OUT, pinout);
    
    if (r != MRAA_SUCCESS){
      mraa_result_print(r);
    }
  }
  else if ((strcmp(inout, "IN") == 0) || (strcmp(inout, "in") == 0))
  {
    r = mraa_gpio_dir(gpio, MRAA_GPIO_IN);

    fprintf(stdout, "Pin %d set to input\n", iopin);

    add_gpio_in_struct(iopin, MRAA_GPIO_IN,pinout);

    if (r != MRAA_SUCCESS){
      mraa_result_print(r);
    }
  }
  else
  {
    fprintf(stderr, "Bad inout value, please use \"in\" or \"out\"\n");
    exit(1);
  }
}

int get_io_state(int gpio_pin, enabledpins pinout)
{
  int i;
  for (i = 0; i < GPIO_NUMBER_PIN; i++)
  {
    if (pinout.gpiost[i] == 0)
    {
      fprintf(stderr, "GPIO %d not initialized\n", gpio_pin);
      exit(1);
    }
    else if (pinout.gpionb[i] == gpio_pin)
    {
      return pinout.gpiost[i];
    } 
  }
}

void state_gpio(int gpio_pin, enabledpins pinout, int state)
{
  // Check si l'état est bien de 0 ou 1
  if ((state != 1) && (state != 0))
  {
    fprintf(stderr, "Please use 0 or 1 as state for %d\n",
      gpio_pin);
    exit(1);
  }

  //Check si le pin du gpio est correct
  if ((gpio_pin != 2) && (gpio_pin != 4) && 
    (gpio_pin != 7) && (gpio_pin != 8) && 
    (gpio_pin != 12) && (gpio_pin != 13))
  {
    fprintf(stderr, "%d is not a good value for a gpio. Please use 2,4,7,8,12 or 13\n",gpio_pin);
    exit(1);     
  }
  
  mraa_gpio_context gpio;
  gpio = mraa_gpio_init(gpio_pin);

  mraa_result_t r = MRAA_SUCCESS;
  r = mraa_gpio_dir(gpio, get_io_state(gpio_pin,pinout));


  r = mraa_gpio_write(gpio, state);
  if (r != MRAA_SUCCESS) {
    mraa_result_print(r);
  } 
  else {
    if (state == 0)
    {
      printf("%d off\n",state);
    }
    else
    {
      printf("%d on\n", state);
    }
  }
}

int check_good_pin(int pin_number)
{
  if ((pin_number == 3) || (pin_number == 5) || 
    (pin_number == 6) || (pin_number == 9) || 
    (pin_number == 10) ||(pin_number == 11))
  {
    return 0;
  }
  
  fprintf(stderr, "%d is not a good value for PWM. Please use 3,5,6,9,10 or 11\n",pin_number);
  return 1;
}

int check_if_enabled(int pin_number, enabledpins pinout, char* pin_type)
{
  int i;
  
  if (strcmp(pin_type, "gpio") == 0)
  {
    for (i = 0; i < PWM_NUMBER_PIN; i++)
    {
      if (pinout.pwmnb[i] == pin_number)
      {
        fprintf(stderr, "pin %d has already been initialized.\n", 
          pin_number);
        return 1;
      }
      if (pinout.pwmnb[i] == 0)
      {
        fprintf(stderr, "Pin %d not initialized yet.\n", 
          pin_number);
        return 0;
      }
    }  
  }
  else
  {
    for (i = 0; i < GPIO_NUMBER_PIN; i++)
    {
      if (pinout.gpionb[i] == pin_number)
      {
        fprintf(stderr, "pin %d has already been initialized.\n", 
          pin_number);
        return 1;
      }
      if (pinout.gpionb[i] == 0){
        fprintf(stderr, "Pin %d not initialized yet.\n", 
          pin_number);
        return 0;
      }
    }
  }
}

void add_pin_to_struct(int pin_number, int frequency, enabledpins pinout)
{
  int i;


  for (i = 0; i < PWM_NUMBER_PIN; i++)
  {
    // I don't need to check if it has already been enabled
    // It has been done earlier
    if (pinout.pwmnb[i] == 0)
    {
      pinout.pwmnb[i] = pin_number;
      pinout.pwmfreq[i] = frequency;
    }
  }
}

void enable_pwm(int pin_number, enabledpins pinout)
{
  //Check if the pin_number is correct
  int check_flag = -1;

  check_flag = check_good_pin(pin_number);
  if (check_flag == 1)
  {
    exit(1);
  }
  
  //Check if it has already been enabled. have only 6 pwms
  check_flag = check_if_enabled(pin_number, pinout, "pwm");

  if (check_flag == 1)
  {
    exit(1);
  }

  // Initialize the pwm pin
  mraa_pwm_context pwm;

  if (pwm == NULL) 
  {
    fprintf(stderr, "Can't initialiaze pwm pin\n");
    exit(1);
  }
  pwm = mraa_pwm_init(pin_number);
  mraa_pwm_period_us(pwm, DEFAULT_FREQUENCY);
  mraa_pwm_enable(pwm, PWM_ENABLE);

  add_pin_to_struct(pin_number, DEFAULT_FREQUENCY, pinout);

  fprintf(stdout, "pin %d enabled\n", pin_number);
}

void disable_pwm(int pin_number,enabledpins pinout)
{
  //Check if the pin_number is correct
  int check_flag = -1;

  check_flag = check_good_pin(pin_number);
  if (check_flag == 1)
  {
    exit(1);
  }
  
  //Check if it has already been enabled. have only 6 pwms
  check_flag = check_if_enabled(pin_number,pinout, "pwm");

  if (check_flag == 1)
  {
    exit(1);
  }

  // Initialize the pwm pin
  mraa_pwm_context pwm;

  if (pwm == NULL) 
  {
    fprintf(stderr, "Can't initialiaze pwm pin\n");
    exit(1);
  }
  pwm = mraa_pwm_init(pin_number);
  mraa_pwm_enable(pwm, PWM_DISABLE);

  fprintf(stdout, "pin %d disabled\n", pin_number);
}

int check_pwm_pin_value(float pin_value)
{
  if ((pin_value < 0) && (pin_value > 1))
  {
    fprintf(stdout, "Wrong value. Please use a float between 0 and 1\n");
    return 0;
  }
  return 1;
}

void state_pwm(int pin_number, float pin_value, enabledpins pinout)
{
  //Check if pin_value is correct
  if (check_pwm_pin_value(pin_value) == 0)
  {
    exit(1);
  }

  fprintf(stdout, "Pin value %f is correct\n", pin_value);

  // Check if the pwm pin has been enabled
  int check_flag = -1;
  check_flag = check_if_enabled(pin_number, pinout,"pwm");

  if (check_flag == 1)
  {
    exit(1);
  }

  fprintf(stdout, "PWM %d not enabled \n", pin_number);

  // Access to the pwm
  mraa_pwm_context pwm = mraa_pwm_init(pin_number);;
  
  if (pwm == NULL) {
    exit(1);
  }

  mraa_pwm_write(pwm, pin_value);

  fprintf(stdout, "pwm %d at value %f\n", 
    pin_number, pin_value);
}

void init_mraa_pwms_gpios_enabled(enabledpins pinout)
{
  int i;
  for (i = 0; i < PWM_NUMBER_PIN; i++)
  {
    pinout.pwmnb[i] = 0;
    pinout.pwmfreq[i] = 0;
  }

  for (i = 0; i < GPIO_NUMBER_PIN; i++)
  {
    pinout.gpiost[i] = 0;
    pinout.gpionb[i] = 0;
  }
}


int main ()
{
  mraa_init();

  enabledpins pinout;

  init_mraa_pwms_gpios_enabled(pinout);

  in_out(GPIO_MOTOR_2, pinout, "OUT");
  enable_pwm(PWM_MOTOR_2, pinout);
  
  int direction = 0;

  state_gpio(GPIO_MOTOR_2, pinout, direction);

  float speed = 0;



  while(running == 0)
    {
      speed = speed + 0.2;
    
      if (speed >= 0.8)
      {
        speed = 0;

        if (direction == 0)
        {
          direction = 1;
          printf("dir = 1\n");
        }
        else
        {
          direction = 0;
          printf("dir = 0\n");
        }

        state_gpio(GPIO_MOTOR_2, pinout, direction);
      }
    
      state_pwm(PWM_MOTOR_2, speed, pinout);
      sleep(1);
    }


  return 0;
}