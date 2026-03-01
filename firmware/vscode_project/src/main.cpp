#include <functions.h>
//First Iteration of Swim Clock Code to test on bench with Arduino Uno

#define segCount 4
#define BLANK 0b00000000

//TPIC (ShiftRegister)  Pins
//RCK pin 12 (low is no output)
const int latchPin = 10; //6 beetle

//SRCK pin 13
const int clockPin = 11; //20 beetle

//SER_IN pin 3
const int dataPin = 12; //19 beetle

//digit array 0-9 without DP
const int datArray[10] = {0b11111100, 0b01100000, 0b11011010, 0b11110010, 0b01100110, 0b10110110, 0b10111110, 0b11100000, 0b11111110, 0b11100110};

//updside down digit array 0-9 without DP
const int invdatArray[10] = {0b11111100, 0b00001100, 0b11011010, 0b10011110, 0b00101110, 0b10110110, 0b11110110, 0b00011100, 0b11111110, 0b00111110};

//Testing intervals to cycle through (sec)
const int timearr[10] = {20, 5, 20, 5, 30, 10, 60, 10, 20, 3000};
int intevaltime = 0;

//Millis setup
unsigned long prevMillis;
unsigned long currentMillis;

//decimal Pt
byte dp = 0b00000000;

//Time
unsigned long tcount;
int shiftminsten = 0, shiftmins = 0, shiftsecsten = 0, shiftsecs = 0;
int secs, secsten, mins, minsten;

bool counting = false;
bool flag = false;

void setup() {
  // Arduino TPIC pins initilization to 0 with dp
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  //serial
  Serial.begin(9600);
  
  //Set Diplay Blank
  for (int i = 0; i < segCount; i++) {
    digitalWrite(latchPin, HIGH);
    shiftOut(dataPin, clockPin, LSBFIRST, BLANK);
  }
  digitalWrite(latchPin, LOW);

  delay(1000);
  
  for (int i = 0; i < segCount; i++) {
    digitalWrite(latchPin, HIGH);
    if (i%3 != 0 && i != 0)
      dp = 0b00000001;
    shiftOut(dataPin, clockPin, LSBFIRST, (datArray[0] | dp));
    digitalWrite(latchPin, LOW);
    delay(100);
  }
  //Turn on decimal points
  dp = 0b00000001;
}

void loop() {
  // put your main code here, to run repeatedly:
  currentMillis = millis();

  getInterval();
  countDown();
  //countUp();
  
}

void getInterval()
{
  if (!counting) {
    static int i = 0;
    int curInt = timearr[i++];
    int minutes = curInt / 60, seconds = curInt % 60;

    shiftminsten = minutes / 10;
    shiftmins = minutes % 10;

    shiftsecsten = seconds / 10;
    shiftsecs = seconds % 10;

    counting = true;
    flag = false;
    
    
    // Serial.print(shiftminsten);
    // Serial.print(", ");
    // Serial.print(shiftmins);
    // Serial.print(", ");
    // Serial.print(shiftsecsten);
    // Serial.print(", ");
    // Serial.println(shiftsecs);

  }
}


void countDown()
{

  if (currentMillis - prevMillis >= 1000) {
   

    digitalWrite(latchPin, HIGH);
    shiftOut(dataPin, clockPin, LSBFIRST, datArray[shiftminsten]);
    
    shiftOut(dataPin, clockPin, LSBFIRST, datArray[shiftmins] | dp);
  
    shiftOut(dataPin, clockPin, LSBFIRST, invdatArray[shiftsecsten] | dp);
  
    shiftOut(dataPin, clockPin, LSBFIRST, datArray[shiftsecs]);
    digitalWrite(latchPin, LOW);

    if (flag){
    Serial.println("Done Display");
    counting = false;
    return;
    }

    prevMillis += 1000;
    if (shiftsecs > 0)
      shiftsecs--;
    else if (shiftsecsten > 0) {
      shiftsecsten--;
      shiftsecs = 9;
    }
    else if (shiftmins > 0) {
      shiftmins--;
      shiftsecsten = 5;
      shiftsecs = 9;
    }
    else if (shiftminsten > 0) {
      shiftminsten--;
      shiftmins = 9;
    }
    else {
      flag = true;
    }

  }
}

// void countUp()
// {
//   if (currentMillis - prevMillis >= 1000) {
//     prevMillis += 1000;
   
//     shiftsecs += 1;

//     if (shiftsecs > 9) {
//       shiftsecs = 0;
//       shiftsecsten += 1;
//     }

//     if (shiftsecsten > 5) {
//       shiftsecsten = 0;
//       shiftmins += 1;
//     }

//     if (shiftmins > 9) {
//       shiftmins = 0;
//       shiftminsten += 1;
//     }

//     if (shiftminsten > 5) {
//       shiftminsten = 0;
//     }

    
//     digitalWrite(latchPin, HIGH);
//     shiftOut(dataPin, clockPin, LSBFIRST, datArray[shiftminsten]);
    
//     shiftOut(dataPin, clockPin, LSBFIRST, datArray[shiftmins] | dp);
   
//     shiftOut(dataPin, clockPin, LSBFIRST, invdatArray[shiftsecsten] | dp);
   
//     shiftOut(dataPin, clockPin, LSBFIRST, datArray[shiftsecs]);
//     digitalWrite(latchPin, LOW);
  
// }
// }
