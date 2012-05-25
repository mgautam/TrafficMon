#ifndef ROAD_H
#define ROAD_H 1


//#include "car.h"
#include "intersection.h"
#include <iostream>

class car;

class road
{
 public:
  //constructor
  road(intersection* init, intersection* final)
  {
    this->init = init;
    this->final = final;
    this->length = (final->x - init->x) + (final->y - init->y) - 1; //manhattan 

    car* cars[length];
    this->cars = cars;


    init->out[init->in_count] = this;
    final->in[final->out_count] = this;
  };

  void write_state(FILE* output)
  {
    fprintf(output, "%d %d %d %d\n", this->init->x, this->init->y, this->final->x, this->final->y);
  }

  bool car_can_move_to(int new_pos)
  {
    return true;
  }


  //variables
  int id;

  int x;
  int y;
  int length;

  car** cars;

  intersection* init;
  intersection* final;

  int lights_1;
  int lights_2;
};

#endif


