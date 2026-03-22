#include "sd2.h"

const char *filename = "INTS.txt";

File clockData;

sdType SD_CARD::actions[4] = {
    actions[0] = sd_open, actions[1] = sd_write,
    actions[2] = sd_read, actions[3] = sd_close
};


void sd_init(uint8_t CS_PIN)
{

  Serial.println("Init SD card...");

  if (!SD.begin(CS_PIN)) {
    Serial.println("Init SD Failed!");
    //Add Fail Message?
  }
  Serial.println("Init SD done.");
}


bool sd_open(sdData& data)
{
    clockData = SD.open(filename, data.mode);
        if (!clockData) {
            Serial.println("SD Fail Start");
            return false;
        }
    return true;
}

bool sd_write(sdData& data)
{
    if (clockData) {
        Serial.print("Writing...");
        clockData.write(reinterpret_cast<uint8_t*>(&data.intervalIn), sizeof(data.intervalIn));
        return true;
    }
    else 
        Serial.println("SD Fail Write");
    return false;
}

bool sd_read(sdData& data)
{
    if (clockData.available() >= sizeof(data.intervalOut)) {
        clockData.read(reinterpret_cast<uint8_t*>(&data.intervalOut), sizeof(data.intervalOut));
        return true;
    }
    Serial.println("SD Read Fail");
    return false;
}


bool sd_close(sdData& data)
{
    if (clockData) {
        clockData.close();
        Serial.println("close");
        return true;
    }
    Serial.println("close Fail");
    return false;
}

void sd_shutdown(void)
{
    if (clockData) {
        clockData.close();
    }
}

// void sd_clear(void)
// {
//     if(SD.exists(filename)){
//         SD.remove(filename);
//     }
// }


