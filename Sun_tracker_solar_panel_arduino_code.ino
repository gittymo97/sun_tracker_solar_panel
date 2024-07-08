
 
/* Servo motor with Arduino example code. Position and sweep. More info: https://www.makerguides.com/ */
 
// Include the servo library:
#include <Servo.h>
#include <BH1750.h>
BH1750 GY30; // instantiate a sensor event object
 
 
 
 
// Create a new servo object:
Servo positioning_servo;
Servo tilting_servo;
Servo folding_servo;
Servo cleaning_servo;
// Define the servo pin:
#define servoPin 9
#define servoPin 10
#define servoPin 11
#define servoPin 12
 
#define tilt_threshold 30.0
#define cloud_threshold 70.0
#define on_off_threshold 10.0
#define position_threshold 20.0
 
// Create a variable to store the servo position:
//int folding_angle = 0;
float lux=0.0;
float lux_previous=0.0;
float periodic_lux=0.0;
int position_past_angle=0;
int tilt_past_angle=75;
int  folding_past_angle=160; 
 
 
void setup() {
 
  Serial.begin(9600); // launch the serial monitor
  Wire.begin(); // Initialize the I2C bus for use by the BH1750 library  
  GY30.begin(); // Initialize the sensor object
  Serial.println("Flux Workshop Example");
  
  // Attach the Servo variable to a pin:
  positioning_servo.attach(9);
  tilting_servo.attach(10);
  folding_servo.attach(11);
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  
 
}
//////// ON OFF FEEDBACK SYSTEM////////////
int Set_Reset()
  {
  lux=GY30.readLightLevel();
  
if(lux>=on_off_threshold)  // Checking whether the Night is over or not
{
{if(lux<cloud_threshold)  /// During Cloud Time, The system will not operate
return Set_Reset();  //// This recursive call will hold on the code
}
return 1;
  }
else
{ 
 
  int pos=180;
    for (pos = 180; pos >= 0; pos -= 1) //// panel opens here
    {positioning_servo.write(pos);              
    delay(30 );   
    } /// If it is night time, positioning motior and its operating memory will preset
lux_previous=0.0;
position_past_angle=0;
 
return 0;}
  }
  
  
  
//////  CLEANING SERVOMOTIOR/////////
// void cleaning_servomotor()
//  { 
//     cleaning_servo.write(180);
//     delay (5000);
//     int pos=0;
//    for (pos = 180; pos >= 0; pos -= 1) 
//    {cleaning_servo.write(pos);              
//    delay(15); 
//    }
//     }
//  
 
///////  FOLDING SERVOMOTOR ////////////
 
 
void folding_servomotor()
  {
    if((lux>=on_off_threshold)&&(folding_past_angle==160))
    {int pos=160;
    for (pos = 160; pos >= 17; pos -= 1) //// panel opens here
    {folding_servo.write(pos);              
    delay(30 ); 
    }
    folding_past_angle=17;
    } 
 
  
if((lux<on_off_threshold)&&(folding_past_angle==17))
  {
  int pos=17;  //// panel closes here
  for (pos = folding_past_angle; pos <=160; pos += 1) 
    {folding_servo.write(pos);              
    delay(30); 
    }
    folding_past_angle=160;
  }
  }
 
 
 
 
 
 
 
//// Tilt Servomotor System With Feedback
 
  int tilt_feedbacksystem()
  {
 
   lux = GY30.readLightLevel();
  
    if(lux<=(periodic_lux-tilt_threshold))  /// Periodic lux value will store the last tilt position value near mid day time
    {
    return 1;                               /// tilt threshold will hold on the tilt motor for nearly 25 days
    }
    
    else
    return 0;
  
  }
 
 
