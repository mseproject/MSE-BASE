#include <uSTimer2.h>

#include <CharliePlexM.h>

#include <PID_v1.h>



//Define Variables we'll be connecting to
double RightSetpoint, LeftSetpoint, LeftInput, RightInput, RightOutput, LeftOutput;

//Specify the links and initial tuning parameters
PID myLeftPID(&LeftInput, &LeftOutput, &LeftSetpoint,2,5,1, DIRECT);
PID myRightPID(&RightInput, &RightOutput, &RightSetpoint,2,5,1, DIRECT);

const int ciDisplayTime = 100;

char  cTempDump;
char cLEDTable[13]= {'0','0','0','0','0','0','0','0','0','0','0','0','0'};

char cLEDCheck;
char cLEDCheckIndex;

char  cDisplayInput; 
char  cDisplayOutput;

unsigned int uiDisplayTime;
unsigned int uiLedtoDisplay;

unsigned int ui_LastBtnPress;


unsigned int uiLEDIndex;
unsigned int uiLedOnOff;  

unsigned int uiReadEncoders;  

unsigned int uiLEDMatrixMSB;
unsigned int uiLEDMatrixLSB;

long int liTime;
long int liLastTime;

/*
to use Charieplexing
include both

#include <uSTimer2.h>
#include <CharliePlexM.h>


//CharliePlexM::set(10,11,12, 13);     // Set up Charlieplexing for LEDs
  CharliePlexM::setBtn(10,11,12, 13, 13);     // Set up Charlieplexing for LEDs with button on pin 10
                             
		             set( vc1,     vc2,     vc3)
		    set( vc1,     vc2,     vc3,     vc4) vc = charlieplex port variable 
	    	         DX1,     Dx2,     Dx3,     Dx4  DX = arduino's Digital Ports
			 C1 ,     C2 ,     C3 ,     C4   Chaplieplex pins on MSE-duino brd
			  |        |        |        |
			  |        |      LED1>      |
			  |        | <LED2  |        |
			  |        | LED3>  |        |
			  |      <LED4               |
			  |      LED5>               |
			  |     <LED6       |        |
			  |     LED7>       |        |
			  |  <LED8 |        |        |
			  |  LED9> |        |        |
			  |        |        | <LED10 |
			  |        |    <LED11       |
			  |        |        | LED12> |




*/





  
   

/*

 MSE 2202 Lab 4
 Language: Arduino
 Authors: Michael Naish and Eugen Porter
 Date: 14/02/04
 
 Rev 2 - Updated for MSEduino
 Rev 1 - Initial version
 
 */

#include <Servo.h>
#include <EEPROM.h>


Servo servo_LeftMotor;
Servo servo_RightMotor;
Servo servo_ArmMotor;
Servo servo_GripMotor;

// Uncomment keywords to enable debugging output

//#define DEBUG_MODE_DISPLAY
//#define DEBUG_MOTORS
//#define DEBUG_LINE_TRACKERS
#define DEBUG_ENCODERS
//#define DEBUG_ULTRASONIC
//#define DEBUG_LINE_TRACKER_CALIBRATION
#define DEBUG_MOTOR_CALIBRATION
//#define DEBUG_ARM

boolean bt_Motors_Enabled = true;

//port pin constants
const int ci_encoder_Pin_A[2] = {
  2,3};
const int ci_Ultrasonic_Ping = 5;   //input plug
const int ci_Ultrasonic_Data = 6;   //output plug
const int ci_Mode_Button = 7;
const int ci_Left_Motor = 8;
const int ci_Right_Motor = 9;
const int ci_Arm_Motor = 10;
const int ci_Grip_Motor = 11;
const int ci_Charlieplex_LED1 = 4;
const int ci_Charlieplex_LED2 = 7;
const int ci_Charlieplex_LED3 = 12;
const int ci_Charlieplex_LED4 = 13;
const int ci_Left_Line_Tracker = A0;
const int ci_Middle_Line_Tracker = A1;
const int ci_Right_Line_Tracker = A2;
const int ci_Motor_Speed_Pot = A3;
const int ci_Light_Sensor = A4;
const int ci_Arm_Length_Pot = A5;

