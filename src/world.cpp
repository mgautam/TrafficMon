#include <iostream>
using namespace std;
#include <cstring>

#include "config.h"
#include "world.h"

//constructor
//world::world(int intc, intersection** intersections, int roadc, road** roads, int carc, car** cars)
world::world(int intc, intersection** intersections, int roadc, road** roads)
{

  this->intc = intc;
  this->roadc = roadc;
  //this->carc = carc;

  this->intersections = intersections;
  this->roads = roads;

  //this->cars = cars;

  this->timestamp = 0;

  this->num_spawn = new int[roadc];
  this->next_spawn_time = new long long [roadc];
  memset (this->next_spawn_time, 0, roadc);
}
  
void world::incr_timestamp()
{
  this->timestamp++;
  printf ("\ntime %lld:\n",timestamp);
}

void world::write_state(FILE* output)
{
  write_state(output, true);
}

void world::write_state(FILE* output, bool fixtures)
{
  fprintf (output, "\n\n\n");
  fprintf(output, "time: %lld\n", timestamp);
  if (fixtures)
    {
      fprintf(output, "\nIntersection Count: %d\n", intc);
      for (int i = 0; i < this->intc; i++) {
	fprintf (output, "Intersection:%d\t", i);
	if (this->intersections[i])
	  this->intersections[i]->write_state(output);
      }
      
      fprintf(output, "\nRoad Count: %d\n", roadc);
      for (int i = 0; i < this->roadc; i++) {
	fprintf (output, "Road:%d\t", i);
	this->roads[i]->write_state(output);
      }
    }
  
  fprintf (output,"\n");
  for (int i = 0; i < this->roadc; i++) 
    for (int j = 0; j < this->roads[i]->length; j++)
      if (this->roads[i]->cars[j]) {
	fprintf (output,"Road:%d\t Car: ",i);
	this->roads[i]->cars[j]->write_state(output);
      }
}

void world::spawnCars (int roadIndex, int batchSize) {

  if (batchSize) {
    num_spawn[roadIndex] += batchSize;
  }

  for (int i=0; i < roadc; i++) {
    if (num_spawn[i] > 0 && timestamp >= next_spawn_time[i]
	&& (roads[i]->cars[roads[i]->length-1] == 0)) {
      new car (roads[i],next_spawn_time[i]%3);
      num_spawn[i]--;
      next_spawn_time[i] = this->timestamp + (float)rand ()/(float)RAND_MAX * 10 + 5;
    }
  }

}



void world::updateWorld(void) {

  incr_timestamp();

  spawnCars ();

  for (int i = 0; i < this->roadc; i++) 
    {
      for (int j = -2; j < this->roads[i]->length; j++)
        {
          car* curr_car;
          if ((curr_car = this->roads[i]->cars[j]))
            {
              curr_car->sense();
            }
        }
    }

  for (int i = 0; i < this->roadc; i++) 
    {
      for (int j = -2; j < this->roads[i]->length; j++)
        {
          car* curr_car;
          if ((curr_car = this->roads[i]->cars[j]))
            {
	      if (curr_car->sensed && !curr_car->moved)
		{
		  curr_car->move();
		  //printf ("Road:%d Car:%d NextTurn:%d\n",i,j,curr_car->turn);
		}
            }
        }
    }

  for (int i = 0; i < this->roadc; i++) 
    {
      for (int j = -2; j < this->roads[i]->length; j++)
        {
          car* curr_car;
          if ((curr_car = this->roads[i]->cars[j]))
            {
	      curr_car->moved = false;
	      //printf ("Road:%d Car:%d NextTurn:%d\n",i,j,curr_car->turn);
            }
        }
    }
  
}


