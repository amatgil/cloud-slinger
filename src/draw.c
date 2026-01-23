#ifndef _DRAW_H
#define _DRAW_H

#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <sys/param.h>
#include "definitions.c"
#include "constants.c"

void draw_ball(struct Ball* ball) {
  DrawCircle(ball->x, ball->y, BALL_RADIUS, RED);
}

void draw_slingshot() {
  int x = (int)((float)GetScreenWidth()/2.0f - (float)SLINGSHOT_WIDTH/2.0);
  DrawRectangle(x,
    GetScreenHeight()-SLINGSHOT_HEIGHT,
    SLINGSHOT_WIDTH,
    SLINGSHOT_HEIGHT,
    COLOR_SLINGSHOT);
}

// Inputs are at the center of the ends
void draw_slingshot_string(Vector2 a, Vector2 b) {
  DrawLineEx(a, b, SLINGSHOT_STRING_THICKNESS, COLOR_SLINGSHOT_STRING);
}

Vector2 get_slingshot_focus() {
  Vector2 anchor_base = SLINGSHOT_CENTER;

  float length = MIN(Vector2Distance(GetMousePosition(), anchor_base), SLINGSHOT_MAX_RADIUS);
  Vector2 dir =  Vector2Normalize(Vector2Subtract(GetMousePosition(), anchor_base));
  return Vector2Add(anchor_base, Vector2Scale(dir, length));
}

void draw_slingshot_strings() {
  float x = SLINGSHOT_CENTER.x;
  float y = SLINGSHOT_CENTER.y;
  float dx = SLINGSHOT_WIDTH/2.0;

  Vector2 anchor_left  = (Vector2){ .x = x - dx, .y = y };
  Vector2 anchor_right = (Vector2){ .x = x + dx, .y = y };
  Vector2 anchor_unheld = (Vector2){ .x = x, .y = y+SLINGSHOT_MAX_RADIUS };

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
    b.y = SLINGSHOT_CENTER.y + SLINGSHOT_MAX_RADIUS;
  };

  // Passing a pointer to the stack, but the function doesn't keep it around, so it's fine
  // I miss the borrow checker
  draw_ball(&b);
}

void draw_score(State* st) {
  char buf[256];
  sprintf(buf, "%d", st->score);
  int width = MeasureText(buf, SCORE_FONTSIZE);
  DrawText(buf, GetScreenWidth()/2.0 - width/2.0, SCORE_PADDING_Y, SCORE_FONTSIZE, WHITE);

}

void draw_mouse_circle() {
  Vector2 pos = GetMousePosition();
  DrawCircle(pos.x, pos.y, 5, (Color){255, 255, 255, 80});
}

void draw_slingshot_radius() {
  DrawCircleLines(SLINGSHOT_CENTER.x, SLINGSHOT_CENTER.y, SLINGSHOT_MAX_RADIUS, (Color){255, 0, 0, 80});
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

#endif
