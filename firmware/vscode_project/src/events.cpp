#include "events.h"

static unsigned long currentMillis;
static unsigned long nextInterval;
static unsigned long timeRemaining = 1000; //(Starts paused, so this will set initial time)
bool pausedFlag = false;
mode last_mode;

modeState modeTable[] = {
    {enter_countUp, run_countUp, exit_countUp},
    {enter_countDown, run_countDown, exit_countDown},
    {enter_dataEntry, run_dataEntry, exit_dataEntry}
};

mode handle_events(event btnPressed)
{

  currentMillis = millis();
  if (btnPressed != event::none) {
    set_action(btnPressed);
  }
  
  if (action == actions::running)
    modeTable[(int)m].run();
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
      else if (action == actions::paused) {
        action = actions::running;
      }
      break;
    case event::btn2press:
      if (action == actions::paused) {
        last_mode = m;
        next_mode(m);
        if (m != last_mode) {
            change_state(last_mode);
        }
        Serial.println((int)m);
      }
      break;
    default:
      break;
  }
}


void remTime(void)
{
  timeRemaining = nextInterval - currentMillis;
  pausedFlag = true;
}

void next_mode(mode modein)
{
  m = static_cast<mode>(((static_cast<int>(m) + 1) %
    static_cast<int>(mode::MAX_MODES)));
  disp(m);
}

void change_state(mode prevMode)
{
  modeTable[(int)prevMode].exit();
  last_mode = m;
  modeTable[(int)m].enter();
}

void enter_countUp()
{
  reset_disp();
}
void run_countUp()
{
  if (pausedFlag) {
  nextInterval = currentMillis + timeRemaining; //Will this cause the time to go faster?
  pausedFlag = false;
  }

  if (currentMillis > nextInterval) {
  nextInterval += 1000;
  countUp();
  }
}
void exit_countUp()
{
  ;
}

void enter_countDown()
{
  reset_disp();
}
void run_countDown()
{
  getInterval();

  if (pausedFlag) {
  nextInterval = currentMillis + timeRemaining; //Will this cause the time to go faster?
  pausedFlag = false;
  }

  if (currentMillis > nextInterval) {
    nextInterval += 1000;
    countDown();
  }
}
void exit_countDown()
{
  ;
}
void enter_dataEntry()
{
  ;
}
void run_dataEntry()
{
  ;
}
void exit_dataEntry()
{
  ;
}
