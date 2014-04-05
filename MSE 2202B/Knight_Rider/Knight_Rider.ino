
//unsigned long endtimer, start, present;
int thispin = 4;
unsigned long previousMillis = 0, middleMillis = 0, nextMillis = 0;
unsigned long intervalPrev = 200, intervalBoth = 0, intervalNext = 500;
int ledState = LOW;
int counter = 0;
int counterdown = 0;
int counterup = 0;
void setup() 
{
  // put your setup code here, to run once:
  //initialize pins as outputs
  for (int thispin = 4; thispin < 8; thispin++)
  {
    pinMode (thispin, OUTPUT);  
  }

}

void loop() {
  //int counter = 0;
unsigned long currentMillis = millis();
if((currentMillis - middleMillis >intervalPrev)&&(counterdown==0))
      {digitalWrite((thispin+1),HIGH);middleMillis=currentMillis;}
      else if ((currentMillis - middleMillis >intervalPrev))
      {digitalWrite((thispin-1),HIGH);middleMillis=currentMillis;}
      
  
  if(currentMillis - previousMillis > intervalNext) {
    // save the last time you blinked the LED 
    previousMillis = currentMillis;   

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW)
      ledState = HIGH;
    else
      ledState = LOW;
      
     // if((currentMillis - middleMillis >intervalPrev)&&(counterdown==0))
      //{digitalWrite((thispin+1),HIGH);middleMillis=currentMillis;}
      //else if ((currentMillis - middleMillis >intervalPrev))
      //{digitalWrite((thispin-1),HIGH);middleMillis=currentMillis;}
      
     // if((currentMillis - nextMillis >intervalBoth)&&(counterdown==0))
      //{digitalWrite((thispin+1),LOW);nextMillis=currentMillis;}
      //else if ((currentMillis - nextMillis >intervalBoth))
      //{digitalWrite((thispin-1),LOW);nextMillis=currentMillis;}
      
    // set the LED with the ledState of the variable:
    digitalWrite(thispin, ledState);
    counter++;
    if (counter%2==0)
    {
      if (counterdown == 1)
      {
        thispin--;
      }
      else
    thispin++;
    }
    if(thispin ==4)
    counterdown=0;
    
  
  if (thispin==7)
  counterdown=1;
  
  
  
}
}



