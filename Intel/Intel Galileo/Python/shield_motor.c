#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "mraa.h"
#include "mraa/gpio.h"
#include <errno.h>
#include <signal.h>

#define PWM_NUMBER_PIN 6
#define GPIO_NUMBER_PIN 6
#define PWM_ENABLE 0
#define PWM_DISABLE 1
#define DEFAULT_FREQUENCY 200
#define GPIO_MOTOR_1 2
#define GPIO_MOTOR_2 8
#define PWM_MOTOR_1 3
#define PWM_MOTOR_2 9

typedef struct enabled_pins
{
  int mraa_pwm_pin_number[PWM_NUMBER_PIN];
  int mraa_pwm_frequency[PWM_NUMBER_PIN];
  int mraa_gpio_pin_number[PWM_NUMBER_PIN];
  int mraa_gpio_pin_state[PWM_NUMBER_PIN];
} mraa_pins_enabled;

void state_gpio(int gpio_pin, mraa_pins_enabled my_pins, int state);
void state_pwm(int pin_number, float pin_value,mraa_pins_enabled my_pins);
void enable_pwm(int pin_number, mraa_pins_enabled my_pins);
void disable_pwm(int pin_number,mraa_pins_enabled my_pins);
void in_out(int iopin, char *inout);
int check_good_pin(int pin_number);
int check_if_enabled(int pin_number, mraa_pins_enabled my_pins, char* pin_type);
void add_pin_to_struct(int pin_number, int frequency, mraa_pins_enabled my_pins);
int check_pwm_pin_value(int pin_value);
void init_mraa_pwms_gpios_enabled(mraa_pins_enabled my_pins);
void add_gpio_in_struct(int iopin, int state , mraa_pins_enabled my_pins);

void add_gpio_in_struct(int iopin, int state , mraa_pins_enabled my_pins)
{
  //No need to check if iopin is valid
  //check has been done earlier
  int i;

  for (i = 0; i < GPIO_NUMBER_PIN; i++)
  {
    if (my_pins->mraa_pin_number[i] == 0)
    {
      my_pins->mraa_pin_number[i] = iopin;
      my_pins->mraa_pin_state[i] = state;
    }
  }
}


void in_out(int iopin, mraa_pins_enabled my_pins, char *inout)
{

  int check_flag = -1;

  //Check if the pin has already been initialized in the structure
  check_flag = check_if_enabled(iopin, my_pins, "gpio");
  
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
    r = mraa_gpio_dir(iopin, MRAA_GPIO_OUT);

    add_gpio_in_struct(iopin, MRAA_GPIO_OUT, my_pins);
    
    if (r != MRAA_SUCCESS){
      mraa_result_print(r);
    }
  }
  else if ((strcmp(inout, "IN") == 0) || (strcmp(inout, "in") == 0))
  {
    r = mraa_gpio_dir(iopin, MRAA_GPIO_IN);

    add_gpio_in_struct(iopin, MRAA_GPIO_IN,my_pins);

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

int get_io_state(int gpio_pin, mraa_pins_enabled my_pins)
{
  int i;
  for (i = 0; i < GPIO_NUMBER_PIN; i++)
  {
    if (my_pins->mraa_gpio_pin_state[i] == 0)
    {
      fprintf(stderr, "GPIO %d not initialized\n", gpio_pin);
      exit(1);
    }
    else if (my_pins->mraa_pin_number[i] == gpio_pin)
    {
      return my_pins->mraa_gpio_pin_state[i];
    } 
  }
}

void state_gpio(int gpio_pin, mraa_pins_enabled my_pins, int state)
{
  // Check si l'état est bien de 0 ou 1
  if ((state != 1) && (state != 0))
  {
    fprintf(stderr, "Please use 0 or 1 as state for %d\n",
      gpio_pin);
    exit(1);
  }

  //Check si le pin du gpio est correct
  if ((gpio_pin == 2) || (gpio_pin == 4) || 
    (gpio_pin == 7) || (gpio_pin == 8) || 
    (gpio_pin == 12) ||(gpio_pin == 13))
  {
    fprintf(stderr, "%d is not a good value for a gpio. Please use 2,4,7,8,12 or 13\n",gpio_pin);
    exit(1); 
  }

  r = mraa_gpio_dir(gpio_pin, get_io_state(gpio_pin,my_pins));


  r = mraa_gpio_write(gpio_pin, state);
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
    fprintf(stderr, "%d is not a good value for PWM. Please use 3,5,6,9,10 or 11\n",pin_number);
    return 1;
  }

  return 0;
}

