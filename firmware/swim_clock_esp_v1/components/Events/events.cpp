#include "events.h"

static const char *TAG = "events_debug";

static uint64_t currentMillis;
static uint64_t nextInterval;
static uint64_t blinkInterval;
static uint64_t timeRemaining = 1000; //(Starts paused, so this will set initial time)

bool pausedFlag = false, startFlag = false;
Mode last_mode, prg_mode, m;
Actions action;
modeState modeTable[];
Event dataEntBtn = Event::none;

modeState modeTable[] = {
    {enter_countUp, run_countUp, exit_countUp},
    {enter_countDown, run_countDown, exit_countDown},
    {enter_dataEntry, run_dataEntry, exit_dataEntry},
    {enter_shutdown, run_shutdown, exit_shutdown}
};

//set the starting flag to denote startup settings
void init_events(void)
{
  startFlag = true;
}

//handle_events runs every loop in main. It handles the button presses and runs the selected mode. 
//Calls set_action if button pressed.
Mode handle_events(Event btnPressed)
{
  currentMillis = esp_timer_get_time();
  if (btnPressed != Event::none) {
    set_action(btnPressed);
  }
  modeTable[(int)m].run();

  return m;
}

//set_action takes button press from handle_events and performs the relavent action.
void set_action (Event btnPressed)
{
  switch (btnPressed) {
    case Event::btn1press:
      if (action == Actions::running) {
        action = Actions::paused;
        remTime();
        prg_mode = m;
      }
      else if ((action == Actions::paused) || (action == Actions::cycle)) {
        if (m != last_mode) {
            m = prg_mode;
            change_state(last_mode);
        }
        action = Actions::running;
      }
      break;

    case Event::btn2press:
      if (action == Actions::cycle) {
        next_mode(prg_mode);
        ESP_LOGI(TAG, "%d", (int)prg_mode);
      }
      if ((action == Actions::paused) || (action == Actions::cycle)) {
        action = Actions::cycle; //This action allows for changing modes
        last_mode = prg_mode;
        //disp(prg_mode);
        ESP_LOGI(TAG, "Paused 2 cycle");
        ESP_LOGI(TAG, "%d", (int)prg_mode);
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

void next_mode(Mode modein)
{
  prg_mode = static_cast<Mode>(((static_cast<int>(prg_mode) + 1) %
    static_cast<int>(Mode::MAX_MODES)));
  disp(prg_mode);
}

void change_state(Mode prevMode)
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
  if (Actions::running == action) {
    if (pausedFlag) {
    nextInterval = currentMillis + timeRemaining; //Will this cause the time to go faster?
    pausedFlag = false;
    }

    if (startFlag) {
      nextInterval = currentMillis;
      startFlag = false;
    }

    if (currentMillis - nextInterval >= 1000) {
    nextInterval += 1000;
    //countUp();
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
  //sdEventsInstance.call(SD_OPEN, sdItems);
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
  
 // sdItems.mode = WRITE_MODE;

  //numSec = sd_data_in_format(total);

   if (action == Actions::cycle) {
    return;
   }

  if (action == Actions::paused) {
    for (int i = 0; i < 4; i++) {
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

    // data_entry_disp(dataEntryDisp, numSec);

    if (advanceFlag) {
      nextInterval = currentMillis;

    //   sdItems.intervalIn = numSec;
    //   sdEventsInstance.call(SD_WRITE, sdItems);
      //Serial.println(numSec);

      for (int i = 0; i < 4; i++) {
        total[i] = 0;
      }

      digitSelected = 0;
      advanceFlag = false;
    }
  }

  switch (dataEntBtn) {
    case Event::btn3press:
      ESP_LOGI(TAG, "Btn 3 prg");
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
          ESP_LOGI(TAG, "Prg outside digit bounds");
          break;
      }
      break;
    case Event::btn4press:
      digitSelected += 1;
      digitSelected %= 4;
      for (int i = 0; i < 4; i++) {
        dataEntryDisp.blinkMask[i] = false;
      }
      break;
    case Event::btn5press:
      advanceFlag = true;
      for (int i = 0; i < 4; i++) {
        dataEntryDisp.blinkMask[i] = false;
      }
      digitSelected = 0;
      break;
    default:
      break;
  }
  
  dataEntBtn = Event::none;
}

void exit_dataEntry()
{
  //sdEventsInstance.call(SD_CLOSE, sdItems);
}

void enter_shutdown()
{
  ;
}

void run_shutdown()
{
  ESP_LOGI(TAG, "Shutdown");

 // sdEventsInstance.sd_shutdown();
  
}

void exit_shutdown()
{
  ;
}

// uint16_t sd_data_in_format(uint8_t digits[])
// {
//   uint16_t funcOut;
//   funcOut = static_cast<uint16_t>(digits[0] + digits[1] * 10 + digits[2] * 60 + digits[3] * 600); 

//   //Serial.println(funcOut);

//   return funcOut;
// }