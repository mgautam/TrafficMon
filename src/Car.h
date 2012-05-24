#ifndef CAR_H
#define CAR_H 1


#include "common.h"
#include <iostream>

class car
{
 public:
  //constructor
  car(int road, int intention, int length = 1);
  void enter_road(int new_road, int new_intention);

 protected:
  //variables
  int id;
  int road;
  int intention;

  int inst_speed;
  int length;

};

car::car(int road, int intention, int length)
{
  this->road = road;
  this->intention = intention;
  this->length = length;
}

void car::enter_road(int new_road, int new_intention)
{
  this->road = new_road;
  this->intention = new_intention;
}



#endif
