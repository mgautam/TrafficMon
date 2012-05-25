#ifndef WORLD_H
#define WORLD_H 1


#include "intersection.h"
#include "road.h"
#include "car.h"

#include <iostream>


class world
{
 public:
  //constructor
  world(int intc, intersection** intersections, int roadc, road** roads, int carc, car** cars)
  {
    this->intc = intc;
    this->roadc = roadc;
    this->carc = carc;

    this->intersections = intersections;
    this->roads = roads;

    this->cars = cars;

    this->timestamp = 0;

  }
  
  void incr_timestamp()
  {
    this->timestamp++;
  }

  void write_state(FILE* output)
  {
    fprintf(output, "%d\n", intc);
    for (int i = 0; i < this->intc; i++)
      {
	this->intersections[i]->write_state(output);
      }

    fprintf(output, "%d\n", roadc);
    for (int i = 0; i < this->roadc; i++)
      {
	this->roads[i]->write_state(output);
      }
   
    fprintf(output, "%d\n", carc);
    for (int i = 0; i < this->carc; i++)
      {
	this->cars[i]->write_state(output);
      }

  }



 protected:
  //variables
  int intc;
  int roadc;
  int carc;

  long long timestamp;

  intersection** intersections;
  road** roads;
  car** cars;


};

#endif