int tilting_servomotor(int past_angle)
  {
 
 
if(periodic_lux!=0.0)
{
  // Tell the servo to go to a particular angle:
  
 
 
    delay(40);/// We can Change the delay suitably
if (tilt_feedbacksystem())
 
{ int change=10;  
  tilting_servo.write(past_angle+change);  
    tilt_past_angle=past_angle+change; /// The tilting motor first moves towards one directon
    
    if(tilt_feedbacksystem())
    {tilting_servo.write(past_angle-change);  /// If the previos move is not valid , then the motor will move towards another direction
    tilt_past_angle=past_angle-change;}
  }
float k =GY30.readLightLevel();
return k;//// Periodic lux value will update if the tilt motor rotates, othrwise not.
  }
 
 
 
  
if(periodic_lux==0.0) /// periodic lux =0 condition for initial call.
  /// Initially, we cannot "compare" the periodic lux value, so we need another command to handle the problem.
 
{ int change=1;
  float lux1 = GY30.readLightLevel();
 
tilting_servo.write(past_angle+change);
int new_angle=past_angle+change;
float lux2 = GY30.readLightLevel();
 
if(lux2<lux1)
{tilting_servo.write(past_angle-change);
new_angle=past_angle-change;
lux2 = GY30.readLightLevel();
while(lux2>lux1)
{lux1=lux2;
  tilting_servo.write(new_angle-change);
  new_angle=new_angle-change;
  lux2 = GY30.readLightLevel();
 
}
float k=GY30.readLightLevel();
return k;
}
 
while(lux2>lux1)
{lux1=lux2;
  tilting_servo.write(new_angle+3);
  new_angle=new_angle+3;
  lux2 = GY30.readLightLevel();
 
}
float k=GY30.readLightLevel();
return k;
 
 
  }
  
  }
//// BEGIN OF POSITION FEEDBACK SYSTEM////
 
int position_feedbacksystem()
  { lux = GY30.readLightLevel();
    
    if((!lux_previous)||(lux>lux_previous)) /// Previous lux value will store the sensor value of the last position of position motor
    {lux_previous=lux;
    return 0;} 
 
    
    if((lux<=(lux_previous-position_threshold)))
    {
    return 1;
    }
    
    else
    return 0;
 
  }
 
  ///////END OF POSITION FEEDBACK SYSTEM//////
 
  
 
void positioning_servomotor(int past_angle)
  {
 
  // Tell the servo to go to a particular angle:
  positioning_servo.write(past_angle);
 
 
  // delay(15);/// We can Change the delay suitably
int change=2;
int pos=0;
  if (position_feedbacksystem())
    // Sweep from past_angle to new_angle:
    
    
    {
    for (pos = past_angle; pos >= past_angle+change; pos += 1) { 
    positioning_servo.write(pos);              
    delay(500);                     
  }
        
    position_past_angle=past_angle+change;
    lux_previous = GY30.readLightLevel();
    positioning_servo.write(position_past_angle+change);
    position_past_angle=past_angle+change;
    lux= GY30.readLightLevel();
    
    while(lux>lux_previous) /// This while loop will help adjust the positioning motor after cloud sunny time
    {
       {
    for (pos = past_angle; pos >= past_angle+change; pos += 1) { 
    positioning_servo.write(pos);              
    delay(500);                     
  }
     lux_previous=lux;
     lux= GY30.readLightLevel();
     position_past_angle=past_angle+change;
    }
}
 
 
  }
 
  }
 
void loop() {
 
  folding_servomotor();
if (Set_Reset())
 
{ 
 
  positioning_servomotor(position_past_angle);
  tilting_servo.write(tilt_past_angle);
  
  if((periodic_lux==0)&&(88<position_past_angle)&&(position_past_angle<92))  
   { periodic_lux=GY30.readLightLevel();
  
  
  }
  Serial.println((String)"Light: " + lux + " lx ");
Serial.println((String)"position: " + position_past_angle);
Serial.println((String)"tilt: " + tilt_past_angle);
Serial.println((String)"period: " + periodic_lux);
if((88<position_past_angle)&&(position_past_angle<92))
{
periodic_lux=tilting_servomotor(tilt_past_angle);//// Tilt Motor will run near mid day time
 
}
  
delay(100);
}
}

