#include "face.h"

Face::Face(Adafruit_SSD1306 *display) { this->display = display; }

void Face::drawOpenEye(int x, int y, int r) {
  display->drawCircle(x, y, r, SSD1306_WHITE);
  display->fillCircle(x, y, r, SSD1306_WHITE);
}

void Face::drawClosedEye(int x, int y) {
  display->drawLine(x - 5, y - 1, x + 5, y - 1, SSD1306_WHITE);
  display->drawLine(x - 5, y, x + 5, y, SSD1306_WHITE);
  display->drawLine(x - 5, y + 1, x + 5, y + 1, SSD1306_WHITE);
}

void Face::drawDeadEye(int x, int y) {
  display->drawLine(x - 3, y - 3, x + 3, y + 3, SSD1306_WHITE);
  display->drawLine(x + 3, y - 3, x - 3, y + 3, SSD1306_WHITE);
}

void Face::drawConfusedEye(int x, int y, int r) {
  display->drawCircle(x, y, r, SSD1306_WHITE);

  display->drawCircle(x, y, r - 3, SSD1306_WHITE);

  display->drawPixel(x, y, SSD1306_WHITE);
  display->drawPixel(x + 1, y, SSD1306_WHITE);
  display->drawPixel(x, y + 1, SSD1306_WHITE);

  display->drawLine(x, y - r, x, y - 2, SSD1306_BLACK);
}

void Face::drawSupprisedLeftEye(int x, int y) {
  display->drawLine(x - 5, (y - 1) + 2, x + 5, (y - 1) - 2, SSD1306_WHITE);
  display->drawLine(x - 5, y + 2, x + 5, y - 2, SSD1306_WHITE);
  display->drawLine(x - 5, (y + 1) + 2, x + 5, (y + 1) - 2, SSD1306_WHITE);
}

void Face::drawSupprisedRightEye(int x, int y) {
  display->drawLine(x - 5, (y - 1) - 2, x + 5, (y - 1) + 2, SSD1306_WHITE);
  display->drawLine(x - 5, y - 2, x + 5, y + 2, SSD1306_WHITE);
  display->drawLine(x - 5, (y + 1) - 2, x + 5, (y + 1) + 2, SSD1306_WHITE);
}

void Face::drawAngeredLeftEye(int x, int y) {
  display->drawLine(x - 5, (y - 1) - 2, x + 5, (y - 1) + 2, SSD1306_WHITE);
  display->drawLine(x - 5, y - 2, x + 5, y + 2, SSD1306_WHITE);
  display->drawLine(x - 5, (y + 1) - 2, x + 5, (y + 1) + 2, SSD1306_WHITE);
}

void Face::drawAngeredRightEye(int x, int y) {
  display->drawLine(x - 5, (y - 1) + 2, x + 5, (y - 1) - 2, SSD1306_WHITE);
  display->drawLine(x - 5, y + 2, x + 5, y - 2, SSD1306_WHITE);
  display->drawLine(x - 5, (y + 1) + 2, x + 5, (y + 1) - 2, SSD1306_WHITE);
}

void Face::drawSquintedEye(int x, int y, int r) {
  display->drawCircle(x, y, r, SSD1306_WHITE); // Draw full circle
  display->fillRect(x - r - 1, y, (r * 2) + 3, r + 1,
                    SSD1306_BLACK); // Delete bottom half
}

void Face::drawHappyMouth(int x, int y) {
  for (int i = -4; i <= 4; i++) {
    int yPos = (int)(y + sqrt(16 - i * i));
    display->drawPixel(x + i, yPos, SSD1306_WHITE);
  }
}

void Face::drawSupprisedMouth(int x, int y) {
  display->drawCircle(x - 2, y, 4, SSD1306_WHITE);
  display->fillCircle(x - 2, y, 4, SSD1306_WHITE);
}

void Face::drawAngeredMouth(int x, int y) {
  display->drawLine(x - 4, y - 1, x + 3, y - 1, SSD1306_WHITE);
  display->drawLine(x - 5, y, x + 4, y, SSD1306_WHITE);
  display->drawLine(x - 4, y + 1, x + 3, y + 1, SSD1306_WHITE);
}

void Face::happy(int x, int y) {
  display->clearDisplay();
  drawOpenEye(x, y);
  drawOpenEye(x + 43, y);
  drawHappyMouth(x + 21, y + 28);
  display->display();
}

void Face::supprised(int x, int y) {
  display->clearDisplay();
  drawSupprisedLeftEye(x, y);
  drawSupprisedRightEye(x + 43, y);
  drawSupprisedMouth(x + 21, y + 28);
  display->display();
}

void Face::angered(int x, int y) {
  display->clearDisplay();
  drawAngeredLeftEye(x, y);
  drawAngeredRightEye(x + 43, y);
  drawAngeredMouth(x + 21, y + 28);
  display->display();
}

void Face::confused(int x, int y) {
  display->clearDisplay();
  drawConfusedEye(x, y);
  drawConfusedEye(x + 43, y);
  drawAngeredMouth(x + 21, y + 28);
  display->display();
}

void Face::dead(int x, int y) {
  display->clearDisplay();
  drawDeadEye(x, y);
  drawDeadEye(x + 43, y);
  drawAngeredMouth(x + 21, y + 28);
  display->display();
}
