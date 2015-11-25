#include <TimerOne.h>

#define ROWS 3
#define COLUMNS 6

typedef byte Board[ROWS];

// pins assigned to columns
int columns[COLUMNS] = {8, 9, 10, 11, 12, 13};

// pins assigned to rows
int rows[ROWS] = {5, 6, 7};

// initialize board
Board board = {
  B000000,
  B000000,
  B000000
};

void setupBoard();
void resetBoard(); // turns off all leds
void setLed(int row, int column, boolean on); // turn on/off a specific led
void displayBoard();

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

  Timer1.initialize(1000);
  Timer1.attachInterrupt(displayBoard);
}

void resetBoard() {
  for (int i = 0; i < ROWS; i++) {
    board[i] = B000000;
  }
}

void setLed(int row, int col, boolean on) {
  if (on) {
    bitSet(board[row], col); 
  } else {
    bitClear(board[row], col);
  }
}

void displayBoard() {
  static byte cur_row = 0;
  static byte bit_mask = B00000011;
  
  digitalWrite(rows[cur_row], HIGH); // Turn off previous row

  if (bit_mask == B00110000) {
    bit_mask = B00000011;
    
    if (++cur_row == ROWS) {
      cur_row = 0;
    }
  }
  else {
    bit_mask <<= 2;
  }

  // direct port manipulation
  PORTB &= B11000000;
  PORTB |= board[cur_row] & bit_mask;
  
  digitalWrite(rows[cur_row], LOW); // turn on row
}

