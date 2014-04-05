//turning not 100, play with encoder counts 
// room three distance is too long, its to the right of door need to go to right of door
//play with motor speeds with weight being added on



#include <uSTimer2.h>

#include <CharliePlexM.h>


#include <Servo.h>


int start =1;
int currentPosition = 1;
int targetPosition = 3;

int direct;
// 1 = middle
// 2 = wall

Servo servo_LeftMotor, servo_RightMotor, servo_FrontMotor;

unsigned int leftmotorspeed = 1800, rightmotorspeed=1800;

const int ci_Ultrasonic_Ping_Left = 5;
const int ci_Ultrasonic_Data_Left = 6;

const uint8_t ci_Ultrasonic_Ping_Right = A4;
const uint8_t ci_Ultrasonic_Data_Right = A5;


// vex front left
unsigned long ul_Echo_Time_Left;
// jokes back right
unsigned long ul_Echo_Time_Right;
long l_Echo_Time_Diff = 0;
unsigned long echoaverage;



boolean bt_Heartbeat = true;
boolean bt_3_S_Time_Up = false;
boolean bt_Do_Once = false;
boolean bt_Cal_Initialized = false;

unsigned long ul_3_Second_timer = 0;
unsigned int ui_Robot_State_Index = 0;

double LeftInput, RightInput;

void setup()
{
  Serial.begin(9600);
  
  CharliePlexM::setBtn(4,7,12,13,7);
  CharliePlexM::setEncoders(2,3);
  
  LeftInput = CharliePlexM::ul_LeftEncoder_Count;
  RightInput = CharliePlexM::ul_RightEncoder_Count;

  
  pinMode(ci_Ultrasonic_Ping_Left, OUTPUT);
  pinMode(ci_Ultrasonic_Data_Left, INPUT);
  
  pinMode(ci_Ultrasonic_Ping_Right, OUTPUT);
  pinMode(ci_Ultrasonic_Data_Right, INPUT);
  
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  
  servo_RightMotor.attach(9);
  servo_LeftMotor.attach(8); 
}

