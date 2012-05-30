#ifndef WORLD_H
#define WORLD_H 1


#include "intersection.h"
#include "road.h"
#include "car.h"

extern float x;

class world
{
 public:
  //  world(int intc, intersection** intersections, int roadc, road** roads, int carc, car** cars);
  world(int intc, intersection** intersections, int roadc, road** roads);
  void incr_timestamp();
  void write_state(FILE* output);
  void write_state(FILE* output, bool fixtures);
  void updateWorld(void);
  void viewWorld (void);
  void spawnCar (void);

  //variables
  int intc;
  int roadc;

  long long timestamp;

  intersection** intersections;
  road** roads;

  // The smallest & largest co-ordinate of any intersection present in the world
  int minWorldX, maxWorldX;
  int minWorldY, maxWorldY;
  float scale;

};

#endif


