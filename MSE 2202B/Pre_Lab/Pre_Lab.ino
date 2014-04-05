long previousMillis = 0;        // will store last time LED was updated
long interval = 0;

int led1 = 4;
int led2 = 5;
int led3 = 6;
int led4 = 7;
int thisPin = 4;
int nextPin = 3;
int tState = LOW;
int nState = LOW;

const int buttonPin = 2;
int buttonState = 0;
long prevbuttonMillis = 0;
boolean buttonPressed = false;

const int switchPin = 3;
int switchState = 0;

void setup() {
  Serial.begin(9600);
  
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, HIGH);
  
  pinMode(switchPin, INPUT);
  
}

void loop()
{ 
  switchState = digitalRead(switchPin);
  buttonState = digitalRead(buttonPin);
  unsigned long currentMillis = millis();
  unsigned long buttonMillis = millis();
  
  if(switchState == LOW){
  interval = analogRead(A0);
  interval = map(interval, 0, 1023, 0, 255);
  }
  
  else if(switchState == HIGH){
    
    if (buttonState != buttonPressed && buttonState == LOW){
      interval = (buttonMillis - prevbuttonMillis)/10;}
    else if(buttonState == buttonPressed){
      if(buttonState == LOW){
        prevbuttonMillis = buttonMillis;
        buttonPressed = true;}
     if (buttonState == HIGH){
       buttonPressed = false;}     
  }    
  
  }
  
  if(thisPin == 4 && thisPin > nextPin){
  if(currentMillis - previousMillis > interval) {
    // save the last time you blinked the LED 
    previousMillis = currentMillis;   
    tState = HIGH;
    digitalWrite(thisPin, tState);
  }
  nextPin = 5;
  nState = LOW;
  }
  
  if(thisPin < nextPin && nState == LOW){
  if(currentMillis - previousMillis > interval) {
    // save the last time you blinked the LED 
    previousMillis = currentMillis;
    nState = HIGH;   
    digitalWrite(nextPin, nState);
  }
  }  
  
  if(thisPin < nextPin && nState == HIGH){
    if(currentMillis - previousMillis > interval) {
    // save the last time you blinked the LED 
    previousMillis = currentMillis;   
    tState = LOW;
    digitalWrite(thisPin, tState);
    thisPin = nextPin;
    tState = nState;
    nextPin++;
    nState = LOW;
  }
  }
  
  if(thisPin == 7 && thisPin < nextPin){
  if(currentMillis - previousMillis > interval) {
    // save the last time you blinked the LED 
    previousMillis = currentMillis;   
    tState = HIGH;
    digitalWrite(thisPin, tState);
  }
  nextPin = 6;
  nState = LOW;
  }
  
  if(thisPin > nextPin && nState == LOW){
  if(currentMillis - previousMillis > interval) {
    // save the last time you blinked the LED 
    previousMillis = currentMillis;
    nState = HIGH;   
    digitalWrite(nextPin, nState);
  }
  }  
  
  if(thisPin > nextPin && nState == HIGH){
    if(currentMillis - previousMillis > interval) {
    // save the last time you blinked the LED 
    previousMillis = currentMillis;   
    tState = LOW;
    digitalWrite(thisPin, tState);
    thisPin = nextPin;
    tState = nState;
    nextPin--;
    nState = LOW;
  }
  }
  
}
Chat conversation end

