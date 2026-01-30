#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <time.h>
#include "definitions.c"
#include "draw.c"
#include "constants.c"
#include "sun.c"
#include "domain.c"

f32 DeltaTime;
i32 INITIAL_SCREEN_WIDTH  = 350;
i32 INITIAL_SCREEN_HEIGHT = 700;


State init(bool debug_mode) {
  srand((u32)time(NULL));
  InitWindow(INITIAL_SCREEN_WIDTH, INITIAL_SCREEN_HEIGHT, "Cloud Sling");
  SetTargetFPS(60.0);

  State st = (State){
    .debug_mode = debug_mode,
    .paused = false,
    .baskets = NULL,
    .balls = NULL,
    .clicking_last_frame = false,
    .slingshot_cooldown = 0.0,
    .hp = INITIAL_HP,
    .hp_decrease_vel = INITIAL_HP_DECREASE_VEL,
    .score = 0,
    .textures = {
      .default_purple  = LoadTexture("../assets/default_purple.png"),
      .sun             = LoadTexture("../assets/sun.png"),
      .sun_shooting    = LoadTexture("../assets/sun_shooting.png"),
      .slingshot       = LoadTexture("../assets/slingshot.png"),
      .ball            = LoadTexture("../assets/ball.png"),
      .cloud           = LoadTexture("../assets/cloud.png"),
      .pelican         = LoadTexture("../assets/default_purple.png"),
    },
    .laser_angle = 0.0,
    .laser_magnitude = INFINITY,
    .laser_cooldown = LASER_STARTING_COOLDOWN,
  };

  Basket* cloud_upper = new_basket_cloud(&st.textures.cloud, 1.6f, CLOUD_UPPER_Y_PERCENTAGE*(f32)GetScreenHeight(), 2);
  Basket* cloud_lower = new_basket_cloud(&st.textures.cloud, 0.0f, CLOUD_LOWER_Y_PERCENTAGE*(f32)GetScreenHeight(), 1);
  Basket* birb = new_basket_pelican(&st.textures.pelican, true, CLOUD_LOWER_Y_PERCENTAGE*(f32)GetScreenHeight() + 40, 4);
  cloud_lower->next = cloud_upper;
  cloud_upper->next = birb;
  st.baskets = cloud_lower;

  return st;
}

void update(State* st) {
  {
    Ball* ball = st->balls;
    while (ball) {
      ball->vel_y += GRAVITY_ACCELERATION*DeltaTime;
      ball->y     += ball->vel_y*DeltaTime;
      ball->x     += ball->vel_x*DeltaTime;
      ball->angle += BALL_ROTATE_SPEED*DeltaTime;
      ball = ball->next;
    }
    clear_errant_balls(st);
    handle_ball_baskets_collisions(st);
  }

  {
    Basket* basket = st->baskets;
    while (basket) {
      update_basket_position(basket, DeltaTime);
      basket = basket->next;
    }
    clear_errant_baskets(st);
  }


  advance_laser(st, DeltaTime);

  switch (st->status) {
    case S_Playing: {
      handle_scoring_and_hp(st, DeltaTime);
      throw_laser(st);
      handle_laser_collisions(st);
      update_slingshot_status(st, DeltaTime);
      if (st->debug_mode && IsKeyPressed(KEY_L)) st->laser_cooldown = 0.0;

      if (!IsMouseButtonDown(0) && st->clicking_last_frame && st->slingshot_cooldown <= 0) summon_ball(st);


      handle_possible_loss(st);

      break;
    }
    case S_Dead: {
      char buf[] = "You've died :(";
      DrawText(buf,
        GetScreenWidth()/2 - MeasureText(buf, SCORE_FONTSIZE)/2,
        GetScreenHeight()/2,
        SCORE_FONTSIZE, WHITE);
      break;
    }
  }


  st->clicking_last_frame = IsMouseButtonDown(0); // for next frame!
}

// Assumes we're in drawing mode
void render(State* st) {
  draw_laser(st);
  draw_score(st);
  draw_slingshot(st); draw_slingshot_strings();
  draw_ready_ball(st);
  draw_sun(st);

  Ball* ball = st->balls;
  while (ball) {
    draw_ball(st, ball);
    ball = ball->next;
  }

  Basket* basket = st->baskets;
  while (basket) {
    draw_basket(basket);
    if (st->debug_mode) draw_basket_hitbox(basket);
    basket = basket->next;
  }

  if (st->debug_mode) {
    draw_laser_hitbox(st);
    draw_slingshot_radius();
    draw_mouse_circle();
    draw_laser_range();
    draw_laser_path(st);
    draw_numeric_state_info(st);
  }
}



void check_and_set_dim_from_args(i32 i, i32 argc, char** argv, const char* flag, i32* where) {
  if (strcmp(TextToLower(argv[i]), flag) == 0) {
    if (i+1 >= argc) {
      printf("'%s' given without actual value\n", flag);
      exit(1);
    }

    char* endptr;
    i32 x = (i32)strtol(argv[i+1], &endptr, 10); // Assumeixo que és correcte perquè C va com vol
    if (*endptr != 0) {
      printf("'The argument of '%s' ('%s') was not a valid integer\n", flag, argv[i+1]);
      exit(2);
    }
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

  if (st.debug_mode) {
    printf("Initializing with width=%d and height=%d\n", INITIAL_SCREEN_WIDTH, INITIAL_SCREEN_HEIGHT);
  }

  while (!WindowShouldClose()) {
    DeltaTime = GetFrameTime();
    if (!st.paused) update(&st);
    if (IsKeyPressed(KEY_SPACE)) st.paused = !st.paused;
    if (IsKeyPressed(KEY_D))     st.debug_mode = !st.debug_mode;

    BeginDrawing();
    ClearBackground(COLOR_BACKGROUND);
    render(&st);
    EndDrawing();

  }

  CloseWindow();

  return 0;
}
