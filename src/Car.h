#ifndef CAR_H
#define CAR_H 1


#include "road.h"
#include "intersection.h"
#include <iostream>

class car
{
 public:
  //constructor
  car(road* init_road)
  {
    this->curr_road = init_road;
  }

  void write_state(FILE* output)
  {
    fprintf(output, "%d %d\n", curr_road->init->x + displacement_x, curr_road->init->y + displacement_y);

  }



  void enter_road(int new_road, int new_intention);

  //variables
  int turn;
  int position;
  int displacement_x;
  int displacement_y;
  road* curr_road;
  
};

// car::car(int road, int intention, int length)
// {
//   this->road = road;
//   this->intention = intention;
//   this->length = length;
// }

// void car::enter_road(int new_road, int new_intention)
// {
//   this->road = new_road;
//   this->turn = turn;
// }



#endif
