#ifndef INTERSECTION_H
#define INTERSECTION_H 1

#define MAX_DEGREE 4

#include <iostream>

class road;

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
    fprintf(output, "%d %d %d\n", id, x, y);
  }

  //variables
  int id;
  int x;
  int y;
  
  int in_count;
  int out_count;

  road* in[MAX_DEGREE];
  road* out[MAX_DEGREE];

  

};

#endif