void loop()
{
  
  if((millis() - ul_3_Second_timer) > 3000)
  {
    bt_3_S_Time_Up = true;
  }
  
  if(CharliePlexM::ui_Btn)
  {
    if(bt_Do_Once == false)
    {
      bt_Do_Once = true;
    ui_Robot_State_Index++;
    ui_Robot_State_Index = ui_Robot_State_Index &7;
    ul_3_Second_timer = millis();
    bt_3_S_Time_Up = false;
    bt_Cal_Initialized = false;
    }
  }
else
{
  bt_Do_Once = LOW;
}





switch(ui_Robot_State_Index)
{
  case 0:
  { 
    
    delay(1000);
    Ping_Left();
    Ping_Right();
    
    //l_Echo_Time_Diff = ul_Echo_Time_Left - ul_Echo_Time_Right;
    //Serial.println(l_Echo_Time_Diff);
    break;
  }
  case 1000000:
  {
        servo_LeftMotor.writeMicroseconds(1225);
        servo_RightMotor.writeMicroseconds(1200);
        break;
  }
  case 1:
  {
    while (CharliePlexM::ul_LeftEncoder_Count < 1000 && CharliePlexM::ul_RightEncoder_Count < 1000)
    {
    // straight
    //servo_LeftMotor.writeMicroseconds(1920);
      //  servo_RightMotor.writeMicroseconds(1800);
    Ping_Left();
    Ping_Right();
    l_Echo_Time_Diff = ul_Echo_Time_Left - ul_Echo_Time_Right;
    
    if (start == 1)
    {
      start++;
      servo_LeftMotor.writeMicroseconds(1225);
    servo_RightMotor.writeMicroseconds(1200);
    }
    
   
    if (l_Echo_Time_Diff < 300)
    {
     if (ul_Echo_Time_Left > 1400 && ul_Echo_Time_Right > 1400)
     {
       
      servo_LeftMotor.writeMicroseconds(1225);
      servo_RightMotor.writeMicroseconds(1200);
      
      // Serial.println("DOOR");
     }  
     
     else 
     {
     if(ul_Echo_Time_Left < 1000 && ul_Echo_Time_Right < 1000)
      {
        
        servo_LeftMotor.writeMicroseconds(1170);
        servo_RightMotor.writeMicroseconds(1220);
        direct = 1;
        //Serial.println("MIDDLE");
      }
       if(ul_Echo_Time_Left < 1000 && ul_Echo_Time_Right > 1000)
      {
        
       servo_LeftMotor.writeMicroseconds(1170);
        servo_RightMotor.writeMicroseconds(1220);
        direct = 1;
        //Serial.println("MIDDLE");
      }
     if(ul_Echo_Time_Left > 1000 && ul_Echo_Time_Right > 1000)
     {
   
        servo_LeftMotor.writeMicroseconds(1230);
        servo_RightMotor.writeMicroseconds(1200);
        direct = 1;
       //Serial.println("WALL");
     }
      if(ul_Echo_Time_Left > 700 && ul_Echo_Time_Right < 700)
     {
   
       servo_LeftMotor.writeMicroseconds(1230);
        servo_RightMotor.writeMicroseconds(1200);
        direct = 2;
       //Serial.println("WALL");
     }
     if(ul_Echo_Time_Left < 570 && ul_Echo_Time_Left > 550 && ul_Echo_Time_Right < 570 && ul_Echo_Time_Right > 550)
     {
   
       servo_LeftMotor.writeMicroseconds(1225);
        servo_RightMotor.writeMicroseconds(1200);
        direct = 2;
       //Serial.println("WALL");
     }
     }
    }
    }
    servo_LeftMotor.writeMicroseconds(1500);
        servo_RightMotor.writeMicroseconds(1500);
        delay(2000);
    CharliePlexM::ul_LeftEncoder_Count = 0;
    CharliePlexM::ul_RightEncoder_Count = 0;
    
    while (CharliePlexM::ul_LeftEncoder_Count < 1000 && CharliePlexM::ul_RightEncoder_Count < 1000)
    {
    // straight
    //servo_LeftMotor.writeMicroseconds(1920);
      //  servo_RightMotor.writeMicroseconds(1800);
    Ping_Left();
    Ping_Right();
    l_Echo_Time_Diff = ul_Echo_Time_Left - ul_Echo_Time_Right;
    
    if (start == 2)
    {
      start++;
      servo_LeftMotor.writeMicroseconds(1920);
    servo_RightMotor.writeMicroseconds(1800);
    }
    
   
    if (l_Echo_Time_Diff < 300)
    {
     if (ul_Echo_Time_Left > 1400 && ul_Echo_Time_Right > 1400)
     {
       
      servo_LeftMotor.writeMicroseconds(1920);
      servo_RightMotor.writeMicroseconds(1800);
      
      // Serial.println("DOOR");
     }  
     
     else 
     {
     if(ul_Echo_Time_Left < 1000 && ul_Echo_Time_Right < 1000)
      {
        
        servo_LeftMotor.writeMicroseconds(1950);
        servo_RightMotor.writeMicroseconds(1760);
        direct = 1;
        //Serial.println("MIDDLE");
      }
       if(ul_Echo_Time_Left < 1000 && ul_Echo_Time_Right > 1000)
      {
        
       servo_LeftMotor.writeMicroseconds(1950);
        servo_RightMotor.writeMicroseconds(1760);
        direct = 1;
        //Serial.println("MIDDLE");
      }
     if(ul_Echo_Time_Left > 1000 && ul_Echo_Time_Right > 1000)
     {
   
        servo_LeftMotor.writeMicroseconds(1850);
        servo_RightMotor.writeMicroseconds(1850);
        direct = 1;
       //Serial.println("WALL");
     }
      if(ul_Echo_Time_Left > 700 && ul_Echo_Time_Right < 700)
     {
   
       servo_LeftMotor.writeMicroseconds(1850);
        servo_RightMotor.writeMicroseconds(1850);
        direct = 2;
       //Serial.println("WALL");
     }
     if(ul_Echo_Time_Left < 570 && ul_Echo_Time_Left > 550 && ul_Echo_Time_Right < 570 && ul_Echo_Time_Right > 550)
     {
   
       servo_LeftMotor.writeMicroseconds(1920);
        servo_RightMotor.writeMicroseconds(1800);
        direct = 2;
       //Serial.println("WALL");
     }
     }
    }
    }
    servo_LeftMotor.writeMicroseconds(1500);
        servo_RightMotor.writeMicroseconds(1500);
    CharliePlexM::ul_LeftEncoder_Count = 0;
    CharliePlexM::ul_RightEncoder_Count = 0;
    echoaverage = ((ul_Echo_Time_Left + ul_Echo_Time_Right) / 2);
    
    if(echoaverage <=800)
    {
      Serial.println(1);
    }
    else if( echoaverage > 800 && echoaverage <= 900)
    {
      Serial.println(2);
    }
    else if(echoaverage >900 && echoaverage <=1000)
    {
      Serial.println(3);
    }
    else if(echoaverage >1000 && <= 1100)
    {
      Serial.println(4);
    }
    else
    {
      Serial.println(5);
    }
    //sending a 1 value means we are findng mail, ie aligning with mail box
     ui_Robot_State_Index = 2;
        
       
  }
  case 2:
  {
    int incoming;
    //finding mail case
    Serial.available();
    while(Serial.available())
    {
      incoming = Serial.parseInt();
      if(incoming == 5)
      {
        servo_LeftMotor.writeMicroseconds(1920);
        servo_RightMotor.writeMicroseconds(1800);
        delay(10);
        //move backwards
      }
      else 
      {
        servo_LeftMotor.writeMicroseconds(1500);
        servo_RightMotor.writeMicroseconds(1500);
        ui_Robot_State_Index = 3;

        
      }
      servo_LeftMotor.writeMicroseconds(1500);
        servo_RightMotor.writeMicroseconds(1500);
    }
     servo_LeftMotor.writeMicroseconds(1500);
        servo_RightMotor.writeMicroseconds(1500);
    break;
  }
  
  case 3:
  {
    servo_LeftMotor.writeMicroseconds(1900);
    servo_RightMotor.writeMicroseconds(1100);
    break;
  }
  
  /*hile (CharliePlexM::ul_LeftEncoder_Count < 1000 && CharliePlexM::ul_RightEncoder_Count < 1000)
    {
     Ping_Left();
    
     if(ul_Echo_Time_Left > 0 && ul_Echo_Time_Left <= 630)
      {
        
        servo_LeftMotor.writeMicroseconds(1210);
        servo_RightMotor.writeMicroseconds(1150);
      }
     if(ul_Echo_Time_Left <= 1200 && ul_Echo_Time_Left > 630)
     {
   
       servo_LeftMotor.writeMicroseconds(1250);
        servo_RightMotor.writeMicroseconds(1150);
     }
     if(ul_Echo_Time_Left == 0 || ul_Echo_Time_Left >1200)
     {
       
       servo_LeftMotor.writeMicroseconds(1250);
       servo_RightMotor.writeMicroseconds(1145);
     }
    }
    CharliePlexM::ul_LeftEncoder_Count = 0;
    CharliePlexM::ul_RightEncoder_Count = 0;
    
    
 
    delay (500);
    
    while (CharliePlexM::ul_LeftEncoder_Count < 1000 && CharliePlexM::ul_RightEncoder_Count < 1000)
    {
     Ping_Left();
    
     if(ul_Echo_Time_Left > 0 && ul_Echo_Time_Left <= 630)
      {
        
        servo_LeftMotor.writeMicroseconds(1900);
        servo_RightMotor.writeMicroseconds(1800);
      }
     if(ul_Echo_Time_Left <= 1200 && ul_Echo_Time_Left > 630)
     {
   
       servo_LeftMotor.writeMicroseconds(1800);
       servo_RightMotor.writeMicroseconds(1900);
     }
     if(ul_Echo_Time_Left == 0 || ul_Echo_Time_Left >1200)
     {
       
       servo_LeftMotor.writeMicroseconds(1865);
       servo_RightMotor.writeMicroseconds(1790);
     }
    }
    CharliePlexM::ul_LeftEncoder_Count = 0;
    CharliePlexM::ul_RightEncoder_Count = 0;
    servo_LeftMotor.writeMicroseconds(1500);
       servo_RightMotor.writeMicroseconds(1500);
   delay (10000000);
    
    /*
    
    while(CharliePlexM::ul_LeftEncoder_Count < 40)
     {
        servo_LeftMotor.writeMicroseconds(1950);
        servo_RightMotor.writeMicroseconds(1650);
     }
     CharliePlexM::ul_LeftEncoder_Count = 0;
     CharliePlexM::ul_RightEncoder_Count = 0;
     servo_LeftMotor.writeMicroseconds(1500);
     servo_RightMotor.writeMicroseconds(1500);
     delay(100);

     while(CharliePlexM::ul_RightEncoder_Count < 40)
     {
        servo_LeftMotor.writeMicroseconds(1450);
        servo_RightMotor.writeMicroseconds(1050);
     }
     CharliePlexM::ul_LeftEncoder_Count = 0;
     CharliePlexM::ul_RightEncoder_Count = 0;
      servo_LeftMotor.writeMicroseconds(1500);
     servo_RightMotor.writeMicroseconds(1500);
     delay(100);
     
     */
     
     
    //keep this
   //for(int i=0; i<3; i++)
   //{
    // Route(currentPosition, targetPosition);
   //}
   
 
   
   
  // servo_LeftMotor.writeMicroseconds(1500);
  // servo_RightMotor.writeMicroseconds(1500);
  
 /*if(CharliePlexM::ul_LeftEncoder_Count >2000 && CharliePlexM::ul_RightEncoder_Count >2000)
 {
 CharliePlexM::ul_LeftEncoder_Count = 0;
 CharliePlexM::ul_RightEncoder_Count = 0;
 while (CharliePlexM::ul_LeftEncoder_Count < 450)
 {
   servo_LeftMotor.writeMicroseconds(1950);
   servo_RightMotor.writeMicroseconds(1400);
 }
   servo_LeftMotor.writeMicroseconds(1700);
   servo_RightMotor.writeMicroseconds(1700);
 }*/
   
  
  
 
}


}

