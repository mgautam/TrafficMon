#include <iostream>
using namespace std;
#include <cstring>
#include <stdlib.h>

#include "config.h"
#include "world.h"

world::world(int intc, intersection** intersections, int roadc, road** roads)
{

  this->intc = intc;
  this->roadc = roadc;

  this->intersections = intersections;
  this->roads = roads;

  //this->cars = cars;

  this->timestamp = 0;

  this->num_spawn = new int[roadc];
  this->next_spawn_time = new long long [roadc];
  memset (this->next_spawn_time, 0, roadc);

  road_intersection_relations[0] = new int [roadc];
  road_intersection_relations[1] = new int [roadc];

  build_road_intersection_relations ();
    
}

void world::build_road_intersection_relations (void) {
  int minX = 100, maxX = 0;
  for (int i = 0; i < intc; i++) {
    if (!intersections[i])
      continue;

    if ( maxX < intersections[i]->x )
      maxX = intersections[i]->x ;

    if ( minX > intersections[i]->x )
      minX = intersections[i]->x ;
  }

  int geometric_intersection_index;
  for (int r = 0; r < roadc; r++) {
    for (int i = 0; i < intc; i++) {
      if (!intersections[i])
	continue;

      geometric_intersection_index = intersections[i]->y * (maxX - minX) + (intersections[i]->x-minX);
      if ( geometric_intersection_index ==
	   roads[r]->init->y  * (maxX - minX) + (roads[r]->init->x-minX) )
	road_intersection_relations[0][r] = i;

      if ( geometric_intersection_index ==
	   roads[r]->final->y  * (maxX - minX) + (roads[r]->final->x-minX) )
	road_intersection_relations[1][r] = i;
    }
  }
}
  
void world::incr_timestamp()
{
  this->timestamp++;
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
    for (int l = 0; l < this->roads[i]->numlanes; l++)
      for (int j = 0; j < this->roads[i]->length; j++)
	if (this->roads[i]->cars[l][j]) {
	  fprintf (output,"Road:%d\t Car: ",i);
	  this->roads[i]->cars[l][j]->write_state(output);
	}
}

void world::spawnCars (int roadIndex, int batchSize) {

  if (batchSize) {
    num_spawn[roadIndex] += batchSize;
  }

  for (int i=0; i < roadc; i++) {
    int turn = AHEAD;//(float)rand ()/(float) RAND_MAX * 3
    if (num_spawn[i] > 0 && timestamp >= next_spawn_time[i]
	&& (roads[i]->cars[get_lane_index (turn)][roads[i]->length-2] == 0)) {
      // roads[i]->length-2 because roads[i]->length-1 is checked by car in next signal, and further checks are not performed while entering road
      new car (roads[i], turn, roads[i]->length-2);
      num_spawn[i]--;
      next_spawn_time[i] = this->timestamp + (float)rand ()/(float)RAND_MAX * 10 + 5;
    }
  }

}



void world::updateWorld(void) {

  if (roadc)// Same as roadc != 0
    spawnCars ();

  for (int i = 0; i < this->roadc; i++) 
    {
      for (int l = 0; l < this->roads[i]->numlanes; l++)
	for (int j = -2; j < this->roads[i]->length; j++)
	  {
	    car* curr_car;
	    if ((curr_car = this->roads[i]->cars[l][j]))
	      {
		curr_car->sense();
	      }
	  }
    }


  for (int i = 0; i < this->roadc; i++) 
    {
      for (int l = 0; l < this->roads[i]->numlanes; l++)
	for (int j = -2; j < this->roads[i]->length; j++)
	  {
	    car* curr_car;
	    if ((curr_car = this->roads[i]->cars[l][j]))
	      {
		if (curr_car->sensed && !curr_car->moved)
		  {
		    curr_car->move();
		  }
	      }
	  }
    }

  for (int i = 0; i < this->roadc; i++) 
    {
      for (int l = 0; l < this->roads[i]->numlanes; l++)
	for (int j = -2; j < this->roads[i]->length; j++)
	  {
	    car* curr_car;
	    if ((curr_car = this->roads[i]->cars[l][j]))
	      {
		curr_car->moved = false; // Why can't we put this inside car.cpp? like for sensed?
		//printf ("Road:%d Car:%d NextTurn:%d\n",i,j,curr_car->turn);
	      }
	  }
    }
}


