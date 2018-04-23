#include <NewPing.h>  // This is using a update to the ping 
// See this website.  Add the library to your librarys folder C:\Program Files (x86)\Arduino\libraries
// http://playground.arduino.cc/Code/NewPing

//This is part of the NewPing.  Update your pin location here, depending on how you have it set up.
#define TRIGGER_PIN  6
#define ECHO_PIN     5
#define MAX_DISTANCE 200 //Max distance to look in cm

//Function prototypes
/*
void Forward(void); // Turns off all reverse and ramps engines up to max
void Stop(void); //Set speed to zero and turn off forward and reverse
void Turn(void); //Makes a turn to avoid obstacle
*/

class Drive 
{
 private:
      int E1 = 10;  // Passengers Side
      int M1 = 13; // Forward
      int M1_R = 12; // Reverse 
      int E2 = 9;    //Drivers Side                  
      int M2 = 8; // Forward
      int M2_R = 7; // Reverse  
 public:
      void Forward(void);
      void TurnLeft(void);
      void TurnRight(void);
      void Backward(void);
      void Stop(void);
       Drive(void);
};

//This is for the L298N motor controller.  The values are the pin locations
//that you use to connect the ENA(E1), IN1(M1), IN2(M1_R), ENA(E2), IN4(M2), and IN3(M2_R)
//ENA and ENB are the variable power for the motor
//IN1 and IN2 are on/off for forward or reverse for motor A
//IN4 and IN3 are on/off for forward or reverse for motor B
//If one or both motors are running backwards, you can switch the power to the motor (OUT1 and OUT2 or OUT3 and OUT4)
//Or, you can switch the pin connections
//Or, in the code below, you could switch the pin numbers.
//Try it all and see what works best for you.
/*
  
 int E1 = 10;  // Passengers Side
int M1 = 13; // Forward
int M1_R = 12; // Rev 
int E2 = 9;    //Drivers Side                  
int M2 = 8; // Forward
int M2_R = 7; // Rev     
*/

//Motor speed calibration.  Run it on the floor with both values at 255
//It will curve left or right.  Curving left means that the right motor is going faster.
//Curving to the right means the left motor is going faster.
//Leave your slower motor at 255.  Change the value down for you faster motor.
//Repeat until it runs straight (unless you wanted it to run in big circles).
int M1_PWM=255; // Motor 1 was my slower motor
int M2_PWM=255; // Motor 2 was my faster motor (so its max speed is scaled down. Drivers side with respect to the ultrasonic sensor

//Constructor for the Ultrasonic distance finder to create an instance sonar as a global
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);     
             
bool stopped = 1; //Trigger that the motors are stopped.

void setup() 
{ 
  //Set up all of the pins used for the motor as output pins
   /* pinMode(E1, OUTPUT); //speed for passengers side motor
    pinMode(M1, OUTPUT);    //HIGH if forward, LOW not forward
    pinMode(M1_R, OUTPUT);  //HIGH if reverse, LOW not reverse
    
    pinMode(E2, OUTPUT);  //speed for drivers side
    pinMode(M2, OUTPUT); // forward
    pinMode(M2_R, OUTPUT);   //rev
*/
Drive setupDrive();
} 

void loop() //Note, loop repeats infinitely
{ 
  Drive MyDrive;
  int ping_dist; // Variable to store the ping distance.  Distance is in cm
  if (stopped)  //If it is not stopped and the sonar is not triggered, it is moving forward and will stay moving forward
  {
    MyDrive.Forward();
    stopped=0;
  }
  // Both engines are moving forward at this point
  delay(50); // 50 ms delay --- This is a delay to make sure that the sonar has had time to reset.  Some delay here is typically needed
  
  ping_dist=sonar.ping_cm(); // Get a ping value 
  // ping will be zero if nothing detected, or will be a value in cm, if something is detected

  if(ping_dist > 0)
  MyDrive.Stop();
  while (ping_dist>0&&ping_dist<30) // If ping is greater than 0 (that is non-zero), but less than 30, trigger a change angle
  {
  /*  MyDrive.Stop();
    delay(100); //This delay of 0.1 second is in here just to make it easier to see what the motor is doing.
    //This delay is completely unecessary.
    MyDrive.TurnLeft();
    MyDrive.Stop();
    stopped=1; // motor was stopped
    ping_dist=sonar.ping_cm(); // Get a new ping value 
  */
  MyDrive.Stop();
  }  

}

 void Drive::Forward(void)
{
    analogWrite(E1,0); //Speed 0 Motor 1
    analogWrite(E2,0); //Speed 0 Motor 2
    digitalWrite(M1_R,LOW); //Turn off reverse, to make sure
    digitalWrite(M2_R,LOW); //Turn off reverse, to make sure
    digitalWrite(M1,HIGH);  //Motor 1 forward on 
    digitalWrite(M2, HIGH); //Motor 2 forward on
    //You could replace the for loop with just analogWrite(E1,M1_PWM); analogWrite(E2,M2_PWM) 
    for (int i(0);i<101;i++) //This is not really necessary for this system.  But, for a large machine, you would need to ramp up speed gradually
    {
      analogWrite(E1,M1_PWM*i/100); //Ramp motor 1 speed up
      analogWrite(E2,M2_PWM*i/100); //Ramp motor 2 speed up
      delay(10);
    }
  return;
}

void Drive::Stop(void)
{
      for (int i(100);i<0;i--) // Again, you could just set speed to zero for this small system, but for a large system, this ramp down would be needed
    {
      analogWrite(E1,M1_PWM*i/100); //Ramp motor 1 speed down
      analogWrite(E2,M2_PWM*i/100); //Ramp motor 2 speed down
      delay(10);
    }
    digitalWrite(M1_R,LOW); //Turn off reverse
    digitalWrite(M2_R,LOW); //Turn off reverse
    digitalWrite(M1,LOW);  //Turn off forward
    digitalWrite(M2, LOW); //Turn off forward
  return;
}

void Drive::TurnLeft(void)
{
    digitalWrite(M1_R,HIGH); //turn (Motor 1 in reverse motor 2 forward
    digitalWrite(M2,HIGH);
    
        for (int i(0);i<101;i++) //Ramp up speed
    {
      analogWrite(E1,M1_PWM*i/100); //Ramp motor 1 speed down
      analogWrite(E2,M2_PWM*i/100); //Ramp motor 2 speed down
      delay(10);
    }

    delay(50); //This is the time for turning at full speed.  Note that as your batteries run out, the turns will get shorter.
  }

 Drive::Drive()
  {
    int E1 = 10;  // Passengers Side
      int M1 = 13; // Forward
      int M1_R = 12; // Rev 
      int E2 = 9;    //Drivers Side                  
      int M2 = 8; // Forward
      int M2_R = 7; // Rev
    }
