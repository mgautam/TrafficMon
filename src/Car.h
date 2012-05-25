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
    this->init_road = init_road;
  }

  void write_state(FILE* output)
  {
  }



  void enter_road(int new_road, int new_intention);

 protected:
  //variables
  int id;
  int turn;
  road* init_road;

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
