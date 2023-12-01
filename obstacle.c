#include "obstacle.h"

// TODO: Make sure that the obstacle is not created on top of other obstacles
// For now, the obstace is created at a random position such that is does not spawn
// on top of the pins or near the starting position of the ball
Obstacle createObstacle () {
  Obstacle obstacle;
  float scale = 0.1f;

  obstacle.image = LoadImage("./assets/obstacle.png");
  obstacle.texture = LoadTextureFromImage(obstacle.image);

  Vector2 position = { (float)GetRandomValue(400 + obstacle.texture.width * scale, 800 - obstacle.texture.width  * scale), (float)GetRandomValue(350 - obstacle.texture.height  * scale, 750 - obstacle.texture.height  * scale) };
  
  obstacle.sourceRect = (Rectangle) {0, 0, (float)obstacle.texture.width, (float)obstacle.texture.height};
  obstacle.destRect = (Rectangle) {position.x, position.y, obstacle.texture.width * scale, obstacle.texture.height * scale};

  UnloadImage(obstacle.image); 

  return obstacle;
}