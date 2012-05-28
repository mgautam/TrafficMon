#include "common.h"
#include "world.h"
#include "visualize.h"
#include "painter.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h>


static world* simulation;
static painter* ppainter;


void display(void)
{
  simulation->updateWorld();
  ppainter->draw();
  simulation->write_state(stdout,true);
}

void timerCallback (int value)
{
   /* Do timer processing */
   /* maybe glutPostRedisplay(), if necessary */
  if (!stopAnime)
    display();
   /* call back again after elapsedUSecs have passed */
  glutTimerFunc (SIMULATION_INTERVAL, timerCallback, 0);
}

int main (int argc, char* argv[])
{
  //TODO: we need a way of instantiating these by reading a file
  // Add 1 unit to the length of road in order to include size of intersection
  intersection* i1 = new intersection(0,0);
  intersection* i2 = new intersection(0,6);
  intersection* i3 = new intersection(6,0);
  intersection* i4 = new intersection(0,-6);
  intersection* i5 = new intersection(-6,0);
  intersection* i6 = new intersection(6,6);

  intersection** intersections = new intersection*[6];
  intersections[0] = i1;
  intersections[1] = i2;
  intersections[2] = i3;
  intersections[3] = i4;
  intersections[4] = i5;
  intersections[5] = i6;


  //away from the central intersection
  road* r12 = new road (i1, i2);//NORTH
  road* r13 = new road (i1, i3);//EAST
  road* r14 = new road (i1, i4);//SOUTH
  road* r15 = new road (i1, i5);//WEST
  road* r26 = new road (i2, i6);
  road* r36 = new road (i3, i6);

  //towards the central intersection
  road* r21 = new road (i2, i1);
  road* r31 = new road (i3, i1);
  road* r41 = new road (i4, i1);
  road* r51 = new road (i5, i1);
  road* r62 = new road (i6, i2);
  road* r63 = new road (i6, i3);

  road** roads = new road*[12];
  roads[0] = r12;
  roads[1] = r13;
  roads[2] = r14;
  roads[3] = r15;
  roads[4] = r21;
  roads[5] = r31;
  roads[6] = r41;
  roads[7] = r51;
  roads[8] = r26;
  roads[9] = r62;
  roads[10] = r36;
  roads[11] = r63;

  car* c1 = new car(r21); // car enters from init end point of r21.
  car* c2 = new car(r31);
  car* c3 = new car(r41);
  car* c4 = new car(r51);
  car* c5 = new car(r62);


  car* cars[] = {c1,c2,c3,c4,c5};


  simulation = new world(6, intersections, 12, roads, 5, cars);
  //simulation->write_state(stdout);
  ppainter = new painter(simulation, display, timerCallback, argc, argv);
  // ppainter->display = display;
  // ppainter->timerCallback = timerCallback;
  // ppainter->argc = argc;
  // ppainter->argv = argv;

  // Display the initial state of the world
  ppainter->draw();
  ppainter->animate();
  return 0;
}
