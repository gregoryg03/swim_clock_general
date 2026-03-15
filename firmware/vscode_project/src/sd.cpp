#include "sd2.h"

const char *filename = "intervals.txt";
File clockData;

struct sdAction sd = {
    .clear = sd_clear,
    .write = sd_write,
    .read = sd_read
};
//Use sd.clear, sd.write, sd.read

void sd_init(uint8_t CS_PIN)
{

  Serial.println("Init SD card...");

  if (!SD.begin(CS_PIN)) {
    Serial.println("Init SD Failed!");
    //Add Fail Message?
  }
  Serial.println("Init SD done.");
  
  if (!clockData)
    clockData = SD.open(filename, FILE_WRITE);
}


void sd_clear(void)
{
    SD.remove(filename);
}

void sd_write(uint16_t data)
{
    if (!clockData)
        clockData = SD.open(filename, FILE_WRITE);

    if (clockData) {
        clockData.write(reinterpret_cast<uint8_t*>(&data), sizeof(data));
    }
}


uint16_t sd_read(void)
{
    uint16_t output;
    if (!clockData)
        clockData = SD.open(filename, FILE_WRITE);

    if (clockData){
        clockData.read(reinterpret_cast<uint8_t*>(&output), sizeof(output));
    }

    return output;
}
