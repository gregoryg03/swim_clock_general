#include "events.h"



static unsigned long dispMillis, btnMillis;
static unsigned long currentMillis;
static unsigned int remTime;

  currentMillis = millis();

  btn_states = read_button(false);

  // if (currentMillis - btnMillis > 200) {
  //   btnMillis += 200;
  //   Serial.println(btn_states, BIN);
  // }
  


//Move this to the events file when created
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