#include <OLED_I2C.h> //Библиотека для работы с OLED экраном

OLED myOLED(SDA, SCL); //Инициализация экрана 
const unsigned long padWidth = 15;
const uint8_t player_y = 55;
int blocks;
int lives;
int pot;
int playerScore = 0;
int padScrl;
bool resetBall = false;
unsigned int blocks_x [50] = {3, 15, 27, 39, 51, 63, 75, 87, 3, 15, 27, 39, 51, 63, 75, 87, 3, 15, 27, 39, 51, 63, 75, 87};
unsigned int blocks_y [50] = {3, 3, 3, 3, 3, 3, 3, 3, 10, 10, 10, 10, 10, 10, 10, 10, 17, 17, 17, 17, 17, 17, 17, 17, };

extern uint8_t SmallFont[]; //Подключение малого и большого шрифтов
extern uint8_t MegaFont[];

uint8_t ball_x, ball_y = 55;
uint8_t ball_dir_x = 1, ball_dir_y = 1;

unsigned long ball_update;
unsigned long paddle_update;
//long stime, fps = 0, frames = 0;

void drawCourt();
void drawBlock();
void killBlock();
void gameRestart();
void gameOver();
void drawLogo();
void drawMenu();

void setup() {
  //stime = micros();
  myOLED.setFont(SmallFont);
  blocks = 24;
  lives = 3;
  unsigned long start = millis();
  myOLED.begin();
  myOLED.setFont(SmallFont);
  randomSeed(analogRead(7));
  myOLED.update();
  ball_update = millis();
  paddle_update = ball_update;
  randomSeed(analogRead(1));
  resetBall = true;
}

void loop() {
  bool update = false;
  unsigned long time = millis();
  pot = analogRead(A0);
  padScrl = map(pot, 0, 1023, 1, 85);
  //myOLED.printNumI(padScrl, CENTER, 0);

  drawCourt();
  drawHeart();
  myOLED.update();
  myOLED.clrScr();
  if(blocks == 0){
    gameRestart();
  }

  if(lives == 0){
    gameOver();
    gameRestart();
  }
  
  if(resetBall){
    drawCourt();
    myOLED.update();
    myOLED.clrScr();
    ball_x = padScrl+7;
    ball_y = 54;
    do{
      ball_dir_x = random(-1, 2);
    }while(ball_dir_x == 0);

    do{
      ball_dir_y = random(-1, 2);
    }while(ball_dir_y == 0);

    resetBall = false;
  }

  if(time > ball_update){
    uint8_t new_x = ball_x + ball_dir_x;
    uint8_t new_y = ball_y + ball_dir_y;

    if(new_x == 0 || new_x == 100){
      ball_dir_x = -ball_dir_x;
      new_x += ball_dir_x + ball_dir_x;
    }

    if(new_y == 0){
      ball_dir_y = -ball_dir_y;
      new_y += ball_dir_y + ball_dir_y;
    }

    if (new_y == 63) {
      lives--;
      resetBall = true;
    }

    if(new_y == player_y && new_x >= padScrl && new_x <= padScrl+15){
      ball_dir_y = -ball_dir_y;
      new_y += ball_dir_y + ball_dir_y;
    }

    for(int i = 0; i < blocks; i++){ 
      if(new_y == blocks_y[i] || new_y == blocks_y[i]+5){
        if(new_x >= blocks_x[i] && new_x <= blocks_x[i]+10){
          killBlock(i);
          playerScore = playerScore + 2;
          ball_dir_y = -ball_dir_y;
          new_y += ball_dir_y + ball_dir_y;
        }
      }
      if(new_x == blocks_x[i] && new_y >= blocks_y[i] && new_y <= blocks_y[i]+5){
        killBlock(i);
        playerScore = playerScore + 2;
        ball_dir_x = -ball_dir_x;
        new_x += ball_dir_x + ball_dir_x;
      }
      if(new_x == blocks_x[i]+10 && new_y >= blocks_y[i] && new_y <= blocks_y[i]+5){
        killBlock(i);
        playerScore = playerScore + 2;
        ball_dir_x = -ball_dir_x;
        new_x += ball_dir_x + ball_dir_x;
      }
    }

    myOLED.clrPixel(ball_x, ball_y);
    myOLED.setPixel(new_x, new_y);
    ball_x = new_x;
    ball_y = new_y;

    update = true;
  }

  if(time > paddle_update){
    myOLED.drawLine(padScrl, player_y, padScrl+15, player_y);
  }

  for(int i = 0; i < blocks; i++){
    drawBlock(i);
  }
  /*fps += 1000000 / (micros() - stime);
  stime = micros();
  frames++;
  myOLED.printNumI(fps / frames, 0, 0, 3);*/
  //delay(700);
}
void drawBlock(int i){
  myOLED.drawLine(blocks_x[i], blocks_y[i], blocks_x[i]+10, blocks_y[i]);
  myOLED.drawLine(blocks_x[i], blocks_y[i], blocks_x[i], blocks_y[i]+5);
  myOLED.drawLine(blocks_x[i]+10, blocks_y[i], blocks_x[i]+10, blocks_y[i]+5);
  myOLED.drawLine(blocks_x[i], blocks_y[i]+5, blocks_x[i]+11, blocks_y[i]+5);
}

