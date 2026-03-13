#include "sd2.h"

const char *filename = "intervals.txt";


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
    while(1);
  }
  Serial.println("Init SD done.");
}


void sd_clear(void)
{

    SD.remove(filename);
    clockData = SD.open(filename, FILE_WRITE);
}

void sd_write(uint16_t)
{
   ; 
}
uint16_t sd_read(void)
{
    ;
}
