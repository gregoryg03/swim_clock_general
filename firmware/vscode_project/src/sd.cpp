#include "sd2.h"

const char *filename = "intervals.txt";
File clockData;
File readFile;

struct sdAction sd = {
    .clear = sd_clear,
    .write = sd_write,
    .read = sd_read,
    .dismount = sd_close
};
//Use sd.clear, sd.write, sd.read

writeState writeTable[] = {
    {sd_start, sd_write, sd_end}
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


// void sd_clear(void)
// {
//     if(SD.exists(filename)){
//         SD.remove(filename);
//     }
// }

void sd_start(void)
{
    clockData = SD.open(filename, FILE_WRITE);
    if (!clockData)
        Serial.println("SD Fail Start");
}

void sd_write(uint16_t data)
{   
    if (clockData) {
        clockData.println("Test Text");
        clockData.write(reinterpret_cast<uint8_t*>(&data), sizeof(data));
    }
    else
        Serial.println("SD Fail Write");
}

void sd_end(void)
{
    clockData.flush();
    clockData.close();
}

void sd_read_init(void)
{
    readFile = SD.open(filename, FILE_READ);
}

bool sd_read(uint16_t &interval)
{
    if (!readFile)
        return false;
    
    if (readFile.available() >= sizeof(interval)) {
        readFile.read(reinterpret_cast<uint8_t*>(&interval), sizeof(interval));
        return true;
    }

    return false;
}

void sd_close(void)
{
    clockData.close();
}
