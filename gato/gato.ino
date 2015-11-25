#include "board.h"

// #define TEST_BOARD_MODE

#define BOARD_SIZE 3

#define LED_A 0
#define LED_B 1

#define cast_column(col,led) col*2+led

void turnOnLed(int row, int col, int led);
boolean checkCoordinates(int row, int col);

void setup() {
  setupBoard();

  #ifdef TEST_BOARD_MODE
    int i, j;
    
    while (true) {
      for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
          turnOnLed(i, j, LED_A);
          delay(1000);
          
          turnOnLed(i, j, LED_B);
          delay(1000);
        }
      }
      
      resetBoard(); 
    }
  #endif
}

void loop() {
  
}

/** 
 * Each cell in the board has two leds, identified as LED_A
 * and LED_B. The cell is specified with the (x,y) parameters,
 * while led parameter identifies one of these leds.
 */
void turnOnLed(int row, int col, int led) {
  if (checkCoordinates(row, col)) {
    setLed(row, cast_column(col, led), true); // cast the column value to its real value in the board
  }
}

boolean checkCoordinates(int row, int col) {
  return (row >= 0 && row < BOARD_SIZE) && (col >= 0 && col < BOARD_SIZE);
}



