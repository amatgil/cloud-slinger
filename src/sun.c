
#include "definitions.c"
#include "raylib.h"
#include <math.h>

bool have_live_laser(State* st) {
  return st->laser_magnitude < INFINITY;
}

// Throws a laser if the cooldown allows
void throw_laser(State* st) {
  assert(st != NULL);
  f32* magnitude = &st->laser_magnitude;
  f32* angle = &st->laser_angle;
  if (*magnitude < INFINITY) return; // Already a laser live

  *magnitude = 0.0;
  // Subdivide the LASER_MAX_ANGLE-LASER_MIN_ANGLE radians
  // into 2^32 discrete positions (angles grow counterclockwise)
  u32 random = rand();
  *angle = rand();
  // TODO the actual thing
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

  if (have_live_laser(st)) {
     *magnitude += LASER_VEL*DeltaTime;
     if (*magnitude > (f32)GetScreenHeight()) *magnitude = INFINITY;
  } else st->laser_cooldown -= DeltaTime;
}


void handle_laser_collision(State* st) {
  assert(st != NULL);

}


void draw_laser(State* st) {
  // TODO

}
