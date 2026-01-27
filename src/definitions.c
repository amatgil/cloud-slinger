#pragma once

#include <raylib.h>
#include <sys/param.h>
#include <stdlib.h>
#include "types.c"
#include "baskets.c"

typedef struct {
  Texture2D default_purple;
  Texture2D slingshot;
  Texture2D ball;
  Texture2D cloud;
  Texture2D pelican;
} Textures;

typedef struct Ball {
  f32 x;
  f32 y;
  f32 vel_x;
  f32 vel_y;
  f32 angle;
  struct Ball* next;
} Ball;

typedef struct {
  bool debug_mode;
  bool paused;
  Ball* balls;
  bool clicking_last_frame; // were we clicking, last frame?
  f32 cooldown_left; // how long til you can shoot again
  u32 score;
  Textures textures;
  Basket* baskets;
} State ;



void add_ball(State* st, f32 ball_x, f32 ball_y, f32 ball_vel_x, f32 ball_vel_y) {
  Ball* ball = (Ball*)malloc(sizeof(Ball));
  ball->x = ball_x;
  ball->y = ball_y;
  ball->vel_x = ball_vel_x;
  ball->vel_y = ball_vel_y;
  ball->angle = 0;
  ball->next = NULL;

  Ball* prev = st->balls;
  Ball* curr = NULL;
  if (!prev) {
    st->balls = ball;
    return;
  }

  curr = prev->next;
  while (curr) {
    prev = curr;
    curr = curr->next;
  }

  prev->next = ball;
}

u32 count_balls(State* st) {
  u32 count = 0;
  Ball* b = st->balls;
  while (b) {
    count += 1;
    b = b->next;
  }
  return count;
}
u32 count_baskets(State* st) {
  u32 count = 0;
  Basket* b = st->baskets;
  while (b) {
    count += 1;
    b = b->next;
  }
  return count;
}

void remove_ball(State* st, u32 index) {
  assert(st != NULL);
  Ball* prev = NULL;
  Ball* curr = st->balls;
  while (curr) {
    if (index == 0) {
      Ball* next = curr->next;
      if (next == NULL && prev == NULL) st->balls = NULL;
      if (next != NULL && prev == NULL) st->balls = next;
      if (next == NULL && prev != NULL) prev->next = NULL;
      if (next != NULL && prev != NULL) prev->next = next;

      free(curr);   // i miss Drop :(
      return;
    }
    prev = curr;
    curr = curr->next;
    index -= 1;
  }
}


// This is here instead of in baskets.c because i don't want to make
// five thousand header files
void remove_basket(State* st, u32 index) {
  assert(st != NULL);
  Basket* prev = NULL;
  Basket* curr = st->baskets;
  while (curr) {
    if (index == 0) {
      Basket* next = curr->next;
      if (next == NULL && prev == NULL) st->baskets = NULL;
      if (next != NULL && prev == NULL) st->baskets = next;
      if (next == NULL && prev != NULL) prev->next = NULL;
      if (next != NULL && prev != NULL) prev->next = next;

      free(curr);   // i miss Drop :(
      return;
    }
    prev = curr;
    curr = curr->next;
    index -= 1;
  }
}
