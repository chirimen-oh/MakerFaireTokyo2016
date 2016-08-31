#include "enums.h"
#include "util.h"
#include "readcolor.h"

void Util::initCount(){
  count = 0;
  step = 0;
}

void Util::countUp(){
  count++;
  if(count >= STEPCOUNT){
    step++;
    if(0 < READCOLOR.numPattern && READCOLOR.numPattern < MAXSTEP){
      if(step >= READCOLOR.numPattern){
        step = 0;
      }
    }
    if(step >= MAXSTEP){
      step = 0;
    }
    count = 0;
  }

  delay(DELAY_FOR_LOOP);
}

void Util::modeChange(int mode){
  UTIL.initCount();
  READCOLOR.initializeValue();
  UTIL.mode = mode;
}