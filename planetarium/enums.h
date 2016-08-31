#ifndef _ENUM_H_
#define _ENUM_H_

enum MODE{
  BLINK,
  SENSORREAD_DISPLAY
};

enum COLOR{
  RED=0,
  GREEN=1,
  BLUE=2,
  YELLOW=3,
  BLACK=4,
  WHITE=5
};

enum STATUS{
  UNREADY,
  READY,
  READINGCOLOR
};

enum PATTERN{
  LINER,
  EASE
};

#endif // _ENUM_H_
