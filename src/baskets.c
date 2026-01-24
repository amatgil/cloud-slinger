#pragma once

#include "constants.c"
#include "definitions.c"
#include "raylib.h"
#include "types.c"
#include <math.h>
#include <assert.h>
#include <stdio.h>

typedef struct {
  f32 t; f32 psi; // these two make up x
  f32 y;
} BasketCloud;

typedef struct {
  f32 t;
} BasketBird;

typedef enum {
  BK_Cloud,
  BK_Bird,
} BasketKind;

typedef union {
  BasketCloud cloud;
  BasketBird  bird;
} BasketData;
// I wanna go back to rust :(
typedef struct Basket {
  BasketKind kind;
  BasketData data;
  f32 apparent_width;
  f32 apparent_height;
  f32 hitbox_width;
  f32 hitbox_height;
  u32 points; // How many points it gives
  Texture2D texture;
  struct Basket* next;
} Basket;

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

Rectangle basket_hitbox(Basket* basket) {
  assert(basket != NULL);

  switch (basket->kind) {
    case BK_Cloud: {
      BasketCloud* c = (BasketCloud*)&basket->data;
      Rectangle r =  (Rectangle) {
        .x = cloud_position_x(c->t, c->psi) + (basket->hitbox_width)/2.0,
        .y = c->y,
        .width  = basket->apparent_width,
        .height = basket->apparent_height,
      };
      return r;
    }

    case BK_Bird: {
      assert(false);
    }
  }
}

void update_basket_position(Basket* basket, f32 DeltaTime) {
  switch (basket->kind) {
    case BK_Cloud: {
      BasketCloud* c = (BasketCloud*)&basket->data;
      c->t += CLOUD_DT*DeltaTime;
      break;
    }

    case BK_Bird: {
      assert(false);
      break;
    }
  }
}


Basket* new_basket_cloud(Texture2D* texture, f32 psi, f32 y, f32 points) {
  Basket* basket = malloc(sizeof(Basket));
  basket->kind = BK_Cloud;
  basket->data = (BasketData) { .cloud = (BasketCloud){.t = 0.0, .psi = psi, .y = y} };
  basket->apparent_width = CLOUD_WIDTH;
  basket->apparent_height = CLOUD_HEIGHT;
  basket->hitbox_width = CLOUD_WIDTH*CLOUD_BASKET_PERCENTAGE_X;
  basket->hitbox_height = CLOUD_HEIGHT*CLOUD_BASKET_PERCENTAGE_Y;
  basket->points = points;
  basket->texture = *texture; // temporary
  basket->next = NULL;
  return basket;
}