// Charlieplexing LED assignments
const int ci_Left_Line_Tracker_LED = 1;
const int ci_Middle_Line_Tracker_LED = 4;
const int ci_Right_Line_Tracker_LED = 7;
const int ci_Indicator_LED = 3;
const int ci_Heartbeat_LED = 12;

//constants

// EEPROM addresses
const int ci_Right_Motor_Offset_Address_L = 0;
const int ci_Right_Motor_Offset_Address_H = 1;
const int ci_Left_Motor_Offset_Address_L = 2;
const int ci_Left_Motor_Offset_Address_H = 3;
const int ci_Left_Line_Tracker_Dark_Address_L = 4;
const int ci_Left_Line_Tracker_Dark_Address_H = 5;
const int ci_Left_Line_Tracker_Light_Address_L = 6;
const int ci_Left_Line_Tracker_Light_Address_H = 7;
const int ci_Middle_Line_Tracker_Dark_Address_L = 8;
const int ci_Middle_Line_Tracker_Dark_Address_H = 9;
const int ci_Middle_Line_Tracker_Light_Address_L = 10;
const int ci_Middle_Line_Tracker_Light_Address_H = 11;
const int ci_Right_Line_Tracker_Dark_Address_L = 12;
const int ci_Right_Line_Tracker_Dark_Address_H = 13;
const int ci_Right_Line_Tracker_Light_Address_L = 14;
const int ci_Right_Line_Tracker_Light_Address_H = 15;

const int ci_Left_Motor_Stop = 1500;   // 200 for brake mode; 1500 for stop
const int ci_Right_Motor_Stop = 1500;
const int ci_Grip_Motor_Open = 176;    // Experiment to determine appropriate value
const int ci_Grip_Motor_Zero = 90;     //  "
const int ci_Grip_Motor_Closed = 140;  //  "
const int ci_Arm_Pot_Retracted = 1020; //  "
const int ci_Arm_Pot_Extended = 400;   //  "
const int ci_Arm_Pot_Tolerance = 60;   //  "
const int ci_Display_Time = 500;
const int ci_Num_Encoders = 2;
const int ci_Encoder_Steps_Per_Revolution = 90;
const int ci_Line_Tracker_Calibration_Interval = 100;
const int ci_Line_Tracker_Cal_Measures = 20;
const int ci_Motor_Calibration_Time = 5000;

//variables
byte b_LowByte;
byte b_HighByte;
unsigned long ul_Echo_Time;
unsigned int ui_Left_Line_Tracker_Data;
unsigned int ui_Middle_Line_Tracker_Data;
unsigned int ui_Right_Line_Tracker_Data;
unsigned int ui_Motors_Speed;
unsigned int ui_Left_Motor_Speed;
unsigned int ui_Right_Motor_Speed;
unsigned int ui_Arm_Length_Data;

volatile unsigned long ul_encoder_Count[ci_Num_Encoders] = {0,0};
unsigned long ul_encoder_Pos[ci_Num_Encoders] = {0,0};
unsigned long ul_old_Encoder_Pos[ci_Num_Encoders] = {0,0};

unsigned long ul_3_Second_timer = 0;
unsigned long ul_Display_Time;
unsigned long ul_Calibration_Time;

unsigned int ui_Right_Motor_Offset = 0;
unsigned int ui_Left_Motor_Offset = 0;

unsigned int ui_Cal_Count;
unsigned int ui_Left_Line_Tracker_Dark;
unsigned int ui_Left_Line_Tracker_Light;
unsigned int ui_Middle_Line_Tracker_Dark;
unsigned int ui_Middle_Line_Tracker_Light;
unsigned int ui_Right_Line_Tracker_Dark;
unsigned int ui_Right_Line_Tracker_Light;
unsigned int ui_Line_Tracker_Tolerance = 10;

