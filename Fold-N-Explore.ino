/*
The base code for the serial input came from Norwegian Creations (https://www.norwegiancreations.com/2017/12/arduino-tutorial-serial-inputs/).
In addition to this, the wiring code to run and receive a value in cm using the distance sensor was used from How To Mechatronics 
(https://howtomechatronics.com/tutorials/arduino/ultrasonic-sensor-hc-sr04/).

The base code for the stepper motors came from Tutorials Point (https://www.tutorialspoint.com/arduino/arduino_stepper_motor.htm). However, 
this code was mainly used as a syntax and initialization base and was heavily edited to fit the purposes of the project. Additionally, the 
wiring pinout was used from here.

For the IR Portion of the code I used an article from Robotics Back-End 
(https://roboticsbackend.com/arduino-ir-remote-controller-tutorial-setup-and-map-buttons/) as a base which I modified to suit my needs. 
The define statements for the LED's also came from this source.
*/

#include <Stepper.h>
#include <Servo.h>

#include <IRremote.h>

#define IR_RECEIVE_PIN 3

//Defines to set the IR Receiver Values
#define ON_OFF 69
#define LEFT 68
#define RIGHT 67
#define BACK 7
#define PAUSEPLAY 64
#define FORWARD 9
#define CASE_0 22
#define CASE_1 12
#define CASE_2 24
#define CASE_3 94
#define CASE_4 8
#define CASE_5 28
#define CASE_6 90
#define CASE_7 66
#define CASE_8 82
#define CASE_9 74

//Defmines to set the LED pins (both RED and BLUE)
#define RED_1 22
#define RED_2 23
#define RED_3 24
#define RED_4 25
#define RED_5 26
#define BLUE_1 28
#define BLUE_2 29
#define BLUE_3 30
#define BLUE_4 31
#define BLUE_5 32

const int stepsPerLoop = 2;

Stepper rightMotor(500, 44, 46, 40, 42); //Initilizes stepper motor
Stepper leftMotor(500, 39, 41, 43, 45); //Note that both motors were backwards so thats where the negatives come from

Servo servo; //Initilizes Servo

int posS1 = 0; //Position of Servo

char response; //Serial response
char buff; //Serial buffer

int count = 0; //Servo counter

int LED_TYPE = 0; //Picks option for LED

const int trigPin = 9;
const int echoPin = 10;

long duration;
int distance;

//Function to turn all the LED's On
void LED_ALL_ON()
{
  digitalWrite(RED_1, HIGH);
  digitalWrite(RED_2, HIGH);
  digitalWrite(RED_3, HIGH);
  digitalWrite(RED_4, HIGH);
  digitalWrite(RED_5, HIGH);
  digitalWrite(BLUE_1, HIGH);
  digitalWrite(BLUE_2, HIGH);
  digitalWrite(BLUE_3, HIGH);
  digitalWrite(BLUE_4, HIGH);
  digitalWrite(BLUE_5, HIGH);
}

//Function to turn all Red LED's On
void LED_RED_ON()
{
  digitalWrite(RED_1, HIGH);
  digitalWrite(RED_2, HIGH);
  digitalWrite(RED_3, HIGH);
  digitalWrite(RED_4, HIGH);
  digitalWrite(RED_5, HIGH);
}

//Function to turn all Blue LED's On
void LED_BLUE_ON()
{
  digitalWrite(BLUE_1, HIGH);
  digitalWrite(BLUE_2, HIGH);
  digitalWrite(BLUE_3, HIGH);
  digitalWrite(BLUE_4, HIGH);
  digitalWrite(BLUE_5, HIGH);
}

//Function to turn all LED's Off
void LED_ALL_OFF()
{
  digitalWrite(RED_1, LOW);
  digitalWrite(RED_2, LOW);
  digitalWrite(RED_3, LOW);
  digitalWrite(RED_4, LOW);
  digitalWrite(RED_5, LOW);
  digitalWrite(BLUE_1, LOW);
  digitalWrite(BLUE_2, LOW);
  digitalWrite(BLUE_3, LOW);
  digitalWrite(BLUE_4, LOW);
  digitalWrite(BLUE_5, LOW);
}

//Function to blink the RED LED's
void LED_BLINK_RED()
{
  digitalWrite(RED_1, HIGH);
  digitalWrite(RED_2, LOW);
  digitalWrite(RED_3, HIGH);
  digitalWrite(RED_4, LOW);
  digitalWrite(RED_5, HIGH);
  delay(500);
  digitalWrite(RED_1, LOW);
  digitalWrite(RED_2, HIGH);
  digitalWrite(RED_3, LOW);
  digitalWrite(RED_4, HIGH);
  digitalWrite(RED_5, LOW);
}

//Function to blink the BLUE LED's
void LED_BLINK_BLUE()
{
  digitalWrite(BLUE_1, HIGH);
  digitalWrite(BLUE_2, LOW);
  digitalWrite(BLUE_3, HIGH);
  digitalWrite(BLUE_4, LOW);
  digitalWrite(BLUE_5, HIGH);
  delay(500);
  digitalWrite(BLUE_1, LOW);
  digitalWrite(BLUE_2, HIGH);
  digitalWrite(BLUE_3, LOW);
  digitalWrite(BLUE_4, HIGH);
  digitalWrite(BLUE_5, LOW);
}

