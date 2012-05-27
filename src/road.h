#ifndef ROAD_H
#define ROAD_H 1


//#include "car.h"
//#include "intersection.h"
#include <stdio.h>
#include <iostream>
#include <cstring>
using namespace std;

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

class car;
class intersection;
class road
{
 public:
  //constructor
  road(intersection* init, intersection* final);
  road* get_left();
  road* get_right();
  road* get_ahead();
  void write_state(FILE* output);
  void viewRoad (float scale);
  void viewLights (float scale);
  void viewCars ( float scale);
  bool car_can_move_to(int new_pos);

  //variables
  int length;
  car** cars;

  intersection* init;
  intersection* final;

  bool lights[2][3];

  int compass; // The direction this road points in World View
};

#endif


