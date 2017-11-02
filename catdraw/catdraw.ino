#include <TimerOne.h>
#include "board.h"

#define RESET 0
#define DRAW 1
#define cast_column(col,led) col*2+led

void setup() {
  setupBoard();

  Timer1.initialize(1000);
  Timer1.attachInterrupt(displayBoard);

  Serial.begin(9600);
}

void loop() {
  if (Serial.available()) {
    char bytes[4];

    Serial.readBytes(bytes, 4);

    if (bytes[0] == RESET) {
      clearBoard();
    } else {
      setLed(bytes[1], bytes[2], bytes[3]);
    }
  }
}

