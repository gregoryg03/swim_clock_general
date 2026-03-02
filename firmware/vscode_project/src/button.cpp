#include <button.h>

//74HC165 Shift Register
//Data Out Inverse
static int DATA_PIN;
//Clock
static int CLOCK_PIN;
//Shift/Load High = Shift data & Low = Load Data
static int MODE_PIN;

//Debounce
static byte btn_states;
static byte last_read = 0xFF;
static byte last_state = 0xFF;
static unsigned long lastDebounce[BTN_COUNT] = {0};


//Initilize buttons
void buttons_init(uint8_t data, uint8_t clock, uint8_t mode)
{
    DATA_PIN = data;
    CLOCK_PIN = clock;
    MODE_PIN = mode;

    pinMode(DATA_PIN, INPUT);
    pinMode(CLOCK_PIN, OUTPUT);
    pinMode(MODE_PIN, OUTPUT);

    digitalWrite(CLOCK_PIN, LOW);
    digitalWrite(MODE_PIN, LOW);
}

void fill_reg(void)
{
  digitalWrite(MODE_PIN, LOW);
  delayMicroseconds(5);
  digitalWrite(MODE_PIN, HIGH);
  delayMicroseconds(5);
}

//Can choose order based on bool, default is false
byte read_button(bool order)
{
  fill_reg();
  byte value = 0;
  switch (order) {
    //MSBF
    case true: 
      for (int i = 0; i < BTN_COUNT; i++) {
        value |= (digitalRead(DATA_PIN) << i);
        digitalWrite(CLOCK_PIN, HIGH);
        digitalWrite(CLOCK_PIN, LOW);
      }
      break;
    //LSBF
    default:
      for (int i = 0; i < BTN_COUNT; i++) {
        value |= (digitalRead(DATA_PIN) << (7-i));
        digitalWrite(CLOCK_PIN, HIGH);
        digitalWrite(CLOCK_PIN, LOW);
      }
      break;
  }

  return update_state(value);
}

byte update_state(byte value)
{
  byte mask = 1;
  unsigned long curtime = millis();
  
  for (int i = 0; i < BTN_COUNT; i++) {
    if (((value >> i) & mask) != ((last_read >> i) & mask)) {
      lastDebounce[i] = curtime;
      last_read = (last_read & ~(1 << i)) | (((value >> i) & 1) << i);
    }
  }

  for (int i = 0; i < BTN_COUNT; i++) {
    if ((curtime - lastDebounce[i]) > DEBOUNCE) {
      bool currentBit = (value >> i) & 1;
      last_state = (last_state & ~(1 << i)) | (currentBit << i);
    }
  }

return last_state;
}