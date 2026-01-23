#pragma once

#include "constants.c"
#include "definitions.c"
#include "raylib.h"
#include "raymath.h"
#include <math.h>
#include <assert.h>

Vector2 get_slingshot_focus() {
  Vector2 anchor_base = SLINGSHOT_CENTER;

  float length = MIN(Vector2Distance(GetMousePosition(), anchor_base), SLINGSHOT_MAX_RADIUS);
  Vector2 dir =  Vector2Normalize(Vector2Subtract(GetMousePosition(), anchor_base));
  return Vector2Add(anchor_base, Vector2Scale(dir, length));
}


void summon_ball(State* st) {
  assert(st != NULL);

  Vector2 focus = get_slingshot_focus();
  float power_factor = (float)Vector2Distance(focus, SLINGSHOT_CENTER) / SLINGSHOT_MAX_RADIUS;
  float theta = Vector2LineAngle(focus, SLINGSHOT_CENTER) + TAU/2.0;

  int vel_x = -cos(theta) * BALL_MAXIMUM_INITIAL_SPEED*power_factor;
  int vel_y = sin(theta) * BALL_MAXIMUM_INITIAL_SPEED*power_factor;

  add_ball(st, focus.x, focus.y, vel_x, vel_y);
  st->cooldown_left = BALL_COOLDOWN;

}

void check_for_balls_in_clouds(State* st) {
}

// delete balls whose position is absurd
void clear_errant_balls(State* st) {
  // TODO: iterate over, if the position is -10000 < x < 10000, remove_ball(index)
}
