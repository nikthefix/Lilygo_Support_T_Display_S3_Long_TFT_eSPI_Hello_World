/*
  For Lilygo T-Display-S3-Long 
  nikthefix - 28th Nov 2023
  Modified driver code and example sketch using TFT_eSPI in 'sprite only mode' 

  Versions:
  TFT_eSPI 2.5.34 - latest at time of writing
  ESP32 Arduino 3.0.0-alpha2 - latest at time of writing

  Notes:
  As the display doesn't implement a scan orientation hardware rotate - as far as I can see from the current datasheet - we need to use 
  a soft matrix rotation to get a landscape view without messing with TFT_eSPI. This is implemented in lcd_PushColors_rotated_90().
  You can then use lcd_setRotation(2) which IS hardware implemented, to flip the whole thing upside down if you need.
  Code has been stripped down to support QSPI display only.

  Build Options:
  Board  ESP32-S3-Dev
  USB CDC On boot Enabled
  Flash Size 16MB
  Partition Scheme 16M Flash(3MB APP/9.9MB FATFS)
  PSRAM "OPI PSRAM"

  Since ESP32 Arduino 3.0.0-alpha3 is still pretty funky with a lot of existing Arduino driver code it may be necessary to downgrade to V2.xx as the project expands - in the short term
*/

#include "AXS15231B.h"
#include <Arduino.h>
#include <TFT_eSPI.h>
// For bitmap encoding: use Image2lcd, 16bit true colour, MSB First, RGB565, don't include head data, be sure to set max image size, save as .h file.

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);


void draw()
{
 sprite.fillSprite(TFT_BLACK);
 sprite.drawString("Hello World",220,20,4);
 sprite.fillRect(210,100,60,60,TFT_RED);
 sprite.fillRect(280,100,60,60,TFT_GREEN);
 sprite.fillRect(350,100,60,60,TFT_BLUE);
 
 lcd_PushColors_rotated_90(0, 0, 640, 180, (uint16_t*)sprite.getPointer());
}

    
void setup() {
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, LOW);        // turn off backlight asap to minimise power on artifacts
    Serial.begin(115200); 
    axs15231_init();
    //lcd_setRotation(2);             // 180 degree hardware rotate if you want reset / boot buttons at the bottom
    sprite.createSprite(640, 180);    // full screen landscape sprite in psram
    sprite.setSwapBytes(1);
    lcd_fill(0,0,180,640,0x00);       // clear screen
    digitalWrite(TFT_BL, HIGH);       // turn on backlight
}


void loop() {     

    draw();
    delay(1000);

    sprite.fillSprite(TFT_CYAN);
    lcd_PushColors_rotated_90(0, 0, 640, 180, (uint16_t*)sprite.getPointer());
    delay(1000);

    sprite.fillSprite(TFT_ORANGE);
    lcd_PushColors_rotated_90(0, 0, 640, 180, (uint16_t*)sprite.getPointer());
    delay(1000);
    
    sprite.fillSprite(TFT_VIOLET);
    lcd_PushColors_rotated_90(0, 0, 640, 180, (uint16_t*)sprite.getPointer()); 
    delay(1000);
    
    check_for_memory_leaks();
 }


void check_for_memory_leaks() {

    Serial.print(F("Total heap  ")); Serial.println(ESP.getHeapSize());
    Serial.print(F("Free heap   ")); Serial.println(ESP.getFreeHeap());
    Serial.print(F("Total psram ")); Serial.println(ESP.getPsramSize());
    Serial.print(F("Free psram  ")); Serial.println(ESP.getFreePsram());
}
