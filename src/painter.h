#ifndef PAINTER_H
#define PAINTER_H


#include "common.h"
#include "intersection.h"
#include "road.h"
#include "car.h"
#include "world.h"

class painter {
  void draw (world* sim, float scale);
  void draw (car* curr_car, float scale);
  void draw (intersection* curr_intersection, float scale);
  void draw (road* curr_road, float scale);
  void drawLights (road* curr_road, float scale);
};

#endif
