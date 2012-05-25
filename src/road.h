#ifndef ROAD_H
#define ROAD_H 1


#include "intersection.h"
#include <iostream>

class road
{
 public:
  //constructor
  road(intersection* init, intersection* final)
  {
    this->init = init;
    this->final = final;
  };

  void write_state(FILE* output)
  {
    printf("%d %d\n", this->init->id, this->final->id);
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


