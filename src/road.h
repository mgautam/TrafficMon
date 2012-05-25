#ifndef ROAD_H
#define ROAD_H 1


#include "common.h"
#include <iostream>

class intersection;

class road
{
 public:
  //constructor
  road(intersection* init, intersection* final)
  {
    this->init = init;
    this->final = final;
  }

 protected:
  //variables
  int id;

  int x;
  int y;

  intersection* init;
  intersection* final;

  int lights_1;
  int lights_2;
};

#endif


