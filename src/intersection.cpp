#include <stdio.h>
#include <iostream>
using namespace std;

#include <cstring>

#include "common.h"
#include "intersection.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif



  //constructor
 intersection::intersection(int x, int y)
  {
    this->x = x;
    this->y = y;

    this->in_count = 0;
    this->out_count = 0;
  }

  void intersection::write_state(FILE* output)
  {
    fprintf(output, "Coordinates:%2d %2d\n", x, y);
  }

  void intersection::viewIntersection (float scale) {
    float halfIntersectionLen = 0.5;

    glColor3f (1.0f, 0.0f, 0.0f);
    glBegin (GL_QUADS);
    glVertex2f (((float) this->x - halfIntersectionLen) * scale, ((float)this->y + halfIntersectionLen)* scale);
    glVertex2f (((float) this->x + halfIntersectionLen) * scale, ((float)this->y + halfIntersectionLen)* scale);
    glVertex2f (((float) this->x + halfIntersectionLen) * scale, ((float)this->y - halfIntersectionLen)* scale);
    glVertex2f (((float) this->x - halfIntersectionLen) * scale, ((float)this->y - halfIntersectionLen)* scale);
    glEnd ();
  }

  void intersection::controlLights (int PatternID) {
    for (int roadIndex = 0; roadIndex < MAX_DEGREE; roadIndex++)
      //for (int lightType = 0; lightType < 3; lightType++) {
      if (this->in[roadIndex]) {
	memset (this->in[roadIndex]->lights[LEFT],false,3*sizeof(bool));
	memset (this->in[roadIndex]->lights[RIGHT],false,3*sizeof(bool));
        
        //this->in[roadIndex]->lights[LEFT][lightType] = false;
	//this->in[roadIndex]->lights[RIGHT][lightType] = false;
      }
     //}

    switch (PatternID) {
    case NORTHSOUTH_AHEADLEFT:
      if (this->in[NORTH]) {
	this->in[NORTH]->lights[LEFT][GREEN] = true;
	this->in[NORTH]->lights[RIGHT][RED] = true;
      }
      if (this->in[SOUTH]) {
	this->in[SOUTH]->lights[LEFT][GREEN] = true;
	this->in[SOUTH]->lights[RIGHT][RED] = true;
      }
      if (this->in[EAST]) {
	this->in[EAST]->lights[LEFT][RED] = true;
	this->in[EAST]->lights[RIGHT][RED] = true;
      }
      if (this->in[WEST]) {
	this->in[WEST]->lights[LEFT][RED] = true;
	this->in[WEST]->lights[RIGHT][RED] = true;
      }
      break;

    case EASTWEST_AHEADLEFT:
      if (this->in[EAST]) {
	this->in[EAST]->lights[LEFT][GREEN] = true;
	this->in[EAST]->lights[RIGHT][RED] = true;
      }
      if (this->in[WEST]) {
	this->in[WEST]->lights[LEFT][GREEN] = true;
	this->in[WEST]->lights[RIGHT][RED] = true;
      }
      if (this->in[NORTH]) {
	this->in[NORTH]->lights[LEFT][RED] = true;
	this->in[NORTH]->lights[RIGHT][RED] = true;
      }
      if (this->in[SOUTH]) {
	this->in[SOUTH]->lights[LEFT][RED] = true;
	this->in[SOUTH]->lights[RIGHT][RED] = true;
      }
      break;

    case NORTHSOUTH_RIGHT:
      if (this->in[NORTH]) {
	this->in[NORTH]->lights[LEFT][RED] = true;
	this->in[NORTH]->lights[RIGHT][GREEN] = true;
      }
      if (this->in[SOUTH]) {
	this->in[SOUTH]->lights[LEFT][RED] = true;
	this->in[SOUTH]->lights[RIGHT][GREEN] = true;
      }
      if (this->in[EAST]) {
	this->in[EAST]->lights[LEFT][RED] = true;
	this->in[EAST]->lights[RIGHT][RED] = true;
      }
      if (this->in[WEST]) {
	this->in[WEST]->lights[LEFT][RED] = true;
	this->in[WEST]->lights[RIGHT][RED] = true;
      }
      break;

    case EASTWEST_RIGHT:
      if (this->in[EAST]) {
	this->in[EAST]->lights[LEFT][RED] = true;
	this->in[EAST]->lights[RIGHT][GREEN] = true;
      }
      if (this->in[WEST]) {
	this->in[WEST]->lights[LEFT][RED] = true;
	this->in[WEST]->lights[RIGHT][GREEN] = true;
      }
      if (this->in[NORTH]) {
	this->in[NORTH]->lights[LEFT][RED] = true;
	this->in[NORTH]->lights[RIGHT][RED] = true;
      }
      if (this->in[SOUTH]) {
	this->in[SOUTH]->lights[LEFT][RED] = true;
	this->in[SOUTH]->lights[RIGHT][RED] = true;
      }
      break;
    }
  }
