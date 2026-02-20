//First Iteration of Swim Clock Code to test on bench with Arduino Uno

#define segCount 4
#define BLANK 0b00000000

//TPIC (ShiftRegister)  Pins
//RCK pin 12 (low is no output)
const int latchPin = 10; //6 beetle

//SRCK pin 11
const int clockPin = 11; //20 beetle

//SER_IN pin 3
const int dataPin = 12; //19 beetle

//digit array 0-9 without DP
const int datArray[10] = {0b11111100, 0b01100000, 0b11011010, 0b11110010, 0b01100110, 0b10110110, 0b10111110, 0b11100000, 0b11111110, 0b11100110};

//updside down digit array 0-9 without DP
const int invdatArray[10] = {0b11111100, 0b00001100, 0b11011010, 0b10011110, 0b00101110, 0b10110110, 0b11110110, 0b00011100, 0b11111110, 0b00111110};

//Millis setup
unsigned long prevMillis;
unsigned long currentMillis;

//decimal Pt
byte dp = 0b00000000;

//Time
unsigned long tcount;
int shiftminsten = 0, shiftmins = 0, shiftsecsten = 0, shiftsecs = 0;
int secs, secsten, mins, minsten;

void setup() {
  // Arduino TPIC pins initilization to 0 with dp
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  
  //Set Diplay Blank
  for (int i = 0; i < segCount; i++) {
    digitalWrite(latchPin, HIGH);
    shiftOut(dataPin, clockPin, LSBFIRST, BLANK);
  }
  digitalWrite(latchPin, LOW)
  
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
  currentMillis = millis(); //Check what prevmillis is when setLED is called. Somehow starting at 4?

  setLED();
}

void setLED()
{
  if (currentMillis - prevMillis >= 1000) {
    prevMillis += 1000;
   
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

    
    digitalWrite(latchPin, HIGH);
    shiftOut(dataPin, clockPin, LSBFIRST, datArray[shiftminsten]);
    
    shiftOut(dataPin, clockPin, LSBFIRST, datArray[shiftmins] | dp);
   
    shiftOut(dataPin, clockPin, LSBFIRST, invdatArray[shiftsecsten] | dp);
   
    shiftOut(dataPin, clockPin, LSBFIRST, datArray[shiftsecs]);
    digitalWrite(latchPin, LOW);
  
}
}