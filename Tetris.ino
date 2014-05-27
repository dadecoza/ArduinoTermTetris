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

int board[20][10];

int block[7][4][4][4] = 
{
//l-Block
  {
      {
          {1,1,1,1},
          {0,0,0,0},
          {0,0,0,0},
          {0,0,0,0}
      },
      {
          {1,0,0,0},
          {1,0,0,0},
          {1,0,0,0},
          {1,0,0,0}
      },
      {
          {1,1,1,1},
          {0,0,0,0},
          {0,0,0,0},
          {0,0,0,0}
      },
      {
          {1,0,0,0},
          {1,0,0,0},
          {1,0,0,0},
          {1,0,0,0}
      }
  },	
//J-Block
  {
      {
          {1,1,1,0},
          {0,0,1,0},
          {0,0,0,0},
          {0,0,0,0}
      },
      {
          {0,1,0,0},
          {0,1,0,0},
          {1,1,0,0},
          {0,0,0,0}
      },
      {
          {1,0,0,0},
          {1,1,1,0},
          {0,0,0,0},
          {0,0,0,0}
      },
      {
          {1,1,0,0},
          {1,0,0,0},
          {1,0,0,0},
          {0,0,0,0}
      }
  },
//L-Block
  {
      {
          {1,1,1,0},
          {1,0,0,0},
          {0,0,0,0},
          {0,0,0,0}
      },
      {
          {1,1,0,0},
          {0,1,0,0},
          {0,1,0,0},
          {0,0,0,0}
      },
      {
          {0,0,1,0},
          {1,1,1,0},
          {0,0,0,0},
          {0,0,0,0}
      },
      {
          {1,0,0,0},
          {1,0,0,0},
          {1,1,0,0},
          {0,0,0,0}
      }
  },
//O-Block
  {
      {
          {1,1,0,0},
          {1,1,0,0},
          {0,0,0,0},
          {0,0,0,0}
      },
      {
          {1,1,0,0},
          {1,1,0,0},
          {0,0,0,0},
          {0,0,0,0}
      },
      {
          {1,1,0,0},
          {1,1,0,0},
          {0,0,0,0},
          {0,0,0,0}
      },
      {
          {1,1,0,0},
          {1,1,0,0},
          {0,0,0,0},
          {0,0,0,0}
      }
  },
//S-Block
  {
      {
          {0,1,1,0},
          {1,1,0,0},
          {0,0,0,0},
          {0,0,0,0}
      },
      {
          {1,0,0,0},
          {1,1,0,0},
          {0,1,0,0},
          {0,0,0,0}
      },
      {
          {0,1,1,0},
          {1,1,0,0},
          {0,0,0,0},
          {0,0,0,0}
      },
      {
          {1,0,0,0},
          {1,1,0,0},
          {0,1,0,0},
          {0,0,0,0}
      }
  },
//T-Block
  {
      {
          {1,1,1,0},
          {0,1,0,0},
          {0,0,0,0},
          {0,0,0,0}
      },
      {
          {0,1,0,0},
          {1,1,0,0},
          {0,1,0,0},
          {0,0,0,0}
      },
      {
          {0,1,0,0},
          {1,1,1,0},
          {0,0,0,0},
          {0,0,0,0}
      },
      {
          {1,0,0,0},
          {1,1,0,0},
          {1,0,0,0},
          {0,0,0,0}
      }
  },
//Z-Block
  {
      {
          {1,1,0,0},
          {0,1,1,0},
          {0,0,0,0},
          {0,0,0,0}
      },
      {
          {0,1,0,0},
          {1,1,0,0},
          {1,0,0,0},
          {0,0,0,0}
      },
      {
          {1,1,0,0},
          {0,1,1,0},
          {0,0,0,0},
          {0,0,0,0}
      },
      {
          {0,1,0,0},
          {1,1,0,0},
          {1,0,0,0},
          {0,0,0,0}
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
  while (gameOver == 0) {
    posY=0;
    posX=4;
    rotation=0;
    currentBlock = rand() % 7;
    clear_lines();
    draw_block();
    int cont = 1;
    while (cont == 1) {
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
          while (move_down() == 1) {}
        }
      } else {
        cont = move_down();
      }
      delay(60 - (10*level));
    }
    if (posY < 1) gameOver = 1;         
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
      board[y][x] = 0;
    }
  }
}

void clear_lines() {
  int x,y,i,n;
  int newBoard[20][10];
  
  for (y=0;y<20;y++) {
    for (x=0;x<10;x++) {
      newBoard[y][x] = 0;
    }
  }
    
  int ni=19;
  for (i=19;i>=0;i--) {
    int line=1;
    for (n=0;n<10;n++) {
      if (board[i][n] == 0) {
        line = 0;
      }
    }
    if (line == 0) {
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
      if (board[y][x] != 0) {
        Serial.print("#");
      } else {
        Serial.print(".");
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
      if (block[currentBlock][rotation][i][n] != 0) {
        board[i+posY][n+posX] = 1;
      }
    }
  }
}

void clear_block() {
  int i,n;
  for (i=0;i<4;i++) {
    for (n=0;n<4;n++) {
      if (block[currentBlock][rotation][i][n] != 0) {
        board[i+posY][n+posX] = 0;
      }
    }
  }
}

int collision_detected(int y,int x) {
  int i,n;
  for (i=0;i<4;i++) {
    for (n=0;n<4;n++) {
      if (block[currentBlock][rotation][i][n] != 0) {
        if ((i+y) >= 20) {
          return 1;
        } else if ((n+x) >= 10) {
          return 1;
        } else if ((n+x) < 0) {
          return 1;
        } else if ((overlap(i+y,n+x) == 0) && (board[i+y][n+x] != 0)) {
          return 1;
        }
      }
    }
  }
  return 0;
}

int overlap(int y,int x) {
  int diffX = 0;
  int diffY = 0;
  if ((y >= posY)&&(y < posY+4)) {
    diffY = y-posY;
  } else {
    return 0;
  }
  
  if ((x >= posX)&&(x < posX+4)) {
    diffX = x-posX;
  } else {
    return 0;
  }
  
  if (block[currentBlock][rotation][diffY][diffX] != 0) {
      return 1;
  }
  
  return 0;
}

int move_down() {
  if (collision_detected(posY+1,posX) != 1) {
    clear_block();
    posY++;
    draw_block();
    return 1;
  }
  return 0;
}

int move_right() {
  if (collision_detected(posY,posX+1) != 1) {
    clear_block();
    posX++;
    draw_block();
    return 1;
  }
  return 0;
}

int move_left() {
  if (collision_detected(posY,posX-1) != 1) {
    clear_block();
    posX--;
    draw_block();
    return 1;
  }
  return 0;
}

void rotate() {
  clear_block();
  int p = rotation;
  rotation++;
  if (rotation > 3) {
    rotation = 0;
  }
  if (collision_detected(posY,posX) == 1) {
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
