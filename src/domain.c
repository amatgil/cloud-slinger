#pragma once

#include "constants.c"
#include "definitions.c"
#include "raylib.h"
#include "raymath.h"
#include <math.h>
#include <assert.h>

// x position
int cloud_position_x(float t, float psi) {
  return (sin(t*TAU + psi) + 1.0)/2.0 * (GetScreenWidth()-CLOUD_WIDTH);
}

Rectangle cloud_rectangle(float t, float psi, float height_percent) {
  float h = (float)GetScreenHeight();
  return (Rectangle) {
    .x      = cloud_position_x(t, psi),
    .y      = (int)(h*height_percent),
    .width  =  CLOUD_WIDTH,
    .height =  CLOUD_HEIGHT };
}

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
  int vel_y =  sin(theta) * BALL_MAXIMUM_INITIAL_SPEED*power_factor;

  add_ball(st, focus.x, focus.y, vel_x, vel_y);
  st->cooldown_left = BALL_COOLDOWN;

}

Rectangle cloud_basket_hitbox(float t, float psi, float y) {
  i32 w = CLOUD_WIDTH *CLOUD_BASKET_PERCENTAGE_X;
  i32 h = CLOUD_HEIGHT*CLOUD_BASKET_PERCENTAGE_Y;

  return (Rectangle){
    .x = cloud_position_x(t, psi) + (CLOUD_WIDTH-w)/2.0,
    .y = y,
    .width  = w,
    .height = h,
  };

}

bool cloud_basket_collision(struct Ball* ball, float t, float psi, float y) {
  return false; // TODO
}

// only handles _one_ ball. if there are more, the first one in the
// list will get handled
void handle_ball_cloud_baskets(State* st) {
  u32 index = 0;
  struct Ball* b = st->balls;
  while (b) {
    if (b->vel_y > 0) {
      if (cloud_basket_collision(b, st->cloud_t, st->cloud_psi_lower, CLOUD_LOWER_Y)) {
        st->score += POINTS_FOR_LOWER;
        remove_ball(st, index);
        return;
      }
      if (cloud_basket_collision(b, st->cloud_t, st->cloud_psi_upper, CLOUD_UPPER_Y)) {
        st->score += POINTS_FOR_UPPER;
        remove_ball(st, index);
        return;
      }
    }

    b = b->next;
    index += 1;
  }
}

// delete balls whose position is absurd
// deletes a maximum of ONE ball
void clear_errant_balls(State* st) {
  assert(st != NULL);
  struct Ball* b = st->balls;
  u32 index = 0;
  while (b) {
    if (fabs(b->x) > 20*GetScreenWidth() || fabs(b->y) > 20*GetScreenHeight()) {
      remove_ball(st, index);
      return; // otherwise the index will be out of sync wrt the list (it's not DoubleEnded)
    }
    b = b->next;
    index += 1;
  }
}
