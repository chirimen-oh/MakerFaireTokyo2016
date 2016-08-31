#ifndef _UTIL_H_
#define _UTIL_H_

#include "enums.h"
#define UTIL Util::getInstance()
#define STEPCOUNT 100
#define MAXSTEP 10
#define DELAY_FOR_LOOP 10

class Util
{
public:
  static Util& getInstance(){
    static Util inst;
    return inst;
  }

  int count;
  int step;
  int mode;

  void initCount();
  void countUp();
  void modeChange(int mode);
  
};

#endif