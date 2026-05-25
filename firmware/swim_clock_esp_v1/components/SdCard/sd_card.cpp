#include "sd_card.h"

static const char *TAG = "sd_debug";

//Testing intervals to cycle through (sec)
const int timearr[10] = {20, 5, 20, 5, 30, 10, 60, 10, 20, 3000};
int intevaltime = 0;

// static sdData sdDataRead = {READ_MODE, 0, 0, false};
// SD_CARD sdInstanceRead;
//decimal Pt
uint8_t dp = 0b00000000;

//Time
unsigned long tcount;
int shiftminsten = 0, shiftmins = 0, shiftsecsten = 0, shiftsecs = 0;
int secs, secsten, mins, minsten;

bool counting = false;
bool flag = false;
static int status;



void test_sd(void)
{
    ESP_LOGI(TAG, "SD Main");

    static uint16_t curInt = 0;
    static bool open_flag = false;
    static int i = 0;

    if (!counting) {
        curInt = timearr[i++];
        status = curInt;
    }

    int minutes = curInt / 60, seconds = curInt % 60;

    shiftminsten = minutes / 10;
    shiftmins = minutes % 10;

    shiftsecsten = seconds / 10;
    shiftsecs = seconds % 10;

    counting = true;
    flag = false;
}