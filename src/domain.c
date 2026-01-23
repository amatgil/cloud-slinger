#ifndef  _DOMAIN_H
#define _DOMAIN_H

#include "constants.c"
#include "definitions.c"
#include "draw.c"
#include "raylib.h"
#include "raymath.h"
#include <math.h>

// x position
int cloud_position_lower(float t) {
  return (sin(    t*TAU) + 1.0)/2.0 * (GetScreenWidth()-CLOUD_WIDTH);
}
int cloud_position_upper(float t) {
  return (sin(1.6+t*TAU) + 1.0)/2.0 * (GetScreenWidth()-CLOUD_WIDTH);
}

Rectangle cloud_rectangle_lower(float t) {
  float h = (float)GetScreenHeight();
  return (Rectangle) {
    .x      = cloud_position_lower(t),
    .y      = (int)(h*CLOUD_LOWER_Y),
    .width  =  CLOUD_WIDTH,
    .height =  CLOUD_HEIGHT };
}

Rectangle cloud_rectangle_upper(float t) {
  float h = (float)GetScreenHeight();
  return (Rectangle) {
    .x      = cloud_position_upper(t),
    .y      = (int)(h*CLOUD_UPPER_Y),
    .width  =  CLOUD_WIDTH,
    .height =  CLOUD_HEIGHT };
}

void summon_ball(State* st) {
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

#endif
