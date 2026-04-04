#include "sd2.h"

const char *filename = "INTS2.txt";

File clockData;

sdType SD_CARD::actions[4] = {
    actions[0] = sd_open, actions[1] = sd_write,
    actions[2] = sd_read, actions[3] = sd_close
};


void SD_CARD::sd_init(uint8_t CS_PIN)
{
    Serial.println("Init SD card...");
    pinMode(CS_PIN, OUTPUT);
    
    if (!SD.begin(CS_PIN)) {
        Serial.println("Init SD Failed!");
    //Add Fail Message?
    }
    Serial.println("Init SD done.");
}

bool SD_CARD::call(sdAction action, sdData& sdStruct)
{
    if (action >= ACTION_COUT || actions[action] == nullptr) {
        return false;
    }
    return actions[action](sdStruct);
}

bool sd_open(sdData& data)
{
    if (clockData)
        clockData.close();
    Serial.println("SD_OPEN");
    clockData = SD.open(filename, data.mode);
        if (!clockData) {
            Serial.println("SD Fail Start");
            return false;
        }
    return true;
}

bool sd_write(sdData& data)
{
    Serial.println("SD_WRITE");
    if (clockData) {
        Serial.println("Writing...");
        clockData.write(reinterpret_cast<uint8_t*>(&(data.intervalIn)), sizeof(data.intervalIn));
        return true;
    }
    else 
        Serial.println("SD Fail Write");
    return false;
}

bool sd_read(sdData& data)
{
    Serial.println("SD_READ");
    if (clockData.available() >= sizeof(data.intervalOut)) {
        size_t bytesRead = clockData.read(reinterpret_cast<uint8_t*>(&(data.intervalOut)), sizeof(data.intervalOut));
        if (bytesRead <= sizeof(data.intervalOut))
            return true;
    }
    
    Serial.println("SD Read Fail");
    return false;
}


bool sd_close(sdData& data)
{
    Serial.println("SD_CLOSE");
    if (clockData) {
        clockData.close();
        Serial.println("close");
        return true;
    }
    Serial.println("close Fail");
    return false;
}

void SD_CARD::sd_shutdown(void)
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


