//================================================================================================
//                                   1-Motor-Robot-Project
//================================================================================================
/* The goal is to use the ping sensor to detect the ground as the vehicle rotates about its shaft
 and to temporarily turn the motor on while the value remains within a certain range.
 I will supply power with either a s cell Lipo = 8.4 Volt power supply while the control logic remians
at 5 Volts*/

// Arduino Tricket Setup:
//=======================
// Ping Sensor = 5V, GRND, D0 (ECHO), D1 (TRIGGER)

#include <Ultrasonic.h>                       
#include <avr/power.h>
const int controlPin1 = 3;
const int controlPin2 = 4;
const int enablePin = 2;
const int motorSpeed = 255/4;                          // This will be the motor drive value triggered when sensor is within certain constant distance.

// Ultrasonic ping sensor
#define TRIGGER_PIN  1      
#define ECHO_PIN  0         
Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);

float range;
bool turnOn;
int startTime;
int stuckTime;

float checkSonar(void)                                            // Sonar Function to check distance to objects
{   
  float cmMsec, inMsec;
  long microsec = ultrasonic.timing();
  cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
  inMsec = ultrasonic.convert(microsec, Ultrasonic::IN);
  return cmMsec;
}
    
void setup() 
{
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);         // Set frequency to 16 MHz
  pinMode(controlPin1,OUTPUT);
  pinMode(controlPin2,OUTPUT);
  pinMode(enablePin, OUTPUT);
  digitalWrite(controlPin1, HIGH);
  digitalWrite(controlPin2,LOW);
  digitalWrite(enablePin,LOW);
}

void loop() 
{
  startTime = millis();
  while(range > 15)
  {
    turnOn = false;                                                // Floor not detected...dont turn on motor
    stuckTime = millis();
    if((stuckTime - startTime) > 5000)                             // If stuck, Pulse the opposite motor direction to rock backwards
    {                                                              
      digitalWrite(controlPin1, LOW);
      digitalWrite(controlPin2,HIGH);
      delay(100);
      digitalWrite(enablePin,motorSpeed);
      delay(50);
      digitalWrite(enablePin,LOW);
      delay(100);
      digitalWrite(enablePin,motorSpeed);
      delay(50);
      digitalWrite(enablePin,LOW);
      delay(100);
      digitalWrite(enablePin,motorSpeed);
      delay(50);
      digitalWrite(enablePin,LOW);
      delay(100);
      digitalWrite(controlPin1, HIGH);                              // Then return motor direction to original state
      digitalWrite(controlPin2,LOW);
      delay(100);
      break;
    }
    range = checkSonar();
  }
  range = checkSonar();
  if(range <= 5 && !turnOn)                                         // Floor detected, Turn on motor
  {
      digitalWrite(enablePin,motorSpeed);
      delay(60);                                                    // Turn on long enough to flip weighted motor back over
      turnOn = true;                                                // Prevent motor turning back on 
  }
    digitalWrite(enablePin,LOW);                                    // Turn motor back off

}














