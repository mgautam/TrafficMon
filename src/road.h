#ifndef ROAD_H
#define ROAD_H 1


#include "common.h"
#include <iostream>

class road
{
 public:
  //constructor
  road(int id, int x, int y)

 protected:
  //variables
  int id;

  int x;
  int y;

  int initial_node;
  int final_node;

  int lights_1;
  int lights_2;
};

#endif
