//First Iteration of Swim Clock Code to test on bench with Arduino Uno

#include <functions.h>
#include <button.h>

//System States
bool pauseState = false;
bool pauseFalling = false;
bool btnEdgeDetect = false;

//Btn States
static byte btn_states = 0xFF;

//Display Pins
const uint8_t LATCH_PIN = 10;
const uint8_t DISP_CLOCK_PIN = 11;
const uint8_t DISP_DATA_PIN = 12;

//Button Pins
const uint8_t DATA_PIN = 4;
const uint8_t CLOCK_PIN = 5;
const uint8_t MODE_PIN = 6;

void setup() 
{
  Serial.begin(9600);
  displayinit(LATCH_PIN, DISP_CLOCK_PIN, DISP_DATA_PIN);
  Serial.println("DISP INIT");
  //Setup the Buttons
  buttons_init(DATA_PIN, CLOCK_PIN, MODE_PIN);
  Serial.println("BTN INIT");
}

void loop() {
  // put your main code here, to run repeatedly:

  static unsigned long dispMillis, btnMillis;
  static unsigned long currentMillis;
  static unsigned int remTime;

  currentMillis = millis();

  btn_states = read_button(false);

  // if (currentMillis - btnMillis > 200) {
  //   btnMillis += 200;
  //   Serial.println(btn_states, BIN);
  // }
  
  //Edge detect for pause button
  bool pauseBtn = (btn_states >> 7) & 1;
  if (pauseBtn && !btnEdgeDetect) {
      Serial.println("Flip");
      pauseState ^= 1;
      pauseFalling = true;
      if (pauseState == 1) {
        remTime = currentMillis - dispMillis;
      }
  }

  btnEdgeDetect = pauseBtn;


  if (!pauseState) {
    getInterval();
    if (pauseFalling) {
      dispMillis = currentMillis - remTime;
      pauseFalling = false;
    }
    if (currentMillis - dispMillis >= 1000) {
      dispMillis += 1000;
      countDown();
      Serial.println("Done Display");
    }
  }
  // if (currentMillis - dispMillis >= 1000) {
  //   dispMillis += 1000;
  // countUp();
  // } 
}