void Route(int current, int target)
{
  if(current == 1 && target == 2)
  {
  while(CharliePlexM::ul_LeftEncoder_Count < 5793 && CharliePlexM::ul_RightEncoder_Count < 5793)
   {
     Ping_Left();
    
     if(ul_Echo_Time_Left < 700)
      {
        servo_LeftMotor.writeMicroseconds(1300);
        servo_RightMotor.writeMicroseconds(1350);
      }
     if(ul_Echo_Time_Left >= 700)
     {
       servo_LeftMotor.writeMicroseconds(1350);
       servo_RightMotor.writeMicroseconds(1300);
     }
  
  }
  CharliePlexM::ul_LeftEncoder_Count = 0;
  CharliePlexM::ul_RightEncoder_Count = 0;
   servo_LeftMotor.writeMicroseconds(1500);
    servo_RightMotor.writeMicroseconds(1500);
    delay(2000);
  currentPosition = 2;
  targetPosition = 1;
  }
  
  if(current == 2 && target == 1)
  {
    while(CharliePlexM::ul_LeftEncoder_Count < 5793 && CharliePlexM::ul_RightEncoder_Count <5793)
   {
     Ping_Left();
    
     if(ul_Echo_Time_Left < 700)
      {
        servo_LeftMotor.writeMicroseconds(1700);
        servo_RightMotor.writeMicroseconds(1650);
      }
     if(ul_Echo_Time_Left >= 700)
     {
       servo_LeftMotor.writeMicroseconds(1650);
       servo_RightMotor.writeMicroseconds(1700);
     }
  
  }
  CharliePlexM::ul_LeftEncoder_Count = 0;
  CharliePlexM::ul_RightEncoder_Count = 0;
  servo_LeftMotor.writeMicroseconds(1500);
    servo_RightMotor.writeMicroseconds(1500);
    delay(2000);
  currentPosition = 1;
  targetPosition = 3;
  }
  
  
    if(current == 1 && target == 3)
  {
    while(CharliePlexM::ul_LeftEncoder_Count < 906 && CharliePlexM::ul_RightEncoder_Count <906)
   {
     Ping_Left();
    
     if(ul_Echo_Time_Left < 700)
      {
        servo_LeftMotor.writeMicroseconds(1700);
        servo_RightMotor.writeMicroseconds(1650);
      }
     if(ul_Echo_Time_Left >= 700)
     {
       servo_LeftMotor.writeMicroseconds(1650);
       servo_RightMotor.writeMicroseconds(1700);
     }
  
  }
  servo_LeftMotor.writeMicroseconds(1500);
    servo_RightMotor.writeMicroseconds(1500);
    delay(2000);
  CharliePlexM::ul_LeftEncoder_Count = 0;
  CharliePlexM::ul_RightEncoder_Count = 0;
  while (CharliePlexM::ul_LeftEncoder_Count < 390)
 {
   servo_LeftMotor.writeMicroseconds(1950);
   servo_RightMotor.writeMicroseconds(1400);
 }
   CharliePlexM::ul_LeftEncoder_Count = 0;
  CharliePlexM::ul_RightEncoder_Count = 0;
  while(CharliePlexM::ul_LeftEncoder_Count <4075 && CharliePlexM::ul_RightEncoder_Count < 4075)
  {
    Ping_Left();
    
     if(ul_Echo_Time_Left < 700)
      {
        servo_LeftMotor.writeMicroseconds(1700);
        servo_RightMotor.writeMicroseconds(1650);
      }
     if(ul_Echo_Time_Left >= 700)
     {
       servo_LeftMotor.writeMicroseconds(1650);
       servo_RightMotor.writeMicroseconds(1700);
     }
  
  }
    CharliePlexM::ul_LeftEncoder_Count = 0;
    CharliePlexM::ul_RightEncoder_Count = 0;
    servo_LeftMotor.writeMicroseconds(1500);
    servo_RightMotor.writeMicroseconds(1500);
    delay(2000);
   servo_LeftMotor.writeMicroseconds(1500);
    servo_RightMotor.writeMicroseconds(1500);
  
  }
  
  
  
   
}


void Ping_Right()
{
  digitalWrite(ci_Ultrasonic_Ping_Right, HIGH);
  delayMicroseconds(10);
  digitalWrite(ci_Ultrasonic_Ping_Right, LOW);
  ul_Echo_Time_Right = pulseIn(ci_Ultrasonic_Data_Right, HIGH, 10000);
  Serial.println("JOKES: "); 
  Serial.println(ul_Echo_Time_Right, DEC);
  
}



void Ping_Left()
{
  digitalWrite(ci_Ultrasonic_Ping_Left, HIGH);
  delayMicroseconds(10);
  digitalWrite(ci_Ultrasonic_Ping_Left, LOW);
  ul_Echo_Time_Left = pulseIn(ci_Ultrasonic_Data_Left, HIGH, 10000);
 Serial.println("VEX: ");
  Serial.println(ul_Echo_Time_Left, DEC);
  
 
}
