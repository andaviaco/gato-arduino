#include <TimerOne.h>
#include "board.h"

#define LED_A 0
#define LED_B 1
#define cast_column(col,led) col*2+led

void setup() {
  setupBoard();

  Timer1.initialize(1000);
  Timer1.attachInterrupt(displayBoard);

  setLed(1, 2, 1);
  setLed(1, 3, 1);
}

void loop() {
  // put your main code here, to run repeatedly:

}
