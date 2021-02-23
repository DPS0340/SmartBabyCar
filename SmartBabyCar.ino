#include <Servo.h>
#include <Wire.h>

int servoPins[] = {7, 8};
Servo servos[2];
const int SERVOS_LENGTH = 2;
int sensors[2] = {2, 3};
const int SENSORS_LENGTH = 1;
const int limit_rpm = 400;
const int strange_rpm = limit_rpm + 200;
const int switchPin = 10;
int count = 0;

void init_servos();
void set_servos_angle(int angle);
int getRPM(int pin);
void unlock_wheels();
void lock_wheels();

void setup() {
  Serial.begin(9600);
  for(int i=0;i<SENSORS_LENGTH;i++) {
      pinMode(sensors[i], INPUT_PULLUP);
      attachInterrupt(digitalPinToInterrupt(sensors[i]), blink, RISING);
  }
  pinMode(switchPin, INPUT_PULLUP);
  while (!Serial) {
    delay(10); // will pause Zero, Leonardo, etc until serial console opens
  }
  Serial.println("initialized!");
  init_servos();
  lock_wheels();
  delay(500);
}

void init_servos() {
  for(int idx=0;idx<SERVOS_LENGTH;idx++) {
    servos[idx].attach(servoPins[idx]); 
  }
}
void set_servos_angle(int angle) {
  for(int idx=0;idx<SERVOS_LENGTH;idx++) {
     servos[idx].write(angle); 
  }
}

void unlock_wheels() {
  set_servos_angle(0);
}

void lock_wheels() {
  set_servos_angle(45);
}

void loop() {
  int rpm = count/2;
  int switchValue = digitalRead(switchPin);
  Serial.println(rpm);
  if(switchValue == LOW) {
    unlock_wheels();
  } else if(rpm >= limit_rpm) {
    lock_wheels();
  }
  count = 0;
  delay(1000);
}

void blink() {
  count++;
}
