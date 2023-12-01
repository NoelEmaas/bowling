#ifndef PIN_H
#define PIN_H 

#include "raylib.h"

typedef struct {
  bool is_knocked_down;
  float x_pos;
  float y_pos;
  Image image;
  Texture2D texture;
  Rectangle sourceRect;
  Rectangle destRect;
} Pin;

Pin createPin (bool is_knocked_down, float x_pos, float y_pos);
void drawPin (Pin pin);

#endif