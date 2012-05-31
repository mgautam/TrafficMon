#include <iostream>
using namespace std;

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "config.h"
#include "world.h"

#define MARGIN_PADDING 1

extern int num_spawn;

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

  // Calculating world scale to map to our windowed perception of the world
  minWorldX = 0;
  minWorldY = 0;
  maxWorldX = 0;
  maxWorldY = 0;
  for (int i = 0; i < this->intc; i++) {
    if (!this->intersections[i])
      continue;

    if (maxWorldX < this->intersections[i]->x)
      maxWorldX = this->intersections[i]->x;
    if (minWorldX > this->intersections[i]->x)
      minWorldX = this->intersections[i]->x;

    if (maxWorldY < this->intersections[i]->y)
      maxWorldY = this->intersections[i]->y;
    if (minWorldY > this->intersections[i]->y)
      minWorldY = this->intersections[i]->y;
  }

  this->scale = (maxWorldX - minWorldX) > (maxWorldY - minWorldY)?
    1.5/((float)(maxWorldX - minWorldX + MARGIN_PADDING))  :  1.5/((float)(maxWorldY - minWorldY + MARGIN_PADDING));
  //printf ("minX = %d, minY = %d \t maxX = %d, maxY = %d", minWorldX, minWorldY, maxWorldX, maxWorldY);
  
  critic = new learner ();
  performance = 0;

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
  /*
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
    }*/
}

static bool carSpawned = true;
static long long randTime;
void world::spawnCar (void) {
  if (carSpawned) {
    randTime = this->timestamp + (float)rand ()/(float)RAND_MAX * 10 + 5;
    //printf ("Next Car at: t+%lld timeunits\t",randTime-timestamp);
    carSpawned = false;
  }
  else if (this->timestamp >= randTime && this->roads[0]->cars[roads[0]->length-1] == 0) {
#ifdef OPENGL_MODE
    if (num_spawn > 0) {
      --num_spawn;
#endif
      new car (this->roads[0],randTime%3);
      //printf ("Car Spawned:%d\n",(int)randTime%3);
      carSpawned = true;
#ifdef OPENGL_MODE
    }
#endif
    }
}



// For Debugging Purposes only
float x = 0;
static int TrafficPhase = 0;
// For Debugging Purposes only

void world::updateWorld(void) {
  //Put traffic light changing code at the end of the routine 
  //   if you want to give a 1 time unit window for the car to move
  //      after GREEN light is signalled

  // For Debugging Purposes only
  if (TrafficPhase%MIN_TL_SWITCH_INTERVAL == 0) {
    for (int i = 0; i < this->intc; i++)
      {
      if (!this->intersections[i])
	continue;
      this->intersections[i]->controlLights (((int)x)%8);//EASTWEST_RIGHT);x++%4
      }
    x++;
  } 
  TrafficPhase++;
  // For Debugging Purposes only

  incr_timestamp();

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

  spawnCar ();
  
  performance += critic->evaluate (intersections,intc);
  if (this->timestamp % 1000 == 0) {
    printf ("Performance in last 1000 time steps is %d\n", performance);
    performance = 0;
    }
}


