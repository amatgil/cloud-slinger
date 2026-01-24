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

f32 DeltaTime;
i32 INITIAL_SCREEN_WIDTH  = 350;
i32 INITIAL_SCREEN_HEIGHT = 700;


void update(State* st) {
  // t = fract(t+DT*dt)
  st->cloud_t += CLOUD_DT*DeltaTime;
  st->cloud_t = st->cloud_t - (f32)(i32)st->cloud_t;
  Ball* ball = st->balls;
  while (ball) {
    ball->vel_y += GRAVITY_ACCELERATION*DeltaTime;
    ball->y     += ball->vel_y*DeltaTime;
    ball->x     += ball->vel_x*DeltaTime;
    ball->angle += BALL_ROTATE_SPEED*DeltaTime;
    ball = ball->next;
  }
  clear_errant_balls(st);

  if (!IsMouseButtonDown(0) && st->clicking_last_frame && st->cooldown_left == 0) summon_ball(st);
  st->cooldown_left -= DeltaTime;
  st->cooldown_left = MAX(st->cooldown_left, 0);

  handle_ball_cloud_baskets(st);

  st->clicking_last_frame = IsMouseButtonDown(0); // for next frame!
}

void render(State* st) {
  f32 h = (f32)GetScreenHeight();

  draw_cloud(st, st->cloud_psi_lower, CLOUD_LOWER_Y_PERCENTAGE);
  draw_cloud(st, st->cloud_psi_upper, CLOUD_UPPER_Y_PERCENTAGE);
  draw_slingshot(st);
  draw_slingshot_strings();
  draw_ready_ball(st);
  draw_score(st);

  Ball* ball = st->balls;
  while (ball) {
    draw_ball(st, ball);
    ball = ball->next;
  }

  if (st->debug_mode) {
     draw_mouse_circle();
     draw_slingshot_radius();
     draw_numeric_debug_info(st);
     draw_basket_hitbox(st);
  }
}

State init(bool debug_mode) {
  InitWindow(INITIAL_SCREEN_WIDTH, INITIAL_SCREEN_HEIGHT, "Cloud Sling");
  SetTargetFPS(60.0);

  State st = new_state();
  st.debug_mode = debug_mode;

  Texture2D slingshot = LoadTexture("../assets/slingshot.png");
  Texture2D ball      = LoadTexture("../assets/ball.png");

  st.textures.slingshot = slingshot;
  st.textures.ball      = ball;

  return st;
}


void check_and_set_dim_from_args(i32 i, i32 argc, char** argv, char* flag, i32* where) {
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

int main(i32 argc, char** argv) {
  bool debug_mode = false;
  for (i32 i = 1; i < argc; ++i) {
    if (strcmp(TextToLower(argv[i]), "-d") == 0) debug_mode = true;
    check_and_set_dim_from_args(i, argc, argv, "-w", &INITIAL_SCREEN_WIDTH);
    check_and_set_dim_from_args(i, argc, argv, "-h", &INITIAL_SCREEN_HEIGHT);
  }

  State st = init(debug_mode);

  while (!WindowShouldClose()) {
    DeltaTime = GetFrameTime();
    if (!st.paused) update(&st);
    if (IsKeyPressed(KEY_SPACE)) st.paused = !st.paused;

    BeginDrawing();
    ClearBackground(COLOR_BACKGROUND);
    render(&st);
    EndDrawing();

  }

  CloseWindow();

  return 0;
}
