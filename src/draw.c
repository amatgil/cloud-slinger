#pragma once

#include <assert.h>
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <sys/param.h>
#include "constants.c"
#include "domain.c"
#include "definitions.c"



// Assumes we're in drawing mode
void draw_ball(State* st, Ball* ball) {
  assert(st != NULL);
  assert(ball != NULL);
  Rectangle underlying = (Rectangle){
      .x = ball->x,
      .y = ball->y,
      .width  = BALL_RADIUS*2.0,
      .height = BALL_RADIUS*2.0,
  };
  Texture2D* tex = &st->textures.ball;
  DrawTexturePro(
    st->textures.ball,
    (Rectangle){ .x = 0.0, .y = 0.0, .width = (f32)tex->width, .height = (f32)tex->height},
    underlying,
    (Vector2){.x = BALL_RADIUS, .y = BALL_RADIUS },
    ball->angle * RAD2DEG,
    WHITE);
  if(st->debug_mode) DrawCircle((i32)ball->x, (i32)ball->y, BALL_RADIUS, (Color){255, 0, 0, 100});
}

// Assumes we're in drawing mode
void draw_slingshot(State* st) {
  assert(st != NULL);
  f32 x = ((f32)GetScreenWidth()/2.0f - (f32)SLINGSHOT_WIDTH/2.0f);
  Rectangle underlying = (Rectangle){
    .x      = x,
    .y      = (f32)GetScreenHeight()-SLINGSHOT_HEIGHT,
    .width  = SLINGSHOT_WIDTH,
    .height = SLINGSHOT_HEIGHT,
  };

  DrawTexturePro(
    st->textures.slingshot,
    (Rectangle){.x = 0.0, .y = 0.0, .width = (f32)st->textures.slingshot.width, .height = (f32)st->textures.slingshot.height},
    underlying,
    Vector2Zero(), 0.0, WHITE);
}

// Inputs are at the center of the ends
// Assumes we're in drawing mode
void draw_slingshot_string(Vector2 a, Vector2 b) {
  DrawLineEx(a, b, SLINGSHOT_STRING_THICKNESS, COLOR_SLINGSHOT_STRING);
}

// Assumes we're in drawing mode
void draw_slingshot_strings(void) {
  f32 x = SLINGSHOT_CENTER.x;
  f32 y = SLINGSHOT_CENTER.y;
  f32 dx = SLINGSHOT_WIDTH  * SLINGSHOT_STRING_SEPARATION_X;
  f32 dy = SLINGSHOT_HEIGHT * SLINGSHOT_STRING_SEPARATION_Y;

  Vector2 anchor_left  = (Vector2){ .x = x - dx, .y = y + dy };
  Vector2 anchor_right = (Vector2){ .x = x + dx, .y = y + dy };
  Vector2 anchor_unheld = (Vector2){ .x = x, .y = y+(f32)SLINGSHOT_MAX_RADIUS/2 };

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
// Assumes we're in drawing mode
void draw_ready_ball(State* st) {
  assert(st != NULL);
  if (st->slingshot_cooldown > 0) return;


  // If the mouse is clicked, use slingshot focus
  // Otherwise, use the unheld one
  Vector2 t = get_slingshot_focus();
  Ball b = (Ball){
    .x     = t.x,
    .y     = t.y,
    .vel_x = 0,
    .vel_y = 0,
    .next  = NULL
  };
  if (!IsMouseButtonDown(0)) {
    b.x = (f32)SLINGSHOT_CENTER.x;
    b.y = SLINGSHOT_CENTER.y + SLINGSHOT_MAX_RADIUS/2.0f;
  };

  // Passing a pointer to the stack, but the function doesn't keep it around, so it's fine
  // I miss the borrow checker
  draw_ball(st, &b);
}

// Assumes we're in drawing mode
void draw_score(State* st) {
  assert(st != NULL);
  char buf[256];
  sprintf(buf, "%d", st->score);
  i32 width = MeasureText(buf, SCORE_FONTSIZE);
  DrawText(buf, GetScreenWidth()/2 - width/2, SCORE_PADDING_Y, SCORE_FONTSIZE, WHITE);
}

// Assumes we're in drawing mode
void draw_mouse_circle(void) {
  Vector2 pos = GetMousePosition();
  DrawCircle((i32)pos.x, (i32)pos.y, 5, (Color){255, 255, 255, 80});
}

// Assumes we're in drawing mode
void draw_slingshot_radius(void) {
  DrawCircleLines((i32)SLINGSHOT_CENTER.x, (i32)SLINGSHOT_CENTER.y, SLINGSHOT_MAX_RADIUS, (Color){255, 0, 0, 80});
}

// Assumes we're in drawing mode
void draw_basket_hitbox(Basket* basket) {
  assert(basket != NULL);

  Rectangle hitbox = basket_hitbox(basket);
  DrawRectangleLinesEx(hitbox, 2, MAGENTA);
}

// Assumes we're in drawing mode
void draw_numeric_state_info(State* st) {
  char buf[256];

  sprintf(buf, "Cooldown: %.3f", st->slingshot_cooldown);
  DrawText(buf, 5, 5+12*0, 12, WHITE);

  sprintf(buf, "Num baskets: %d", count_baskets(st));
  DrawText(buf, 5, 5+12*1, 12, WHITE);

  sprintf(buf, "Num balls: %d", count_balls(st));
  DrawText(buf, 5, 5+12*2, 12, WHITE);

  sprintf(buf, "Paused: %d", st->paused);
  DrawText(buf, 5, 5+12*3, 12, WHITE);
}
