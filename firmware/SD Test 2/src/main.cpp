#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

const uint8_t CS_PIN = 2;
uint16_t test_int = 0x1234;

const char *filename = "INTS.txt";
static File sdInstance;

void setup() {
  Serial.begin(9600);
  
  Serial.println("Init SD card...");

  Serial.println("Init SD card...");


  if (!SD.begin(CS_PIN)) {
    Serial.println("Init SD Failed!");
    while(1);
  }
  Serial.println("Init SD done.");
  SD.remove(filename);
  Serial.println("File Removed");
  sdInstance = SD.open(filename, FILE_WRITE);

  if (sdInstance) {
    Serial.print("Writing...");
    sdInstance.write((uint8_t*)&test_int, sizeof(test_int));

    //close
    sdInstance.close();
    Serial.println("done.");
  }
  else {
    Serial.println("error opening file.");
  }

}

void loop(void) {
  ;
}