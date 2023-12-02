#ifndef ANGLE_CONTROL_H
#define ANGLE_CONTROL_H

#include "raylib.h"

#include "constants.h"
#include "ball.h"

typedef struct {
  float arrow_angle;
  int angle_speed;
  bool arrow_moving;
} AngleControl;

void drawInputAngle(AngleControl angle_control, Ball ball);
void updateInputAngle (AngleControl *angle_control);

#endif
