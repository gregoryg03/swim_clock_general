//First Iteration of Swim Clock Code to test on bench with Arduino Uno

#include "events.h"
#include "functions.h"
#include "button.h"


//Display Pins
const uint8_t LATCH_PIN = 10;
const uint8_t DISP_CLOCK_PIN = 11;
const uint8_t DISP_DATA_PIN = 12;

//Button Pins
const uint8_t DATA_PIN = 4;
const uint8_t CLOCK_PIN = 5;
const uint8_t MODE_PIN = 6;

//Initilize Globals
actions action = actions::paused;
mode m = mode::idle;

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
  buttons btn;
  static event e;
  e = btn.poll();

  switch (e) {
    case event::btn1press:
      Serial.println("Btn1");
      break;
    case event::btn2press:
      Serial.println("Btn2");
      break;  
    case event::btn3press:
      Serial.println("Btn3");
      break;
    case event::btn4press:
      Serial.println("Btn4");
      break;
    case event::btn5press:
      Serial.println("Btn5");
      break;
    case event::none:
      Serial.println("None");
      break;
    default:
      Serial.println("Dafa");
      break;
  }

  if (e != event::none) {
    m = handle_events(e);
  }
  
  //Serial.println("Main loop");
  // switch (m) {
  //   case mode::idle:
  //     break;
    
  //   case mode::countUp:
  // }

}







