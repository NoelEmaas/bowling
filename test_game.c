#include "raylib.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "scoreboard.h"

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 880

#define BALL_RADIUS 35
#define BALL_DAMPENING 0.99f

#define PINS_NUM 10
#define PIN_RADIUS 20
#define PIN_DISTANCE 30

#define OBSTACLE_NUM 5

// define colors
#define BALLCOLOR (Color){ 92, 75, 192, 255 } 
#define BALLDARKCOLOR (Color){ 65, 50, 150, 255 }
#define BALLSHADOWINNER (Color){ 81, 81, 81, 255 }
#define BALLSHADOWOUTER (Color){ 0, 0, 0, 0 }

typedef struct {
  float x_velocity;
  float y_velocity;
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

typedef struct {
  Image image;
  Texture2D texture;
  Rectangle sourceRect;
  Rectangle destRect;
} Obstacle;

Ball createBall (float x_velocity, float y_velocity, float x_pos);
Player createPlayer (char name[20], int score, Ball ball);
Pin createPin (bool is_knocked_down, float x_pos, float y_pos);
Frame createFrame ();
Obstacle createObstacle ();

void drawScoreBoardFrame ();
void drawUserInputFrame ();
void drawBowlingGameFrame ();
void drawBall (Ball ball);
void drawPin (Pin pin);
void drawFrame (Frame frame);

void updateBall(Ball *ball);
void checkCollision (Ball *ball, Frame *frame, Obstacle *obstacles);
void applyDampening (Ball *ball);
Vector2 computeVelocityFromInput(float power, float angle);

int main () {
  // Initialization
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Basic Window");
  SetTargetFPS(60);

  // Load Background Image
  Image backgroundImage = LoadImage("bowling_bg.png");
  Texture2D backgroundTexture = LoadTextureFromImage(backgroundImage);
  UnloadImage(backgroundImage);

  // TODO: Handle these in different functions for better readability
  // like a function for Player
  // Input power and angle
  float power = 10.0f;
  float angle = 135.0f;

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

    // Check Collision with Pins, Walls, and Obstacles
    checkCollision(&ball, &frame, obstacles);

    // Draw 
    //drawScoreBoardFrame();
    //drawUserInputFrame();
    //drawBowlingGameFrame();
    drawBall(ball);        
    drawFrame(frame);

    EndDrawing();
  }

  CloseWindow();
  return 0;
  }

// Ball's y_pos is set to default value of 700.0f 
// because the control is only in the x-axis
Ball createBall (float x_velocity, float y_velocity, float x_pos) {
  float y_pos = 700.0f;
  return (Ball) { x_velocity, y_velocity, x_pos, y_pos };
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

// TODO: Make sure that the obstacle is not created on top of other obstacles
// For now, the obstace is created at a random position such that is does not spawn
// on top of the pins or near the starting position of the ball
Obstacle createObstacle () {
  Obstacle obstacle;
  float scale = 0.1f;

  obstacle.image = LoadImage("obstacle.png");
  obstacle.texture = LoadTextureFromImage(obstacle.image);

  Vector2 position = { (float)GetRandomValue(400 + obstacle.texture.width * scale, 800 - obstacle.texture.width  * scale), (float)GetRandomValue(350 - obstacle.texture.height  * scale, 800 - obstacle.texture.height  * scale) };
  
  obstacle.sourceRect = (Rectangle) {0, 0, (float)obstacle.texture.width, (float)obstacle.texture.height};
  obstacle.destRect = (Rectangle) {position.x, position.y, obstacle.texture.width * scale, obstacle.texture.height * scale};

  UnloadImage(obstacle.image); 

  return obstacle;
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
  // Draw Ball Shadow
  DrawCircleGradient(ball.x_pos, ball.y_pos, 45, BALLSHADOWINNER, BALLSHADOWOUTER);
  // Draw Ball
  DrawCircleGradient(ball.x_pos, ball.y_pos, BALL_RADIUS, BALLCOLOR, BALLDARKCOLOR);
  //DrawCircle(ball.x_pos, ball.y_pos, BALL_RADIUS, DARKBLUE);
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
