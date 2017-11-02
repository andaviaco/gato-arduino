#ifndef __BOARD_H__
#define __BOARD_H__

#include <Arduino.h>

#define ROWS 3
#define COLUMNS 6

typedef boolean Board[ROWS][COLUMNS];

void setupBoard();
void setLed(int row, int col, boolean on);
void displayBoard();
void clearBoard();

#endif // __BOARD_H__
