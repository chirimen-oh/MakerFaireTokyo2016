#ifndef _LED_H_
#define _LED_H_

#include <Adafruit_NeoPixel.h>
#include "enums.h"

#define LED Led::getInstance()
#define NUMRGBLED 1
#define RGBLED_OUTPIN 1
#define BRIGHTNESS_NONE 0
#define BRIGHTNESS_LOW 50
#define BRIGHTNESS_NORMAL 100
#define BRIGHTNESS_HIGH 255

class Led{
private:
  float currentLed[3];
  float startLed[3];
  int brightness;
  int rand;
public:
  static Led& getInstance(){
    static Led inst;
    return inst;
  }
  Adafruit_NeoPixel RGBLED = Adafruit_NeoPixel(NUMRGBLED, RGBLED_OUTPIN, NEO_RGB + NEO_KHZ800);

  void init();
  void setBrightness(int power);
  void ledOn(int _color);
  void ledOff();
  void ledColor(int r,int g,int b);
  void readPattern();
  void play();
  void returnColor(int color,int array[3]);
  void ease_in_out(int pattern,int out,float rate,int in);
  
};


#endif