#pragma once
#include "constants.c"
#include "raylib.h"
#include "types.c"
#include "domain.c"
#include <stdbool.h> // raylib also imports this so it's whatever

// Assumes we're in drawing mode
bool reset_button(State* st) {
  assert(st != NULL);

  char text[] = "Reset";
  const u32 PAD_X = 10; // One per side
  const u32 PAD_Y = 10; // One per side
  f32 text_height = SCORE_FONTSIZE;
  f32 text_width  = MeasureTextEx(st->font, text, SCORE_FONTSIZE, 0.0).x;

  f32 text_x      = (f32)GetScreenWidth()/2   - text_width/2;
  f32 text_y      = (f32)GetScreenHeight()/2  - text_height/2;

  Rectangle btn = (Rectangle) {
    .x = (f32)text_x - PAD_X,
    .y = (f32)text_y - PAD_Y,
    .width  = (f32)text_width  + PAD_X*2,
    .height = (f32)text_height + PAD_Y*2,
  };

  DrawRectangleRounded(btn, 0.7f, 10, (Color){147, 153, 178, 255});
  DrawTextEx(st->font, text, (Vector2){.x=text_x,.y=text_y}, SCORE_FONTSIZE, 0.0, WHITE);

  bool am_clicked = IsMouseButtonDown(0) &&
                      CheckCollisionCircleRec(GetMousePosition(), 5, btn);
  return am_clicked;
}
