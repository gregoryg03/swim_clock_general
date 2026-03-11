#include "functions.h"
#include "events.h"

//TPIC (ShiftRegister)  Pins
//RCK pin 12 (low is no output)
static int latchPin; //6 beetle

//SRCK pin 13
static int clockPin; //20 beetle

//SER_IN pin 3
static int dataPin; //19 beetle

//digit array 0-9 without DP
const int datArray[10] = {0b11111100, 0b01100000, 0b11011010, 0b11110010, 0b01100110, 0b10110110, 0b10111110, 0b11100000, 0b11111110, 0b11100110};

//updside down digit array 0-9 without DP
const int invdatArray[10] = {0b11111100, 0b00001100, 0b11011010, 0b10011110, 0b00101110, 0b10110110, 0b11110110, 0b00011100, 0b11111110, 0b00111110};

//Random display items       d            n           u         p            g
const int symArray[5] = {0b01111010, 0b00101010, 0b00111000, 0b11001110, 0b11110110};
const int symArrayinv[5] = {0b11001110, 0b00101010, 0b11000100, 0b01111010, 0b11110110};

//Testing intervals to cycle through (sec)
const int timearr[10] = {20, 5, 20, 5, 30, 10, 60, 10, 20, 3000};
int intevaltime = 0;

//decimal Pt
byte dp = 0b00000000;

//Time
unsigned long tcount;
int shiftminsten = 0, shiftmins = 0, shiftsecsten = 0, shiftsecs = 0;
int secs, secsten, mins, minsten;

bool counting = false;
bool flag = false;
static int curInt, status;

void displayinit(uint8_t LATCH, uint8_t CLOCK, uint8_t DATA)
{
  // Arduino TPIC pins initilization to 0 with dp
  latchPin = LATCH;
  clockPin = CLOCK;
  dataPin = DATA;

  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  
  //Set Diplay Blank
  digitalWrite(latchPin, LOW);
  for (int i = 0; i < segCount; i++) {
    shiftOut(dataPin, clockPin, LSBFIRST, BLANK);
  }
  digitalWrite(latchPin, HIGH);

  delay(1000);
  
  digitalWrite(latchPin, LOW);
  for (int i = 0; i < segCount; i++) { 
    if (i%3 != 0 && i != 0)
      dp = 1;
    shiftOut(dataPin, clockPin, LSBFIRST, (datArray[0] | dp));
    dp = 0;
  }
  digitalWrite(latchPin, HIGH);
  delay(1000);
}


void getInterval()
{
  if (!counting) {
    static int i = 0;
    curInt = timearr[i++];
    status = curInt;
    int minutes = curInt / 60, seconds = curInt % 60;

    shiftminsten = minutes / 10;
    shiftmins = minutes % 10;

    shiftsecsten = seconds / 10;
    shiftsecs = seconds % 10;

    counting = true;
    flag = false;
  }
}
void countDown()
{
  static const byte dp = 1;

    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, datArray[shiftminsten]);
    
    shiftOut(dataPin, clockPin, LSBFIRST, datArray[shiftmins] | dp);
  
    shiftOut(dataPin, clockPin, LSBFIRST, invdatArray[shiftsecsten] | dp);
  
    shiftOut(dataPin, clockPin, LSBFIRST, datArray[shiftsecs]);
    digitalWrite(latchPin, HIGH);

    if (--status == 0) {
      flag = true;
    }

    if (flag){
    counting = false;
    return;
    }

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

void countUp()
{
    shiftsecs += 1;

    if (shiftsecs > 9) {
      shiftsecs = 0;
      shiftsecsten += 1;
    }

    if (shiftsecsten > 5) {
      shiftsecsten = 0;
      shiftmins += 1;
    }

    if (shiftmins > 9) {
      shiftmins = 0;
      shiftminsten += 1;
    }

    if (shiftminsten > 5) {
      shiftminsten = 0;
    }

    
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, datArray[shiftminsten]);
    
    shiftOut(dataPin, clockPin, LSBFIRST, datArray[shiftmins] | dp);
   
    shiftOut(dataPin, clockPin, LSBFIRST, invdatArray[shiftsecsten] | dp);
   
    shiftOut(dataPin, clockPin, LSBFIRST, datArray[shiftsecs]);
    digitalWrite(latchPin, HIGH);
}

void disp(mode setIcon)
{
  switch (setIcon) {
    case mode::countDown:
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, LSBFIRST, 0b00000000);
      
      shiftOut(dataPin, clockPin, LSBFIRST, 0b00000000);
    
      shiftOut(dataPin, clockPin, LSBFIRST, symArrayinv[0]);
    
      shiftOut(dataPin, clockPin, LSBFIRST, symArray[1]);
      digitalWrite(latchPin, HIGH);
    
      break;
    case mode::countUp:
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, LSBFIRST, 0b00000000);
      
      shiftOut(dataPin, clockPin, LSBFIRST, 0b00000000);
    
      shiftOut(dataPin, clockPin, LSBFIRST, symArrayinv[2]);
    
      shiftOut(dataPin, clockPin, LSBFIRST, symArray[3]);
      digitalWrite(latchPin, HIGH);

      break;
    case mode::dataEntry:
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, LSBFIRST, 0b00000000);
      
      shiftOut(dataPin, clockPin, LSBFIRST, 0b00000000);
    
      shiftOut(dataPin, clockPin, LSBFIRST, symArrayinv[3]);
    
      shiftOut(dataPin, clockPin, LSBFIRST, symArray[4]);
      digitalWrite(latchPin, HIGH);

      break;
  }
}