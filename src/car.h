#ifndef CAR_H
#define CAR_H 1



#include "intersection.h"
#include "road.h"
#include <iostream>

class car
{
 public:


  //constructor
  car(road* init_road)
  {
    this->curr_road = init_road;
    this->position = init_road->length;
  }

  void write_state(FILE* output)
  {
    fprintf(output, "%d %d\n", curr_road->init->x + displacement_x, curr_road->init->y + displacement_y);
  }

  bool can_move()
  {
    if (!curr_road->cars[position-1])
      return true;
    if (curr_road->cars[position-1]->can_move())
      return true;
  }

  void move()
  {
    this->position--;
    curr_road->cars[position] = 0;
    curr_road->cars[position-1] = this;
  }



  //variables
  int turn;
  int position;
  int displacement_x;
  int displacement_y;
  road* curr_road;

  void enter_road(int new_road, int new_intention);

  
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
