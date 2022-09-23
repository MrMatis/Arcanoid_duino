/*
  Arcanoid_h - Библиотека для реализации графики Arcanoid.
  Написал Максим М.Э. 16 марта 2022.
  Не релизнул, я жадный :P

//#ifndef Arcanoid_h
#define Arcanoid_h
#include <OLED_I2C.h>

OLED myOLED(SDA, SCL);
class Draw {
  public:
    int x;
    int y;
    void drawBlock() {
      myOLED.drawLine(x, y, x+5, y);
      myOLED.drawLine(x, y, x, y+2);
      myOLED.drawLine(x+5, y, x, y+2);
      myOLED.drawLine(x, y+2, x+5, y);
    }

    
    void drawCourt() {
      myOLED.drawLine(0, 0, 127, 0);
      myOLED.drawLine(0, 0, 0, 64);
      myOLED.drawLine(127, 63, 0, 63);
      myOLED.drawLine(127, 64, 127, 0);
    }
};
*/
