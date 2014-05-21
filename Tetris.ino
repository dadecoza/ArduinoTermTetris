//#include <time.h>
#include <stdio.h>
#include <stdlib.h>


int posX = 5;
int posY = 5;
int currentBlock = 0;
int rotation = 0;

int score = 0;

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
    
    start();
}

void start() {
    //initialize board
    init_board();


    posX = 3;
    posY = 0;
    currentBlock = 0;
    rotation = 0;
    score = 0;
    
    //reset terminal
    Serial.write(27);
    Serial.print("c");

    //disable line wrap
    Serial.write(27);
    Serial.print("[7l");
    
    //hide cursor
    Serial.write(27);
    Serial.print("[?25l");
    
    //clear screen
    Serial.write(27);
    Serial.print("[2J"); 
    
    //home cursor
    Serial.write(27);
    Serial.print("[H");
    
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

    //printf("%c[5;5H", (char) 27);
    //srand(time(NULL));

}


void loop() {
        posY=0;
	posX=4;
        rotation=0;
        currentBlock = rand() % 7;
        draw_block();
        int cont = 1;
	while (cont == 1) {
            Serial.write(27);
            Serial.print("[H");
            clear_lines();
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
            }
            cont = move_down();
            delay(20);
        }
        score++;    
}

int init_board(void) {
    int x,y;
    for (y=0;y<20;y++) {
        for (x=0;x<10;x++) {
            board[y][x] = 0;
        }
    }
}

int clear_lines(void) {
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
        }
    }
 
    for (i=0;i<20;i++) {
        for (n=0;n<10;n++) {
	  board[i][n] = newBoard[i][n];
        }
    }
}

int draw_board(void) {
    int x,y;
    for (y=0;y<20;y++) {
        for (x=0;x<10;x++) {
            if (board[y][x] != 0) {
                Serial.print("#");
            } else {
                Serial.print(".");
	    }
	}
        Serial.println();
    }
}

int draw_block(void) {
    int i,n;
    for (i=0;i<4;i++)
        for (n=0;n<4;n++)
            if (block[currentBlock][rotation][i][n] != 0)
                board[i+posY][n+posX] = 1;
}


int clear_block(void) {
    int i,n;
    for (i=0;i<4;i++)
        for (n=0;n<4;n++)
            if (block[currentBlock][rotation][i][n] != 0)
                board[i+posY][n+posX] = 0;    
}

int collision_detected(int y,int x) {
    int i,n;
    for (i=0;i<4;i++) {
        for (n=0;n<4;n++) {
            if (block[currentBlock][rotation][i][n] != 0) {
                if ((i+y) >= 20)
                    return 1;
                else if ((n+x) >= 10)
                    return 1;
                else if ((n+x) < 0)
                    return 1;
                else if ((overlap(i+y,n+x) == 0) && (board[i+y][n+x] != 0))
                    return 1;
            }
        }
    }
    return 0;
}

int overlap(int y,int x) {
    int diffX = 0;
    int diffY = 0;
    if ((y >= posY)&&(y < posY+4))
        diffY = y-posY;
    else
        return 0;
    
    if ((x >= posX)&&(x < posX+4))
        diffX = x-posX;
    else
        return 0;
    
    if (block[currentBlock][rotation][diffY][diffX] != 0)
        return 1;
    
    return 0;
}

int move_down(void) {
    if (collision_detected(posY+1,posX) != 1) {
        clear_block();
        posY++;
        draw_block();
        return 1;
    }
    return 0;
}

int move_right(void) {
    if (collision_detected(posY,posX+1) != 1) {
        clear_block();
        posX++;
        draw_block();
        return 1;
    }
    return 0;
}

int move_left(void) {
    if (collision_detected(posY,posX-1) != 1) {
        clear_block();
        posX--;
        draw_block();
        return 1;
    }
    return 0;
}

int rotate(void) {
	clear_block();
	int p = rotation;
	rotation++;
	if (rotation > 3)
		rotation = 0;
	if (collision_detected(posY,posX) == 1) {
		rotation = p;
	}
	draw_block();
}
