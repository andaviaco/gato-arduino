// ------------------ fisical board -----------------
#define ROWS 3
#define COLUMNS 6

// pins assigned to columns
int columns[COLUMNS] = {8, 9, 10, 11, 12, 13};

// pins assigned to rows
int rows[ROWS] = {5, 6, 7};

boolean board[ROWS][COLUMNS] = {
  {0,0,0,0,0,0},
  {0,0,0,0,0,0},
  {0,0,0,0,0,0}  
};
  
void clearBoard() {
  int i, j;

  for (i = 0; i < ROWS; i++) {
    for(j = 0; j < COLUMNS; j++) {
      board[i][j] = false;
    }
  }
}
    
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
    
boolean getLedState(int row, int col) {
  return board[row][col];
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
   delay(5);
}
// ----------------------------------------
#define TEST

#define BOARD_SIZE 3

// Each cell in the board has two leds, identified as LED_A and LED_B
#define LED_A 0
#define LED_B 1
#define cast_column(col,led) col*2+led  // cast virtual board column to fisical board column

#define PLAYER_A 'R'
#define PLAYER_B 'G'

#define SEP ',' // coodinates separator

typedef char TicTacBoard[BOARD_SIZE][BOARD_SIZE];

// game logic functions
boolean checkCoordinates(int row, int col);
void clearVirtualBoard();
void setTurn(char turn);
boolean isCellAvailable(int row, int col);
void checkForWinner();

// game variables
char turn;
int turn_count;
int max_turns;
TicTacBoard virtual_board;
boolean play = true;

// turn indicator leds
int player_a_led = 3;
int player_b_led = 2;

void setup() {
  setupBoard();
  clearVirtualBoard();
  
  pinMode(player_a_led, OUTPUT);
  pinMode(player_b_led, OUTPUT);

  Serial.begin(9600);
  
  setTurn(PLAYER_A);
  turn_count = 0;
  max_turns = BOARD_SIZE * BOARD_SIZE;
}

void loop() {
  String msg; 
  int row, col, pos;
  
  displayBoard();
  
  if (Serial.available()) {
    Serial.println("Receiving data...");
    
    msg = Serial.readString();
    
    if (msg.equals("reset")) {
      clearBoard();
      clearVirtualBoard();
      turn_count = 0;
      play = true;
      Serial.println("Game reset!");
    } else if (play) {
      
      pos = msg.indexOf(SEP);
    
      if (pos != -1) {
        row = msg.substring(0, pos).toInt();
        col = msg.substring(pos + 1).toInt();
      } else {
        Serial.println("Invalid input!");
        return;
      }
      
      Serial.print("Row: ");
      Serial.println(row);
      
      Serial.print("Col: ");
      Serial.println(col);
      Serial.println();
      
      if (checkCoordinates(row, col)) {
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
          Serial.println("Cell is already in use!");        
        }
      } else {
        Serial.println("Wrong coordinates!");
      }
    } else {    
      Serial.println("Write reset to reset the game");
    }
  }
}

void clearVirtualBoard() {
  int i, j;
  
  // initialze virtual board
  for (i = 0; i < BOARD_SIZE; i++) {
    for(j = 0; j < BOARD_SIZE; j++) {
      virtual_board[i][j] = -1;
    }
  }
}

boolean checkCoordinates(int row, int col) {
  return (row >= 0 && row < BOARD_SIZE) && (col >= 0 && col < BOARD_SIZE);
}

boolean isCellAvailable(int row, int col) {
  return !getLedState(row, cast_column(col, LED_A)) &&
         !getLedState(row, cast_column(col, LED_B));
}

void checkForWinner() {
  // check columns
  boolean finish;
  int i, j;
  
  finish = false;
  
  char cell;
  
  // check rows
  for (i = 0; i < BOARD_SIZE && !finish; i++) {
    cell = virtual_board[i][0];
    
    if (cell != -1) {
      for (j = 1; j < BOARD_SIZE; j++) {
        if (cell != virtual_board[i][j]) break;
      }
      
      finish = (j == BOARD_SIZE); 
    }
  }
  
  // check columns
  for (i = 0; i < BOARD_SIZE && !finish; i++) {
    cell = virtual_board[0][i];
    
    if (cell != -1) {
      for (j = 1; j < BOARD_SIZE; j++) {
        if (cell != virtual_board[j][i]) break;
      }
      
      finish = (j == BOARD_SIZE); 
    }
  }
  
  // check diagonals
  if (!finish && virtual_board[0][0] != -1) {
    cell = virtual_board[0][0];
    for (i = 1; i < BOARD_SIZE && !finish; i++) {
      if (cell != virtual_board[i][i]) break;
    }
  
    finish = (i == BOARD_SIZE);
  }
  
  if (!finish && virtual_board[BOARD_SIZE-1][0] != -1) {
    cell = virtual_board[BOARD_SIZE-1][0];
    for (i = 1; i < BOARD_SIZE; i++) {
      if (cell != virtual_board[BOARD_SIZE - (1 + i)][i]) break;
    }
    
    finish = (i == BOARD_SIZE);
  }
  
  if (finish) {
    Serial.print("Winner: ");
    Serial.println(cell);
    play = false;
  } else if (turn_count == max_turns) {
    Serial.println("Draw");
    play = false;
  }
  
  return;
}

void setTurn(char new_turn) {
  if (new_turn == PLAYER_A) {
    digitalWrite(player_a_led, HIGH);
    digitalWrite(player_b_led, LOW);
  } else {
    digitalWrite(player_a_led, LOW);
    digitalWrite(player_b_led, HIGH);
  }
  
  turn = new_turn;
}
