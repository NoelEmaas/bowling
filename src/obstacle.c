#include <stdlib.h>
#include <stdio.h>

#include "obstacle.h"

Obstacle createObstacle () {
  Obstacle obstacle;
  float scale = 0.1f;

  obstacle.image = LoadImage("./assets/obstacle.png");
  obstacle.texture = LoadTextureFromImage(obstacle.image);

  Vector2 position = { (float)GetRandomValue(400 + obstacle.texture.width * scale, 800 - obstacle.texture.width  * scale), (float)GetRandomValue(400 - obstacle.texture.height  * scale, 700 - obstacle.texture.height  * scale) };
  
  obstacle.sourceRect = (Rectangle) {0, 0, (float)obstacle.texture.width, (float)obstacle.texture.height};
  obstacle.destRect = (Rectangle) {position.x, position.y, obstacle.texture.width * scale, obstacle.texture.height * scale};

  UnloadImage(obstacle.image); 

  return obstacle;
}

Obstacle* createObstacles (int numObstacles) {
  Obstacle *obstacles = (Obstacle*)malloc(sizeof(Obstacle) * numObstacles);
  for (int i = 0; i < numObstacles; ++i) {
    obstacles[i] = createObstacle();

    // Check if the obstacle is created on top of other obstacles
    if (i > 0) {
      for (int j = 0; j < i; ++j) {
        printf("Checking collision between obstacle %d and %d\n", i, j);
        printf("Obstacle %d: x: %f, y: %f\n", i, obstacles[i].destRect.x, obstacles[i].destRect.y);
        printf("Obstacle %d: x: %f, y: %f\n", j, obstacles[j].destRect.x, obstacles[j].destRect.y);
        if (CheckCollisionRecs(obstacles[i].destRect, obstacles[j].destRect)) {
          printf("Collision detected\n");
          obstacles[i] = createObstacle();
          j = 0;
        }
      }
    }
  }

  return obstacles;
}

void drawObstacles (Obstacle *obstacles, int numObstacles) {
  for (int i = 0; i < numObstacles; ++i) {
    DrawTexturePro(obstacles[i].texture, obstacles[i].sourceRect, obstacles[i].destRect, (Vector2) { 0, 0 }, 0.0f, WHITE);
  }
}