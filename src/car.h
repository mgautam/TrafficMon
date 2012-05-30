#ifndef CAR_H
#define CAR_H 1


#include <stdio.h>
#include <iostream>
//#include <cassert>

#include "road.h"

using namespace std;

typedef struct {
  float r,g,b;
} COLOR;

class car
{
 public:
  car (road* init_road, int next_turn);
  car (road* init_road, int next_turn, int position);
  void  setCar (road* init_road, int next_turn, int position);
  void write_state(FILE* output);
  int move();
  void sense();
  void make_turn();

  //variables
  int turn;
  int wait;
  int position;

  int displacement_x;
  int displacement_y;
  COLOR color;

  road* curr_road;

  void escape_city();
  void enter_road(int new_road, int new_intention);

  bool moved;
  bool sensed;
  
};


#endif
