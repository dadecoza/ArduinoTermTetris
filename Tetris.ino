/*
  Terminal Tetris
  
  This sketch is a game of Tetris that you can play in a serial terminal.
  
  Keys: 
    i = left
    p = right
    o = rotate
    space = drop
    
 baud: 9600
 
 created 23 May  2014
 by Johannes le Roux (dade@dade.co.za) 
*/


#include <stdio.h>
#include <stdlib.h>
#include <EEPROM.h>


int posX,posY,currentBlock,rotation,gameOver,score,highScore,level;

boolean board[20][10];

boolean const block[7][4][4][4]= 
{
//l-Block
  {
      {
          {true,true,true,true},
          {false,false,false,false},
          {false,false,false,false},
          {false,false,false,false}
      },
      {
          {true,false,false,false},
          {true,false,false,false},
          {true,false,false,false},
          {true,false,false,false}
      },
      {
          {true,true,true,true},
          {false,false,false,false},
          {false,false,false,false},
          {false,false,false,false}
      },
      {
          {true,false,false,false},
          {true,false,false,false},
          {true,false,false,false},
          {true,false,false,false}
      }
  },	
//J-Block
  {
      {
          {true,true,true,false},
          {false,false,true,false},
          {false,false,false,false},
          {false,false,false,false}
      },
      {
          {false,true,false,false},
          {false,true,false,false},
          {true,true,false,false},
          {false,false,false,false}
      },
      {
          {true,false,false,false},
          {true,true,true,false},
          {false,false,false,false},
          {false,false,false,false}
      },
      {
          {true,true,false,false},
          {true,false,false,false},
          {true,false,false,false},
          {false,false,false,false}
      }
  },
//L-Block
  {
      {
          {true,true,true,false},
          {true,false,false,false},
          {false,false,false,false},
          {false,false,false,false}
      },
      {
          {true,true,false,false},
          {false,true,false,false},
          {false,true,false,false},
          {false,false,false,false}
      },
      {
          {false,false,true,false},
          {true,true,true,false},
          {false,false,false,false},
          {false,false,false,false}
      },
      {
          {true,false,false,false},
          {true,false,false,false},
          {true,true,false,false},
          {false,false,false,false}
      }
  },
//O-Block
  {
      {
          {true,true,false,false},
          {true,true,false,false},
          {false,false,false,false},
          {false,false,false,false}
      },
      {
          {true,true,false,false},
          {true,true,false,false},
          {false,false,false,false},
          {false,false,false,false}
      },
      {
          {true,true,false,false},
          {true,true,false,false},
          {false,false,false,false},
          {false,false,false,false}
      },
      {
          {true,true,false,false},
          {true,true,false,false},
          {false,false,false,false},
          {false,false,false,false}
      }
  },
//S-Block
  {
      {
          {false,true,true,false},
          {true,true,false,false},
          {false,false,false,false},
          {false,false,false,false}
      },
      {
          {true,false,false,false},
          {true,true,false,false},
          {false,true,false,false},
          {false,false,false,false}
      },
      {
          {false,true,true,false},
          {true,true,false,false},
          {false,false,false,false},
          {false,false,false,false}
      },
      {
          {true,false,false,false},
          {true,true,false,false},
          {false,true,false,false},
          {false,false,false,false}
      }
  },
//T-Block
  {
      {
          {true,true,true,false},
          {false,true,false,false},
          {false,false,false,false},
          {false,false,false,false}
      },
      {
          {false,true,false,false},
          {true,true,false,false},
          {false,true,false,false},
          {false,false,false,false}
      },
      {
          {false,true,false,false},
          {true,true,true,false},
          {false,false,false,false},
          {false,false,false,false}
      },
      {
          {true,false,false,false},
          {true,true,false,false},
          {true,false,false,false},
          {false,false,false,false}
      }
  },
//Z-Block
  {
      {
          {true,true,false,false},
          {false,true,true,false},
          {false,false,false,false},
          {false,false,false,false}
      },
      {
          {false,true,false,false},
          {true,true,false,false},
          {true,false,false,false},
          {false,false,false,false}
      },
      {
          {true,true,false,false},
          {false,true,true,false},
          {false,false,false,false},
          {false,false,false,false}
      },
      {
          {false,true,false,false},
          {true,true,false,false},
          {true,false,false,false},
          {false,false,false,false}
      }
  }
}; 

void setup()
{
  Serial.begin(9600);
  delay(100);
  //reset terminal
  Serial.write(27);
  Serial.print("c");

  //disable line wrap
  Serial.write(27);
  Serial.print("[7l");
  start();
}

void start() {
  //initialize board
  init_board();

  //initialize variables
  posX = 3;
  posY = 0;
  currentBlock = 0;
  rotation = 0;
  score = 0;
  level = 1;
  gameOver = 0;
  
  //save_high_score(); //just to initialize it the firstime
  highScore = get_high_score();
  
  
  //hide cursor
  Serial.write(27);
  Serial.print("[?25l");
  
  //clear screen;
  Serial.write(27);
  Serial.print("[2J");
  
  Serial.println("Press any key to start!");
  int c = 0;
  while( c== 0) {
    while (Serial.available() > 0) {
      c = Serial.read();
    }
  }
  
  //clear screen
  Serial.write(27);
  Serial.print("[2J"); 
  
  //home cursor
  Serial.write(27);
  Serial.print("[H");

  draw_board(); 
}

