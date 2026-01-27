
#include "constants.c"
#include "definitions.c"
#include "raylib.h"
#include "raymath.h"
#include <assert.h>
#include <math.h>

bool laser_is_live(State* st) {
  return st->laser_magnitude < INFINITY;
}

void start_new_cooldown(State* st) {
  f32 range = LASER_MAX_COOLDOWN-LASER_MIN_COOLDOWN;
  st->laser_cooldown = (f32)rand()/(f32)RAND_MAX * range  + LASER_MIN_COOLDOWN;
}

// Throws a laser if the cooldown allows
void throw_laser(State* st) {
  assert(st != NULL);
  if (laser_is_live(st) || st->laser_cooldown >= 0) return;

  f32* magnitude = &st->laser_magnitude;
  f32* angle = &st->laser_angle;

  *magnitude = 0.0;
  st->laser_cooldown = 0.0;

  f32 random = ((f32)rand()) / (f32)RAND_MAX;
  f32 range = LASER_MAX_ANGLE - LASER_MIN_ANGLE;
  *angle = (f32)random * range + LASER_MIN_ANGLE;
}

// Reduces cooldown and/or moves laser forward
// If the laser is off-screen, the cooldown is decreased
// If it's on-screen,          it's moved forward
//
// Also checks if the laser has become being off-screen and
// "despawns" it
void advance_laser(State* st, f32 DeltaTime) {
  assert(st != NULL);
  f32* magnitude = &st->laser_magnitude;

  if (laser_is_live(st)) *magnitude += LASER_VEL*DeltaTime;
  else st->laser_cooldown -= DeltaTime;

  if (*magnitude > (f32)MAX(GetScreenWidth(), GetScreenHeight())) {
    *magnitude = INFINITY;
    start_new_cooldown(st);
  }
}


bool laser_ball_collision(State* st, Ball* ball) {
  assert(st != NULL);
  assert(ball != NULL);

}

void handle_laser_collisions(State* st) {
  assert(st != NULL);

}


// Assumes we're in drawing mode
void draw_laser(State* st) {
  assert(st != NULL);
  Vector2 center = Vector2Scale(
    Vector2Rotate(
      (Vector2){.x=0.0, .y=1.0},
      -st->laser_angle),
    st->laser_magnitude);

  Rectangle r = (Rectangle) {
    .x = center.x + LASER_HEIGHT*cosf(st->laser_angle)/2.0f,
    .y = center.y - LASER_HEIGHT*cosf(st->laser_angle)/2.0f,
    .width  = LASER_WIDTH,
    .height = LASER_HEIGHT,
  };
  f32 angle = -(TAU/2.0f + TAU/4.0f + st->laser_angle);
  DrawRectanglePro(r, Vector2Zero(), angle*RAD2DEG, COLOR_LASER);
}

// Assumes we're in drawing mode
void draw_sun(State* st) {
  assert(st != NULL);
  Texture2D* tex = &st->textures.sun;
  DrawTexturePro(
    *tex,
    (Rectangle){.x = 0.0, .y = 0.0, .width = (f32)tex->width, .height = (f32)tex->height},
    (Rectangle){.x = 0.0, .y = 0.0, .width = SUN_RADIUS, .height = SUN_RADIUS},
    Vector2Zero(), 0.0, WHITE);
}

// Assumes we're in drawing mode
void draw_laser_range(void) {
  Color c = (Color){255, 100, 0, 100};
  f32 mag = (f32)MAX(GetScreenHeight(), GetScreenWidth());
  Vector2 low = Vector2Rotate((Vector2){.x=0.0, .y=1.0}, -LASER_MIN_ANGLE);
  Vector2 high = Vector2Rotate((Vector2){.x=0.0, .y=1.0}, -LASER_MAX_ANGLE);
  DrawLineV(Vector2Zero(), Vector2Scale(low, mag), c);
  DrawLineV(Vector2Zero(), Vector2Scale(high, mag), c);
}

// Assumes we're in drawing mode
void draw_laser_path(State* st) {
  Vector2 dir = Vector2Rotate((Vector2){.x=0.0, .y=1.0}, -st->laser_angle);
  f32 mag = (f32)MAX(GetScreenHeight(), GetScreenWidth());
  DrawLineV(Vector2Zero(),Vector2Scale(dir, mag), YELLOW);
}
