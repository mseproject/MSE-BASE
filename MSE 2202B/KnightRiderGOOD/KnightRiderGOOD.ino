int thispin = 4;
unsigned long prevbuttonMillis, previousMillis = 0, middleMillis, nextMillis = 0;
unsigned long intervalPrev = 100, intervalBoth = 0, intervalNext = 250;
int ledState = LOW;
int counter = 0;
int counterdown = 0;
int counterup = 0;
int buttonState;
boolean buttonPressed = false;
long val;

void setup() {
  // put your setup code here, to run once:
  for (int thispin = 4; thispin < 8; thispin++)
  {
    pinMode (thispin, OUTPUT);

}
  pinMode (3, INPUT);
  pinMode (2, INPUT);
  digitalWrite (2, HIGH);
  val = 0;
  middleMillis = 0;
  prevbuttonMillis = 0;
}

void loop() {
  unsigned long currentMillis = millis(), buttonMillis = millis();
  
  buttonState = digitalRead (2);
  if (digitalRead(3) == LOW)
  {
    val = analogRead(A0);
    val = map(val, 0, 1023, 0, 255);
  }
  
 else if (digitalRead(3) == HIGH)
  {
    if (buttonState != buttonPressed && buttonState == LOW){
      val = (buttonMillis - prevbuttonMillis)/10;}
    else if(buttonState == buttonPressed){
      if(buttonState == LOW){
        prevbuttonMillis = buttonMillis;
        buttonPressed = true;}
     if (buttonState == HIGH){
       buttonPressed = false;}     
    }
  }
  // put your main code here, to run repeatedly:
if (currentMillis - middleMillis > val)
{
  middleMillis = currentMillis;
  digitalWrite (thispin, HIGH);
  counter++;
  if ((counter == 4)&&(counterdown != 1))
  digitalWrite((thispin+1), HIGH);
  if ((counter == 5)&&(counterdown != 1))
  {
    counter = 0;
    digitalWrite(thispin, LOW);
    thispin++;
  }
  
  if ((counter == 4)&&(counterdown == 1))
    digitalWrite((thispin-1), HIGH);
  if ((counter == 5)&&(counterdown == 1))
  {
    counter = 0;
    digitalWrite(thispin, LOW);
    thispin--;
  }
  if (thispin == 7)
  counterdown = 1;
  if (thispin == 4)
  counterdown = 0;
  
 
  
  
}


  }
