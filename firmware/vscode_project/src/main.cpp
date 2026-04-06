//First Iteration of Swim Clock Code to test on bench with Arduino Uno

#include "events.h"
#include "functions.h"
#include "button.h"
#include "sd2.h"


//Display Pins
const uint8_t LATCH_PIN = 8;
const uint8_t DISP_CLOCK_PIN = 9;
const uint8_t DISP_DATA_PIN = 10;

//Button Pins
const uint8_t DATA_PIN = 4;
const uint8_t CLOCK_PIN = 5;
const uint8_t MODE_PIN = 6;

//SD Pins
const uint8_t CS_PIN = 2;
SD_CARD sdInit;

//Initilize Globals
actions action = actions::paused; //Was running
mode m = mode::countUp;

void setup() 
{
  Serial.begin(9600);
  displayinit(LATCH_PIN, DISP_CLOCK_PIN, DISP_DATA_PIN);
  Serial.println("DISP INIT");
  //Setup the Buttons
  buttons_init(DATA_PIN, CLOCK_PIN, MODE_PIN);
  Serial.println("BTN INIT");
  
  sdInit.sd_init(CS_PIN);
  Serial.println("SD_INIT");

  init_events();
}

void loop() {
  // put your main code here, to run repeatedly:
  buttons btn;
  static event buttonPress;
  static mode currentMode;

  //Read the buttons
  buttonPress = btn.poll();

  //Perform action based on buttons and clock
  currentMode = handle_events(buttonPress);
}


