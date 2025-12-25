#ifndef FACE_H
#define FACE_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

class Face {
public:
  Face(Adafruit_SSD1306 *display);

  void drawOpenEye(int x, int y, int r = 5);
  void drawClosedEye(int x, int y);
  void drawDeadEye(int x, int y);
  void drawConfusedEye(int x, int y, int r = 5);
  void drawSupprisedLeftEye(int x, int y);
  void drawSupprisedRightEye(int x, int y);
  void drawAngeredLeftEye(int x, int y);
  void drawAngeredRightEye(int x, int y);
  void drawSquintedEye(int x, int y, int r = 5);
  void drawHappyMouth(int x, int y);
  void drawSupprisedMouth(int x, int y);
  void drawAngeredMouth(int x, int y);

  void happy(int x, int y);
  void supprised(int x, int y);
  void angered(int x, int y);
  void confused(int x, int y);
  void dead(int x, int y);

private:
  Adafruit_SSD1306 *display;
};

#endif
