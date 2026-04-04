#include "events.h"

static unsigned long currentMillis;
static unsigned long nextInterval;
static unsigned long blinkInterval;
static unsigned long timeRemaining = 1000; //(Starts paused, so this will set initial time)
bool pausedFlag = false;
mode last_mode, prg_mode;
event dataEntBtn = event::none;

static sdData sdItems = {WRITE_MODE, 0, 0, true};
SD_CARD sdEventsInstance;

modeState modeTable[] = {
    {enter_countUp, run_countUp, exit_countUp},
    {enter_countDown, run_countDown, exit_countDown},
    {enter_dataEntry, run_dataEntry, exit_dataEntry},
    {enter_shutdown, run_shutdown, exit_shutdown}
};

mode handle_events(event btnPressed)
{

  currentMillis = millis();
  if (btnPressed != event::none) {
    set_action(btnPressed);
  }
  
  // if (action == actions::running)
  //   modeTable[(int)m].run();

  modeTable[(int)m].run(); //test
  return m;
}

void set_action (event btnPressed)
{
  switch (btnPressed) {
    case event::btn1press:
      if (action == actions::running) {
        action = actions::paused;
        remTime();
        prg_mode = m;

      }
      else if (action == actions::paused) {
        if (m != last_mode) {
            m = prg_mode;
            change_state(last_mode);
        }
        
        action = actions::running;
      }
      break;
    case event::btn2press:
      if (action == actions::paused) {
        last_mode = prg_mode;
        next_mode(prg_mode);
        Serial.println((int)prg_mode);
      }
      break;
    default:
      dataEntBtn = btnPressed;
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
  prg_mode = static_cast<mode>(((static_cast<int>(prg_mode) + 1) %
    static_cast<int>(mode::MAX_MODES)));
  disp(prg_mode);
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
  if (actions::running == action) {
    if (pausedFlag) {
    nextInterval = currentMillis + timeRemaining; //Will this cause the time to go faster?
    pausedFlag = false;
    }

    if (currentMillis - nextInterval >= 1000) {
    nextInterval += 1000;
    countUp();
    }
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
  if (action == actions::running) {
    getInterval();

    if (pausedFlag) {
      nextInterval = currentMillis + timeRemaining; //Will this cause the time to go faster?
      pausedFlag = false;
    }

    if (currentMillis - nextInterval >= 1000) {
      nextInterval += 1000;
      countDown();
    }
  }
}
void exit_countDown()
{
  ;
}
void enter_dataEntry()
{
  reset_disp();
  //sd_clear();
  sdEventsInstance.call(SD_OPEN, sdItems);
  nextInterval = currentMillis;
  blinkInterval = currentMillis;

}

void run_dataEntry()
{
  static uint8_t total[4] = {0};
  static uint8_t digitSelected = 0;
  static bool advanceFlag = false;
  static uint16_t numSec = 0;
  static dispStruct dataEntryDisp = {};
  
  sdItems.mode = WRITE_MODE;

  numSec = sd_data_in_format(total);


  if (actions::paused == action) {
    for (int i = 0; i < segCount; i++) {
      dataEntryDisp.blinkMask[i] = false;
    }
  }
  else {
    dataEntryDisp.blinkMask[digitSelected] = true;
  }

  if ((currentMillis - nextInterval >= 100) || advanceFlag) {
    nextInterval += 100;

    if ((currentMillis - blinkInterval >= BLINK_RATE)) {
      blinkInterval += BLINK_RATE;
      dataEntryDisp.blinkState = !dataEntryDisp.blinkState;
    }

    data_entry_disp(dataEntryDisp, numSec);

    if (advanceFlag) {
      nextInterval = currentMillis;

      sdItems.intervalIn = numSec;
      sdEventsInstance.call(SD_WRITE, sdItems);
      //Serial.println(numSec);

      for (int i = 0; i < 4; i++) {
        total[i] = 0;
      }

      digitSelected = 0;
      advanceFlag = false;
    }
  }

  switch (dataEntBtn) {
    case event::btn3press:
      Serial.println("Btn 3 prg");
      switch (digitSelected) {
        case 0:
          total[0]++;
          total[0] %= 10;
          break;
        case 1:
          total[1]++;
          total[1] %= 6;
          break;
        case 2:
          total[2]++;
          total[2] %= 10;
          break;
        case 3:
          total[3]++;
          total[3] %= 7;
          break;
        default:
          Serial.println("Prg outside digit bounds");
          break;
      }
      break;
    case event::btn4press:
      digitSelected += 1;
      digitSelected %= 4;
      for (int i = 0; i < segCount; i++) {
        dataEntryDisp.blinkMask[i] = false;
      }
      break;
    case event::btn5press:
      advanceFlag = true;
      break;
    default:
      break;
  }
  
  dataEntBtn = event::none;
}

void exit_dataEntry()
{
  sdEventsInstance.call(SD_CLOSE, sdItems);
}

void enter_shutdown()
{
  ;
}

void run_shutdown()
{
  Serial.println("Close SD");

  sdEventsInstance.sd_shutdown();
  delay(1000000);
}

void exit_shutdown()
{
  ;
}

uint16_t sd_data_in_format(uint8_t digits[])
{
  uint16_t funcOut;
  funcOut = static_cast<uint16_t>(digits[0] + digits[1] * 10 + digits[2] * 60 + digits[3] * 600); 

  //Serial.println(funcOut);

  return funcOut;
}