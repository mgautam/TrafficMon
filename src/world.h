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

#define WINDOW_WIDTH 768
#define WINDOW_HEIGHT 1024


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

    worldX = 0;
    worldY = 0;
    for (int i = 0; i < this->intc; i++) {
      if (worldX < this->intersections[i]->x)
	worldX = this->intersections[i]->x;
      if (worldY < this->intersections[i]->y)
	worldY = this->intersections[i]->y;
    }

    float scale = ((float)WINDOW_WIDTH/(float)worldX)>((float)WINDOW_HEIGHT/(float)worldY)?
    ((float)WINDOW_WIDTH)/((float)worldX)
    :((float)WINDOW_HEIGHT)/((float)worldY);

    for (int i = 0; i < this->intc; i++)
      this->intersections[i]->viewIntersection (scale);

    for (int i = 0; i < this->roadc; i++)
      this->roads[i]->viewRoad (scale);

    for (int i = 0; i < this->carc; i++)
      this->cars[i]->viewCar (scale);

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

  int worldX;
  int worldY;
};

#endif


