#include "raylib.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800

#define BALL_RADIUS 35

#define PINS_NUM 10
#define PIN_RADIUS 20
#define PIN_DISTANCE 30

// TODO: Add variables to allow for spin 
typedef struct {
  float velocity;
  float x_pos;
  float y_pos;
} Ball;

typedef struct {
  char name[20];
  int score;
  Ball ball;
} Player;

typedef struct {
  bool is_knocked_down;
  float x_pos;
  float y_pos;
} Pin;

typedef struct {
  Pin pins[PINS_NUM];
} Frame;

Ball createBall (float velocity, float x_pos);
Player createPlayer (char name[20], int score, Ball ball);
Pin createPin (bool is_knocked_down, float x_pos, float y_pos);
Frame createFrame ();

void drawScoreBoardFrame ();
void drawUserInputFrame ();
void drawBowlingGameFrame ();
void drawBall (Ball ball);
void drawPin (Pin pin);
void drawFrame (Frame frame);

void updateBall(Ball *ball);
void updatePin(Pin *pin);
void checkCollision (Ball *ball, Frame *frame);

int main () {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Basic Window");
  SetTargetFPS(60);

  Ball ball = createBall(10.0f, 600.0f);    
  Frame frame = createFrame();

  while(!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // Update
    updateBall(&ball);

    // Check Collision
    checkCollision(&ball, &frame);

    // Draw 
    drawScoreBoardFrame();
    drawUserInputFrame();
    drawBowlingGameFrame();
    drawBall(ball);        
    drawFrame(frame);

    EndDrawing();
  }

  CloseWindow();
  return 0;
  }

// Ball's y_pos is set to default value of 700.0f 
// because the control is only in the x-axis
Ball createBall (float velocity, float x_pos) {
  float y_pos = 700.0f;
  return (Ball) { velocity, x_pos,  y_pos};
}

Player createPlayer (char name[20], int score, Ball ball) {
  Player player;
  strcpy(player.name, name);
  player.score = score;
  player.ball = ball;
  return player;
}

Frame createFrame () {
  Frame frame;
  for (int i = 0; i < PINS_NUM; ++i) {
    if (i < 4) {
      frame.pins[i] = createPin(false, 400.0f + (i * (PIN_RADIUS * 2 + PIN_DISTANCE)), 100.0f);
    } else if (i < 7) {
      frame.pins[i] = createPin(false, 400.0f + ((i - 4) * (PIN_RADIUS * 2 + PIN_DISTANCE)) + PIN_RADIUS + PIN_DISTANCE / 2, 100.0f + PIN_RADIUS * 2 + PIN_DISTANCE);
    } else if (i < 9) {
      frame.pins[i] = createPin(false, 400.0f + ((i - 7) * (PIN_RADIUS * 2 + PIN_DISTANCE)) + PIN_RADIUS * 2 + PIN_DISTANCE, 100.0f + PIN_RADIUS * 4 + PIN_DISTANCE * 2);
    } else {
      frame.pins[i] = createPin(false, 400.0f + ((i - 9) * (PIN_RADIUS * 2 + PIN_DISTANCE)) + PIN_RADIUS * 3 + PIN_DISTANCE * 1.5, 100.0f + PIN_RADIUS * 6 + PIN_DISTANCE * 3);
    }
  }

  return frame;
}

// Pin's x_pos is added by 90 to center the pins
Pin createPin (bool is_knocked_down, float x_pos, float y_pos) {
  return (Pin) { is_knocked_down, x_pos + 90, y_pos };
}

void drawScoreBoardFrame () {
  Rectangle frame = {20, 20, 360, 760};
  Rectangle frameBorder = {0, 0, 400, SCREEN_HEIGHT};
  DrawRectangleRec(frame, BLACK);
  DrawRectangleLinesEx(frame, 2, GREEN);
  DrawRectangleLinesEx(frameBorder, 20, BROWN);

  // Draw Table Rows
  int numRows = 14;
  int curr_y_pos = 22;
  for (int i = 0; i < numRows; ++i) {
    Rectangle row = {22, curr_y_pos, 356, 54};
    DrawRectangleRec(row, BLACK);
    DrawRectangleLinesEx(row, 2, GREEN);
    curr_y_pos += 54;
  }
}

void drawUserInputFrame () {
  Rectangle rec = {800, 0, 400, SCREEN_HEIGHT};
  DrawRectangleRec(rec, BLACK);
  DrawRectangleLinesEx(rec, 20, BROWN);
}

void drawBowlingGameFrame () {
  Rectangle rect = {400, 0, 400, SCREEN_HEIGHT};
  DrawRectangleRec(rect, BLACK);
  DrawLineEx((Vector2) { rect.x, rect.y }, (Vector2) { rect.x + rect.width, rect.y }, 40, BROWN);
  DrawLineEx((Vector2) { rect.x, rect.y + rect.height }, (Vector2) { rect.x + rect.width, rect.y + rect.height }, 40, BROWN);
}

void drawBall (Ball ball) {
  DrawCircle(ball.x_pos, ball.y_pos, BALL_RADIUS, DARKBLUE);
}

void drawPin (Pin pin) {
  if (!pin.is_knocked_down) {
    DrawCircle(pin.x_pos, pin.y_pos, PIN_RADIUS, DARKGRAY);
  }
}

void drawFrame (Frame frame) {
  for (int i = 0; i < PINS_NUM; ++i) {
    drawPin(frame.pins[i]);
  }
}

// TODO: Slow down the ball's velocity as it moves up the screen.
// For now, the ball's velocity decreases by 0.01f every frame until it reaches 0.0f
void updateBall(Ball *ball) {
  ball->y_pos -= ball->velocity;

  // Do not display the ball if it is beyond the border of the frames
  if (ball->y_pos < 20.0f + BALL_RADIUS) {
    ball->y_pos = BALL_RADIUS + 20.0f;
    ball->velocity = 0.0f;
  }

  // Velocity should not be negative
  if (ball->velocity > 0.0f) {
    ball->velocity -= 0.01f;
  } else {
    ball->velocity = 0.0f;
  }
}

// TODO: Update the ball's velocity when it hits the pins
// For now, the ball's velocity decreases by 1.0f when it hits the pins
void checkCollision (Ball *ball, Frame *frame) {
  for (int i = 0; i < PINS_NUM; ++i) {
    if (!frame->pins[i].is_knocked_down) {
      if (CheckCollisionCircles((Vector2) { ball->x_pos, ball->y_pos }, BALL_RADIUS, (Vector2) { frame->pins[i].x_pos, frame->pins[i].y_pos }, PIN_RADIUS)) {
        frame->pins[i].is_knocked_down = true;
        ball->velocity -= 1.0f;
      }
    }
  }
}



