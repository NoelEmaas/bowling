#include "angle_control.h"

#include <math.h>

void updateInputAngle (AngleControl *angle_control) {
  if (angle_control->arrow_moving) {
    angle_control->arrow_angle += angle_control->angle_speed;
    if (angle_control->arrow_angle >= MAX_ANGLE || angle_control->arrow_angle <= MIN_ANGLE) {
      angle_control->angle_speed *= -1;
    }
  }
}

void drawInputAngle (AngleControl angle_control, Ball ball) {
  if (!ball.is_released) {
    DrawLineEx(
      (Vector2){ ball.x_pos, ball.y_pos },
      (Vector2){ ball.x_pos + cosf(angle_control.arrow_angle * DEG2RAD) * 100, ball.y_pos - sinf(angle_control.arrow_angle * DEG2RAD) * 100}, 2, RED
    );
  }
}
