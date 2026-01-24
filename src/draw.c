#pragma once

#include <assert.h>
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <sys/param.h>
#include "definitions.c"
#include "constants.c"
#include "domain.c"

void draw_cloud(State* st, f32 psi, f32 height_percent) {
  assert(st != NULL);
  DrawRectangleRec(cloud_rectangle(st->cloud_t, psi, height_percent), COLOR_CLOUD);
}


void draw_ball(State* st, struct Ball* ball) {
  assert(st != NULL);
  assert(ball != NULL);
  Rectangle underlying = (Rectangle){
      .x = ball->x - BALL_RADIUS,
      .y = ball->y - BALL_RADIUS,
      .width  = BALL_RADIUS*2.0,
      .height = BALL_RADIUS*2.0,
  };
  DrawTexturePro(
    st->textures.ball,
    (Rectangle){ .x = 0.0, .y = 0.0, .width = st->textures.ball.width, .height = st->textures.ball.height},
    underlying,
    Vector2Zero(),
    ball->angle * RAD2DEG,
    WHITE);
  if(st->debug_mode) DrawCircle(ball->x, ball->y, BALL_RADIUS, (Color){255, 0, 0, 100});
}

void draw_slingshot(State* st) {
  assert(st != NULL);
  i32 x = (i32)((f32)GetScreenWidth()/2.0f - (f32)SLINGSHOT_WIDTH/2.0);
  Rectangle underlying = (Rectangle){
    .x      = x,
    .y      = GetScreenHeight()-SLINGSHOT_HEIGHT,
    .width  = SLINGSHOT_WIDTH,
    .height = SLINGSHOT_HEIGHT,
  };

  DrawTexturePro(
    st->textures.slingshot,
    (Rectangle){.x = 0.0, .y = 0.0, .width = st->textures.slingshot.width, .height = st->textures.slingshot.height},
    underlying,
    Vector2Zero(), 0.0, WHITE);
}

// Inputs are at the center of the ends
void draw_slingshot_string(Vector2 a, Vector2 b) {
  DrawLineEx(a, b, SLINGSHOT_STRING_THICKNESS, COLOR_SLINGSHOT_STRING);
}

void draw_slingshot_strings() {
  f32 x = SLINGSHOT_CENTER.x;
  f32 y = SLINGSHOT_CENTER.y;
  f32 dx = SLINGSHOT_WIDTH  * SLINGSHOT_STRING_SEPARATION_X;
  f32 dy = SLINGSHOT_HEIGHT * SLINGSHOT_STRING_SEPARATION_Y;

  Vector2 anchor_left  = (Vector2){ .x = x - dx, .y = y + dy };
  Vector2 anchor_right = (Vector2){ .x = x + dx, .y = y + dy };
  Vector2 anchor_unheld = (Vector2){ .x = x, .y = y+SLINGSHOT_MAX_RADIUS/2.0 };

  if (!IsMouseButtonDown(0)) {
    draw_slingshot_string(anchor_left, anchor_unheld);
    draw_slingshot_string(anchor_right, anchor_unheld);
    return;
  }

  Vector2 target = get_slingshot_focus();
  draw_slingshot_string(anchor_left, target);
  draw_slingshot_string(anchor_right, target);

}

// Draws ball in slingshot focus when cooldown permits, does nothing otherwise
// Balls don't actually exist until they're thrown!
void draw_ready_ball(State* st) {
  assert(st != NULL);
  if (st->cooldown_left > 0) return;


  // If the mouse is clicked, use slingshot focus
  // Otherwise, use the unheld one
  Vector2 t = get_slingshot_focus();
  struct Ball b = (struct Ball){
    .x     = t.x,
    .y     = t.y,
    .vel_x = 0,
    .vel_y = 0,
    .next  = NULL
  };
  if (!IsMouseButtonDown(0)) {
    b.x = SLINGSHOT_CENTER.x;
    b.y = SLINGSHOT_CENTER.y + SLINGSHOT_MAX_RADIUS/2.0;
  };

  // Passing a pointer to the stack, but the function doesn't keep it around, so it's fine
  // I miss the borrow checker
  draw_ball(st, &b);
}

void draw_score(State* st) {
  assert(st != NULL);
  char buf[256];
  sprintf(buf, "%d", st->score);
  i32 width = MeasureText(buf, SCORE_FONTSIZE);
  DrawText(buf, GetScreenWidth()/2.0 - width/2.0, SCORE_PADDING_Y, SCORE_FONTSIZE, WHITE);

}

void draw_mouse_circle() {
  Vector2 pos = GetMousePosition();
  DrawCircle(pos.x, pos.y, 5, (Color){255, 255, 255, 80});
}

void draw_slingshot_radius() {
  DrawCircleLines(SLINGSHOT_CENTER.x, SLINGSHOT_CENTER.y, SLINGSHOT_MAX_RADIUS, (Color){255, 0, 0, 80});
}

void draw_basket_hitbox(State* st) {
  assert(st != NULL);

  i32 thickness = 2;
  i32 h = GetScreenHeight();

  Rectangle lower_basket = cloud_basket_hitbox(st->cloud_t, st->cloud_psi_lower, h*CLOUD_LOWER_Y_PERCENTAGE);
  DrawRectangleLinesEx(lower_basket, thickness, MAGENTA);

  Rectangle upper_basket = cloud_basket_hitbox(st->cloud_t, st->cloud_psi_upper, h*CLOUD_UPPER_Y_PERCENTAGE);
  DrawRectangleLinesEx(upper_basket, thickness, MAGENTA);
}

void draw_numeric_debug_info(State* st) {
  char buf[256];

  sprintf(buf, "Cooldown: %.3f", st->cooldown_left);
  DrawText(buf, 5, 5+12*0, 12, WHITE);

  sprintf(buf, "Cloud t: %.3f", st->cloud_t);
  DrawText(buf, 5, 5+12*1, 12, WHITE);

  sprintf(buf, "Num balls: %d", count_balls(st));
  DrawText(buf, 5, 5+12*2, 12, WHITE);
}
