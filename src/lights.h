#ifndef ROAD_H
#define ROAD_H 1


#include "common.h"
#include <iostream>

class lights
{
 public:
  //constructor
  road(int int_1_id, int int_2_id);

 protected:
  //variables
  int int_1_id;
  int int_2_id;

  lights* lights_1;
  lights* lights_2;



};

#endif
