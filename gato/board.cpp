#include "board.h"

// pins assigned to columns
int columns[COLUMNS] = {8, 9, 10, 11, 12, 13};

// pins assigned to rows
int rows[ROWS] = {5, 6, 7};

Board board = {
  {0,0,0,0,0,0},
  {0,0,0,0,0,0},
  {0,0,0,0,0,0}  
};

void setupBoard() {
  int i;
  
  // set pins as outputs
  for (i=0; i < ROWS; i++) {
    pinMode(rows[i], OUTPUT);
    digitalWrite(rows[i], HIGH);
  }

  for (i=0; i < COLUMNS; i++) {
    pinMode(columns[i], OUTPUT);
    digitalWrite(columns[i], LOW);
  }
}
    
void setLed(int row, int col, boolean on) {
  board[row][col] = on;
}
  
void displayBoard() {
  static byte cur_row = 0; // current active row

  digitalWrite(rows[cur_row], HIGH); // Turn off previous row
  
  // display next row, wrap if necesary
  if (++cur_row == ROWS) {
    cur_row = 0;
  }
  
  // enable/disable corresponding columns
  for(int i = 0; i < COLUMNS; i++) {
     if (board[cur_row][i]) {
       digitalWrite(columns[i], HIGH);
     } else {
       digitalWrite(columns[i], LOW);
     }
   }
    
   digitalWrite(rows[cur_row], LOW); // enable current row
}

void clearBoard() {
  int i, j;

  for (i=0; i < ROWS; i++) {
    for(j=0; j < COLUMNS; j++) {
      board[i][j] = false;
    }
  }
}
