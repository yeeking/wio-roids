#include <LovyanGFX.hpp>
#include "Asteroids.h"

static LGFX lcd;                 
//static LGFX_Sprite arc(&lcd); 
static LGFX_Sprite scrBuff(&lcd); 

static auto transpalette = 0;           

AsteroidsGame* game;
bool ready;
bool noFire;

int x, y, speed; 

void setup() {
  ready = false;
  noFire = false;
    x = 0;
    y = 0; 
    speed = 20;
    Serial.begin(115200);
    pinMode(WIO_KEY_A, INPUT_PULLUP);
    pinMode(WIO_KEY_B, INPUT_PULLUP);
    pinMode(WIO_KEY_C, INPUT_PULLUP);
    pinMode(WIO_5S_UP, INPUT_PULLUP);
    pinMode(WIO_5S_DOWN, INPUT_PULLUP);
    pinMode(WIO_5S_LEFT, INPUT_PULLUP);
    pinMode(WIO_5S_RIGHT, INPUT_PULLUP);
    pinMode(WIO_5S_PRESS, INPUT_PULLUP);
    pinMode(WIO_BUZZER, OUTPUT);

    lcd.init();
    lcd.setRotation(1);
    lcd.setBrightness(255);
    lcd.setColorDepth(16);
    lcd.clear();
    lcd.fillScreen(TFT_BLACK);

    scrBuff.setColorDepth(16);
    scrBuff.createSprite(320, 240);

  //  arc.fillScreen(transpalette);
  //  arc.fillArc(80,80,60,80,0,360,0xC424);
  //  arc.setPivot(80,80);
    lcd.setTextColor(0xFFE0, 0);
    
    pinMode(WIO_BUZZER, OUTPUT);

    //randomSeed(analogRead(0));
}
 
 /** draw the sent ship to the sent screen buffer*/
void drawShip(LGFX_Sprite* scrBuff, Ship* ship)
{
  Vec2D* pos = ship->getPos();
  int32_t x = (int32_t) pos->x;
  int32_t y = (int32_t) pos->y;
  int32_t size = 10;
  scrBuff->drawTriangle(x, y-size, x-size, y, x+size, y, 0x0F0F);
  Vec2D pos2 = Vec2D::solveTriangle(ship->getAngle(), 10);
  scrBuff->fillRect(pos2.x + x, pos2.y + y - 10, 8, 8, 0xF00F);
  
  //scrBuff->drawRect(0, 0, 50, 50, 0xFFFF);
}

void drawBullets(LGFX_Sprite* scrBuff, Bullet* bullets[], int bulletCount)
{
  Vec2D* pos;
  int32_t x;//
  int32_t y;//
  int32_t size = 6;
  for (auto i=0;i<bulletCount;++i)
  {
    //if (bullets[i]->getVisible())
    if (true)
    {
      Bullet* bull = bullets[i];
      pos = bull->getPos();
      x = (int32_t) pos->x;
      y = (int32_t) pos->y;
      scrBuff->fillEllipse(x, y, size, size, 0x00F);
    }
  }
  
}

void drawAsteroids(LGFX_Sprite* scrBuff, Asteroid* asteroids[], int asteroidCount)
{
  Vec2D* pos;
  int32_t x;//
  int32_t y;//
  int32_t size = 10;
  for (auto i=0;i<asteroidCount;i++)
  {
    if (asteroids[i]->getVisible())
    {
      Asteroid* roid = asteroids[i];
      pos = roid->getPos();
      x = (int32_t) pos->x;
      y = (int32_t) pos->y;
      scrBuff->drawEllipse(x, y, size, size, 0xFFFF);
    }
  }
  
}
void loop() {
  if (!ready)
  {
    if (digitalRead(WIO_KEY_C) == LOW)  {   //  C button to fire
      ready = true;
      // the trick, william potter is to 
      // seed the random number generator
      srand (millis());  
      game = new AsteroidsGame(320, 240);
    }
  }
  else {
    if (digitalRead(WIO_KEY_C) == LOW && !noFire)  {   //  C button to fire
      //  lcd.fillScreen(TFT_BLACK); 
      game->ship->fire();     
      noFire = true;
      //while(digitalRead(WIO_KEY_C) == LOW);
    }
    if (digitalRead(WIO_KEY_C) == HIGH)  {   //  C button to fire
      noFire = false;
    }
    if (digitalRead(WIO_KEY_B) == LOW && !noFire)  {   //  C button to fire
      game->ship->applyThrust(0.02);    
    }
   
    if (digitalRead(WIO_5S_RIGHT) == LOW) {   
      game->ship->rotateRight(0.1);
    }
    if (digitalRead(WIO_5S_LEFT) == LOW) {  
      game->ship->rotateLeft(0.1);
    }
    if (digitalRead(WIO_5S_UP) == LOW) {    // increase figure on cursor
    //lcd.fillScreen(TFT_BLUE);
      //y-=speed;
      //while(digitalRead(WIO_5S_UP) == LOW);// y-=speed;
    }
    if (digitalRead(WIO_5S_DOWN) == LOW) {    // decrease figure on cursor
    //lcd.fillScreen(TFT_YELLOW);
      //y+=speed;
      //  while(digitalRead(WIO_5S_DOWN) == LOW);// y+= speed;    
    }
    scrBuff.clear();
    
    if (game->gameOver)
    {
      scrBuff.setCursor(50, 50);
      scrBuff.setTextColor(0xF00F, 0); 
      scrBuff.setTextSize(4);
      scrBuff.printf("GAME OVER!");
      
    }
    else 
    {
      //lcd.fillScreen(TFT_BLACK);
      game->update();
      drawShip(&scrBuff, game->ship);
      drawAsteroids(&scrBuff, game->asteroids, game->asteroidCount);
      drawBullets(&scrBuff, game->ship->bullets, game->ship->bulletCount);
    }
    scrBuff.setCursor(0, 0);
    scrBuff.setTextColor(0xFFE0, 0); 
    scrBuff.setTextSize(3);
    scrBuff.printf("%i:%i", game->lives, game->score); 
    
    //drawBullets()
    scrBuff.pushSprite(&lcd, 0, 0);
  } 
}   //  end of loop