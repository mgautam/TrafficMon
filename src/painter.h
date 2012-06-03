#ifndef PAINTER_H
#define PAINTER_H

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "config.h"
#include "intersection.h"
#include "road.h"
#include "car.h"
#include "world.h"

class painter {



public:
  float scale;

  static void (*timerCallback) (int);
  //  static void (*

  painter(world* sim);
  painter(world* simulation, void (*display) (void), void (*timerCallback) (int), int argc, char** argv);

  void draw ();
  void draw (car* curr_car);
  void draw (intersection* curr_intersection);
  void draw (road* curr_road);
  void drawLights (road* curr_road);

  void output(GLfloat, GLfloat, char*);
  void animate();

  world* simulation;
};

#endif
