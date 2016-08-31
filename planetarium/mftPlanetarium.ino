#include <avr/sleep.h>
#include <TinyWireM.h>
//#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>
#include <avr/eeprom.h>

#include "enums.h"
#include "readcolor.h"
#include "led.h"
#include "util.h"

#define BTNPIN 3


int Power __attribute__ ((section (".noinit")));

/*
const int Rx = 3; // this is physical pin 2
const int Tx = 4; // this is physical pin 3
SoftwareSerial mySerial(Rx, Tx);
*/

void PowerDown () { //PowerDown() function can be used for powering down anywhere within loop() function too
  endProc();
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  ADCSRA &= ~(1<<ADEN);       // Turn off ADC to save power
  sleep_enable();
  sleep_cpu();
}

void endProc(){
  LED.ledOff();
}

void setup(){
  READCOLOR.init();
  LED.init();

  //serial out
  //mySerial.begin(9600);
  pinMode(BTNPIN,INPUT);

  UTIL.mode = BLINK;
  UTIL.initCount();
  LED.readPattern();

  Power = !Power;
  if (!Power) PowerDown();
}

void loop(){
  
  if(digitalRead(BTNPIN)==LOW){
    if(UTIL.mode == BLINK){
      UTIL.modeChange(SENSORREAD_DISPLAY);
    }else{
      UTIL.modeChange(BLINK);
    }
    delay(20);
    while(digitalRead(BTNPIN)==LOW){}
  }
  
  switch(UTIL.mode){
    case BLINK:
      LED.play();
      break;
    case SENSORREAD_DISPLAY:
      READCOLOR.mainProc();
      break;
    default:
      break;
  }
  
  UTIL.countUp();
}
