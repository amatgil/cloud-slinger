#pragma once

#include <raylib.h>
#include <sys/param.h>
#include <stdlib.h>
#include "types.c"

#define TAU 6.28318530717958647692

typedef struct {
  Texture2D slingshot;
  Texture2D ball;
} Textures;

typedef struct {
  bool debug_mode;
  f32 cloud_t; // how far along movement, [0..1)
  f32 cloud_psi_lower;
  f32 cloud_psi_upper;
  struct Ball* balls;
  bool clicking_last_frame; // were we clicking, last frame?
  f32 cooldown_left; // how long til you can shoot again
  u32 score;
  Textures textures;
} State ;


State new_state() {
  Textures textures = (Textures){};
  return (State){
    .debug_mode = false,
    .cloud_t = 0.0,
    .cloud_psi_lower = 0.0,
    .cloud_psi_upper = TAU/4 + 0.1,
    .balls = NULL,
    .clicking_last_frame = false,
    .cooldown_left = 0.0,
    .score = 0,
    .textures = textures
  };
}

struct Ball {
  f32 x;
  f32 y;
  f32 vel_x;
  f32 vel_y;
  f32 angle;
  struct Ball* next;
};

void add_ball(State* st, i32 ball_x, i32 ball_y, i32 ball_vel_x, i32 ball_vel_y) {
  struct Ball* ball = (struct Ball*)malloc(sizeof(struct Ball));
  ball->x = ball_x;
  ball->y = ball_y;
  ball->vel_x = ball_vel_x;
  ball->vel_y = ball_vel_y;
  ball->angle = 0;
  ball->next = NULL;

  struct Ball* prev = st->balls;
  struct Ball* curr = NULL;
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
  struct Ball* b = st->balls;
  while (b) {
    count += 1;
    b = b->next;
  }

  return count;
}

void remove_ball(State* st, u32 index) {
  struct Ball* prev = NULL;
  struct Ball* curr = st->balls;
  while (curr) {
    if (index == 0) {
      struct Ball* next = curr->next;
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
