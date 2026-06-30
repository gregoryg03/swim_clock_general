#include "events.h"
#include "display.h"

static const char *TAG = "events_debug";

static uint64_t currentMillis;
static uint64_t nextInterval;
static uint64_t blinkInterval;
static uint64_t timeRemaining = 1000; //(Starts paused, so this will set initial time)

bool pausedFlag = false, startFlag = false;
Mode selected, current;
Actions action;

Event dataEntBtn = Event::none;

static dispStruct downDisp{},
                  upDisp{},
                  pgDisp{},
                  modeDisp{};

static int status;

static uint8_t sd_data;

modeState modeTable[] = {
    {enter_countUp, run_countUp, exit_countUp},
    {enter_countDown, run_countDown, exit_countDown},
    {enter_dataEntry, run_dataEntry, exit_dataEntry},
    {enter_shutdown, run_shutdown, exit_shutdown}
};

//test array
const int timearr[10] = {20, 5, 20, 5, 30, 10, 60, 10, 20, 3000};


//set the starting flag to denote startup settings
void init_events(void)
{
  downDisp.count_flag = true;
  downDisp.dp_t[1] = true;
  downDisp.dp_t[2] = true;

  upDisp.dp_t[1] = true;
  upDisp.dp_t[2] = true;

  pgDisp.dp_t[1] = true;
  pgDisp.dp_t[2] = true;

  startFlag = true;

  current = Mode::countUp;
  selected = Mode::countUp;
  action = Actions::paused;
}

//handle_events runs every loop in main. It handles the button presses and runs the selected mode. 
//Calls set_action if button pressed.
Mode handle_events(Event btnPressed)
{
  currentMillis = esp_timer_get_time();
  if (btnPressed != Event::none) {
    set_action(btnPressed);
    ESP_LOGI(TAG, "handle_events BTN");
  }
  
  modeTable[(int)current].run();

  return current;
}

