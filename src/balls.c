#ifndef _BALLS_H
#define _BALLS_H

struct Ball {
  int x;
  int y;
  int vel_x;
  int vel_y;
  struct Ball* next;
};

#endif
