#pragma once
#include "constants.c"
#include "raylib.h"
#include "types.c"
#include <stdbool.h> // raylib also imports this so it's whatever

// Assumes we're in drawing mode
bool show_reset_button(void) {
  char text[] = "Reset";
  const u32 PAD_X = 10; // One per side
  const u32 PAD_Y = 10; // One per side
  u32 text_height = SCORE_FONTSIZE;
  u32 text_width  = (u32)MeasureText(text, SCORE_FONTSIZE);

  u32 text_x      = (u32)GetScreenWidth()/2   - text_width/2;
  u32 text_y      = (u32)GetScreenHeight()/2  - text_height/2;

  Rectangle btn = (Rectangle) {
    .x = (f32)text_x - PAD_X,
    .y = (f32)text_y - PAD_Y,
    .width  = (f32)text_width  + PAD_X*2,
    .height = (f32)text_height + PAD_Y*2,
  };

  DrawRectangleRounded(btn, 0.7f, 10, (Color){147, 153, 178, 255});
  DrawText(text, (i32)text_x, (i32)text_y, SCORE_FONTSIZE, WHITE);

  bool am_clicked = IsMouseButtonDown(0) &&
                      CheckCollisionCircleRec(GetMousePosition(), 5, btn);
  return am_clicked;
}