unsigned int  ui_Robot_State_Index = 0;
//0123456789ABCDEF
unsigned int  ui_Mode_Indicator[6] = {
  0x00,    //B0000000000000000,  //stop
  0x00FF,  //B0000000011111111,  //Run
  0x0F0F,  //B0000111100001111,  //Calibrate line tracker light level
  0x3333,  //B0011001100110011,  //Calibrate line tracker dark level
  0xAAAA,  //B1010101010101010,  //Calibrate motors
  0xFFFF}; //B1111111111111111};
unsigned int  ui_Mode_Indicator_Index = 0;

//display Bits 0,1,2,3, 4, 5, 6,  7,  8,  9,  10,  11,  12,  13,   14,   15
int  iArray[16] = {
  1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,65536};
int  iArrayIndex = 0;


int i_Left_Motor_Calibration = 0;
int i_Right_Motor_Calibration = 0;

boolean bt_Heartbeat = true;
boolean bt_3_S_Time_Up = false;
boolean bt_Do_Once = false;
boolean bt_Cal_Initialized = false;

void setup() {
  Serial.begin(9600);
  Serial.begin(9600);
 //CharliePlexM::set(8,9,10,11);     // Set up Charlieplexing for LEDs
 CharliePlexM::setBtn(4,7,12,13,7);     // Set up Charlieplexing for LEDs with button on pin 10
 CharliePlexM::setEncoders(2,3);
 //CharliePlexM::set(10,11,12);     // Set up Charlieplexing for LEDs
 //CharliePlexM::setBtn(10,11,2,2);     // Set up Charlieplexing for LEDs with button on pin 10
  
  cLEDCheck = 1;
 
  uiLedOnOff = 0;
  uiDisplayTime = ciDisplayTime;
  uiLedtoDisplay = 0;
  cDisplayOutput = 'h';
  cDisplayInput = 'H';
  liTime = 0;
  liLastTime = 0;
  uiLEDMatrixMSB = 0;
  uiLEDMatrixLSB = 0; 
  ui_LastBtnPress = 1;
  
  uiReadEncoders = 0;




  //initialize the variables we're linked to
  LeftInput = CharliePlexM::ul_LeftEncoder_Count;
  RightInput = CharliePlexM::ul_RightEncoder_Count;;
  RightSetpoint = 100;
  LeftSetpoint = 100;

  //turn the PID on
  myLeftPID.SetMode(AUTOMATIC);
  myRightPID.SetMode(AUTOMATIC);


  // set up ultrasonic
  pinMode(ci_Ultrasonic_Ping, OUTPUT);
  pinMode(ci_Ultrasonic_Data, INPUT);

  // set up drive motors
  pinMode(ci_Left_Motor, OUTPUT);
  servo_LeftMotor.attach(ci_Left_Motor);
  pinMode(ci_Right_Motor, OUTPUT);
  servo_RightMotor.attach(ci_Right_Motor);

  // set up arm motors
  pinMode(ci_Arm_Motor, OUTPUT);
  servo_ArmMotor.attach(ci_Arm_Motor);
  pinMode(ci_Grip_Motor, OUTPUT);
  servo_GripMotor.attach(ci_Grip_Motor);
  servo_GripMotor.write(ci_Grip_Motor_Zero);

  pinMode(ci_Middle_Line_Tracker, INPUT);
  pinMode(ci_Left_Line_Tracker, INPUT);
  pinMode(ci_Right_Line_Tracker, INPUT);

  pinMode(ci_Motor_Speed_Pot, INPUT);
  pinMode(ci_Arm_Length_Pot, INPUT);

  // read saved values from EEPROM
  b_LowByte = EEPROM.read(ci_Right_Motor_Offset_Address_L);
  b_HighByte = EEPROM.read(ci_Right_Motor_Offset_Address_H);
  ui_Right_Motor_Offset = word(b_HighByte, b_LowByte);
  b_LowByte = EEPROM.read(ci_Left_Motor_Offset_Address_L);
  b_HighByte = EEPROM.read(ci_Left_Motor_Offset_Address_H);
  ui_Left_Motor_Offset = word(b_HighByte, b_LowByte);
  b_LowByte = EEPROM.read(ci_Left_Line_Tracker_Dark_Address_L);
  b_HighByte = EEPROM.read(ci_Left_Line_Tracker_Dark_Address_H);
  ui_Left_Line_Tracker_Dark = word(b_HighByte, b_LowByte);
  b_LowByte = EEPROM.read(ci_Left_Line_Tracker_Light_Address_L);
  b_HighByte = EEPROM.read(ci_Left_Line_Tracker_Dark_Address_H);
  ui_Left_Line_Tracker_Light = word(b_HighByte, b_LowByte);
  b_LowByte = EEPROM.read(ci_Middle_Line_Tracker_Dark_Address_L);
  b_HighByte = EEPROM.read(ci_Left_Line_Tracker_Dark_Address_H);
  ui_Middle_Line_Tracker_Dark = word(b_HighByte, b_LowByte); 
  b_LowByte = EEPROM.read(ci_Middle_Line_Tracker_Light_Address_L);
  b_HighByte = EEPROM.read(ci_Left_Line_Tracker_Dark_Address_H);
  ui_Middle_Line_Tracker_Light = word(b_HighByte, b_LowByte);
  b_LowByte = EEPROM.read(ci_Right_Line_Tracker_Dark_Address_L);
  b_HighByte = EEPROM.read(ci_Left_Line_Tracker_Dark_Address_H);
  ui_Right_Line_Tracker_Dark = word(b_HighByte, b_LowByte);
  b_LowByte = EEPROM.read(ci_Right_Line_Tracker_Light_Address_L);
  b_HighByte = EEPROM.read(ci_Left_Line_Tracker_Dark_Address_H);
  ui_Right_Line_Tracker_Light = word(b_HighByte, b_LowByte);
  ui_Line_Tracker_Tolerance = 75;
  
}

