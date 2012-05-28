#include "factory.h"

void factory::create_world(world** sim)
{

  //TODO: we need a way of instantiating these by reading a file
  // Add 1 unit to the length of road in order to include size of intersection
  intersection** intersections = new intersection*[6];
  intersections[0] = new intersection(0,0); 
  intersections[1] = new intersection(0,6); 
  intersections[2] = new intersection(6,0); 
  intersections[3] = new intersection(0,-6);
  intersections[4] = new intersection(-6,0);
  intersections[5] = new intersection(6,6); 

  road** roads = new road*[12];
  //away from the central intersection
  roads[0] = new road (intersections[0], intersections[1]);
  roads[1] = new road (intersections[0], intersections[2]);
  roads[2] = new road (intersections[0], intersections[3]);
  roads[3] = new road (intersections[0], intersections[4]);
  roads[4] = new road (intersections[1], intersections[5]);
  roads[5] = new road (intersections[2], intersections[5]);

  //towards the central intersection
  roads[6] = new road (intersections[1], intersections[0]);
  roads[7] = new road (intersections[2], intersections[0]);
  roads[8] = new road (intersections[3], intersections[0]);
  roads[9] = new road (intersections[4], intersections[0]);
  roads[10]= new road (intersections[5], intersections[1]);
  roads[11]= new road (intersections[5], intersections[2]);

  car** cars = new car*[5];
  cars[0] = new car(roads[6]); 
  cars[1] = new car(roads[7]); 
  cars[2] = new car(roads[8]); 
  cars[3] = new car(roads[9]); 
  cars[4] = new car(roads[11]);

  *sim = new world(6, intersections, 12, roads, 5, cars);
}
