#include "player.h"

#include <string.h>

Player createPlayer (char name[20], int score, Ball ball) {
  Player player;
  strcpy(player.name, name);
  player.score = score;
  player.ball = ball;
  return player;
}