void loop()
{
  if((millis() - ul_3_Second_timer) > 3000)
  {
    bt_3_S_Time_Up = true;
  }

  // button-based mode selection
  if(CharliePlexM::ui_Btn)
  {
    if(bt_Do_Once == false)
    {
      bt_Do_Once = true;
      ui_Robot_State_Index++;
      ui_Robot_State_Index = ui_Robot_State_Index & 7;
      ul_3_Second_timer = millis();
      bt_3_S_Time_Up = false;
      bt_Cal_Initialized = false;
    }
  }
  else
  {
    bt_Do_Once = LOW;
  }



  //modes
  switch(ui_Robot_State_Index)
  {
  case 0:    //Robot stopped
    {
      readLineTrackers();
      Ping();
      servo_LeftMotor.writeMicroseconds(ci_Left_Motor_Stop); 
      servo_RightMotor.writeMicroseconds(ci_Right_Motor_Stop); 
      servo_GripMotor.write(ci_Grip_Motor_Zero);
      ui_Arm_Length_Data = analogRead(ci_Arm_Length_Pot);
#ifdef DEBUG_ARM           
      Serial.print("Arm pot: ");
      Serial.println(ui_Arm_Length_Data);
#endif
      if(ui_Arm_Length_Data < (ci_Arm_Pot_Retracted - ci_Arm_Pot_Tolerance))
      {
        Serial.println("Retracting arm");
        servo_ArmMotor.writeMicroseconds(1750);
      }
      else
      {
        servo_ArmMotor.writeMicroseconds(ci_Left_Motor_Stop);
      }

      ui_Mode_Indicator_Index = 0;
      break;
    } 
  case 1:    //Robot Run after 3 seconds
    {
      if(bt_3_S_Time_Up)
      {
        // read pot to set top motor speed
        ui_Motors_Speed = analogRead(ci_Motor_Speed_Pot);
        ui_Motors_Speed = map(ui_Motors_Speed, 0, 1023, 1650, 1900);        // motors stall below 1650; too fast above 1900
        // apply motor offsets
        ui_Left_Motor_Speed = constrain(ui_Motors_Speed - ui_Left_Motor_Offset, 1650, 1900);
        ui_Right_Motor_Speed = constrain(ui_Motors_Speed - ui_Right_Motor_Offset, 1650, 1900);

        // read levels from line tracking sensors
        readLineTrackers();

        // read encoder counts
        //         ul_encoder_Pos[0] = CharliePlexM::ul_LeftEncoder_Count;
        //         ul_encoder_Pos[1] = CharliePlexM::ul_RightEncoder_Count;
        for(int i=0; i < ci_Num_Encoders; i++)
        {
          if(ul_encoder_Pos[i] != ul_old_Encoder_Pos[i])
          {
#ifdef DEBUG_ENCODERS           
            Serial.print("Encoder ");
            Serial.print(i);
            Serial.print(": ");
            Serial.println(ul_encoder_Pos[i], DEC);
#endif
            ul_old_Encoder_Pos[i] = ul_encoder_Pos[i];  
          }
        }
         LeftInput = CharliePlexM::ul_LeftEncoder_Count;
         RightInput = CharliePlexM::ul_RightEncoder_Count;
  
         myLeftPID.Compute();
         myRightPID.Compute();
         servo_LeftMotor.writeMicroseconds(LeftOutput); 
         servo_RightMotor.writeMicroseconds(RightOutput); 

        // Line tracking code here...



        // set motor speeds
        if(bt_Motors_Enabled)
        {
           servo_LeftMotor.writeMicroseconds(ui_Left_Motor_Speed);
           servo_RightMotor.writeMicroseconds(ui_Right_Motor_Speed);
        }
        else
        {  
           servo_LeftMotor.writeMicroseconds(ci_Left_Motor_Stop); 
           servo_RightMotor.writeMicroseconds(ci_Right_Motor_Stop); 
        }
#ifdef DEBUG_MOTORS  
        Serial.print("Motors: Pot= ");
        Serial.print(ui_Motors_Speed);
        Serial.print(" , Left = ");
        Serial.print(ui_Left_Motor_Speed);
        Serial.print(" . Right = ");
        Serial.println(ui_Right_Motor_Speed);
#endif    

        ui_Mode_Indicator_Index = 1;
      }
      break;
    } 
  case 2:    //Calibrate line tracker light levels after 3 seconds
    {
      if(bt_3_S_Time_Up)
      {
        if(!bt_Cal_Initialized)
        {
          bt_Cal_Initialized = true;
          ui_Left_Line_Tracker_Light = 0;
          ui_Middle_Line_Tracker_Light = 0;
          ui_Right_Line_Tracker_Light = 0;
          ul_Calibration_Time = millis();
          ui_Cal_Count = 0;
        }
        else if((millis() - ul_Calibration_Time) > ci_Line_Tracker_Calibration_Interval)
        {
          ul_Calibration_Time = millis();
          readLineTrackers();
          ui_Left_Line_Tracker_Light += ui_Left_Line_Tracker_Data;
          ui_Middle_Line_Tracker_Light += ui_Middle_Line_Tracker_Data;
          ui_Right_Line_Tracker_Light += ui_Right_Line_Tracker_Data;
          ui_Cal_Count++;
        }
        if(ui_Cal_Count == ci_Line_Tracker_Cal_Measures)
        {
          ui_Left_Line_Tracker_Light /= ci_Line_Tracker_Cal_Measures;
          ui_Middle_Line_Tracker_Light /= ci_Line_Tracker_Cal_Measures;
          ui_Right_Line_Tracker_Light /= ci_Line_Tracker_Cal_Measures;
#ifdef DEBUG_LINE_TRACKER_CALIBRATION
          Serial.print("Light Levels: Left = ");
          Serial.print(ui_Left_Line_Tracker_Light,DEC);
          Serial.print(", Middle = ");
          Serial.print(ui_Middle_Line_Tracker_Light,DEC);
          Serial.print(", Right = ");
          Serial.println(ui_Right_Line_Tracker_Light,DEC);
#endif           
          EEPROM.write(ci_Left_Line_Tracker_Light_Address_L, lowByte(ui_Left_Line_Tracker_Light));
          EEPROM.write(ci_Left_Line_Tracker_Light_Address_H, highByte(ui_Left_Line_Tracker_Light));
          EEPROM.write(ci_Middle_Line_Tracker_Light_Address_L, lowByte(ui_Middle_Line_Tracker_Light));
          EEPROM.write(ci_Middle_Line_Tracker_Light_Address_H, highByte(ui_Middle_Line_Tracker_Light));
          EEPROM.write(ci_Right_Line_Tracker_Light_Address_L, lowByte(ui_Right_Line_Tracker_Light));
          EEPROM.write(ci_Right_Line_Tracker_Light_Address_H, highByte(ui_Right_Line_Tracker_Light));
          ui_Robot_State_Index = 0;    // go back to Mode 0
        }
        ui_Mode_Indicator_Index = 2; 
      }
      break;
    }
  case 3:    // Calibrate line tracker dark levels after 3 seconds
    {
      if(bt_3_S_Time_Up)
      {
        if(!bt_Cal_Initialized)
        {
          bt_Cal_Initialized = true;
          ui_Left_Line_Tracker_Dark = 0;
          ui_Middle_Line_Tracker_Dark = 0;
          ui_Right_Line_Tracker_Dark = 0;
          ul_Calibration_Time = millis();
          ui_Cal_Count = 0;
        }
        else if((millis() - ul_Calibration_Time) > ci_Line_Tracker_Calibration_Interval)
        {
          ul_Calibration_Time = millis();
          readLineTrackers();
          ui_Left_Line_Tracker_Dark += ui_Left_Line_Tracker_Data;
          ui_Middle_Line_Tracker_Dark += ui_Middle_Line_Tracker_Data;
          ui_Right_Line_Tracker_Dark += ui_Right_Line_Tracker_Data;
          ui_Cal_Count++;
        }
        if(ui_Cal_Count == ci_Line_Tracker_Cal_Measures)
        {
          ui_Left_Line_Tracker_Dark /= ci_Line_Tracker_Cal_Measures;
          ui_Middle_Line_Tracker_Dark /= ci_Line_Tracker_Cal_Measures;
          ui_Right_Line_Tracker_Dark /= ci_Line_Tracker_Cal_Measures;
#ifdef DEBUG_LINE_TRACKER_CALIBRATION
          Serial.print("Dark Levels: Left = ");
          Serial.print(ui_Left_Line_Tracker_Dark,DEC);
          Serial.print(", Middle = ");
          Serial.print(ui_Middle_Line_Tracker_Dark,DEC);
          Serial.print(", Right = ");
          Serial.println(ui_Right_Line_Tracker_Dark,DEC);
#endif           
          EEPROM.write(ci_Left_Line_Tracker_Dark_Address_L, lowByte(ui_Left_Line_Tracker_Dark));
          EEPROM.write(ci_Left_Line_Tracker_Dark_Address_H, highByte(ui_Left_Line_Tracker_Dark));
          EEPROM.write(ci_Middle_Line_Tracker_Dark_Address_L, lowByte(ui_Middle_Line_Tracker_Dark));
          EEPROM.write(ci_Middle_Line_Tracker_Dark_Address_H, highByte(ui_Middle_Line_Tracker_Dark));
          EEPROM.write(ci_Right_Line_Tracker_Dark_Address_L, lowByte(ui_Right_Line_Tracker_Dark));
          EEPROM.write(ci_Right_Line_Tracker_Dark_Address_H, highByte(ui_Right_Line_Tracker_Dark));
          ui_Robot_State_Index = 0;    // go back to Mode 0
        }
        ui_Mode_Indicator_Index = 3;
      }
      break;
    } 
  case 4:    //Calibrate motor straightness after 3 seconds
    {
      if(bt_3_S_Time_Up)
      {
        ul_encoder_Pos[0] = CharliePlexM::ul_LeftEncoder_Count;
        ul_encoder_Pos[1] = CharliePlexM::ul_RightEncoder_Count;
        Serial.println("CASE 4");
        if(!bt_Cal_Initialized)
        {
          bt_Cal_Initialized = true;
          ul_encoder_Count[0] = 0;
          ul_encoder_Count[1] = 0;
          // read pot to set top motor speed
          ui_Motors_Speed = analogRead(ci_Motor_Speed_Pot);
          ui_Motors_Speed = map(ui_Motors_Speed, 0, 1023, 1650, 1900);     // motor stalls below 1650; too fast above 1900 
          // set motor speeds         
          ui_Left_Motor_Speed = constrain(ui_Motors_Speed, 1650, 1900);
          ui_Right_Motor_Speed = constrain(ui_Motors_Speed, 1650, 1900);
          ul_Calibration_Time = millis();
          servo_LeftMotor.writeMicroseconds(ui_Left_Motor_Speed);
          servo_RightMotor.writeMicroseconds(ui_Right_Motor_Speed);
        }
        else if((millis() - ul_Calibration_Time) > ci_Motor_Calibration_Time)
        {
          servo_LeftMotor.writeMicroseconds(ci_Left_Motor_Stop); 
          servo_RightMotor.writeMicroseconds(ci_Right_Motor_Stop); 

          ul_encoder_Pos[0] = ul_encoder_Count[0];
          ul_encoder_Pos[1] = ul_encoder_Count[1];
#ifdef DEBUG_ENCODERS           
          for(int i=0; i < ci_Num_Encoders; i++)
          {
            Serial.print("Encoder ");
            Serial.print(i);
            Serial.print(": ");
            Serial.println(ul_encoder_Pos[i], DEC);
            Serial.println(ul_encoder_Count[0]);
            Serial.println(ul_encoder_Count[1]);

            
          }
#endif
          if(ul_encoder_Count[1] > ul_encoder_Count[0])
          {
            ui_Right_Motor_Offset = 0;
            ui_Left_Motor_Offset = (ul_encoder_Count[1] - ul_encoder_Count[0]);  // May have to update this if different calibration time is used
          }
          else
          {
            ui_Right_Motor_Offset = (ul_encoder_Count[0] - ul_encoder_Count[1]); // May have to update this if different calibration time is used
            ui_Left_Motor_Offset = 0; 
          }
#ifdef DEBUG_MOTOR_CALIBRATION
          Serial.print("Motor Offsets: Right = ");
          Serial.print(ui_Right_Motor_Offset);
          Serial.print(", Left = ");
          Serial.println(ui_Left_Motor_Offset);
#endif

          EEPROM.write(ci_Right_Motor_Offset_Address_L, lowByte(ui_Right_Motor_Offset));
          EEPROM.write(ci_Right_Motor_Offset_Address_H, highByte(ui_Right_Motor_Offset));
          EEPROM.write(ci_Left_Motor_Offset_Address_L, lowByte(ui_Left_Motor_Offset));
          EEPROM.write(ci_Left_Motor_Offset_Address_H, highByte(ui_Left_Motor_Offset));

          ui_Robot_State_Index = 0;    // go back to Mode 0 
        }
        ui_Mode_Indicator_Index = 4;
        break;
      } 
    }
  }


  if((millis() - ul_Display_Time) > ci_Display_Time)
  {
    ul_Display_Time = millis();

#ifdef DEBUG_MODE_DISPLAY  
    Serial.print("Mode: ");
    Serial.println(ui_Mode_Indicator[ui_Mode_Indicator_Index], DEC);
#endif
    bt_Heartbeat = !bt_Heartbeat;
    CharliePlexM::Write(12, bt_Heartbeat);
    digitalWrite(13, bt_Heartbeat);
    Indicator();
  }
} 


