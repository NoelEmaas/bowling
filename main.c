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

void drawInputForceBarFrame();
void drawInputForce(float power);
void handleInput(Ball *ball, AngleControl *angle_control);
int getScore(Frame *frame);
void playRound(Scoreboard *scoreboard, Frame *frame, Ball *ball, AngleControl *angle_control, int *throw_no, int *current_player);
void playRoundPlayer1(Scoreboard *scoreboard, Frame *frame, Ball *ball, AngleControl *angle_control, int *throw_no);
void playRoundPlayer2(Scoreboard *scoreboard, Frame *frame, Ball *ball, AngleControl *angle_control, int *throw_no);

int main () {
  // Create Window and set FPS
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Bowling Game");
  SetTargetFPS(60);

  // Load Background Image
  Image backgroundImage = LoadImage("./assets/bowling_bg.png");
  Texture2D backgroundTexture = LoadTextureFromImage(backgroundImage);  
  UnloadImage(backgroundImage);

  // Initialize game objects
  AngleControl angle_control = { 90, 0, 1, true };
  Ball ball = createBall(0.0f, 0.0f, 600.0f, 780.0f, false);    
  Frame frame = createFrame();
  Scoreboard scoreboard = createScoreboard();
  Obstacle *obstacles = createObstacles(OBSTACLE_NUM);

  // Initialize game variables
  int throw_no = 1;
  int current_player = 1;

  while(!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawTexture(backgroundTexture, 0, 0, WHITE);

    if (!scoreboard.game_over) {
      if (scoreboard.current_round == ROUND_NUM) {
        setWinner(&scoreboard);
        scoreboard.game_over = true;
      } else {
        playRound(&scoreboard, &frame, &ball, &angle_control, &throw_no, &current_player);
      }

      // Update Ball 
      updateBall(&ball);

      // Check Collision with Pins, Walls, and Obstacles
      checkCollision(&ball, &frame, obstacles);

      // Draw Game Objects
      drawObstacles(obstacles, OBSTACLE_NUM);
      drawBall(ball);     
      drawInputAngle(angle_control, ball);   
    }

    displayScoreboard(&scoreboard, current_player);
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

void drawInputForceBarFrame () {
  Rectangle bar_border = {500, 830, 200, 20};
  DrawRectangleRec(bar_border, BARBGCOLOR);
  DrawRectangleLinesEx(bar_border, 2, BARBORDERCOLOR);
}

void handleInput (Ball *ball, AngleControl *angle_control) {
  Vector2 velocity = { 0.0f, 0.0f };

  updateInputAngle(angle_control);
  drawInputForceBarFrame();

  // Aim the arrow and increase force while space is being pressed
  if (IsKeyDown(KEY_SPACE)) {
    angle_control->arrow_moving = false;
    angle_control->arrow_power += 0.5f;
    if (angle_control->arrow_power >= MAX_FORCE) {
      angle_control->arrow_power = MAX_FORCE;
    }
    drawInputForce(angle_control->arrow_power);
  }
  
  //  Apply force to the ball when space key is released
  if (IsKeyReleased(KEY_SPACE)) {
    ball->is_released = true;
    velocity = computeVelocityFromInput(angle_control->arrow_power, angle_control->arrow_angle);
    ball->x_velocity = velocity.x;
    ball->y_velocity = velocity.y;

    printf("Angle: %f\n", angle_control->arrow_angle);
    printf("Velocity: x: %f, y: %f\n", ball->x_velocity, ball->y_velocity);
  }
}

int getScore (Frame *frame) {
  int score = 0;
  for (int i = 0; i < PINS_NUM; ++i) {
    if (frame->pins[i].is_knocked_down && !frame->pins[i].is_knocked_down_prev) {
      score += 1;

      frame->pins[i].is_knocked_down_prev = true;
    }
  }
  return score;
}

void resetFrame (Frame *frame) {
  for (int i = 0; i < PINS_NUM; ++i) {
    frame->pins[i].is_knocked_down = false;
    frame->pins[i].is_knocked_down_prev = false;
  }
}

void playRound (Scoreboard *scoreboard, Frame *frame, Ball *ball, AngleControl *angle_control, int *throw_no, int *current_player) {  
  if (*current_player == 1) {
    playRoundPlayer1(scoreboard, frame, ball, angle_control, throw_no);

    if (*throw_no > 2) {
      *current_player = 2;
      *throw_no = 1;
    }
  } else {
    playRoundPlayer2(scoreboard, frame, ball, angle_control, throw_no);

    if (*throw_no > 2) {
      *current_player = 1;
      *throw_no = 1;
    }
  }
} 

void playRoundPlayer1(Scoreboard *scoreboard, Frame *frame, Ball *ball, AngleControl *angle_control, int *throw_no) {
  if (!ball->is_released) {
    handleInput(ball, angle_control);
  } 

  if (ball->is_released && ball->x_velocity == 0.0f && ball->y_velocity == 0.0f) {    
    int score = getScore(frame);

    printf("Throw %d\n", *throw_no);
    roll(scoreboard, 1, *throw_no, score);

    if (*throw_no < 2) {
      *throw_no = 2;
      printf("Throw %d\n", *throw_no);
    } else {
      resetFrame(frame);

      *throw_no = 3;
    }

    ball->is_released = false;
    ball->x_pos = 600.0f;
    ball->y_pos = 780.0f;
    ball->x_velocity = 0.0f;
    ball->y_velocity = 0.0f;
    angle_control->arrow_power = 0.0f;
    angle_control->arrow_angle = 90.0f;
    angle_control->arrow_moving = true;

    if (score == PINS_NUM) {
      resetFrame(frame);
    }
  }
}

void playRoundPlayer2(Scoreboard *scoreboard, Frame *frame, Ball *ball, AngleControl *angle_control, int *throw_no) {
  if (!ball->is_released) {
    handleInput(ball, angle_control);
  } 

  if (ball->is_released && ball->x_velocity == 0.0f && ball->y_velocity == 0.0f) {    
    int score = getScore(frame);

    printf("Throw %d\n", *throw_no);
    roll(scoreboard, 2, *throw_no, score);

    if (*throw_no < 2) {
      *throw_no = 2;
      printf("Throw %d\n", *throw_no);
    } else {
      *throw_no = 3;

      *frame = createFrame();
      nextRound(scoreboard);
    }

    ball->is_released = false;
    ball->x_pos = 600.0f;
    ball->y_pos = 780.0f;
    ball->x_velocity = 0.0f;
    ball->y_velocity = 0.0f;
    angle_control->arrow_power = 0.0f;
    angle_control->arrow_angle = 90.0f;
    angle_control->arrow_moving = true;

    if (score == PINS_NUM) {
      for (int i = 0; i < PINS_NUM; ++i) {
        frame->pins[i].is_knocked_down = false;
      }
    }
  }
}