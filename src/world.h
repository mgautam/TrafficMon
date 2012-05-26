#ifndef WORLD_H
#define WORLD_H 1


#include "intersection.h"
#include "road.h"
#include "car.h"

#include <iostream>
using namespace std;

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500


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

    // Calculating world scale to map to our windowed perception of the world
    maxWorldX = 0;
    maxWorldY = 0;
    for (int i = 0; i < this->intc; i++) {
      if (maxWorldX < this->intersections[i]->x)
	maxWorldX = this->intersections[i]->x;
      if (maxWorldY < this->intersections[i]->y)
	maxWorldY = this->intersections[i]->y;
    }

    this->scale = ((float)WINDOW_WIDTH/(float)maxWorldX)>((float)WINDOW_HEIGHT/(float)maxWorldY)?
      1.0/((float)maxWorldX)  :  1.0/((float)maxWorldY);

  }
  
  void incr_timestamp()
  {
    this->timestamp++;
  }

  void write_state(FILE* output)
  {
    write_state(output, true);
  }

  void write_state(FILE* output, bool fixtures)
  {
    if (fixtures)
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
      }

    fprintf(output, "%d\n", carc);
    for (int i = 0; i < this->carc; i++)
      {
	this->cars[i]->write_state(output);
      }
  }

  void viewWorld (void) {

    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();

    for (int i = 0; i < this->intc; i++)
      this->intersections[i]->viewIntersection (this->scale);

    for (int i = 0; i < this->roadc; i++)
      this->roads[i]->viewRoad (this->scale);

    for (int i = 0; i < this->carc; i++)
      this->cars[i]->viewCar (this->scale);

    glutSwapBuffers ();
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

  // The largest co-ordinate of any intersection present in the world
  int maxWorldX;
  int maxWorldY;
  float scale;
};

#endif