void Indicator()
{
  //display routine, if true turn on led
  CharliePlexM::Write(ci_Indicator_LED,!(ui_Mode_Indicator[ui_Mode_Indicator_Index] & (iArray[iArrayIndex])));
  iArrayIndex++;
  iArrayIndex = iArrayIndex & 15;
}


// read values from line trackers and update status of line tracker LEDs
void readLineTrackers()
{
  ui_Left_Line_Tracker_Data = analogRead(ci_Left_Line_Tracker);
  ui_Middle_Line_Tracker_Data = analogRead(ci_Middle_Line_Tracker);
  ui_Right_Line_Tracker_Data = analogRead(ci_Right_Line_Tracker);

  if(ui_Left_Line_Tracker_Data > (ui_Left_Line_Tracker_Dark - ui_Line_Tracker_Tolerance))
  {
    CharliePlexM::Write(ci_Left_Line_Tracker_LED, HIGH);
  }
  else
  { 
    CharliePlexM::Write(ci_Left_Line_Tracker_LED, LOW);
  }
  if(ui_Middle_Line_Tracker_Data > (ui_Middle_Line_Tracker_Dark - ui_Line_Tracker_Tolerance))
  {
    CharliePlexM::Write(ci_Middle_Line_Tracker_LED, HIGH);
  }
  else
  { 
    CharliePlexM::Write(ci_Middle_Line_Tracker_LED, LOW);
  }
  if(ui_Right_Line_Tracker_Data > (ui_Right_Line_Tracker_Dark - ui_Line_Tracker_Tolerance))
  {
    CharliePlexM::Write(ci_Right_Line_Tracker_LED, HIGH);
  }
  else
  { 
    CharliePlexM::Write(ci_Right_Line_Tracker_LED, LOW);
  }

#ifdef DEBUG_LINE_TRACKERS
  Serial.print("Trackers: Left = ");
  Serial.print(ui_Left_Line_Tracker_Data,DEC);
  Serial.print(", Middle = ");
  Serial.print(ui_Middle_Line_Tracker_Data,DEC);
  Serial.print(", Right = ");
  Serial.println(ui_Right_Line_Tracker_Data,DEC);
#endif

}

// measure distance to target using ultrasonic sensor  
void Ping()
{
  //Ping Ultrasonic
  //Send the Ultrasonic Range Finder a 10 microsecond pulse per tech spec
  digitalWrite(ci_Ultrasonic_Ping, HIGH);
  delayMicroseconds(10);  //The 10 microsecond pause where the pulse in "high"
  digitalWrite(ci_Ultrasonic_Ping, LOW);
  //use command pulseIn to listen to Ultrasonic_Data pin to record the
  //time that it takes from when the Pin goes HIGH until it goes LOW 
  ul_Echo_Time = pulseIn(ci_Ultrasonic_Data, HIGH, 10000);

  // Print Sensor Readings
#ifdef DEBUG_ULTRASONIC
  Serial.print("Time (microseconds): ");
  Serial.print(ul_Echo_Time, DEC);
  Serial.print(", Inches: ");
  Serial.print(ul_Echo_Time/148); //divide time by 148 to get distance in inches
  Serial.print(", cm: ");
  Serial.println(ul_Echo_Time/58); //divide time by 58 to get distance in cm 
#endif
}  