void loop() {
  while (!gameOver) {
    posY=0;
    posX=4;
    rotation=0;
    currentBlock = rand() % 7;
    clear_lines();
    draw_block();
    boolean cont = true;
    while (cont) {
      Serial.write(27);
      Serial.print("[H");
      draw_board();
      if (Serial.available() > 0) {
        int c = Serial.read();
        if (c == 105) {
          move_left();
        } else if (c == 112) {
          move_right();
        } else if (c == 111) {
          rotate();
        } else if (c == 32) {
          while (move_down()) {}
        }
      } else {
        cont = move_down();
      }
      delay(60 - (10*level));
    }
    if (posY < 1) gameOver = true;         
  }
  
  if (score > highScore) {
    save_high_score();
  }
  
  start();  
}

void init_board() {
  int x,y;
  for (y=0;y<20;y++) {
    for (x=0;x<10;x++) {
      board[y][x] = false;
    }
  }
}

void clear_lines() {
  int x,y,i,n;
  boolean newBoard[20][10];
  
  for (y=0;y<20;y++) {
    for (x=0;x<10;x++) {
      newBoard[y][x] = false;
    }
  }
    
  int ni=19;
  for (i=19;i>=0;i--) {
    boolean line = true;
    for (n=0;n<10;n++) {
      if (!board[i][n]) {
        line = false;
      }
    }
    if (!line) {
      for (n=0;n<10;n++) {
        newBoard[ni][n] = board[i][n];
      }
      ni--;
    } else {
      score++;
      if ((score % 10) == 0) {
        level++;
      }
    }
  }
 
  for (i=0;i<20;i++) {
    for (n=0;n<10;n++) {
      board[i][n] = newBoard[i][n];
    }
  }
}

void draw_board() {
  int x,y;
  for (y=0;y<20;y++) {
    for (x=0;x<10;x++) {
      if (board[y][x]) {
        Serial.print('#');
      } else {
        Serial.print('.');
      }
    }
    if (y == 2) {
      Serial.print(" Level:");
      Serial.println(level);
    } else if (y == 4) {
      Serial.print(" Score:");
      Serial.println(score);
    } else if (y == 6) {
      Serial.print(" High Score:");
      Serial.println(highScore);
    }else {
      Serial.println();
    }
  }
}

void draw_block() {
  int i,n;
  for (i=0;i<4;i++) {
    for (n=0;n<4;n++) {
      if (block[currentBlock][rotation][i][n]) {
        board[i+posY][n+posX] = true;
      }
    }
  }
}

void clear_block() {
  int i,n;
  for (i=0;i<4;i++) {
    for (n=0;n<4;n++) {
      if (block[currentBlock][rotation][i][n]) {
        board[i+posY][n+posX] = false;
      }
    }
  }
}

boolean collision_detected(int y,int x) {
  int i,n;
  for (i=0;i<4;i++) {
    for (n=0;n<4;n++) {
      if (block[currentBlock][rotation][i][n]) {
        if ((i+y) >= 20) {
          return true;
        } else if ((n+x) >= 10) {
          return true;
        } else if ((n+x) < 0) {
          return true;
        } else if ((overlap(i+y,n+x) == 0) && (board[i+y][n+x])) {
          return true;
        }
      }
    }
  }
  return false;
}

boolean overlap(int y,int x) {
  int diffX = 0;
  int diffY = 0;
  if ((y >= posY)&&(y < posY+4)) {
    diffY = y-posY;
  } else {
    return false;
  }
  
  if ((x >= posX)&&(x < posX+4)) {
    diffX = x-posX;
  } else {
    return false;
  }
  
  if (block[currentBlock][rotation][diffY][diffX]) {
      return true;
  }
  
  return false;
}

boolean move_down() {
  if (!collision_detected(posY+1,posX)) {
    clear_block();
    posY++;
    draw_block();
    return true;
  }
  return false;
}

boolean move_right() {
  if (!collision_detected(posY,posX+1)) {
    clear_block();
    posX++;
    draw_block();
    return true;
  }
  return false;
}

boolean move_left() {
  if (!collision_detected(posY,posX-1)) {
    clear_block();
    posX--;
    draw_block();
    return true;
  }
  return false;
}

void rotate() {
  clear_block();
  int p = rotation;
  rotation++;
  if (rotation > 3) {
    rotation = 0;
  }
  if (collision_detected(posY,posX)) {
    rotation = p;
  }
  draw_block();
}

int get_high_score() {
  long two = EEPROM.read(0);
  long one = EEPROM.read(1);
  return ((two << 0) & 0xFF) + ((one << 8) & 0xFFFF);
}


void save_high_score() {
  EEPROM.write(0, (score & 0xFF));
  EEPROM.write(1, ((score >> 8) & 0xFF));
}
