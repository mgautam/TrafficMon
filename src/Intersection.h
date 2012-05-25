#ifndef INTERSECTION_H
#define INTERSECTION_H 1

#define MAX_DEGREE 4

#include "road.h"
#include <iostream>

class intersection
{
 public:
  //constructor
  intersection(int x, int y)
  {
    this->x = x;
    this->y = y;
  }

  void write_state(FILE* output)
  {
    
  }


 protected:
  //variables
  int id;

  int x;
  int y;
  
  road* in[MAX_DEGREE];
  road* out[MAX_DEGREE];

  

};

#endif
