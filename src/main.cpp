#include "common.h"
#include "world.h"

int main(int argc, char* argv[])
{
  //TODO: we need a way of instantiating these by reading a file
  intersection* i1 = new intersection(0,0);
  intersection* i2 = new intersection(4,0);
  intersection* i3 = new intersection(0,4);
  intersection* i4 = new intersection(-4,0);
  intersection* i5 = new intersection(0,-4);

  intersection* intersections[5]  = {i1, i2, i3, i4, i5};


  //away from the central intersection
  road* r12 = new road(i1, i2);
  road* r13 = new road(i1, i3);
  road* r14 = new road(i1, i4);
  road* r15 = new road(i1, i5);

  //towards the central intersection
  road* r21 = new road(i2, i1);
  road* r31 = new road(i3, i1);
  road* r41 = new road(i4, i1);
  road* r51 = new road(i5, i1);

  road* roads[8] = {r12, r13, r14, r15, r21, r31, r41, r51};


  car* c1 = new car(r21); // car enters from init end point of r21.
  
  car* cars[] = {c1};





  world* simulation = new world(5, intersections, 8, roads, 1, cars);
  simulation->write_state(stdout);
  
  while (true)
    {
      simulation->incr_timestamp();
      simulation->write_state(stdout, false);

    }






}
