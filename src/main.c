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

float DeltaTime;

bool DebugSymbols = false;

void update(State* st) {
  // t = fract(t+DT*dt)
  st->cloud_t += CLOUD_DT*DeltaTime;
  st->cloud_t = st->cloud_t - (float)(int)st->cloud_t;

  struct Ball* ball = st->balls;
  while (ball) {
    ball->vel_y += GRAVITY_ACCELERATION*DeltaTime;
    ball->y     += ball->vel_y*DeltaTime;
    ball->x     += ball->vel_x*DeltaTime;
    ball = ball->next;
  }
}

void render(State* st) {
  float theta = st->cloud_t*2.0*M_PI;

  float lower_x = (float)CLOUD_WIDTH + ((float)GetScreenWidth() - 2.0f*(float)CLOUD_WIDTH)*sin(theta);
  float upper_x = (float)CLOUD_WIDTH + ((float)GetScreenWidth() - 2.0f*(float)CLOUD_WIDTH)*sin(1.6+theta);
  float screen_h = (float)GetScreenHeight();

  DrawRectangle(lower_x, (int)(screen_h*CLOUD_LOWER_Y), CLOUD_WIDTH, CLOUD_HEIGHT, WHITE);
  DrawRectangle(upper_x, (int)(screen_h*CLOUD_UPPER_Y), CLOUD_WIDTH, CLOUD_HEIGHT, WHITE);


  draw_slingshot();
  draw_slingshot_strings();

  if (DebugSymbols) draw_mouse_circle();

  struct Ball* ball = st->balls;
  while (ball) {
    draw_ball(ball);
    ball = ball->next;
  }
}

void check_and_set_dim_from_args(int i, int argc, char** argv, char* flag, int* where) {
  if (strcmp(TextToLower(argv[i]), flag) == 0) {
    if (i+1 >= argc) {
      printf("'%s' given without actual value\n", flag);
      exit(1);
    }
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
    ClearBackground(BLACK);
    update(&st);
    render(&st);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
