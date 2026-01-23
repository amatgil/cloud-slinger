#include <raylib.h>
#include <raymath.h>
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
  struct Ball* ball = st->balls;
  while (ball) {
    ball->vel_y += GRAVITY_ACCELERATION*DeltaTime;
    ball->y     += ball->vel_y*DeltaTime;
    ball->x     += ball->vel_x*DeltaTime;
    ball = ball->next;
  }
  clear_errant_balls(st);

  if (!IsMouseButtonDown(0) && st->clicking_last_frame && st->cooldown_left == 0) summon_ball(st);
  st->cooldown_left -= DeltaTime;
  st->cooldown_left = MAX(st->cooldown_left, 0);

  check_for_balls_in_clouds(st);

  st->clicking_last_frame = IsMouseButtonDown(0); // for next frame!
}

void render(State* st) {
  float h = (float)GetScreenHeight();

  draw_slingshot(st);
  draw_slingshot_strings();
  draw_ready_ball(st);
  draw_score(st);

  struct Ball* ball = st->balls;
  while (ball) {
    draw_ball(st, ball);
    ball = ball->next;
  }

  if (DebugSymbols) {
     draw_mouse_circle();
     draw_slingshot_radius();
     draw_numeric_debug_info(st);
  }
}

State init() {
  InitWindow(INITIAL_SCREEN_WIDTH, INITIAL_SCREEN_HEIGHT, "Cloud Sling");
  SetTargetFPS(60.0);

  State st = new_state();

  Texture2D slingshot = LoadTexture("../assets/slingshot.png");
  Texture2D ball      = LoadTexture("../assets/ball.png");

  st.textures.slingshot = slingshot;
  st.textures.ball      = ball;

  return st;
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

  State st = init();

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
