#include <TVout.h>
#include <fontALL.h>
#include <stdlib.h>
TVout TV;

#define WHEEL_ONE_PIN 0 //analog
#define WHEEL_TWO_PIN 1 //analog
#define BUTTON_ONE_PIN 2 //digital
#define TVvert_res 96
#define TVhorz_res 120
#define PADDLE_HEIGHT 12
#define PADDLE_WIDTH 2
#define IN_GAME 0 //in game state
#define IN_MENU 1 //in menu state
#define GAME_OVER 2 //game over state

#define RIGHT_PADDLE_X (TVhorz_res-4)
#define LEFT_PADDLE_X 2

#define LEFT_SCORE_X (TVhorz_res/2-15)
#define RIGHT_SCORE_X (TVhorz_res/2+10)
#define SCORE_Y 4
 
#define MAX_Y_VELOCITY 3
#define PLAY_TO 3
unsigned char x,y;
int Player1 = 0;
int Player2 = 0;
int P1Y = 0;
int P2Y = 0;
unsigned char ballX = 0;
unsigned char ballY = 0;
int P1S = 0;
int P2S = 0;
int rightPaddleY = 0;
int leftPaddleY = 0;
int frame = 0;
int leftPlayerScore = 0;
int rightPlayerScore = 0;
#define LEFT 0
#define RIGHT 1
char ballVolX = 1;
char ballVolY = 1;
int state = IN_GAME;
void Entradas(){
  P1Y = analogRead(WHEEL_ONE_PIN);
  P2Y = analogRead(WHEEL_TWO_PIN);
 /* Serial.print(P1Y);
  Serial.print(" ");
  Serial.print(P2Y);
  Serial.print("\n");
  //P1Y = map(P1Y,65,965,0,1023);
 // P2Y = map(P2Y,64,963,0,1023);
 /* if(P1Y<0)P1Y=0;
  if(P2Y<0)P2Y=0;
  if(P1Y>1023)P1Y=1023;
  if(P2Y>1023)P2Y=1023;*/
}

void drawGameScreen() {
  TV.clear_screen();
  //draw right paddle
  rightPaddleY = ((P1Y/ 8) * (TVvert_res-PADDLE_HEIGHT))/ 128;
  x = RIGHT_PADDLE_X;
  for(int i=0; i<PADDLE_WIDTH; i++) {
    TV.draw_line(x+i,rightPaddleY,x+i,rightPaddleY+PADDLE_HEIGHT,1);
  }


  TV.draw_rect(0,0,TVhorz_res-1,TVvert_res-1,WHITE);
 
  //draw left paddle
  leftPaddleY = ((P2Y / 8) * (TVvert_res-PADDLE_HEIGHT))/ 128;
  x = LEFT_PADDLE_X;
  for(int i=0; i<PADDLE_WIDTH; i++) {
    TV.draw_line(x+i,leftPaddleY,x+i,leftPaddleY+PADDLE_HEIGHT,1);
  }
 //TV.select_font(font4x6);
  TV.println(LEFT_SCORE_X,SCORE_Y, itoa(leftPlayerScore,2,10));
  TV.println(RIGHT_SCORE_X,SCORE_Y, itoa(rightPlayerScore,2,10));

  for(int i=1; i<TVvert_res - 4; i+=6) {
    TV.draw_line(TVhorz_res/2,i,TVhorz_res/2,i+3, 1);
  }

  //draw ball
  TV.set_pixel(ballX, ballY, 1);
}

void playerScored(byte player) {
  if(player == LEFT) leftPlayerScore++;
  if(player == RIGHT) rightPlayerScore++;
  TV.tone( 500,300 );
  
 
  ballVolX = -ballVolX;
}


void setup()  {
  Serial.begin(9600);
  x=0;
  y=0;
  TV.begin(PAL,120,96);
  ballX = TVhorz_res / 2;
  ballY = TVvert_res / 2;
  TV.select_font(font6x8);
  TV.print(15,30, "WELCOME PLAYERS:");
  TV.print(5,65,"GET READY TO FIGHT!");
  TV.delay_frame(300);
 
}  

void loop() {
Entradas();

     ballX += ballVolX;
      ballY += ballVolY;
 
      if(ballY <= 1 || ballY >= TVvert_res-1) {
        ballVolY = -ballVolY;
        TV.tone( 2000,30  );
      }
      if(ballVolX < 0 && ballX == LEFT_PADDLE_X+PADDLE_WIDTH-1 && ballY >= leftPaddleY && ballY <= leftPaddleY + PADDLE_HEIGHT) {
        ballVolX = -ballVolX;
        ballVolY += 2 * ((ballY - leftPaddleY) - (PADDLE_HEIGHT / 2)) / (PADDLE_HEIGHT / 2);
        TV.tone( 2000,30  );
      }
      if(ballVolX > 0 && ballX == RIGHT_PADDLE_X && ballY >= rightPaddleY && ballY <= rightPaddleY + PADDLE_HEIGHT) {
        ballVolX = -ballVolX;
        ballVolY += 2 * ((ballY - rightPaddleY) - (PADDLE_HEIGHT / 2)) / (PADDLE_HEIGHT / 2);
        TV.tone( 2000,30  );
      }
 
      //limit vertical speed
      if(ballVolY > MAX_Y_VELOCITY) ballVolY = MAX_Y_VELOCITY;
      if(ballVolY < -MAX_Y_VELOCITY) ballVolY = -MAX_Y_VELOCITY;
 
      if(ballX <= 1) {
        playerScored(RIGHT);
        if(leftPlayerScore == PLAY_TO || rightPlayerScore == PLAY_TO) {
          TV.print(30,30, "GAME  OVER");
          TV.print(16,55, "PLAYER");
          if(leftPlayerScore>rightPlayerScore){ TV.print(45,55," ONE WINS");}
          else {TV.print(45,55," TWO WINS");}
          TV.println(LEFT_SCORE_X,SCORE_Y, itoa(leftPlayerScore,2,10));
          TV.println(RIGHT_SCORE_X,SCORE_Y, itoa(rightPlayerScore,2,10));
          TV.delay_frame(200);
    leftPlayerScore = 0;
    rightPlayerScore = 0;
   }
      }
      if(ballX >= TVhorz_res - 1) {
        playerScored(LEFT);
        if(leftPlayerScore == PLAY_TO || rightPlayerScore == PLAY_TO) {
          TV.print(30,30, "GAME  OVER");
          TV.print(16,55, "PLAYER");
          if(leftPlayerScore>rightPlayerScore){ TV.print(55,55," ONE WINS");}
          else {TV.print(55,55," TWO WINS");}
          TV.println(LEFT_SCORE_X,SCORE_Y, itoa(leftPlayerScore,2,10));
          TV.println(RIGHT_SCORE_X,SCORE_Y, itoa(rightPlayerScore,2,10));
          TV.delay_frame (200);
    leftPlayerScore = 0;
    rightPlayerScore = 0;
   }
      }
      
   drawGameScreen();
   
  TV.delay_frame(1);
  if(++frame == 60) frame = 0; 
}
