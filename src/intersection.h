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
    glColor3f (1.0f, 0.0f, 0.0f);
    glBegin (GL_QUADS);
    glVertex2f (((float) this->x - 0.5) * scale, ((float)this->y + 0.5)* scale);
    glVertex2f (((float) this->x + 0.5) * scale, ((float)this->y + 0.5)* scale);
    glVertex2f (((float) this->x + 0.5) * scale, ((float)this->y - 0.5)* scale);
    glVertex2f (((float) this->x - 0.5) * scale, ((float)this->y - 0.5)* scale);
    glEnd ();
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
