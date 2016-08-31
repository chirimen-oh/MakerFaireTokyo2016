

#include <Adafruit_NeoPixel.h>
#include <avr/eeprom.h>
#include "enums.h"
#include "led.h"
#include "readcolor.h"
#include "util.h"

void Led::init(){
  RGBLED.begin();
  ledOff();
  setBrightness(BRIGHTNESS_NORMAL);
}

void Led::setBrightness(int power){
  brightness = power;
}

void Led::ledOn(int _color){
  
  switch(_color){
    case BLACK://black
      RGBLED.setBrightness(BRIGHTNESS_NONE);
      RGBLED.setPixelColor(0,0,0,0);
      break;
    case RED://red
      RGBLED.setBrightness(brightness);
      RGBLED.setPixelColor(0,255,0,0);
      break;
    case BLUE://blue
      RGBLED.setBrightness(brightness);
      RGBLED.setPixelColor(0,0,0,255);
      break;
    case GREEN://green
      RGBLED.setBrightness(brightness);
      RGBLED.setPixelColor(0,0,255,0);
      break;
    case YELLOW://yellow
      RGBLED.setBrightness(brightness);
      RGBLED.setPixelColor(0,255,255,0);
      break;
    case WHITE:
      RGBLED.setBrightness(brightness);
      RGBLED.setPixelColor(0,255,255,255);
      break;
  }
  
  RGBLED.show();
}

void Led::ledColor(int r,int g,int b){
  RGBLED.setBrightness(brightness);
  RGBLED.setPixelColor(0,r,g,b);
  RGBLED.show();
}

void Led::ledOff(){
  RGBLED.setBrightness(BRIGHTNESS_NONE);
  RGBLED.setPixelColor(0,0,0,0);
  RGBLED.show();
}

void Led::readPattern(){
  uint8_t value;
  eeprom_busy_wait();
  value = eeprom_read_byte((uint8_t *)BLINK_PATTERN_ROM);
  if(0<(int)value && (int)value < MAX_COLOR_PATTERN){
    READCOLOR.numPattern = (int)value;
    int i=0;
    eeprom_busy_wait(); 
    value = eeprom_read_byte((uint8_t *)(BLINK_PATTERN_ROM+i+1));
    while(value != 0xFF){  
      READCOLOR.colorPattern[i] = (int)value; 
      i++;
      eeprom_busy_wait(); 
      value = eeprom_read_byte((uint8_t *)(BLINK_PATTERN_ROM+i+1));
    }
  }else{
    READCOLOR.numPattern = 5;
    READCOLOR.colorPattern[0] = RED;
    READCOLOR.colorPattern[1] = GREEN;
    READCOLOR.colorPattern[2] = BLUE;
    READCOLOR.colorPattern[3] = YELLOW;
    READCOLOR.colorPattern[4] = BLACK;
  }
  
}
void Led::play(){  
  //ledOn(READCOLOR.colorPattern[UTIL.step]);
  if(UTIL.step <= 0){
    if(UTIL.count == 0){setBrightness(BRIGHTNESS_NORMAL);}
    ease_in_out(EASE,READCOLOR.colorPattern[UTIL.step],1,READCOLOR.colorPattern[READCOLOR.numPattern-1]);
  }else{
    ease_in_out(LINER,READCOLOR.colorPattern[UTIL.step],0.5,-1);
  }

  if(UTIL.count % 5 == 0){
    rand = (analogRead(4) % 4 - 2)*15;
  }

  setBrightness(BRIGHTNESS_NORMAL + rand);
  

}

void Led::returnColor(int color,int *array){
  switch(color){
    case RED:
      array[0] = 255;
      array[1] = 0;
      array[2] = 0;
      break;
    case GREEN:
      array[0] = 0;
      array[1] = 255;
      array[2] = 0;
      break;
    case BLUE:
      array[0] = 0;
      array[1] = 0;
      array[2] = 255;
      break;
    case YELLOW:
      array[0] = 255;
      array[1] = 255;
      array[2] = 0;
      break;
    case BLACK:
      array[0] = 0;
      array[1] = 0;
      array[2] = 0;
      break;
    default:
      break;
  }
}

void Led::ease_in_out(int pattern,int out,float rate,int in){

  //int start[3];
  int target[3];
  if(UTIL.count == 0){
    if(in < 0){
      startLed[0] = currentLed[0];
      startLed[1] = currentLed[1];
      startLed[2] = currentLed[2];
    }else{
      returnColor(in,target);
      startLed[0] = (float)target[0];
      startLed[1] = (float)target[1];
      startLed[2] = (float)target[2];
    }
  }
  
  returnColor(out,target);
  
  int i;
  float t = (float)UTIL.count / STEPCOUNT;
  if(UTIL.count == 0){
    for(int i=0;i<3;i++){
      currentLed[i] = (float)startLed[i];
    }
  }else{
    if(UTIL.count <= (STEPCOUNT/2)){
      //t = (float)UTIL.count / STEPCOUNT;
      switch(pattern){
        case EASE:
          t = t*t;
          break;
      }
    }else{
      //t = (float)UTIL.count / STEPCOUNT;
      switch(pattern){
        case EASE:
          t = -1 * t * (t-2);
          break;
      }

    }
    for(int i=0;i<3;i++){
      currentLed[i] = (target[i] - startLed[i]) * rate * t + startLed[i];
    }
  }
   
  ledColor((int)currentLed[0],(int)currentLed[1],(int)currentLed[2]);
  
}

