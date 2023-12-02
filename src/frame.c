#include "frame.h"
#include <string.h>

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

void drawFrameBorder () {
  Rectangle frameBorder = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
  DrawRectangleLinesEx(frameBorder, 20, FRAMEBORDERCOLOR);
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
void drawFrame (Frame frame) {
  for (int i = 0; i < PINS_NUM; ++i) {
    drawPin(frame.pins[i]);
  }
  drawFrameBorder();
}
