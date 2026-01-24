#pragma once

#include "types.c"

typedef struct {
  f32 t;
  f32 psi;
} BasketCloud;

typedef struct {
  f32 t;
} BasketBird;

typedef enum {
  Cloud,
  Bird,
} BasketKind;

// I wanna go back to rust :(
typedef struct {
  BasketKind kind;
  union {
    BasketCloud cloud;
    BasketBird  bird;
  } data;
  struct Basket* next;
} Basket;
