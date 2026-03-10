#include "events.h"

static unsigned long currentMillis;
static unsigned long nextInterval;
static unsigned long timeRemaining = 1000; //(Starts paused, so this will set initial time)
bool pausedFlag = true;

mode handle_events(event btnPressed)
{
  currentMillis = millis();
  set_action(btnPressed);
  perform_action();
  return m;
}

void set_action (event btnPressed)
{
  switch (btnPressed) {
    case event::btn1press:
      if (action == actions::running) {
        action = actions::paused;
        remTime();
      }
      else if (action == actions::paused)
        action = actions::running;
      break;
    default:
      break;
  }
}

//Add the mode swiching functionallity, will need to figure out long press (might have to change edge detect or add another condition)

void perform_action(void)
{
  switch (action) {
    case actions::running:
      getInterval();
      if (pausedFlag) {
        nextInterval = currentMillis + timeRemaining; //Will this cause the time to go faster?
        pausedFlag = false;
      }
      if (currentMillis > nextInterval) {
        nextInterval += 1000;
        if (m == mode::countDown) 
          countDown();
        else if (m == mode::countUp)
          countUp();
      }
    default:
      break;
  }
}

void remTime(void)
{
  timeRemaining = nextInterval - currentMillis;
  pausedFlag = true;
}


//  Serial.println(btn_states, BIN);
//  Serial.println("Done Display");