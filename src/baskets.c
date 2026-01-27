#pragma once

#include "raylib.h"
#include "constants.c"
#include "raymath.h"
#include "types.c"
#include <math.h>
#include <assert.h>
#include <stdlib.h>


typedef struct {
  f32 t; f32 psi; // these two make up x
  f32 y;
} BasketCloud;

typedef struct {
  f32 x;
  f32 y;
  bool going_left;
} BasketPelican;

typedef enum {
  BK_Cloud,
  BK_Pelican,
  BK_HotAirBalloon,
} BasketKind;

// I wanna go back to rust :(
typedef union {
  BasketCloud cloud;
  BasketPelican pelican;
} BasketData;
typedef struct Basket {
  BasketKind kind;
  BasketData data;
  f32 apparent_width;
  f32 apparent_height;
  f32 hitbox_width;
  f32 hitbox_height;
  u32 points; // How many points it gives
  Texture2D* texture;
  struct Basket* next;
} Basket;

// x position
f32 cloud_position_x(f32 t, f32 psi) {
  f32 angle = t*TAU + psi;
  return (sinf(angle) + 1.0f)/2.0f * (f32)(GetScreenWidth()-CLOUD_WIDTH);
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
      BasketPelican* b = (BasketPelican*)&basket->data;
      // TODO: Fix this, depends on the texture a lot (and also on going_left)
      return (Rectangle) {
        .x = b->x + 10,
        .y = b->y + 10,
        .width = 20,
        .height = 30,
      };
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
      BasketPelican* b = (BasketPelican*)&basket->data;
      if (b->going_left) b->x -= PELICAN_VEL_X*DeltaTime;
      else               b->x += PELICAN_VEL_X*DeltaTime;
      break;
    }

    case BK_HotAirBalloon: {
      assert(false);
    }
  }
}


Basket* new_basket_cloud(Texture2D* texture, f32 psi, f32 y, u32 points) {
  Basket basket = (Basket){
    .kind = BK_Cloud,
    .data = (BasketData) { .cloud = (BasketCloud){.t = 0.0, .psi = psi, .y = y} },
    .apparent_width = CLOUD_WIDTH,
    .apparent_height = CLOUD_HEIGHT,
    .hitbox_width = CLOUD_WIDTH*CLOUD_BASKET_PERCENTAGE_X,
    .hitbox_height = CLOUD_HEIGHT*CLOUD_BASKET_PERCENTAGE_Y,
    .points = points,
    .texture = texture, // temporary
    .next = NULL,
  };
  Basket* ptr = malloc(sizeof(Basket));
  *ptr = basket;
  return ptr;
}

Basket* new_basket_pelican(Texture2D* texture, bool going_left, f32 y, u32 points) {
  f32 x = going_left ? (f32)GetScreenWidth() : 0.0f ;

  Basket basket = (Basket){
    .kind = BK_Pelican,
    .data = (BasketData) { .pelican = (BasketPelican){.x = x, .y = y, .going_left = going_left} },
    .apparent_width = PELICAN_WIDTH,
    .apparent_height = PELICAN_HEIGHT,
    .hitbox_width = PELICAN_WIDTH*CLOUD_BASKET_PERCENTAGE_X,
    .hitbox_height = PELICAN_HEIGHT*CLOUD_BASKET_PERCENTAGE_Y,
    .points = points,
    .texture = texture, // temporary
    .next = NULL,
  };
  Basket* ptr = malloc(sizeof(Basket));
  *ptr = basket;
  return ptr;
}

// Assumes we're in drawing mode
void draw_basket(Basket* basket) {
  assert(basket != NULL);

  switch (basket->kind) {
    case BK_Cloud: {
      BasketCloud* c = (BasketCloud*)&basket->data;
      Texture2D* tex = basket->texture;
      DrawTexturePro(
        *basket->texture,
        (Rectangle){ .x = 0.0, .y = 0.0, .width = (f32)tex->width, .height = (f32)tex->height},
        (Rectangle){
          .x      = cloud_position_x(c->t, c->psi),
          .y      = c->y,
          .width  = basket->apparent_width,
          .height = basket->apparent_height},
        Vector2Zero(), 0.0, WHITE);
      break;
    }

    case BK_Pelican: {
      BasketPelican* b = (BasketPelican*)&basket->data;
      Texture2D* tex = basket->texture;
      DrawTexturePro(
        *basket->texture,
        (Rectangle){ .x = 0.0, .y = 0.0, .width = (f32)tex->width, .height = (f32)tex->height},
        (Rectangle){
          .x      = b->x,
          .y      = b->y,
          .width  = basket->apparent_width,
          .height = basket->apparent_height},
        Vector2Zero(), 0.0, WHITE);
      break;
    }

    case BK_HotAirBalloon: {
      assert(false);
      break;
    }
  }
}
