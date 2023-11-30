#include "raylib.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

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

// min and max angles for input
#define MAX_ANGLE 135
#define MIN_ANGLE 45

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
  Image image;
  Texture2D texture;
  Rectangle sourceRect;
  Rectangle destRect;
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

typedef struct {
  float arrow_angle;
  int angle_speed;
  bool ball_released;
  bool arrow_moving;
} AngleControl;

Ball createBall (float x_velocity, float y_velocity, float x_pos);
Player createPlayer (char name[20], int score, Ball ball);
Pin createPin (bool is_knocked_down, float x_pos, float y_pos);
void createTriangleFrame(Frame *frame, float startX, float startY);
void createHollowSquareFrame(Frame *frame, float startX, float startY);
void createDiamondFrame(Frame *frame, float startX, float startY);
Frame createFrame ();
Obstacle createObstacle ();

void drawScoreBoardFrame ();
void drawUserInputFrame ();
void drawBowlingGameFrame ();
void drawBall (Ball ball);
void drawPin (Pin pin);
void drawFrame (Frame frame);
void drawInputAngle(AngleControl angle_control, Ball ball);

void updateInputAngle (AngleControl *angle_control);
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

  srand(time(NULL));
  int random = (rand() % 3) + 1;
  int startY = 100.0f;

  switch (random) {
        case 1: // Triangle
            createTriangleFrame(&frame, 400.0f, startY);
            break;
        case 2: // Hollow Square
            createHollowSquareFrame(&frame, 440.0f, startY);
            break;
        case 3: // Diamond
            createDiamondFrame(&frame, 510.0f, startY);
            break;
        default:
            break;
    }

  return frame;
} 
void createTriangleFrame(Frame *frame, float startX, float startY) {
    for (int i = 0; i < PINS_NUM; ++i) {
      if (i < 4) {
        frame->pins[i] = createPin(false, 400.0f + (i * (PIN_RADIUS * 2 + PIN_DISTANCE)), 100.0f);
      } else if (i < 7) {
        frame->pins[i] = createPin(false, 400.0f + ((i - 4) * (PIN_RADIUS * 2 + PIN_DISTANCE)) + PIN_RADIUS + PIN_DISTANCE / 2, 100.0f + PIN_RADIUS * 2 + PIN_DISTANCE);
      } else if (i < 9) {
        frame->pins[i] = createPin(false, 400.0f + ((i - 7) * (PIN_RADIUS * 2 + PIN_DISTANCE)) + PIN_RADIUS * 2 + PIN_DISTANCE, 100.0f + PIN_RADIUS * 4 + PIN_DISTANCE * 2);
      } else {
        frame->pins[i] = createPin(false, 400.0f + ((i - 9) * (PIN_RADIUS * 2 + PIN_DISTANCE)) + PIN_RADIUS * 3 + PIN_DISTANCE * 1.5, 100.0f + PIN_RADIUS * 6 + PIN_DISTANCE * 3);
      }
    }
}

void createHollowSquareFrame(Frame *frame, float startX, float startY) {
  for (int i = 0; i < 3; ++i) {
      frame->pins[i] = createPin(false, 440.0f + (i * (PIN_RADIUS * 2 + PIN_DISTANCE)), startY);
    }

    for (int i = 3; i < 5; ++i) {
      frame->pins[i] = createPin(false, 440.0f + (2 * (PIN_RADIUS * 2 + PIN_DISTANCE)), startY + ((i - 2) * (PIN_RADIUS * 2 + PIN_DISTANCE)));
    }

    for (int i = 5; i < 8; ++i) {
      frame->pins[i] = createPin(false, 440.0f + ((7 - i) * (PIN_RADIUS * 2 + PIN_DISTANCE)), startY + (2 * (PIN_RADIUS * 2 + PIN_DISTANCE)));
    }

    for (int i = 8; i < 10; ++i) {
      frame->pins[i] = createPin(false, 440.0f, startY + ((9 - i) * (PIN_RADIUS * 2 + PIN_DISTANCE)));
    }
}


