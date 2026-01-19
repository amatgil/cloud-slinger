#include <raylib.h>

typedef struct {
  Vector2 cloud_lower;
  Vector2 cloud_upper;
  Balls balls;
} State ;

int main(void) {
  InitWindow(800, 450, "Cloud Sling");

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("Congrats! You created your first window!", 190, 200, 20,
             LIGHTGRAY);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
