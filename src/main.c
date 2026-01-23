#include <raylib.h>
#include <raymath.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include "definitions.c"
#include "draw.c"
#include "constants.c"
#include "domain.c"

float DeltaTime;

bool DebugSymbols = false;

void update(State* st) {
  // t = fract(t+DT*dt)
  st->cloud_t += CLOUD_DT*DeltaTime;
  st->cloud_t = st->cloud_t - (float)(int)st->cloud_t;
  printf("t = %f\n", st->cloud_t);
  struct Ball* ball = st->balls;
  while (ball) {
    ball->vel_y += GRAVITY_ACCELERATION*DeltaTime;
    ball->y     += ball->vel_y*DeltaTime;
    ball->x     += ball->vel_x*DeltaTime;
    ball = ball->next;
  }
  clear_errant_balls(st);

  if (!IsMouseButtonDown(0) && st->clicking_last_frame) summon_ball(st);


  st->clicking_last_frame = IsMouseButtonDown(0); // for next frame!
}

void render(State* st) {
  float lower_x = cloud_position_lower(st->cloud_t);
  float upper_x = cloud_position_upper(st->cloud_t);
  float screen_h = (float)GetScreenHeight();

  DrawRectangle(lower_x, (int)(screen_h*CLOUD_LOWER_Y), CLOUD_WIDTH, CLOUD_HEIGHT, WHITE);
  DrawRectangle(upper_x, (int)(screen_h*CLOUD_UPPER_Y), CLOUD_WIDTH, CLOUD_HEIGHT, WHITE);


  draw_slingshot();
  draw_slingshot_strings();
  draw_ready_ball();

  struct Ball* ball = st->balls;
  while (ball) {
    draw_ball(ball);
    ball = ball->next;
  }

  if (DebugSymbols) {
     draw_mouse_circle();
     draw_slingshot_radius();
  }
}

void check_and_set_dim_from_args(int i, int argc, char** argv, char* flag, int* where) {
  if (strcmp(TextToLower(argv[i]), flag) == 0) {
    if (i+1 >= argc) {
      printf("'%s' given without actual value\n", flag);
      exit(1);
    }
    // TODO: Actual error checking
    long x = strtol(argv[i+1], NULL, 10); // Assumeixo que és correcte perquè C va com vol
    *where = x;
    i += 1;
  }

}

int main(int argc, char** argv) {
  for (int i = 1; i < argc; ++i) {
    if (strcmp(TextToLower(argv[i]), "-d") == 0) DebugSymbols = true;
    check_and_set_dim_from_args(i, argc, argv, "-w", &INITIAL_SCREEN_WIDTH);
    check_and_set_dim_from_args(i, argc, argv, "-h", &INITIAL_SCREEN_HEIGHT);
  }

  State st = new_state();

  InitWindow(INITIAL_SCREEN_WIDTH, INITIAL_SCREEN_HEIGHT, "Cloud Sling");

  SetTargetFPS(60.0);

  add_ball(&st, 100, 100, 0, -100);
  add_ball(&st, 100, 200, 0, 0);
  add_ball(&st, 100, 300, 0, 0);
  while (!WindowShouldClose()) {
    DeltaTime = GetFrameTime();
    BeginDrawing();
    ClearBackground(COLOR_BACKGROUND);
    update(&st);
    render(&st);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
