#ifndef INTERSECTION_H
#define INTERSECTION_H 1

#define MAX_DEGREE 4

#include <stdio.h>
#include <iostream>
using namespace std;

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

class road;
class intersection
{
 public:
  //constructor
  intersection(int x, int y)
  {
    this->x = x;
    this->y = y;

    this->in_count = 0;
    this->out_count = 0;

  }

  void write_state(FILE* output)
  {
    fprintf(output, "%d %d %d\n", id, x, y);
  }

  void viewIntersection (float scale) {
    float halfIntersectionLen = 0.5;

    glColor3f (1.0f, 0.0f, 0.0f);
    glBegin (GL_QUADS);
    glVertex2f (((float) this->x - halfIntersectionLen) * scale, ((float)this->y + halfIntersectionLen)* scale);
    glVertex2f (((float) this->x + halfIntersectionLen) * scale, ((float)this->y + halfIntersectionLen)* scale);
    glVertex2f (((float) this->x + halfIntersectionLen) * scale, ((float)this->y - halfIntersectionLen)* scale);
    glVertex2f (((float) this->x - halfIntersectionLen) * scale, ((float)this->y - halfIntersectionLen)* scale);
    glEnd ();
  }

  void TrafficController (int PatternID) {
    for (int roadIndex = 0; roadIndex < MAX_DEGREE; roadIndex++)
      for (int lightSet = 0; lightSet < 2; lightSet++)
	for (int lightType = 0; lightType < 3; lightType++)
	  this->in[roadIndex]->lights[lightSet][lightType] = FALSE;

    switch (PatternID) {
    case NORTHSOUTH_AHEADLEFT:
      this->in[NORTH]->lights[0][GREEN] = true;
      this->in[SOUTH]->lights[0][GREEN] = true;
      break;

    case EASTWEST_AHEADLEFT:
      this->in[EAST]->lights[0][GREEN] = true;
      this->in[WEST]->lights[0][GREEN] = true;
      break;

    case NORTHSOUTH_RIGHT:
      this->in[NORTH]->lights[1][GREEN] = true;
      this->in[SOUTH]->lights[1][GREEN] = true;

    case EASTWEST_RIGHT:
      this->in[EAST]->lights[1][GREEN] = true;
      this->in[WEST]->lights[1][GREEN] = true;
      break;
    }
  }

  //variables
  int id;
  int x;
  int y;
  
  int in_count;
  int out_count;

  road* in[MAX_DEGREE];
  road* out[MAX_DEGREE];

  

};

#endif