//Function to make a wave with RED LED's
void LED_WAVE_RED()
{
  digitalWrite(RED_1, HIGH);
  delay(100);
  digitalWrite(RED_1, LOW);
  digitalWrite(RED_2, HIGH);
  delay(100);
  digitalWrite(RED_2, LOW);
  digitalWrite(RED_3, HIGH);
  delay(100);
  digitalWrite(RED_3, LOW);
  digitalWrite(RED_4, HIGH);
  delay(100);
  digitalWrite(RED_4, LOW);
  digitalWrite(RED_5, HIGH);
}

//Function to make a wave with BLUE LED's
void LED_WAVE_BLUE()
{
  digitalWrite(BLUE_1, HIGH);
  delay(100);
  digitalWrite(BLUE_1, LOW);
  digitalWrite(BLUE_2, HIGH);
  delay(100);
  digitalWrite(BLUE_2, LOW);
  digitalWrite(BLUE_3, HIGH);
  delay(100);
  digitalWrite(BLUE_3, LOW);
  digitalWrite(BLUE_4, HIGH);
  delay(100);
  digitalWrite(BLUE_4, LOW);
  digitalWrite(BLUE_5, HIGH);
}

//Function to move Forward
void forward()
{
  for (int i=0; i < stepsPerLoop; i++)
  {
    rightMotor.step(-50);
    leftMotor.step(-50);
  }
}

//Function to move LEFT
void left()
{
  for (int i=0; i < stepsPerLoop; i++)
  {
    rightMotor.step(50);
    leftMotor.step(-50);
    leftMotor.step(-50);
    leftMotor.step(-50);
  }
}

//Function to move RIGHT
void right()
{
  for (int i=0; i < stepsPerLoop; i++)
  {
    rightMotor.step(-50);
    rightMotor.step(-50);
    rightMotor.step(-50);
    leftMotor.step(50);
  }
}

//Function to move BACK
void back()
{
  for (int i=0; i < stepsPerLoop; i++)
  {
    rightMotor.step(50);
    leftMotor.step(50);
  }
}


void setup() 
{

  rightMotor.setSpeed(60); //Set stepper motor speeds
  leftMotor.setSpeed(60);

  servo.attach(8); //Initilize servo

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  Serial.begin(9600);

  IrReceiver.begin(IR_RECEIVE_PIN); //Begin IR Receiver

  delay(2000);

  Serial.println("Type w, a, s, or d to choose the direction of movement.");
}

void loop() {

  //Norwegian Creations Code Begins (Serial Input)
  if(Serial.available())
  {
    //my code
    buff = Serial.read();
    if (buff == 'w' || buff == 'W' || buff == 'a' || buff == 'A' || buff == 's' || buff == 'S' || buff == 'd' || buff == 'D') //prevents it from resetting the value prematurely
    { response = buff; }
    //end my code

    Serial.print("You typed: ");
    Serial.println(response);
  }
  //Norwegian Creations Code Ends

  //Robotics Back-End Code Begins (IR)
  if (IrReceiver.decode()) {
    IrReceiver.resume();
    int command = 0;
    command = IrReceiver.decodedIRData.command;
    Serial.print(command);
    switch (command) {
    //Robotics Back-End Code Ends (They used switch cases but I changed the contents of them)
      case LEFT:
      {
        response = 'a';
        break;
      }

      case RIGHT:
      {
        response = 'd';
        break;
      }

      case FORWARD:
      {
        response = 'w';
        break;
      }

      case BACK:
      {
        response = 's';
        break;
      }

      case PAUSEPLAY:
      {
        count = 0;
        break;
      }

      case ON_OFF:
      {
        LED_ALL_OFF();
        LED_TYPE = -1; //To turn the LED's off
        break;
      }

      case CASE_0:
      {
        LED_TYPE = 0;
        break;
      }
      
      case CASE_1:
      {
        LED_TYPE = 1;
        break;
      }

      case CASE_4:
      {
        LED_TYPE = 4;
        break;
      }

      case CASE_7:
      {
        LED_TYPE = 7;
        break;
      }

      case CASE_3:
      {
        LED_TYPE = 3;
        break;
      }

      case CASE_6:
      {
        LED_TYPE = 6;
        break;
      }

      case CASE_9:
      {
        LED_TYPE = 9;
        break;
      }
    }
  }

  //Sets Motors to Move Based on User Input
  if (response == 'w' || response == 'W')
  { forward(); }
  else if (response == 'a' || response == 'A')
  { left(); }
  else if (response == 's' || response == 'S')
  { back(); }
  else if (response == 'd' || response == 'D')
  { right(); }

  //Sets LED pattern based on IR Input
  if (LED_TYPE == 0)
  { LED_ALL_ON(); }
  else if (LED_TYPE == 1)
  { LED_BLINK_RED(); }
  else if (LED_TYPE == 4)
  { LED_WAVE_RED(); }
  else if (LED_TYPE == 7)
  { LED_RED_ON(); }
  else if (LED_TYPE == 3)
  { LED_BLINK_BLUE(); }
  else if (LED_TYPE == 6)
  { LED_WAVE_BLUE(); }
  else if (LED_TYPE == 9)
  { LED_BLUE_ON(); }

  //How to Mechatronics Code Begins (Distance Sensor)
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); // Sets the trigPin on HIGH state for 10 micro seconds
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH); // Reads the echoPin, returns the sound wave travel time in microseconds
  distance = duration * 0.034 / 2; // Calculating the distance
  //How to Mechatronics Code Ends
  
  //moves the central servo motor
  if (distance < 30 || count < 10 )
  {
    if(distance < 30)
    { count = 0; }
    posS1 =+ 20;
    count++;
  }
  else
  {
    posS1 = 0;
  }
  servo.write(posS1); // tell servo to go to position in variable 'pos'
}
