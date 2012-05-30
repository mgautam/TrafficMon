#include <iostream>
using namespace std;

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "common.h"
#include "world.h"

#define MARGIN_PADDING 1

//constructor
world::world(int intc, intersection** intersections, int roadc, road** roads, int carc, car** cars)
{
  this->intc = intc;
  this->roadc = roadc;
  this->carc = carc;

  this->intersections = intersections;
  this->roads = roads;

  this->cars = cars;

  this->timestamp = 0;

  // Calculating world scale to map to our windowed perception of the world
  minWorldX = 0;
  minWorldY = 0;
  maxWorldX = 0;
  maxWorldY = 0;
  for (int i = 0; i < this->intc; i++) {
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
  */
}

// For Debugging Purposes only
float x = 0;
bool amberPhase = false;
// For Debugging Purposes only

void world::updateWorld(void) {
  // For Debugging Purposes only
  if (!amberPhase) {
  for (int i = 0; i < this->intc; i++)
    this->intersections[i]->controlLights (((int)x)%4);//EASTWEST_RIGHT);x++%4
  x = x+1;
  } else
  for (int i = 0; i < this->intc; i++)
    this->intersections[i]->controlLights (((int)x)%2+4);
  amberPhase = !amberPhase;

  // For Debugging Purposes only

  incr_timestamp();

  for (int i = 0; i < this->roadc; i++) 
    {
      for (int j = -2; j < this->roads[i]->length; j++)
        {
          car* curr_car;
          if ((curr_car = this->roads[i]->cars[j]))
            {
              curr_car->move();
              printf ("Road:%d Car:%d NextTurn:%d\n",i,j,curr_car->turn);
            }
        }
    }
}


