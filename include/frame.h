#ifndef FRAME_H
#define FRAME_H

#include "raylib.h"
#include "constants.h"
#include "pin.h"

#include <time.h>
#include <stdlib.h>

typedef struct {
  Pin pins[PINS_NUM];
} Frame;

Frame createFrame ();
void createTriangleFrame(Frame *frame, float startX, float startY);
void createHollowSquareFrame(Frame *frame, float startX, float startY);
void createDiamondFrame(Frame *frame, float startX, float startY);
void drawFrame (Frame frame);
void drawScoreBoardFrame ();
void drawUserInputFrame ();
void drawBowlingGameFrame ();
#endif
