#ifndef PAINTER_H
#define PAINTER_H


#include "config.h"
#include "intersection.h"
#include "road.h"
#include "car.h"
#include "world.h"

class painter {



public:
  float scale;

  static void (*display) (void);
  static void (*timerCallback) (int);

  painter(world* sim);
  painter(world* simulation, void (*display) (void), void (*timerCallback) (int), int argc, char** argv);

  void draw ();
  void draw (car* curr_car);
  void draw (intersection* curr_intersection);
  void draw (road* curr_road);
  void drawLights (road* curr_road);

  void animate();

  world* simulation;
};

#endif
