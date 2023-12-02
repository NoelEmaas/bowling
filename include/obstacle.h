#ifndef OBSTACLE_H 
#define OBSTACLE_H

#include "raylib.h"

typedef struct {
  Image image;
  Texture2D texture;
  Rectangle sourceRect;
  Rectangle destRect;
} Obstacle;

Obstacle createObstacle ();
#endif 