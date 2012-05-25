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

    init->out[init->in_count] = this;
    final->in[final->out_count] = this;
  };

  void write_state(FILE* output)
  {
    fprintf(output, "%d %d %d %d\n", this->init->x, this->init->y, this->final->x, this->final->y);
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


