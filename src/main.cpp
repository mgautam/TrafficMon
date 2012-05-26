#include "common.h"
#include "world.h"
#include "visualize.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h>

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
  car* c2 = new car(r12);
  car* c3 = new car(r41);
  car* c4 = new car(r14);

  car* cars[] = {c1,c2,c3,c4};


  world* simulation = new world(5, intersections, 8, roads, 1, cars);
  //simulation->write_state(stdout);

  glutInit (&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize (WINDOW_WIDTH,WINDOW_HEIGHT);

  glutCreateWindow ("World View");
  glDisable (GL_DEPTH_TEST);
  //  glutDisplayFunc (simulation->viewWorld);
  glutKeyboardFunc (handleKeyPress);
  glutReshapeFunc (handleResize);


  while (true) {

    simulation->incr_timestamp();
    simulation->viewWorld ();
    
    //simulation->write_state(stdout, false);    
    sleep (1);    
  }
      

}
