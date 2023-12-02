#ifndef PLAYER_H
#define PLAYER_H

#include "ball.h"

typedef struct {
  char name[20];
  int score;
  Ball ball;
} Player;

Player createPlayer (char name[20], int score, Ball ball);
#endif