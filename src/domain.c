#pragma once

#include "constants.c"
#include "definitions.c"
#include "raylib.h"
#include "raymath.h"
#include <math.h>
#include <assert.h>
#include <stdio.h>

// x position
i32 cloud_position_x(f32 t, f32 psi) {
  return (sin(t*TAU + psi) + 1.0)/2.0 * (GetScreenWidth()-CLOUD_WIDTH);
}

Rectangle cloud_rectangle(f32 t, f32 psi, f32 height_percent) {
  f32 h = (f32)GetScreenHeight();
  return (Rectangle) {
    .x      = cloud_position_x(t, psi),
    .y      = (i32)(h*height_percent),
    .width  =  CLOUD_WIDTH,
    .height =  CLOUD_HEIGHT };
}

Vector2 get_slingshot_focus() {
  Vector2 anchor_base = SLINGSHOT_CENTER;

  f32 length = MIN(Vector2Distance(GetMousePosition(), anchor_base), SLINGSHOT_MAX_RADIUS);
  Vector2 dir =  Vector2Normalize(Vector2Subtract(GetMousePosition(), anchor_base));
  return Vector2Add(anchor_base, Vector2Scale(dir, length));
}


void summon_ball(State* st) {
  assert(st != NULL);

  Vector2 focus = get_slingshot_focus();
  f32 power_factor = (f32)Vector2Distance(focus, SLINGSHOT_CENTER) / SLINGSHOT_MAX_RADIUS;
  f32 theta = Vector2LineAngle(focus, SLINGSHOT_CENTER) + TAU/2.0;

  i32 vel_x = -cos(theta) * BALL_MAXIMUM_INITIAL_SPEED*power_factor;
  i32 vel_y =  sin(theta) * BALL_MAXIMUM_INITIAL_SPEED*power_factor;

  add_ball(st, focus.x, focus.y, vel_x, vel_y);
  st->cooldown_left = BALL_COOLDOWN;

}

Rectangle cloud_basket_hitbox(f32 t, f32 psi, f32 y) {
  i32 w = CLOUD_WIDTH *CLOUD_BASKET_PERCENTAGE_X;
  i32 h = CLOUD_HEIGHT*CLOUD_BASKET_PERCENTAGE_Y;

  return (Rectangle){
    .x = cloud_position_x(t, psi) + (CLOUD_WIDTH-w)/2.0,
    .y = y,
    .width  = w,
    .height = h,
  };

}

// Is there a collision between the cloud defined by (t, psi, y) and ball?
bool cloud_basket_collision(struct Ball* ball, f32 t, f32 psi, f32 y) {
  assert(ball != NULL);
  Rectangle hitbox = cloud_basket_hitbox(t, psi, y);
  Vector2 pos = (Vector2){.x = ball->x, .y = ball->y};

  return CheckCollisionCircleRec(pos, BALL_RADIUS, hitbox);
}

// only handles _one_ ball. if there are more, the first one in the
// list will get handled
void handle_ball_cloud_baskets(State* st) {
  assert(st != NULL);

  u32 index = 0;
  struct Ball* b = st->balls;
  while (b) {
    if (b->vel_y > 0) {
      if (cloud_basket_collision(b, st->cloud_t, st->cloud_psi_lower, GetScreenHeight()*CLOUD_LOWER_Y_PERCENTAGE)) {
        if (st->debug_mode) printf("Collision with ball (x=%f,y=%f,i=%d) with lower cloud\n", b->x, b->y, index);
        st->score += POINTS_FOR_LOWER;
        remove_ball(st, index);
        return;
      }
      if (cloud_basket_collision(b, st->cloud_t, st->cloud_psi_upper, GetScreenHeight()*CLOUD_UPPER_Y_PERCENTAGE)) {
        if (st->debug_mode) printf("Collision with ball (x=%f,y=%f,i=%d) with upper cloud\n", b->x, b->y, index);
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
