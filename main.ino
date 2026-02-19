//Define Connections to 74HC595

//ST_CP pin 12 (low is no output)
const int latchPin = 10; //6 beetle
//SH_CP pin 11
const int clockPin = 11; //20 beetle
//DS pin 14
const int dataPin = 12; //19 beetle

int datArray[10] = {0b11111100, 0b01100000, 0b11011010, 0b11110010, 0b01100110, 0b10110110, 0b10111110, 0b11100000, 0b11111110, 0b11100110};


void setup() {
  // put your setup code here, to run once:
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void loop() {
  // Count from 0 to 255 and display in binary
//digitalWrite(latchPin, HIGH);
//shiftOut(dataPin, clockPin, MSBFIRST, datArray[8] );
//digitalWrite(latchPin, LOW);
//
  int i = 0;  
  int dp = 0;
//  for (i;i<10;i++) {
//    digitalWrite(latchPin, HIGH);
//    shiftOut(dataPin, clockPin, LSBFIRST, datArray[i]+dp);
//    digitalWrite(latchPin, LOW);
//    delay(1000);
//   // dp ^= 1;
//  }
  int staticTest = 0b11111100111111001111110011111100;
    digitalWrite(latchPin, HIGH);
    shiftOut(dataPin, clockPin, LSBFIRST, staticTest);
    digitalWrite(latchPin, LOW);
    delay(1000);


//  digitalWrite(latchPin, HIGH);
//  shiftOut(dataPin, clockPin, LSBFIRST, datArray[1]);
//  digitalWrite(latchPin, LOW);

//  for (int numberToDisplay = 0; numberToDisplay < 256; numberToDisplay++) {
//    digitalWrite(latchPin, LOW);
//
//    shiftOut(dataPin, clockPin, MSBFIRST, numberToDisplay);
//
//    digitalWrite(latchPin, HIGH);
//
//    delay(500);
//  }
}
