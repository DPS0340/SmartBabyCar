#include <Servo.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

int servoPins[] = {7, 8};
Servo servos[2];
const int SERVOS_LENGTH = 1;
int startAngle = 0;
Adafruit_MPU6050 mpu;
sensors_event_t a, g, temp;
float speedX = 0.0f, speedY = 0.0f; // 수평 보드, 수직 칩에서는 센서 가속도 X가 위아래 방향이므로 제외
// 엘리베이터 같은 상황 고려하여 위아래 X는 사용하지 않음
float limit_speed = 15.0f;

void init_servos();
void set_servos_angle(int angle);
void integral_things();
bool is_over_speed();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial) {
    delay(10); // will pause Zero, Leonardo, etc until serial console opens
  }
  init_servos();
  unlock_wheels();

  Serial.print("Finding MPU6050..");

  // Try to initialize
  if (!mpu.begin()) {
    Serial.println("Failed..");
  }
  Serial.println("Found!");
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
  set_servos_angle(90);
}

void lock_wheels() {
  set_servos_angle(0);
}

void loop() {
  // put your main code here, to run repeatedly:
  integral_acceleration();
  if(is_over_speed()) {
    lock_wheels();
  } else {
    unlock_wheels();
  }
}

void integral_acceleration() {
  mpu.getEvent(&a, &g, &temp);
  speedX += a.acceleration.y;
  speedY += a.acceleration.z;
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");
  
  Serial.print("Speed X: ");
  Serial.print(speedX);
  Serial.print(", Y: ");
  Serial.print(speedY);
}

bool is_over_speed() {
  return speedX + speedY >= limit_speed;
}
