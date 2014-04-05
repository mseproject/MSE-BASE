
#include <Servo.h>

Servo servo_extend_motor;
Servo servo_envelope_motor;

const int extend_motor_pin = 8;
const int envelope_motor_pin = 9;



const int ci_Ultrasonic_Ping = 5;   //input plug
const int ci_Ultrasonic_Data = 6;   //output plug

unsigned long ul_Echo_Time=4000;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  pinMode (extend_motor_pin, OUTPUT);
  servo_extend_motor.attach(extend_motor_pin);
  
  pinMode (envelope_motor_pin, OUTPUT);
  servo_envelope_motor.attach(envelope_motor_pin);
  
  
  pinMode(ci_Ultrasonic_Ping, OUTPUT);
  pinMode(ci_Ultrasonic_Data, INPUT);

}
void loop()

{
  Ping();
 while (ul_Echo_Time > 1900)
 {
   Ping();
  servo_extend_motor.writeMicroseconds(1800);
  delay(100);
  servo_extend_motor.writeMicroseconds(1500);
  delay(2000);
  //Serial.println(ul_Echo_Time, DEC);
 }
 servo_extend_motor.writeMicroseconds(1500);
 servo_envelope_motor.writeMicroseconds(1800);
  Serial.println(ul_Echo_Time, DEC);
 
}




void Ping() {
  // put your main code here, to run repeatedly:

//Ping Ultrasonic
  //Send the Ultrasonic Range Finder a 10 microsecond pulse per tech spec
  digitalWrite(ci_Ultrasonic_Ping, HIGH);
  //delayMicroseconds(100);  //The 10 microsecond pause where the pulse in "high"
  digitalWrite(ci_Ultrasonic_Ping, LOW);
  //use command pulseIn to listen to Ultrasonic_Data pin to record the
  //time that it takes from when the Pin goes HIGH until it goes LOW 
  ul_Echo_Time = pulseIn(ci_Ultrasonic_Data, HIGH, 10000);
  delay(60);

  // Print Sensor Readings

 // Serial.print("Time (microseconds): ");
  //Serial.print(ul_Echo_Time, DEC);
  //Serial.print(", Inches: ");
  //Serial.print(ul_Echo_Time/148); //divide time by 148 to get distance in inches
  //Serial.print(", cm: ");
  //Serial.println(ul_Echo_Time/58); //divide time by 58 to get distance in cm 
}
