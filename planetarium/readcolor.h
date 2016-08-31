#ifndef _READCOLOR_H_
#define _READCOLOR_H_

#include <TinyWireM.h>

#define READCOLOR Readcolor::getInstance()
#define NUMCOLOR 5
#define MAX_COLOR_PATTERN 10
#define S11059Adrs 0x2a
#define BLINK_PATTERN_ROM 0x00


class Readcolor{
  private:
    const int decideColorThreshold = 50;
    int countColor[NUMCOLOR];
    int sensorStatus;
    int currentCOLOR;
    int r=0,g=0,b=0;
    float h=0,s=0,v=0;

  public:
    static Readcolor& getInstance(){
      static Readcolor inst;
      return inst;
    }

    int colorPattern[MAX_COLOR_PATTERN];
    int numPattern=0;

    void mainProc();
    void init();
    void initializeValue();
    int sensorRead(byte reg);
    void colorRead();
    void rgb2hsv();
    int detectColor();
    void decidePattern(int _color);
    void handleLed();
    void finishReadcolor();
    void writePattern();
};

#endif