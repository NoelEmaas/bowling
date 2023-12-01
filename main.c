#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"

#include "constants.h"
#include "scoreboard.h"
#include "ball.h"
#include "player.h"
#include "obstacle.h"
#include "pin.h"
#include "angle_control.h"

void drawInputForceBarFrame(Ball ball);
void drawInputForce(float power);

int main () {
  // Initialization
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Basic Window");
  SetTargetFPS(60);

  // Load Background Image
  Image backgroundImage = LoadImage("./assets/bowling_bg.png");
  Texture2D backgroundTexture = LoadTextureFromImage(backgroundImage);
  UnloadImage(backgroundImage);

  // Ball angle input
  AngleControl angle_control = { 90, 1, true };

  // TODO: Handle these in different functions for better readability
  // like a function for Player
  // Input power and angle
  float power = 0.0f;
  float angle = angle_control.arrow_angle;

  // Compute velocity from input
  Vector2 velocity = computeVelocityFromInput(power, angle);
  float x_velocity = velocity.x;
  float y_velocity = velocity.y;

  printf("x_velocity: %f\n", x_velocity);
  printf("y_velocity: %f\n", y_velocity);

  // Create Ball and Frame
  Ball ball = createBall(x_velocity, y_velocity, 600.0f, 780.0f, false);    
  Frame frame = createFrame();
  Scoreboard scoreboard = createScoreboard();

  // Create Obstacles
  Obstacle obstacles[OBSTACLE_NUM];
  for (int i = 0; i < OBSTACLE_NUM; ++i) {
    obstacles[i] = createObstacle();

    // Check if the obstacle is created on top of other obstacles
    if (i > 0) {
      for (int j = 0; j < i; ++j) {
        printf("Checking collision between obstacle %d and %d\n", i, j);
        printf("Obstacle %d: x: %f, y: %f\n", i, obstacles[i].destRect.x, obstacles[i].destRect.y);
        printf("Obstacle %d: x: %f, y: %f\n", j, obstacles[j].destRect.x, obstacles[j].destRect.y);
        while (CheckCollisionRecs(obstacles[i].destRect, obstacles[j].destRect)) {
          obstacles[i] = createObstacle();
        }
      }
    }
  } 

  while(!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // Draw Background Image
    DrawTexture(backgroundTexture, 0, 0, WHITE);

    displayScoreboard(&scoreboard, 1);
    // Draw Obstacles
    for (int i = 0; i < OBSTACLE_NUM; ++i) {
      DrawTexturePro(obstacles[i].texture, obstacles[i].sourceRect, obstacles[i].destRect, (Vector2) { 0, 0 }, 0.0f, WHITE);
    }

    // Update
    updateBall(&ball);
    updateInputAngle(&angle_control);

    // Check Collision with Pins, Walls, and Obstacles
    checkCollision(&ball, &frame, obstacles);
    
    drawInputForceBarFrame(ball);

    // Aim the arrow and increase force while space is being pressed
    if (IsKeyDown(KEY_SPACE) && !ball.is_released) {
      angle_control.arrow_moving = false;
      angle = angle_control.arrow_angle;
      power += 0.5f;
      if (power >= MAX_FORCE) {
        power = MAX_FORCE;
      }
      drawInputForce(power);
    }
    
    //  Apply force to the ball when space key is released
    if (IsKeyReleased(KEY_SPACE) && !ball.is_released) {
      ball.is_released = true;
      velocity = computeVelocityFromInput(power, angle);
      ball.x_velocity = velocity.x;
      ball.y_velocity = velocity.y;
    }
    
    drawBall(ball);     
    drawInputAngle(angle_control, ball);   
    drawFrame(frame);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}

void drawInputForce (float power) {
  float curr_force = power / MAX_FORCE;
  float force_bar_width = curr_force * 196;
  Rectangle force_bar = {502, 832, force_bar_width, 16};
  DrawRectangleRec(force_bar, RED);
}

void drawInputForceBarFrame (Ball ball) {
  if (!ball.is_released) {
    Rectangle bar_border = {500, 830, 200, 20};
    DrawRectangleRec(bar_border, BARBGCOLOR);
    DrawRectangleLinesEx(bar_border, 2, BARBORDERCOLOR);
  }
}
