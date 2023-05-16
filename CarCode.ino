#include <Servo.h>
Servo myservo;
int pos = 150;
#define FLAME_SENSOR_PIN A0 // Pin to read flame sensor output
#define LED_PIN_FLAME 2 
#define buzzer 2
#define IN1 3
#define IN2 4
#define IN3 9
#define IN4 10
#define en1 5
#define en2 6
#define TRIG_FRONT 8
#define ECHO_FRONT A5
#define TRIG_LEFT A1
#define ECHO_LEFT A2
#define TRIG_RIGHT A3
#define ECHO_RIGHT A4
#define LED_PIN_FRONT 12
#define LED_PIN_BACK  7
#define DIST_THRESHOLD 50
#define servoo 11
bool mode = false;
bool fire = false;
char Direction, cases, casesf;
void forward();
void Back();
void Left();
void Right();
void stope();
int readSensor(int, int);
int time_delay = 100;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  // Initialize motor driver pins as outputs
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(en1, OUTPUT);
  pinMode(en2, OUTPUT);
  pinMode(LED_PIN_FRONT,OUTPUT);
  pinMode(LED_PIN_BACK,OUTPUT);
  stope();

  // UltraSonic Sensors

  pinMode(TRIG_FRONT, OUTPUT);
  pinMode(ECHO_FRONT, INPUT);
  pinMode(TRIG_LEFT, OUTPUT);
  pinMode(ECHO_LEFT, INPUT);
  pinMode(TRIG_RIGHT, OUTPUT);
  pinMode(ECHO_RIGHT, INPUT);

  pinMode(FLAME_SENSOR_PIN, INPUT); 
  pinMode(LED_PIN_FLAME, OUTPUT); 
  pinMode(buzzer,OUTPUT);
  myservo.attach(servoo);
  myservo.write(pos);
}

 void loop() {
   if(Serial.available()){
        Direction = Serial.read();
        switch(Direction){
          case 'F':{
            forward();
            break;
          }
          case 'B':{
            Back();
            break;
          }
          case 'L':{
            Left();
            break;
          }
          case 'R':{
            Right();
            break;
          }
          case 'S':{
            stope();
            break;
          }
          case 'A':{
            mode = true;
            break;
          }
          case 'T':{
            fire = true;
            break;
          }
      }
     }
    while (mode){
  // put your main code here, to run repeatedly:
        Serial.println("IAM HERE IN MODE==1");
        cases = Serial.read();
        int front_dist = readSensor(TRIG_FRONT, ECHO_FRONT);
        int left_dist = readSensor(TRIG_LEFT, ECHO_LEFT);
        int right_dist = readSensor(TRIG_RIGHT, ECHO_RIGHT);
        Serial.print("Front: ");
        Serial.print(front_dist);
        Serial.print(", Left: ");
        Serial.print(left_dist);
        Serial.print(", Right: ");
        Serial.println(right_dist);
          if (front_dist > DIST_THRESHOLD) {
          // Move forward
            delay(time_delay);
            forward();
            Serial.println("F");
      }
       else if(right_dist > DIST_THRESHOLD){
              // Move right
              delay(time_delay);
              Right();
              Serial.println("R");
        }
       else if(left_dist > DIST_THRESHOLD){
              // Move left
              delay(time_delay);
              Left();
              Serial.println("L");
        }
        else{
            // Stop the motor
              digitalWrite(LED_PIN_BACK,HIGH);
              stope();
              Serial.println("S");
        }
    
        if(cases == 'a'){
          mode = false;
          stope();
          break;
        }   
    }
    while(fire) {
      int flameValue = analogRead(FLAME_SENSOR_PIN); 
    
      Serial.print("Flame sensor value: ");
      Serial.println(flameValue); 
  
      if (flameValue > 850) {
        Serial.println("No flame detected!");
      } 
      else if(flameValue > 512 && flameValue <800) { 
        digitalWrite(LED_PIN_FLAME, HIGH);
        delay(500);
        digitalWrite(LED_PIN_FLAME, LOW);
        delay(500);
        digitalWrite(buzzer,HIGH);
        delay(500);
        digitalWrite(buzzer,LOW);
        delay(500);
        Serial.println("Flame detected.");
      }
      else {
        digitalWrite(buzzer,HIGH);
        digitalWrite(LED_PIN_FLAME, HIGH);
        myservo.write(0);
        delay(1500);
      }
      casesf = Serial.read();
      if(casesf == 't'){
        fire = false;
        break;
      }
    }
 
 }
  void forward(){
    digitalWrite(IN1,HIGH); 
    digitalWrite(IN2,LOW); 
    digitalWrite(IN3,HIGH); 
    digitalWrite(IN4,LOW);
    analogWrite(en1,65);
    analogWrite(en2,65); 
    digitalWrite(LED_PIN_FRONT,HIGH);
  }
  void Back(){
    digitalWrite(IN1,LOW); 
    digitalWrite(IN2,HIGH); 
    digitalWrite(IN3,LOW); 
    digitalWrite(IN4,HIGH);
    analogWrite(en1,65);
    analogWrite(en2,65); 
  }
  void Left(){
    digitalWrite(IN1,HIGH); 
    digitalWrite(IN2,LOW); 
    digitalWrite(IN3,LOW); 
    digitalWrite(IN4,HIGH);
    analogWrite(en1,60);
    analogWrite(en2,60); 
    digitalWrite(LED_PIN_FRONT,HIGH);
  }
  void Right(){
    digitalWrite(IN1,LOW); 
    digitalWrite(IN2,HIGH); 
    digitalWrite(IN3,HIGH); 
    digitalWrite(IN4,LOW);
    analogWrite(en1,60);
    analogWrite(en2,60); 
    digitalWrite(LED_PIN_BACK,HIGH);
  }
  void stope(){
    digitalWrite(IN1,LOW); 
    digitalWrite(IN2,LOW); 
    digitalWrite(IN3,LOW); 
    digitalWrite(IN4,LOW);
    analogWrite(en1,60);
    analogWrite(en2,60); 
  }
  int readSensor(int trig_pin, int echo_pin) {
  // Send ultrasonic pulse
  digitalWrite(trig_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);
  
  long duration = pulseIn(echo_pin, HIGH);
  
  // Convert duration to distance in cm
  int distance = duration * 0.034 / 2;

  return distance;
  }
