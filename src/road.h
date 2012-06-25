#ifndef ROAD_H
#define ROAD_H 1

#include <stdio.h>
#include <iostream>
using namespace std;

class car;
class intersection;
class road
{
 public:
  //constructor
  road(intersection* init, intersection* final);
  void set_cdf_turn_patterns (float *pdf);
  road* get_next(int turn);
  void write_state(FILE* output);
  bool car_can_move_to(int new_pos);
  int get_random_turn_from_cdf ();
  
  //variables
  int length;
  car** cars;

  intersection* init;
  intersection* final;

  int lights[2];

  int compass; // The direction this road points in World View

 // The distribution of cars turning on each side
  float traffic_pattern_cdf[4];
};

#endif
