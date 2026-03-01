#include <Arduino.h>

//74HC165 Shirft Register
//Data Out Inverse
const int data = 4;
//Clock
const int clock = 5;
//Shift/Load High = Shift data & Low = Load Data
const int mode = 6;
//timekeeping
unsigned long previous_millis, current_millis;

// put function declarations here:
void fill_reg(void);
byte read_button(bool);


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
  static byte btn_states;

  current_millis = millis();

  if (previous_millis - current_millis > 200) {
    previous_millis += 200;
    fill_reg();
    btn_states = read_button(true);
    Serial.println(btn_states, BIN);
  }
}

void fill_reg(void)
{
  digitalWrite(mode, LOW);
  delayMicroseconds(5);
  digitalWrite(mode, HIGH);
  delayMicroseconds(5);
}

byte read_button(bool order)
{
  byte value = 0b00000000;
  switch (order) {
    //LSBF
    case true: 
      for (int i = 0; i < 8; i++) {
        value |= (digitalRead(data) << i);
        digitalWrite(clock, HIGH);
        digitalWrite(clock, LOW);
      }
    break;
    
    //MSBF
    default:
      for (int i = 0; i < 8; i++) {
        value |= (digitalRead(data) << (7-i));
        digitalWrite(clock, HIGH);
        digitalWrite(clock, LOW);
      }
    break;
  }

  return value;
}



