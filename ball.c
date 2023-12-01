#include "ball.h"


// Ball's y_pos is set to default value of 700.0f 
// because the control is only in the x-axis
Ball createBall (float x_velocity, float y_velocity, float x_pos, float y_pos, bool is_released) {
  return (Ball) { x_velocity, y_velocity, x_pos, y_pos, is_released};
}

void drawBall (Ball ball) {
  // Draw Ball Shadow
  DrawCircleGradient(ball.x_pos, ball.y_pos + 10, 38, BALLSHADOWINNER, BALLSHADOWOUTER);
  // Draw Ball
  DrawCircleGradient(ball.x_pos, ball.y_pos, BALL_RADIUS, BALLCOLOR, BALLDARKCOLOR);
}

void updateBall(Ball *ball) {
  // Update the ball's position
  ball->x_pos += ball->x_velocity;
  ball->y_pos -= ball->y_velocity;

  // Apply dampening to the ball
  applyDampening(ball);

  // Stop the ball if its moving too slow
  if (ball->x_velocity < 0.1f && ball->x_velocity > -0.1f && ball->y_velocity < 0.1f && ball->y_velocity > -0.1f) {
    ball->x_velocity = 0.0f;
    ball->y_velocity = 0.0f;
  }
}

void checkCollision (Ball *ball, Frame *frame, Obstacle *obstacles) {
  // Check Collision with Pins    
  for (int i = 0; i < PINS_NUM; ++i) {
    if (!frame->pins[i].is_knocked_down) {
      if (CheckCollisionCircles((Vector2) { ball->x_pos, ball->y_pos }, BALL_RADIUS, (Vector2) { frame->pins[i].x_pos, frame->pins[i].y_pos }, PIN_RADIUS)) {
        frame->pins[i].is_knocked_down = true;
        printf("Pin %d is knocked down\n", i + 1);
        applyDampening(ball);
      }
    }
  }

  // Check Collision with Obstacles
  for (int i = 0; i < OBSTACLE_NUM; ++i) {
    if (CheckCollisionRecs((Rectangle) { ball->x_pos - BALL_RADIUS, ball->y_pos - BALL_RADIUS, BALL_RADIUS * 2, BALL_RADIUS * 2 }, obstacles[i].destRect)) {
      ball->x_velocity *= -BALL_DAMPENING;
      // ball->y_velocity *= -BALL_DAMPENING; // If y_velocity is changed, it causes the ball to get stuck in the obstacle
    }
  }

  // Check Collision with Walls
  // Bounce the ball off the left and right walls
  if (ball->x_pos > 780 - BALL_RADIUS || ball->x_pos < 420 + BALL_RADIUS) {
    ball->x_velocity *= -BALL_DAMPENING;
  }

  // Bounce the ball off the top wall
  if (ball->y_pos < 20.0f + BALL_RADIUS) {
    ball->y_velocity *= -BALL_DAMPENING;
  }
}

void applyDampening (Ball *ball) {
  ball->x_velocity *= BALL_DAMPENING;
  ball->y_velocity *= BALL_DAMPENING;
}

Vector2 computeVelocityFromInput(float power, float angle) {
  return (Vector2) { power * cos(angle * DEG2RAD), power * sin(angle * DEG2RAD) };
}

