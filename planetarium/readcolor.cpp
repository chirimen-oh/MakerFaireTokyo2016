#include <avr/eeprom.h>
#include "enums.h"
#include "readcolor.h"
#include "led.h"
#include "util.h"

void Readcolor::mainProc(){
  colorRead();
  rgb2hsv();
  decidePattern(detectColor());
  handleLed();
}

void Readcolor::init(){
  TinyWireM.begin();
  
  //初期化
  TinyWireM.beginTransmission(S11059Adrs);
  TinyWireM.write(0x0);
  TinyWireM.write(0x89);
  TinyWireM.endTransmission();
  TinyWireM.beginTransmission(S11059Adrs);
  TinyWireM.write(0x0);
  TinyWireM.write(0x09);
  TinyWireM.endTransmission();
}

void Readcolor::initializeValue(){
  sensorStatus = UNREADY;
  currentCOLOR = BLACK;
  for(int i=0;i<NUMCOLOR;i++){
    countColor[i]=0;
  }
}

int Readcolor::sensorRead(byte reg){
  byte ret;
  TinyWireM.beginTransmission(S11059Adrs);
  TinyWireM.write(reg);
  TinyWireM.endTransmission();
  TinyWireM.requestFrom(S11059Adrs,1);
  while(TinyWireM.available()){
    ret = TinyWireM.read();
  }
  return ret;
}

void Readcolor::colorRead(){
  r = sensorRead(0x03) << 8|sensorRead(0x04);
  g = sensorRead(0x05) << 8|sensorRead(0x06);
  b = sensorRead(0x07) << 8|sensorRead(0x08);
}

void Readcolor::rgb2hsv(){
  float max = r;
  float min = r;
  int maxcolor = 0;
  if(max < g){
    max = g;
    maxcolor = 1;
  }
  if(max < b){
    max = b;
    maxcolor = 2;
  }
  if(min > g){
    min = g;
  }
  if(min > b){
    min = b;
  }

  if(maxcolor == 0){ 
    h = 60 * ( (g - b) / (max - min) );
  }else if(maxcolor == 1){
    h = 60 * ( (b - r) / (max - min) ) + 120;
  }else{
    h = 60 * ( (r - g) / (max - min) ) + 240;
  }
  if(h < 0){
    h += 360;
  }
  s = (max - min) / max;
  v = max;
}


int Readcolor::detectColor(){
  int tmpcolor;
  if(r+g+b < 30){
    //black
    tmpcolor = BLACK;
  }else {
    if(h < 30 || 330 < h){
      //red
      tmpcolor = RED;
    }else if(210 < h && h < 240){
      //blue
      tmpcolor = BLUE;
    }else if(90 < h && h < 150){
      //green
      tmpcolor = GREEN;
    }else if(30 < h && h < 90){
      //yellow
      tmpcolor = YELLOW;
    }
  }
  return tmpcolor;
}
void Readcolor::decidePattern(int _color){
  countColor[_color]++;
  for(int j=0;j<NUMCOLOR;j++){
    if(countColor[j] >= decideColorThreshold){

      switch(sensorStatus){
        case UNREADY:
          if(_color == BLACK){
            sensorStatus = READY;
          }
          break;
        case READY:
          if(_color != BLACK){
            sensorStatus = READINGCOLOR;
            //最初の色追加処理
            colorPattern[0] = _color;
            numPattern = 1;
          }
          break;
        case READINGCOLOR:
        /*
          if(_color == BLACK){
            //end of read color mode
            finishReadcolor();
          }else if(_color != currentCOLOR){
            //色追加処理
            colorPattern[numPattern] = _color;
            numPattern++;
            if(numPattern >= MAX_COLOR_PATTERN){
              //end of read color mode
              finishReadcolor();
            }
          }
        */
          colorPattern[numPattern] = _color;
          numPattern++;
          if(numPattern >= MAX_COLOR_PATTERN-1){
            //end of read color mode
            finishReadcolor();
          }
          break;
        default:
          break;
      }
      currentCOLOR = _color;

      for(int i=0;i<NUMCOLOR;i++){
        countColor[i]=0;
      }
      break;
    }
  }
}

void Readcolor::handleLed(){
  switch(sensorStatus){
    case UNREADY:
      LED.ledOn(WHITE);
      break;
    case READY:
      LED.ledOn(BLACK);
      break;
    case READINGCOLOR:
      LED.ledOn(currentCOLOR);
      break;
    default:
      break;
  }
}

void Readcolor::finishReadcolor(){
  writePattern();
  //UTIL.mode = BLINK;
  UTIL.modeChange(BLINK);
}

void Readcolor::writePattern(){
  int _color;
  eeprom_busy_wait(); 
  eeprom_write_byte((uint8_t *)(BLINK_PATTERN_ROM), (uint8_t)numPattern);
  for(int i=0;i<numPattern;i++){  
    _color = colorPattern[i]; 
    eeprom_busy_wait(); 
    eeprom_write_byte((uint8_t *)(BLINK_PATTERN_ROM+i+1), (uint8_t)_color);
  }
  eeprom_busy_wait(); 
  eeprom_write_byte((uint8_t *)(BLINK_PATTERN_ROM+numPattern+1), 0xFF);
}