void killBlock(int _i){
  for (int i = _i; i < blocks; ++i){
    blocks_x[i] = blocks_x[i + 1];
    blocks_y[i] = blocks_y[i + 1];
  }
  --blocks;
}

void drawCourt(){
  myOLED.drawLine(0, 0, 127, 0);
  myOLED.drawLine(0, 0, 0, 64);
  myOLED.drawLine(127, 63, 0, 63);
  myOLED.drawLine(127, 64, 127, 0);
  myOLED.drawLine(100, 0, 100, 63);
  myOLED.printNumI(lives, 102, 3);
  myOLED.print("scr:", 102, 10);
  myOLED.printNumI(playerScore, 102, 19);
}

void drawHeart(){
  myOLED.setPixel(111, 3);
  myOLED.setPixel(112, 3);
  myOLED.setPixel(114, 3);
  myOLED.setPixel(115, 3);
  myOLED.setPixel(110, 4);
  myOLED.setPixel(111, 4);
  myOLED.setPixel(112, 4);
  myOLED.setPixel(113, 4);
  myOLED.setPixel(114, 4);
  myOLED.setPixel(115, 4);
  myOLED.setPixel(116, 4);
  myOLED.setPixel(110, 5);
  myOLED.setPixel(111, 5);
  myOLED.setPixel(112, 5);
  myOLED.setPixel(113, 5);
  myOLED.setPixel(114, 5);
  myOLED.setPixel(115, 5);
  myOLED.setPixel(116, 5);  
  myOLED.setPixel(111, 6);
  myOLED.setPixel(112, 6);
  myOLED.setPixel(113, 6);
  myOLED.setPixel(114, 6);
  myOLED.setPixel(115, 6);
  myOLED.setPixel(112, 7);
  myOLED.setPixel(113, 7);
  myOLED.setPixel(114, 7);
  myOLED.setPixel(113, 8);
}

void drawLogo(){
  
}

void drawMenu(){
  
}

void gameWin(){
  myOLED.clrScr();
  myOLED.update();
  myOLED.print("YOU WIN", CENTER, 32);
}

void gameOver(){
  myOLED.clrScr();
  myOLED.update();
  myOLED.print("GAME OVER", CENTER, 32);
  myOLED.invert(true);
  myOLED.update();
  delay(500);
  myOLED.invert(false);
  myOLED.update();
  delay(500);
  myOLED.invert(true);
  myOLED.update();
  delay(500);
  myOLED.invert(false);
  myOLED.update();
  delay(500);
  myOLED.invert(true);
  myOLED.update();
  delay(500);
  myOLED.invert(false);
  myOLED.update();
  delay(500);
  myOLED.invert(true);
  myOLED.update();
  delay(500);
  myOLED.clrScr();
  myOLED.invert(false);
  myOLED.update();
  delay(1000);
}

void gameRestart(){
  asm volatile ("jmp 0");
}