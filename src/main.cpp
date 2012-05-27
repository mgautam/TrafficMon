#include "common.h"
#include "world.h"
#include "visualize.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h>


static world* simulation;

void display(void)
{
  printf("called from display\n");

  simulation->updateWorld();
  simulation->viewWorld();
  simulation->write_state(stdout,false);

}

static void timerCallback (int value)
{
  printf("called from timer\n");
   /* Do timer processing */
   /* maybe glutPostRedisplay(), if necessary */
  display();
   /* call back again after elapsedUSecs have passed */
  glutTimerFunc (2000, timerCallback, 0);
}

int main(int argc, char* argv[])
{
  //TODO: we need a way of instantiating these by reading a file
  // Add 1 unit to the length of road in order to include size of intersection
  intersection* i1 = new intersection(0,0);
  intersection* i2 = new intersection(0,6);
  intersection* i3 = new intersection(6,0);
  intersection* i4 = new intersection(0,-6);
  intersection* i5 = new intersection(-6,0);

  intersection** intersections = new intersection*[5];
  intersections[0] = i1;
  intersections[1] = i2;
  intersections[2] = i3;
  intersections[3] = i4;
  intersections[4] = i5;



  //away from the central intersection
  road* r12 = new road(i1, i2);//NORTH
  road* r13 = new road(i1, i3);//EAST
  road* r14 = new road(i1, i4);//SOUTH
  road* r15 = new road(i1, i5);//WEST

  //towards the central intersection
  road* r21 = new road(i2, i1);
  road* r31 = new road(i3, i1);
  road* r41 = new road(i4, i1);
  road* r51 = new road(i5, i1);

  road** roads = new road*[8];
  roads[0] = r12;
  roads[1] = r13;
  roads[2] = r14;
  roads[3] = r15;
  roads[4] = r21;
  roads[5] = r31;
  roads[6] = r41;
  roads[7] = r51;


  car* c1 = new car(r21); // car enters from init end point of r21.
  car* c2 = new car(r31);
  car* c3 = new car(r41);
  car* c4 = new car(r51);

  car* cars[] = {c1,c2,c3,c4};


  simulation = new world(5, intersections, 8, roads, 4, cars);
  //simulation->write_state(stdout);

  glutInit (&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize (WINDOW_WIDTH,WINDOW_HEIGHT);

  glutCreateWindow ("World View");
  glDisable (GL_DEPTH_TEST);
  glutDisplayFunc (display);
  glutKeyboardFunc (handleKeyPress);
  glutReshapeFunc (handleResize);

  //  while (true) {

  //    simulation->incr_timestamp();
  //    simulation->viewWorld ();
    
  //   //
  //   sleep (3);
  // }

  simulation->write_state(stdout,false);
  glutTimerFunc (10, timerCallback, 0);

  glutMainLoop();

  return 0;
}
