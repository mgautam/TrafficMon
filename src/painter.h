#ifndef PAINTER_H
#define PAINTER_H


#include "common.h"
#include "intersection.h"
#include "road.h"
#include "car.h"
#include "world.h"

class painter {



public:
  static void (*display) (void);
  static void (*timerCallback) (int);
  // static int argc;
  // static char** argv;

  painter(world* sim);
  painter(world* simulation, void (*display) (void), void (*timerCallback) (int), int argc, char** argv);

  void draw ();
  void draw (car* curr_car, float scale);
  void draw (intersection* curr_intersection, float scale);
  void draw (road* curr_road, float scale);
  void drawLights (road* curr_road, float scale);


  void animate();

  world* simulation;
};

#endif