//set_action takes button press from handle_events and performs the relavent action.
void set_action (Event btnPressed)
{
  switch (btnPressed) {
    case Event::btn1press:
      ESP_LOGI(TAG, "Cur M: %d", static_cast<int>(current));
      ESP_LOGI(TAG, "Sel M: %d", static_cast<int>(selected));

      ESP_LOGI(TAG, "TRANSITION %d -> %d",
         (int)current,
         (int)selected);

      if (action == Actions::running) {
        action = Actions::paused;
        remTime();
        selected = current;
      }
      else if ((action == Actions::paused) || (action == Actions::cycle)) {
        if (selected != current) {
              change_state(current, selected);
              current = selected;
        }

        action = Actions::running;
        ESP_LOGI(TAG, "Running");
      }
      break;

    case Event::btn2press:
      if (action == Actions::cycle) {
        next_mode(selected);
        ESP_LOGI(TAG, "Cycle to %d", (int)selected);
        helper_disp_pg(selected);
      }
      else if (action == Actions::paused) {
        action = Actions::cycle; //This action allows for changing modes
        ESP_LOGI(TAG, "%d", static_cast<int>(selected));
        helper_disp_pg(selected);
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
  selected = static_cast<Mode>(((static_cast<int>(modein) + 1) %
    static_cast<int>(Mode::MAX_MODES)));
  
    ESP_LOGI(TAG, "NEW SELECTED: %d", (int)selected);
}

//This function changes the Mode for the clock. Set by a Btn 1 press
void change_state(Mode m_prev, Mode m_next)
{
  modeTable[(int)m_prev].exit();
  modeTable[(int)m_next].enter();
}

void enter_countUp()
{
  reset_disp();
}

void run_countUp()
{
  //ESP_LOGI(TAG, "Running CountUp");
  if (Actions::running == action) {
    if (pausedFlag) {
      nextInterval = currentMillis + timeRemaining; //Will this cause the time to go faster?
      pausedFlag = false;
    }

    if (startFlag) {
      nextInterval = currentMillis;
      startFlag = false;
    }

    if (currentMillis - nextInterval >= 1000000) {
    nextInterval += 1000000;
    ESP_LOGI(TAG, "Disp CountUp");
    countUp(&upDisp);
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
  //reset timer?
}
void run_countDown()
{
  if (action == Actions::running) {
    get_interval(&downDisp);

    if (pausedFlag) {
      nextInterval = currentMillis + timeRemaining; //Will this cause the time to go faster?
      pausedFlag = false;
    }

    if (currentMillis - nextInterval >= 1000000) {
      nextInterval += 1000000;
      countDown(&downDisp);
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
  nextInterval = currentMillis - 1000;
  blinkInterval = currentMillis;
  //ESP_LOGI(TAG, "Ent Data ent");


}

void run_dataEntry()
{
  static uint8_t total[4] = {0};
  static uint8_t digitSelected = 0;
  static bool advanceFlag = false;
  static uint16_t numSec = 0;
  uint8_t sd_lsb = 0, sd_msb = 0;
  
 // sdItems.mode = WRITE_MODE;

  // ESP_LOGI(TAG, "Data Ent %d", (int)action);

  numSec = sd_data_in_format(total);

   if (action == Actions::cycle) {
    return;
   }

  if (action == Actions::paused) {
    for (int i = 0; i < 4; i++) {
      pgDisp.blinkMask[i] = false;
    }
  }
  else {
    pgDisp.blinkMask[digitSelected] = true;
  }

  if ((currentMillis - nextInterval >= 100000) || advanceFlag) {
    nextInterval += 100000;

    if ((currentMillis - blinkInterval >= BLINK_RATE)) {
      blinkInterval += BLINK_RATE;
      pgDisp.blinkState = !pgDisp.blinkState;
    }

    data_entry_disp(numSec);

    if (advanceFlag) {
      nextInterval = currentMillis;

    //   sdItems.intervalIn = numSec;
    //   sdEventsInstance.call(SD_WRITE, sdItems);


        sd_lsb = numSec & 0xFF;

        sd_msb = (numSec >> 8) & 0xFF;
      

      sd_write_data(&sd_lsb);
      sd_write_data(&sd_msb);

      ESP_LOGI(TAG, "Sec written: %u", (unsigned int)numSec);

      //sd_write_data(&sd_data);

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
        pgDisp.blinkMask[i] = false;
      }
      break;
    case Event::btn5press:
      advanceFlag = true;
      for (int i = 0; i < 4; i++) {
        pgDisp.blinkMask[i] = false;
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
  reset_disp();
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

void get_interval(dispStruct *dispData_t)
{

    static uint16_t curInt = 0;
    static int test_arr_index = 0;



    if (!dispData_t->counting) {
        //curInt = timearr[test_arr_index++];

        uint16_t sd_read_test = 0;
        uint8_t sd_r_lsb = 0, sd_r_msb = 0;

        sd_read_data(&sd_r_msb);
        sd_read_data(&sd_r_lsb);

        
        curInt  = (((uint16_t)sd_r_msb << 8) | sd_r_lsb);

        ESP_LOGI(TAG, "disp_test_next_int: %u", (unsigned int)sd_read_test);
        

        

        status = curInt;
        int minutes = curInt / 60, seconds = curInt % 60;

        dispData_t->digitarr[3] = minutes / 10;
        dispData_t->digitarr[2] = minutes % 10;

        dispData_t->digitarr[1] = seconds / 10;
        dispData_t->digitarr[0] = seconds % 10;

        dispData_t->counting = true;
        dispData_t->count_flag = false;
    }
}

void countDown(dispStruct *dispData_countD_t)
{

    disp_set(dispData_countD_t, DISP_DIG);

    if (--status == 0) {
       dispData_countD_t->count_flag = true;
    }

    if (dispData_countD_t->count_flag){
        dispData_countD_t->counting = false;
        return;
    }

    if (dispData_countD_t->digitarr[0] > 0)
        dispData_countD_t->digitarr[0]--;

    else if (dispData_countD_t->digitarr[1] > 0) {
      dispData_countD_t->digitarr[1]--;
      dispData_countD_t->digitarr[0] = 9;
    }
    else if (dispData_countD_t->digitarr[2] > 0) {
      dispData_countD_t->digitarr[2]--;
      dispData_countD_t->digitarr[1] = 5;
      dispData_countD_t->digitarr[0] = 9;
    }
    else if (dispData_countD_t->digitarr[3] > 0) {
      dispData_countD_t->digitarr[3]--;
      dispData_countD_t->digitarr[2] = 9;
    }
    else {
        dispData_countD_t->count_flag = true;
    }
}

void countUp(dispStruct *dispData_countU_t)
{
  static const int dp = 1;
    dispData_countU_t->digitarr[0] += 1;

    if (dispData_countU_t->digitarr[0] > 9) {
      dispData_countU_t->digitarr[0] = 0;
      dispData_countU_t->digitarr[1] += 1;
    }

    if (dispData_countU_t->digitarr[1] > 5) {
      dispData_countU_t->digitarr[1] = 0;
      dispData_countU_t->digitarr[2] += 1;
    }

    if (dispData_countU_t->digitarr[2] > 9) {
      dispData_countU_t->digitarr[2] = 0;
      dispData_countU_t->digitarr[3] += 1;
    }

    if (dispData_countU_t->digitarr[3] > 6) {
      dispData_countU_t->digitarr[3] = 0;
    }

    
    disp_set(dispData_countU_t, DISP_DIG);
}


void helper_disp_pg(Mode dispMode)
{
    switch (dispMode)
  {
    case Mode::countDown:
      modeDisp.digitarr[0] = 1;
      modeDisp.digitarr[1] = 0;
      modeDisp.digitarr[2] = 6;
      modeDisp.digitarr[3] = 6;
      break;
    case Mode::countUp:
      modeDisp.digitarr[1] = 2;
      modeDisp.digitarr[0] = 3;
      modeDisp.digitarr[2] = 6;
      modeDisp.digitarr[3] = 6;
      break;
    case Mode::dataEntry:
      modeDisp.digitarr[1] = 3;
      modeDisp.digitarr[0] = 4;
      modeDisp.digitarr[2] = 6;
      modeDisp.digitarr[3] = 6;
      break;
    default:
      modeDisp.digitarr[1] = 5;
      modeDisp.digitarr[0] = 5;
      modeDisp.digitarr[2] = 6;
      modeDisp.digitarr[3] = 6;
  }
  ESP_LOGI(TAG, "Disp set sym switch");
  disp_set(&modeDisp, DISP_SYM);
}

uint16_t sd_data_in_format(uint8_t digits[])
{
  uint16_t funcOut;
  funcOut = static_cast<uint16_t>(digits[0] + digits[1] * 10 + digits[2] * 60 + digits[3] * 600); 

  //Serial.println(funcOut);

  return funcOut;
}

//Runs the logic for the program mode display
void data_entry_disp(uint16_t secss)
{  
  uint8_t minutes = secss / 60, seconds = secss % 60;


  pgDisp.digitarr[3] = minutes / 10;
  pgDisp.digitarr[2] = minutes % 10;

  pgDisp.digitarr[1] = seconds / 10;
  pgDisp.digitarr[0] = seconds % 10;

  if (pgDisp.digitarr[0] > 9) {
    pgDisp.digitarr[0] = 0;
    pgDisp.digitarr[1] += 1;
  }

  if (pgDisp.digitarr[1] > 5) {
    pgDisp.digitarr[3] = 0;
    pgDisp.digitarr[2] += 1;
  }

  if (pgDisp.digitarr[2] > 9) {
    pgDisp.digitarr[2] = 0;
    pgDisp.digitarr[3] += 1;
  }

  if (pgDisp.digitarr[3] > 6) {
    pgDisp.digitarr[3] = 0;
  }

  disp_set(&pgDisp, DISP_DIG);
    
}

