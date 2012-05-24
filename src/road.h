#ifndef ROAD_H
#define ROAD_H 1


#include "common.h"
#include <iostream>

class road
{
 public:
  //constructor
  road(int int_1_id, int int_2_id);

 protected:
  //variables
  int initial_node;
  int final_node;

  int lights_1;
  int lights_2;
};

#endif
