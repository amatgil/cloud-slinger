#pragma once

#include "constants.c"
#include "definitions.c"
#include "raylib.h"
#include "raymath.h"
#include <math.h>
#include <assert.h>
#include <stdio.h>

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

// Is there a collision between the cloud defined by (t, psi, y) and ball?
bool ball_basket_collision(Ball* ball, Basket* basket) {
  assert(ball != NULL);
  assert(basket != NULL);
  Rectangle hitbox = basket_hitbox(basket);
  Vector2 pos = (Vector2){.x = ball->x, .y = ball->y};

  return CheckCollisionCircleRec(pos, BALL_RADIUS, hitbox);
}

// only handles _one_ ball. if there are more, the first one in the
// list will get handled
void handle_ball_baskets_collisions(State* st) {
  assert(st != NULL);

  // forall ball, foreach basket, check collision
  u32 index = 0;
  Ball* ball = st->balls;
  while (ball) {
    Basket* basket = st->baskets;
    if (ball->vel_y > 0) {
      while (basket) {
        if (ball_basket_collision(ball, basket)) {
          if (st->debug_mode) printf("Collision with ball (x=%f,y=%f,i=%d) with lower cloud\n", ball->x, ball->y, index);
          st->score += basket->points;
          remove_ball(st, index);
          return;
        }
        basket = basket->next;
      }
    }

    ball = ball->next;
    index += 1;
  }
}

// delete balls whose position is absurd
// deletes a maximum of ONE ball
void clear_errant_balls(State* st) {
  assert(st != NULL);
  Ball* b = st->balls;
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
