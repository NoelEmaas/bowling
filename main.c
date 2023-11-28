#include "raylib.h"

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800

void drawScoreBoardFrame ();
void drawUserInputFrame ();
void drawBowlingGameFrame ();

int main () {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Basic Window");
  SetTargetFPS(60);

  float velocity = 4.0f;
  float y_pos = 700.0f;
  float x_pos = 581.5f;

  while(!WindowShouldClose()) {
    y_pos -= velocity;
    BeginDrawing();
    ClearBackground(RAYWHITE);
      
    drawScoreBoardFrame();
    drawUserInputFrame();
    drawBowlingGameFrame();
    DrawCircle(x_pos, y_pos, 35, DARKBLUE);
  
    EndDrawing();
  }

  CloseWindow();
  return 0;
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


