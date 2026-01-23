#ifndef _CONSTANTS_H
#define _CONSTANTS_H

#include <raylib.h>

#define CLOUD_WIDTH  50
#define CLOUD_HEIGHT 25
// CLOUD_{LOWER, UPPER}_Y are percentages of screenheight
#define CLOUD_LOWER_Y 0.35
#define CLOUD_UPPER_Y 0.15
#define CLOUD_DT 0.1
#define BALL_RADIUS 8

int INITIAL_SCREEN_WIDTH  = 350;
int INITIAL_SCREEN_HEIGHT = 700;

#define SLINGSHOT_WIDTH 40
#define SLINGSHOT_HEIGHT 150
#define SLINGSHOT_STRING_THICKNESS 5
#define SLINGSHOT_MAX_RADIUS 100

#define GRAVITY_ACCELERATION 100
#define BALL_MAXIMUM_INITIAL_SPEED 350

#define COLOR_BACKGROUND       (Color){ 24,   24,  37, 255 }
#define COLOR_SLINGSHOT        (Color){ 137, 180, 250, 255 }
#define COLOR_BALL             (Color){ 243, 139, 168, 255 }
#define COLOR_SLINGSHOT_STRING (Color){ 205, 214, 244, 255 }

#endif
