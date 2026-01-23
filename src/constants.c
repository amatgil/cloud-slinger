#ifndef _CONSTANTS_H
#define _CONSTANTS_H

#include <raylib.h>

#define CLOUD_WIDTH  120
#define CLOUD_HEIGHT 25
// CLOUD_{LOWER, UPPER}_Y are percentages of screenheight
#define CLOUD_LOWER_Y 0.15
#define CLOUD_UPPER_Y 0.35
#define CLOUD_DT 0.1
#define BALL_RADIUS 8

#define INITIAL_SCREEN_WIDTH  350
#define INITIAL_SCREEN_HEIGHT INITIAL_SCREEN_WIDTH * 2

#define SLINGSHOT_WIDTH 40
#define SLINGSHOT_HEIGHT 150
#define SLINGSHOT_STRING_THICKNESS 5
#define SLINGSHOT_STRING_COLOR WHITE
#define SLINGSHOT_MAX_RADIUS 100

typedef struct {
  float cloud_t; // how far along movement, [0..1)
  struct Ball* balls;
  Vector2 held_position; // (-1, -1) si no està held
} State ;


#endif
