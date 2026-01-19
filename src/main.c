#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "balls.c"

#define CLOUD_WIDTH  120
#define CLOUD_HEIGHT 25
#define CLOUD_LOWER_Y 0.15
#define CLOUD_UPPER_Y 0.35
#define CLOUD_DT 0.1
#define BALL_RADIUS 8

#define INITIAL_SCREEN_WIDTH  350
#define INITIAL_SCREEN_HEIGHT INITIAL_SCREEN_WIDTH * 2

#define SLINGSHOT_WIDTH 40
#define SLINGSHOT_HEIGHT 150


float DeltaTime;

typedef struct {
  float cloud_lower_t; // how far along movement, [0..1)
  float cloud_upper_t; // how far along movement, [0..1)
  struct Ball* balls;
} State ;

State new_state() {
  return (State){0.0, 0.0, NULL};
}

void add_ball(State* st, int ball_x, int ball_y, int ball_vel_x, int ball_vel_y) {
  struct Ball* ball = (struct Ball*)malloc(sizeof(struct Ball));
  ball->x = ball_x;
  ball->y = ball_y;
  ball->vel_x = ball_vel_x;
  ball->vel_y = ball_vel_y;
  ball->next = NULL;

  struct Ball* prev = st->balls;
  struct Ball* curr = NULL;
  if (!prev) {
    st->balls = ball;
    return;
  }

  curr = prev->next;
  while (curr) {
    prev = curr;
    curr = curr->next;
  }

  prev->next = ball;
}

void draw_ball(struct Ball* ball) {
  DrawCircle(ball->x, ball->y, BALL_RADIUS, RED);
}

void draw_slingshot() {
  int x = (int)((float)GetScreenWidth()/2.0f - (float)SLINGSHOT_WIDTH/2.0);
  DrawRectangle(x,
                GetScreenHeight()-SLINGSHOT_HEIGHT,
                SLINGSHOT_WIDTH,
                SLINGSHOT_HEIGHT,
                BLUE);
}

void render(State* st) {
  // idea: e^(-f(x)^2) where f(x) = max(x+a, 0) + min(x-a,0)
  float theta = st->cloud_lower_t*2.0*M_PI;

  int lower_x = (float)CLOUD_WIDTH + ((float)GetScreenWidth() - 2.0f*(float)CLOUD_WIDTH)*sin(theta);
  int upper_x = (float)CLOUD_WIDTH + ((float)GetScreenWidth() - 2.0f*(float)CLOUD_WIDTH)*sin(1.6+theta);
  st->cloud_lower_t += CLOUD_DT*DeltaTime;
  st->cloud_upper_t += CLOUD_DT*DeltaTime;

  st->cloud_lower_t = st->cloud_lower_t - (float)(int)st->cloud_lower_t;
  st->cloud_upper_t = st->cloud_upper_t - (float)(int)st->cloud_upper_t;

  float screen_h = (float)GetScreenHeight();
  DrawRectangle(lower_x, (int)(screen_h*CLOUD_LOWER_Y), CLOUD_WIDTH, CLOUD_HEIGHT, WHITE);
  DrawRectangle(upper_x, (int)(screen_h*CLOUD_UPPER_Y), CLOUD_WIDTH, CLOUD_HEIGHT, WHITE);


  draw_slingshot();

  struct Ball* ball = st->balls;
  while (ball) {
    draw_ball(ball);
    ball = ball->next;
  }
}



int main(void) {
  State st = new_state();

  InitWindow(INITIAL_SCREEN_WIDTH, INITIAL_SCREEN_HEIGHT, "Cloud Sling");

  SetTargetFPS(60.0);

  add_ball(&st, 100, 100, 10, 10);
  add_ball(&st, 100, 200, 10, 10);
  add_ball(&st, 100, 300, 10, 10);
  while (!WindowShouldClose()) {
    DeltaTime = GetFrameTime();
    BeginDrawing();
    ClearBackground(BLACK);
    render(&st);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
