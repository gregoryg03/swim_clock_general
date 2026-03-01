#include <Arduino.h>

//74HC165 Shirft Register

//Clock Inhibit
const int clk_inhib = 3;

//Data Out Inverse
const int data = 4;

//Clock
const int clock = 5;

//Shift/Load High = Shift data & Low = Load Data
const int mode = 6;


// put function declarations here:
byte read_button(byte);


void setup() {
  // put your setup code here, to run once:
  //pinMode(clk_inhib, OUTPUT);
  //digitalWrite(clk_inhib, HIGH);

  pinMode(data, INPUT);

  pinMode(clock, OUTPUT);
  digitalWrite(clock, LOW);

  pinMode(mode, OUTPUT);
  digitalWrite(mode, LOW);

  Serial.begin(9600);
  Serial.println("Setup Compete");
}

void loop() {
  // put your main code here, to run repeatedly:
  
  byte btn_states;
 //byte mask = 0b01111100;

  digitalWrite(mode, LOW);
  delayMicroseconds(5);
  digitalWrite(mode, HIGH);
  delayMicroseconds(5);

  //digitalWrite(clk_inhib, LOW);

  //btn_states = /* mask & */ shiftIn(data, clock, LSBFIRST);

  btn_states = read_button(btn_states);

  //digitalWrite(clk_inhib, HIGH);

  Serial.println(btn_states, BIN);
  delay(200);
  
}

byte read_button(byte value)
{

  for (int i = 0; i < 8; i++) {
    value <<= 1;
    value |= digitalRead(data);
    digitalWrite(clock, HIGH);
    digitalWrite(clock, LOW);
  }

  return value;
}



