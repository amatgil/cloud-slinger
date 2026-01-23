#ifndef  _DOMAIN_H
#define _DOMAIN_H

#include "constants.c"
#include "definitions.c"
#include "draw.c"
#include "raymath.h"
#include <math.h>

void summon_ball(State* st) {
  Vector2 target = get_slingshot_focus();
  Vector2 center = get_slingshot_center();
  float power_factor = (float)Vector2Distance(target, center) / SLINGSHOT_MAX_RADIUS;
  float theta = Vector2LineAngle(target, center) + TAU/2.0;

  int vel_x = -cos(theta) * BALL_MAXIMUM_INITIAL_SPEED*power_factor;
  int vel_y = sin(theta) * BALL_MAXIMUM_INITIAL_SPEED*power_factor;

  printf("Summoning ball: vel = (%d, %d), theta = %f\n", vel_x, vel_y, theta);
  add_ball(st, target.x, target.y, vel_x, vel_y);

}

// delete balls whose position is absurd
void clear_errant_balls(State* st) {
  // TODO: iterate over, if the position is -10000 < x < 10000, remove_ball(index)
}

#endif
