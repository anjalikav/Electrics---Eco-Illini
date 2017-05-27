// include the library code:
#include <LiquidCrystal.h>    //for the LCD Display
#include <Time.h>             //for the timer

//GLOBAL VARS ------------------------------------

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

boolean running = true;
int laps = 1;
int diameter = 617;   //~617mm in diameter, remeasured to be 61.45 cm

const int sensor = 8;
const unsigned long sampleTime = 150;   //ms it takes to complete one revolution?
const int buttonPinLap = 10;
const int buttonPinReset = 9;
const int ledPin = 13;
int lapButtonState = 0;
int resetButtonState = 0;

int sensorState = 0, lastState = 0;
int count = 0;

unsigned long startTimer = 0;
float distance;
float avg;
int kount;
time_t elapsedTime;

//------------------------------------------------


void setup() {
  // initialize the sensor pin as an input:
  pinMode(sensor, INPUT);     
  pinMode(buttonPinLap, INPUT);
  pinMode(buttonPinReset, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(sensor, HIGH); // turn on the pullup
  
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.setCursor(9,1);
  lcd.print("AVG:");
  lcd.setCursor(0,0);
  lcd.print("T:");
  lcd.setCursor(0,1);
  lcd.print("Speed:");
  lcd.setCursor(4,0);
  lcd.print(":");
  lcd.setCursor(9,0);
  lcd.print("Lap:");
  lcd.setCursor(15, 1);
  lcd.print(" ");
  lcd.setCursor(7,0);
  lcd.print("  ");
  lcd.setCursor(14, 0);
  lcd.print("  ");
  lcd.setCursor(8, 1);
  lcd.print(" ");
  lcd.setCursor(15,1);
  lcd.print(" ");
}

void loop() {
  lapButtonState = digitalRead(buttonPinLap);
  resetButtonState = digitalRead(buttonPinReset);
  //CODE FOR ON BUTTON -------------------------------------------------------------
  if (lapButtonState == HIGH) {
    // turn LED on:
    running = true;
    digitalWrite(ledPin,HIGH);
    laps++;
    lcd.setCursor(13,0);
    lcd.print(laps);

    //startTimer = 0;
  }
  //CODE FOR RESET BUTTON ----------------------------------------------------------
  if (resetButtonState == HIGH) {
    running = false;
    digitalWrite(ledPin,LOW);

    setTime(0);
    laps = 1;
    count = 0;
    kount = 0;
    distance = 0;
    avg = 0;
    lcd.clear();
    //must reset miles, distance, average, 
  }
  //--------------------------------------------------------------------------------
  if (running == true){
    elapsedTime=now();
    lcd.setCursor(9,1);
    lcd.print("AVG:");
    lcd.setCursor(0,0);
    lcd.print("T:");
    lcd.setCursor(0,1);
    
    lcd.print("Speed:");
    lcd.setCursor(4,0);
    lcd.print(":");
    lcd.setCursor(9,0);
    lcd.print("Lap:");
    lcd.setCursor(15, 1);
    lcd.print(" ");
    lcd.setCursor(7,0);
    lcd.print("  ");
    lcd.setCursor(14, 0);
    lcd.print("  ");
    lcd.setCursor(8, 1);
    lcd.print(" ");
    lcd.setCursor(15,1);
    lcd.print(" ");
    
    lcd.setCursor(2,0);
    lcd.print(minute(elapsedTime));
    lcd.setCursor(5,0);
    lcd.print(second(elapsedTime));

    kount = 0;
    boolean kflag = LOW;
    unsigned long currentTime = 0;
    unsigned long startTime = millis();
    //Counts spokes during sample time
    while (currentTime <= sampleTime)
    {
      sensorState = digitalRead(sensor);
      //Counts the spokes that pass by the sensor
      if (sensorState && !lastState)
      {
        kount++;
        count++;
        
      }
      currentTime=millis()-startTime;
      lastState = sensorState;
    }
    int kount2rpm = int(60000./float(sampleTime))*kount;
    int speed1 = kount2rpm*(3.14*diameter*60/1000*3.28/5280)/16;
    if (speed1 == 0){
      lcd.setCursor(6, 1);
      lcd.print("0  ");
    }
    if (speed1 < 10){
      lcd.setCursor(6, 1);
      lcd.print(" ");
      lcd.setCursor(7, 1);
      lcd.print(speed1);
    } 
    else {
      lcd.setCursor(6, 1);
      lcd.print(speed1);
    }
    
    //miles =  9.6 - (count*3.14*diameter/1000*3.28/5280/16); //SAE miles to go before end of the run
    distance = (count*3.14*diameter/1000*3.28/5280/16);     //total distance traveled
    avg = (distance/((millis()-startTimer)*.000000277778));
   
    if (avg < 10){
     lcd.setCursor(13, 1);
     lcd.print(" ");
     lcd.setCursor(14, 1);
     lcd.print(avg);
     lcd.setCursor(15, 1);
     lcd.print(" ");
    } 
    else {
      lcd.setCursor(13,1);
      lcd.print(avg);
      lcd.setCursor(15, 1);
      lcd.print(" ");
    }
  }
}
