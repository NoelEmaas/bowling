#include "pin.h"

Pin createPin(bool is_knocked_down, float x_pos, float y_pos) {
    Pin pin;
    float scale = 1.0f; 

    pin.image = LoadImage("./assets/bowling_pin.png");
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

void drawPin(Pin pin) {
  if (!pin.is_knocked_down) {
    DrawTexturePro(pin.texture, pin.sourceRect, pin.destRect, (Vector2){0, 0}, 0.0f, WHITE);
  }
}