int check_if_enabled(int pin_number, mraa_pins_enabled my_pins, char* pin_type)
{
  int i;
  
  if (strcmp(pin_type, "gpio") == 0)
  {
    for (i = 0; i < PWM_NUMBER_PIN; i++)
    {
      if (my_pins->mraa_pwm_pin_number[i] == pin_number)
      {
        fprintf(stderr, "pin %d has already been initialized.\n", 
          pin_number);
        return 1;
      }
      if (my_pins->mraa_pwm_pin_number[i] == 0){
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
      if (my_pins->mraa_gpio_pin_number[i] == pin_number)
      {
        fprintf(stderr, "pin %d has already been initialized.\n", 
          pin_number);
        return 1;
      }
      if (my_pins->mraa_gpio_pin_number[i] == 0){
        fprintf(stderr, "Pin %d not initialized yet.\n", 
          pin_number);
        return 0;
      }
    }
  }
  
}

void add_pin_to_struct(int pin_number, int frequency, mraa_pins_enabled my pins)
{
  int i;

  for (i = 0; i < PWM_NUMBER_PIN; i++)
  {
    // I don't need to check if it has already been enabled
    // It has been done earlier
    if (my_pins->mraa_pins_enabled[i] == 0)
    {
      my_pins->mraa_pins_enabled[i] = pin_number;
      my_pins->mraa_pwm_frequency[i] = frequency;
    }
  }
}

void enable_pwm(int pin_number, mraa_pins_enabled my_pins)
{
  //Check if the pin_number is correct
  int check_flag = -1;

  check_flag = check_good_pin(pin_number);
  if (check_flag == 1)
  {
    exit(1);
  }
  
  //Check if it has already been enabled. have only 6 pwms
  check_flag = check_if_enabled(pin_number, my_pins, "pwm");

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

  add_pin_to_struct(pin_number, DEFAULT_FREQUENCY, my_pins);

  fprintf(stdout, "pin %d enabled\n", pin_number);
}

void disable_pwm(int pin_number,mraa_pins_enabled my_pins)
{
  //Check if the pin_number is correct
  int check_flag = -1;

  check_flag = check_good_pin(pin_number);
  if (check_flag == 1)
  {
    exit(1);
  }
  
  //Check if it has already been enabled. have only 6 pwms
  check_flag = check_if_enabled(pin_number,my_pins, "pwm");

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

int check_pwm_pin_value(int pin_value)
{
  if ((pin_value < 0) || (pin_value > 1))
  {
    fprintf(stderr, "Wrong value. Please use a float between 0 and 1\n");
    return 0;
  }
  return 1;
}

void state_pwm(int pin_number, float pin_value,mraa_pins_enabled my_pins)
{
  //Check if pin_value is correct
  if (check_pwm_pin_value(pin_value) == 1)
  {
    exit(1);
  }

  // Check if the pwm pin has been enabled
  int check_flag = -1;
  check_flag = check_if_enabled(pin_number, my_pins,"pwm");

  if (check_flag == 1)
  {
    exit(1);
  }

  // Access to the pwm
  mraa_pwm_context pwm;
  mraa_pwm_write(pwm, pin_value);
  //float output = mraa_pwm_read(pwm);
  fprintf(stdout, "pwm %d at value %d\n", 
    pin_number, pin_value);
}

void init_mraa_pwms_gpios_enabled(mraa_pins_enabled my pins)
{
  int i;
  for (i = 0; i < PWM_NUMBER_PIN; i++)
  {
    my_pins->mraa_pwm_pin_number[i] = 0;
    my_pins->mraa_pwm_pin_frequency[i] = 0;
  }

  for (i = 0; i < GPIO_NUMBER_PIN; i++)
  {
    my_pins->mraa_gpio_pin_state[i] = 0;
    my_pins->mraa_gpio_pin_number[i] = 0;
  }
}

int main ()
{
  mraa_init();

  struct mraa_pins_enabled *my_pins = 
    malloc(sizeof(struct mraa_pins_enabled));

  init_mraa_pwms_gpios_enabled(my_pins);

  //gpio output
  in_out(GPIO_MOTOR_1, my_pins, "OUT");
  in_out(GPIO_MOTOR_2, my_pins, "OUT");
  enable_pwm(PWM_MOTOR_1, my_pins);
  enable_pwm(PWM_MOTOR_2, my_pins);
//void state_gpio(int gpio_pin, int state)

  state_gpio(GPIO_MOTOR_1, 1);
  state_gpio(GPIO_MOTOR_2, 1);


  // motor 1 forward fast
  state_pwm(PWM_MOTOR_1, 0.9, my_pins);

  // motor 1 forward slow
  state_pwm(PWM_MOTOR_1, 0.5, my_pins);

  // motor 2 backward fast
  state_pwm(PWM_MOTOR_2, 0.9, my_pins);

  // motor 2 backward slow
  state_pwm(PWM_MOTOR_2, 0.5, my_pins);
  
  // motor 1 OFF
  state_gpio(GPIO_MOTOR_1, 0);
  
  // motor 2 OFF
  state_gpio(GPIO_MOTOR_2, 0);

  return 0;
}