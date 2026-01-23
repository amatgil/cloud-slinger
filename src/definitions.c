#ifndef _DEFS_H
#define _DEFS_H

#include <raylib.h>
#include <sys/param.h>
#include <stdlib.h>

typedef struct {
  float cloud_t; // how far along movement, [0..1)
  struct Ball* balls;
  Vector2 held_position; // (-1, -1) si no està held
} State ;


State new_state() {
  return (State){0.0, NULL, (Vector2){0.0, 0.0} };
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
      if (next == NULL && prev == NULL) {};
      if (next != NULL && prev == NULL) st->balls = next;
      if (next == NULL && prev != NULL) prev->next = NULL;
      if (next != NULL && prev != NULL) prev->next = next;

      free(curr);   // i miss Drop :(
    }
    return;
  }
  prev = curr;
  curr = curr->next;
  index -= 1;
}

#endif
