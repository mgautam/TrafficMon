#ifndef WORLD_H
#define WORLD_H 1


#include "intersection.h"
#include "road.h"
#include "car.h"
//#include "learner.h"

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
  void spawnCars (int roadIndex = 0, int batchSize = 0);
  //void spawnCars (int roadIndex, int batchSize);

  void saveWorld (FILE *output);
  void loadWorld (FILE *input);

  //variables
  int intc;
  int roadc;
  int carc;

  long long timestamp;

  intersection** intersections;
  road** roads;

  // World has the control for spawning cars
  int *num_spawn;
  long long *next_spawn_time;
};

#endif


