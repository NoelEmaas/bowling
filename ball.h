#ifndef BALL_H
#define BALL_H

#include "raylib.h"
#include "obstacle.h"
#include "frame.h"

#include <math.h>
#include <stdio.h>

typedef struct {
  float x_velocity;
  float y_velocity;
  float x_pos;
  float y_pos;
} Ball;

Ball createBall (float x_velocity, float y_velocity, float x_pos);
void drawBall (Ball ball);
void updateBall(Ball *ball);
void checkCollision (Ball *ball, Frame *frame, Obstacle *obstacles);
void applyDampening (Ball *ball);
Vector2 computeVelocityFromInput(float power, float angle);

#endif 