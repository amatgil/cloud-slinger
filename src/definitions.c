#pragma once

#include <raylib.h>
#include <sys/param.h>
#include <stdlib.h>

#define TAU 6.28318530717958647692

typedef struct {
  Texture2D slingshot;
  Texture2D ball;
} Textures;

typedef struct {
  float cloud_t; // how far along movement, [0..1)
  float cloud_psi_lower;
  float cloud_psi_upper;
  struct Ball* balls;
  bool clicking_last_frame; // were we clicking, last frame?
  float cooldown_left; // how long til you can shoot again
  int score;
  Textures textures;
} State ;


State new_state() {
  Textures textures = (Textures){};
  return (State){
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
  float x;
  float y;
  float vel_x;
  float vel_y;
  struct Ball* next;
};

void add_ball(State* st, int ball_x, int ball_y, int ball_vel_x, int ball_vel_y) {
  struct Ball* ball = (struct Ball*)malloc(sizeof(struct Ball));
  ball->x = ball_x;
  ball->y = ball_y;
  ball->vel_x = ball_vel_x;
  ball->vel_y = ball_vel_y;
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

int count_balls(State* st) {
  int count = 0;
  struct Ball* b = st->balls;
  while (b) {
    count += 1;
    b = b->next;
  }

  return count;
}

void remove_ball(State* st, int index) {
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