void createDiamondFrame(Frame *frame, float startX, float startY) {
  frame->pins[0] = createPin(false, 510.0f, startY);

  frame->pins[1] = createPin(false, 510.0f - (PIN_RADIUS + PIN_DISTANCE / 2), startY + (PIN_RADIUS * 2 + PIN_DISTANCE));
  frame->pins[2] = createPin(false, 510.0f + (PIN_RADIUS + PIN_DISTANCE / 2), startY + (PIN_RADIUS * 2 + PIN_DISTANCE));

  frame->pins[3] = createPin(false, 510.0f - (PIN_RADIUS + PIN_DISTANCE / 2), startY + (PIN_RADIUS * 4 + PIN_DISTANCE * 3));
  frame->pins[4] = createPin(false, 510.0f + (PIN_RADIUS + PIN_DISTANCE / 2), startY + (PIN_RADIUS * 4 + PIN_DISTANCE * 3));

  frame->pins[5] = createPin(false, 510.0f, startY + (PIN_RADIUS * 6 + PIN_DISTANCE * 4));

  frame->pins[6] = createPin(false, 510.0f - (PIN_RADIUS * 2 + PIN_DISTANCE), startY + (PIN_RADIUS * 3 + PIN_DISTANCE * 2));
  frame->pins[7] = createPin(false, 510.0f + (PIN_RADIUS * 2 + PIN_DISTANCE), startY + (PIN_RADIUS * 3 + PIN_DISTANCE * 2));
  frame->pins[8] = createPin(false, 510.0f - (PIN_RADIUS * 2 + PIN_DISTANCE), startY + (PIN_RADIUS * 3 + PIN_DISTANCE * 2));
  frame->pins[9] = createPin(false, 510.0f + (PIN_RADIUS * 2 + PIN_DISTANCE), startY + (PIN_RADIUS * 3 + PIN_DISTANCE * 2));
}

Pin createPin(bool is_knocked_down, float x_pos, float y_pos) {
    Pin pin;
    float scale = 1.0f; 

    pin.image = LoadImage("bowling_pin.png");
    pin.texture = LoadTextureFromImage(pin.image);

    Vector2 position = {x_pos, y_pos};
    pin.sourceRect = (Rectangle){0, 0, (float)pin.texture.width, (float)pin.texture.height};
    pin.destRect = (Rectangle){(position.x + 90) - pin.texture.width * scale / 2, position.y - pin.texture.height * scale / 2, pin.texture.width * scale, pin.texture.height * scale};

    UnloadImage(pin.image); 

    pin.is_knocked_down = is_knocked_down;
    pin.x_pos = x_pos + 90;
    pin.y_pos = y_pos;

    return pin;
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
  DrawCircleGradient(ball.x_pos, ball.y_pos + 10, 38, BALLSHADOWINNER, BALLSHADOWOUTER);
  // Draw Ball
  DrawCircleGradient(ball.x_pos, ball.y_pos, BALL_RADIUS, BALLCOLOR, BALLDARKCOLOR);
}

void drawPin(Pin pin) {
  if (!pin.is_knocked_down) {
    DrawTexturePro(pin.texture, pin.sourceRect, pin.destRect, (Vector2){0, 0}, 0.0f, WHITE);
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

void updateInputAngle (AngleControl *angle_control) {
  if (!angle_control->ball_released) {
    angle_control->arrow_angle += angle_control->angle_speed;
    if (angle_control->arrow_angle >= MAX_ANGLE || angle_control->arrow_angle <= MIN_ANGLE) {
      angle_control->angle_speed *= -1;
    }
  }
}

void drawInputAngle (AngleControl angle_control, Ball ball) {
  if (angle_control.arrow_moving) {
    DrawLineEx(
      (Vector2){ ball.x_pos, ball.y_pos },
      (Vector2){ ball.x_pos + cosf(angle_control.arrow_angle * DEG2RAD) * 100, ball.y_pos - sinf(angle_control.arrow_angle * DEG2RAD) * 100}, 2, RED
    );
  }
}