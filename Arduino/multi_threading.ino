
/*
	Arduino can't use multithreading. But you can simulate it using micros 
	and millis arduinos functions.
*/

// Declaring 2 outputs
int led1 = 13;
int led2 = 12;
int thread = 1;
long long int _time = 0;

// the setup routine runs once when you press reset:
void setup() {
  // initialize the digital pin as an output.
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
}

void thread1()
{
	if (_time + 100 <= millis()) {
		Serial.println("Thread 1");
		digitalWrite(led1, HIGH);   // turn the LED on (HIGH is the voltage level)
		digitalWrite(led2, LOW);   // turn the LED on (HIGH is the voltage level)
		delay(10);
		_time = micros();
	}
}

void thread2()
{
	if (_time + 100 <= millis()) {
		Serial.println("Thread 2");
		digitalWrite(led2, HIGH);   // turn the LED on (HIGH is the voltage level)
		digitalWrite(led1, LOW);   // turn the LED on (HIGH is the voltage level)
		delay(10);
		_time = millis();
	}
}

// the loop routine runs over and over again forever:
void loop() {
	Serial.println("Starting the routine");
	_time = millis();
	//Alternate thread1 and thread2.
	thread1();
	thread2();
}