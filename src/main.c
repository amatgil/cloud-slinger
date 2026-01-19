#include <raylib.h>
#include "balls.c"
#include <stdlib.h>

#define CLOUD_WIDTH  50
#define CLOUD_HEIGHT 10
#define BALL_RADIUS 8

#define INITIAL_SCREEN_WIDTH  350
#define INITIAL_SCREEN_HEIGHT INITIAL_SCREEN_WIDTH * 2

typedef struct {
  Vector2 cloud_lower;
  Vector2 cloud_upper;
  struct Ball* balls;
} State ;

State new_state() {
  Vector2 lower = (Vector2){10, 10};
  Vector2 upper = (Vector2){30, 50};
  return (State){lower, upper, NULL};
}

void draw_cloud(Vector2 pos) {
  DrawRectangle(pos.x, pos.y,
                CLOUD_WIDTH, CLOUD_HEIGHT, WHITE);
}
void draw_ball(struct Ball* ball) {
  DrawCircle(ball->x, ball->y, BALL_RADIUS, RED);
}

void render(State* state) {
  draw_cloud(state->cloud_lower);
  draw_cloud(state->cloud_upper);
  struct Ball* ball = state->balls;
  while (ball) {
    draw_ball(ball);
    ball = ball->next;
  }
}



int main(void) {
  State st = new_state();

  InitWindow(INITIAL_SCREEN_WIDTH,
             INITIAL_SCREEN_HEIGHT,
             "Cloud Sling");

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);
    render(&st);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
