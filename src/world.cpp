#include <iostream>
using namespace std;

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "common.h"
#include "world.h"

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
      else
	minWorldX = this->intersections[i]->x;

      if (maxWorldY < this->intersections[i]->y)
	maxWorldY = this->intersections[i]->y;
      else
	minWorldY = this->intersections[i]->y;
    }

    this->scale = (maxWorldX - minWorldX) > (maxWorldY - minWorldY)?
      1.5/((float)(maxWorldX - minWorldX))  :  1.5/((float)(maxWorldY - minWorldY));

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

// For Debugging Purposes only
static int x;
// For Debugging Purposes only

  void world::updateWorld(void) {
    incr_timestamp();
    for (int r = 0; r < this->roadc; r++)
      {
	road* curr_road = this->roads[r];

	if (curr_road->cars[0])
	  curr_road->cars[0]->make_turn();

	for (int c = 1; c < curr_road->length; c++)
	  {
	    if (curr_road->cars[c])
	      curr_road->cars[c]->move();
	  }
      }

    // For Debugging Purposes only
     for (int i = 0; i < this->intc; i++)
       this->intersections[i]->controlLights (x++%4);//EASTWEST_RIGHT);
    // For Debugging Purposes only
  }


  void world::viewWorld (void) {

    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();

    for (int i = 0; i < this->intc; i++)
      this->intersections[i]->viewIntersection (this->scale);

    for (int i = 0; i < this->roadc; i++) {
      this->roads[i]->viewRoad (this->scale);
      this->roads[i]->viewLights (this->scale);
      this->roads[i]->viewCars (this->scale);
    }

    // for (int i = 0; i < this->carc; i++)
    //   this->cars[i]->viewCar (this->scale);

    glutSwapBuffers ();
  }
