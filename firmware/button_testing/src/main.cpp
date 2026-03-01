#include <Arduino.h>

#define BTN_COUNT 8
#define DEBOUNCE 20 //time in ms

//74HC165 Shirft Register
//Data Out Inverse
const int data = 4;
//Clock
const int clock = 5;
//Shift/Load High = Shift data & Low = Load Data
const int mode = 6;
//timekeeping
unsigned long previous_millis, current_millis;
//Debounce
static byte btn_states;
static byte last_read = 0b11111111;
static byte last_state = 0b11111111;
static unsigned long lastDebounce[BTN_COUNT] = {0};

// put function declarations here:
void fill_reg(void);
byte read_button(byte, bool);
byte update_state(byte);


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
  

  current_millis = millis();

  // if (current_millis - previous_millis > 200) {
  //   previous_millis += 200;
  //   fill_reg();
  //   btn_states = read_button(btn_states, false);
  //   Serial.println(btn_states, BIN);
  // }

  fill_reg();
  btn_states = read_button(btn_states, false);
  if (current_millis - previous_millis > 200) {
    previous_millis += 200;
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

//Can choose order based on bool, default is false
byte read_button(byte states, bool order)
{
  byte value = 0b00000000;
  switch (order) {
    //MSBF
    case true: 
      for (int i = 0; i < BTN_COUNT; i++) {
        value |= (digitalRead(data) << i);
        digitalWrite(clock, HIGH);
        digitalWrite(clock, LOW);
      }
      break;
    //LSBF
    default:
      for (int i = 0; i < BTN_COUNT; i++) {
        value |= (digitalRead(data) << (7-i));
        digitalWrite(clock, HIGH);
        digitalWrite(clock, LOW);
      }
      break;
  }

  return states = update_state(value);
}

byte update_state(byte value)
{
  byte mask = 0b00000001;
  // static byte last_read = 0b11111111;
  // static byte last_state = 0b11111111;
  // static unsigned long lastDebounce[BTN_COUNT] = {0};
  if (1) {
  //if (value != last_state) {  
    
    for (int i = 0; i < BTN_COUNT; i++) {
      if (((value >> i) & mask) != ((last_read >> i) & mask)) {
        lastDebounce[i] = millis();
        last_read = (last_read & ~(1 << i)) | (((value >> i) & 1) << i);
      }
    }

    for (int i = 0; i < BTN_COUNT; i++) {
      if ((millis() - lastDebounce[i]) > DEBOUNCE) {
        bool currentBit = (value >> i) & 1;
        last_state = (last_state & ~(1 << i)) | (currentBit << i);
      }
    }
}

return last_state;
}



