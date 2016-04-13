#include <TimerOne.h>
#include "board.h"

#define BOARD_SIZE 3

// Each cell in the board has two leds, identified as LED_A and LED_B
#define LED_A 0
#define LED_B 1
#define cast_column(col,led) col*2+led  // cast virtual board column to fisical board column

#define PLAYER_A 'O'
#define PLAYER_B 'X'
#define EMPTY -1

#define SEP ',' // coodinates separator

typedef char TicTacBoard[BOARD_SIZE][BOARD_SIZE];

boolean checkCoordinates(int row, int col);
void clearVirtualBoard();
void setTurn(char turn);
boolean isCellAvailable(int row, int col);
void checkForWinner();

// game variables
char winner;
char turn;
int turn_count;
int max_turns;

TicTacBoard virtual_board;

boolean play;

int line[BOARD_SIZE][2]; // winer line coordinates

// turn indicator leds
int turn_leds[2] = {2, 3};

void setup() {
  setupBoard();
  clearVirtualBoard();
  
  Timer1.initialize(1000);
  Timer1.attachInterrupt(displayBoard);
  
  pinMode(turn_leds[0], OUTPUT);
  pinMode(turn_leds[1], OUTPUT);

  Serial.begin(9600);
  
  winner = EMPTY;
  setTurn(PLAYER_A);
  turn_count = 0;
  max_turns = BOARD_SIZE * BOARD_SIZE;
  play = true;
}

void loop() {
  static boolean state = true; 
  String msg; 
  int row, col, pos;
  
   // If there's a winner, then turn off/on winner line
  if (winner != EMPTY) {  
    int led = (winner == PLAYER_A) ? LED_A : LED_B; // Get winner led color
    
    // Display winner line
    for (int i=0; i < BOARD_SIZE; i++) {
      setLed(line[i][0], cast_column(line[i][1], led), state);
    }
    
    state = !state;
    
    delay(500);
  }
  
  if (Serial.available()) {
    char bytes[3];
    
    Serial.readBytes(bytes, 3);
    
    if (bytes[0] == 0) {
      // Handle reset command
      clearBoard();
      clearVirtualBoard();
      
      winner = EMPTY;
      turn_count = 0;
      play = true;
    } else if (bytes[0] == 1) {
      int row = bytes[1];
      int col = bytes[2];
      
      if (play && checkCoordinates(row, col)) {
        if (isCellAvailable(row, col)) {
          if (turn == PLAYER_A) {
            setLed(row, cast_column(col, LED_A), true);
          } else {
            setLed(row, cast_column(col, LED_B), true);
          }
          
          virtual_board[row][col] = turn;
          
          turn_count = turn_count + 1;
          
          checkForWinner();
          
          if (turn == PLAYER_A) {
            setTurn(PLAYER_B);
          } else {
            setTurn(PLAYER_A);
          }
          
        } else {
          // Serial.println("Cell is already in use!");        
        }
      } else {
        // Serial.println("Wrong coordinates!");
      }
    } else {    
      // Serial.println("Write reset to reset the game");
    }
    
    bytes[0] = (winner != EMPTY) ? 1 : ((play == false) ? 2 : 0);
    bytes[1] = winner;
    bytes[2] = turn;
    
    Serial.write(bytes, 3);
  }
}

void clearVirtualBoard() {
  int i, j;
  
  for (i = 0; i < BOARD_SIZE; i++) {
    for(j = 0; j < BOARD_SIZE; j++) {
      virtual_board[i][j] = EMPTY;
    }
  }
}

boolean checkCoordinates(int row, int col) {
  return (row >= 0 && row < BOARD_SIZE) && (col >= 0 && col < BOARD_SIZE);
}

boolean isCellAvailable(int row, int col) {
  return virtual_board[row][col] == EMPTY;
}

void checkForWinner() {
  // check columns
  boolean end_game;
  int row, col;
  char cell;
  
  end_game = false;
    
  // check rows
  for (row = 0; row < BOARD_SIZE && !end_game; row++) {
    cell = virtual_board[row][0];
       
    if (cell != EMPTY) {
      line[0][0] = row;
      line[0][1] = 0;

      for (col = 1; col < BOARD_SIZE; col++) {
        if (cell != virtual_board[row][col]) break;
        
        line[col][0] = row;
        line[col][1] = col;
      }
      
      end_game = (col == BOARD_SIZE); 
    }
  }
  
  // check columns
  for (col = 0; col < BOARD_SIZE && !end_game; col++) {
    cell = virtual_board[0][col];
    
    if (cell != EMPTY) {
      line[0][0] = 0;
      line[0][1] = col;
      
      for (row = 1; row < BOARD_SIZE; row++) {
        if (cell != virtual_board[row][col]) break;
        
        line[row][0] = row;
        line[row][1] = col;
      }
      
      end_game = (row == BOARD_SIZE); 
    }
  }
  
  // check diagonals
  if (!end_game && (cell = virtual_board[0][0]) != EMPTY) {   
    line[0][0] = 0;
    line[0][1] = 0;
    
    for (row = 1; row < BOARD_SIZE; row++) {
      col = row;

      if (cell != virtual_board[row][col]) break;
      
      line[row][0] = row;
      line[row][1] = col;
    }
  
    end_game = (row == BOARD_SIZE);
  }
  
  row = BOARD_SIZE-1;
  if (!end_game && (cell = virtual_board[row][0]) != EMPTY) {
    line[0][0] = row;
    line[0][1] = 0;
 
    for (col = 1; col < BOARD_SIZE; col++) {
      row = BOARD_SIZE - (1 + col);
      
      if (cell != virtual_board[row][col]) break;
      
      line[col][0] = row;
      line[col][1] = col;
    }
    
    end_game = (col == BOARD_SIZE);
  }
  
  if (end_game) {
    winner = cell;
    // Serial.print("Winner: ");
    // Serial.println(winner);
    play = false;
  } else if (turn_count == max_turns) {
    // Serial.println("Draw");
    play = false;
  }
}

void setTurn(char new_turn) {
  if (new_turn == PLAYER_A) {
    digitalWrite(turn_leds[0], HIGH);
    digitalWrite(turn_leds[1], LOW);
  } else {
    digitalWrite(turn_leds[1], HIGH);
    digitalWrite(turn_leds[0], LOW);
  }
  
  turn = new_turn;
}

void blinkWinnerLine() {
  static boolean state = true;
  
  int led = (winner == PLAYER_A) ? LED_A : LED_B;
  
  for (int i=0; i < BOARD_SIZE; i++) {
    setLed(line[i][0], cast_column(line[i][1], led), state);
  }
  
  state = !state;
}
