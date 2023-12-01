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

int main () {
  // Initialization
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Basic Window");
  SetTargetFPS(60);

  // Load Background Image
  Image backgroundImage = LoadImage("./assets/bowling_bg.png");
  Texture2D backgroundTexture = LoadTextureFromImage(backgroundImage);
  UnloadImage(backgroundImage);

  // Ball angle input
  AngleControl angle_control = { 90, 1, false, true };

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
  Ball ball = createBall(x_velocity, y_velocity, 600.0f);    
  Frame frame = createFrame();
  Scoreboard scoreboard = createScoreboard();

  // Create Obstacles
  Obstacle obstacles[OBSTACLE_NUM];
  for (int i = 0; i < OBSTACLE_NUM; ++i) {
    obstacles[i] = createObstacle();
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

    // start the ball movement when space is pressed
    if (IsKeyPressed(KEY_SPACE)) {
      angle_control.ball_released = true;
      angle_control.arrow_moving = false;
      angle = angle_control.arrow_angle;
      power = 10.0f;
      velocity = computeVelocityFromInput(power, angle);
      ball.x_velocity = velocity.x;
      ball.y_velocity = velocity.y;
    }

    // Draw 
    //drawScoreBoardFrame();
    //drawUserInputFrame();
    //drawBowlingGameFrame();
    drawBall(ball);     
    drawInputAngle(angle_control, ball);   
    drawFrame(frame);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}

