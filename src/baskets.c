#pragma once

#include "constants.c"
#include "raylib.h"
#include "types.c"
#include <math.h>
#include <assert.h>
#include <stdlib.h>

typedef struct {
  f32 t; f32 psi; // these two make up x
  f32 y;
} BasketCloud;

typedef struct {
  f32 t;
} BasketBird;

typedef enum {
  BK_Cloud,
  BK_Pelican,
  BK_HotAirBalloon,
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
  bool marked_for_despawn;
  struct Basket* next;
} Basket;

// x position
f32 cloud_position_x(f32 t, f32 psi) {
  f64 angle = (f64)(t*TAU + psi);
  return ((f32)sin(angle) + 1.0f)/2.0f * (f32)(GetScreenWidth()-CLOUD_WIDTH);
}

Rectangle cloud_rectangle(f32 t, f32 psi, f32 height_percent) {
  f32 h = (f32)GetScreenHeight();
  return (Rectangle) {
    .x      = cloud_position_x(t, psi),
    .y      = h*height_percent,
    .width  =  CLOUD_WIDTH,
    .height =  CLOUD_HEIGHT };
}

Rectangle basket_hitbox(Basket* basket) {
  assert(basket != NULL);

  switch (basket->kind) {
    case BK_Cloud: {
      BasketCloud* c = (BasketCloud*)&basket->data;
      f32 padding_x = (basket->apparent_width - basket->hitbox_width)/2.0f;
      f32 padding_y = (basket->apparent_height - basket->hitbox_height); // Flush against bottom
      Rectangle r =  (Rectangle) {
        .x = cloud_position_x(c->t, c->psi) + padding_x,
        .y = c->y + padding_y,
        .width  = basket->hitbox_width,
        .height = basket->hitbox_height,
      };
      return r;
    }

    case BK_Pelican: {
      assert(false);
    }
    case BK_HotAirBalloon: {
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

    case BK_Pelican: {
      assert(false);
      break;
    }

    case BK_HotAirBalloon: {
      assert(false);
    }
  }
}


Basket* new_basket_cloud(Texture2D* texture, f32 psi, f32 y, u32 points) {
  Basket basket = {
    .kind = BK_Cloud,
    .data = (BasketData) { .cloud = (BasketCloud){.t = 0.0, .psi = psi, .y = y} },
    .apparent_width = CLOUD_WIDTH,
    .hitbox_width = CLOUD_WIDTH*CLOUD_BASKET_PERCENTAGE_X,
    .hitbox_height = CLOUD_HEIGHT*CLOUD_BASKET_PERCENTAGE_Y,
    .marked_for_despawn = false,
    .points = points,
    .texture = *texture, // temporary
    .next = NULL,
  };
  Basket* ptr = malloc(sizeof(Basket));
  *ptr = basket;
  return ptr;
}
