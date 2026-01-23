#ifndef _DEFS_H
#define _DEFS_H

#include <raylib.h>
#include <sys/param.h>
#include <stdlib.h>

#define TAU 6.28318530717958647692

#define NOTHING_HELD (Vector2){-1, -1}
typedef struct {
  float cloud_t; // how far along movement, [0..1)
  struct Ball* balls;
  bool clicking_last_frame; // were we clicking, last frame?
  float cooldown_left; // how long til you can shoot again
} State ;


State new_state() {
  return (State){0.0, NULL, false, 0.0 };
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

#endif
