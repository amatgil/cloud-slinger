#pragma once

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

#define SLINGSHOT_WIDTH 160
#define SLINGSHOT_HEIGHT (SLINGSHOT_WIDTH * 1.14)
#define SLINGSHOT_STRING_THICKNESS 5
#define SLINGSHOT_STRING_SEPARATION_X 0.3
#define SLINGSHOT_STRING_SEPARATION_Y 0.02
#define SLINGSHOT_MAX_RADIUS 150
#define SLINGSHOT_CENTER (Vector2){.x = GetScreenWidth()/2.0, .y = GetScreenHeight()-SLINGSHOT_HEIGHT}

#define SCORE_FONTSIZE  20
#define SCORE_PADDING_Y 10

#define GRAVITY_ACCELERATION 100
#define BALL_MAXIMUM_INITIAL_SPEED 350

#define COLOR_BACKGROUND       (Color){ 24,   24,  37, 255 }
#define COLOR_SLINGSHOT        (Color){ 137, 180, 250, 255 }
#define COLOR_BALL             (Color){ 243, 139, 168, 255 }
#define COLOR_SLINGSHOT_STRING (Color){ 205, 214, 244, 255 }
#define COLOR_CLOUD            (Color){ 230, 255, 230, 255 }
#define COLOR_SCORE            (Color){ 230, 255, 230, 255 }

#define BALL_COOLDOWN 1
