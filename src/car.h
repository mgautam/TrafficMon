#ifndef CAR_H
#define CAR_H 1



#include "road.h"
#include <stdio.h>
#include <iostream>
#include <cassert>
using namespace std;

typedef struct {
  float r,g,b;
} COLOR;

class car
{
 public:
  car (road* init_road, int next_turn);
  car (road* init_road, int next_turn, int _position);
  void  setCar (road* init_road, int next_turn, float _position);
  void write_state(FILE* output);
  bool can_move();
  void move();
  void make_turn();

  //variables
  int turn;
  int wait;
  float position;

  int displacement_x;
  int displacement_y;
  COLOR color;

  road* curr_road;

  void enter_road(int new_road, int new_intention);

  
};


#endif
