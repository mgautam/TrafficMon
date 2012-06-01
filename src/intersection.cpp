#include <stdio.h>
#include <iostream>
using namespace std;
#include <cstring>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "config.h"
#include "intersection.h"

//constructor
intersection::intersection(int x, int y)
{
  this->x = x;
  this->y = y;

  this->in_count = 0;
  this->out_count = 0;

  memset (this->in, 0, MAX_DEGREE*sizeof(road*));
  memset (this->out, 0, MAX_DEGREE*sizeof(road*));//not necessary
}

void intersection::controlLights (int PatternID) {

  this->pattern_id = PatternID;

  for (int direction = 0; direction < 4; direction++)
    if (this->in[direction]) {
      this->in[direction]->lights[LEFT] = RED;
      this->in[direction]->lights[RIGHT] = RED;
    }

  switch (PatternID) {
  case NORTHSOUTH_AHEADLEFT:
    if (this->in[NORTH]) 
      this->in[NORTH]->lights[LEFT] = GREEN;
    if (this->in[SOUTH]) 
      this->in[SOUTH]->lights[LEFT] = GREEN;
    break;

  case EASTWEST_AHEADLEFT:
    if (this->in[EAST]) 
      this->in[EAST]->lights[LEFT] = GREEN;
    if (this->in[WEST]) 
      this->in[WEST]->lights[LEFT] = GREEN;
    break;

  case NORTHSOUTH_RIGHT:
    if (this->in[NORTH]) 
      this->in[NORTH]->lights[RIGHT] = GREEN;
    if (this->in[SOUTH]) 
      this->in[SOUTH]->lights[RIGHT] = GREEN;
    break;

  case EASTWEST_RIGHT:
    if (this->in[EAST]) 
      this->in[EAST]->lights[RIGHT] = GREEN;
    if (this->in[WEST]) 
      this->in[WEST]->lights[RIGHT] = GREEN;
    break;

  case NORTHSOUTH_AL_AMBER:
    if (this->in[NORTH]) 
      this->in[NORTH]->lights[LEFT] = AMBER;
    if (this->in[SOUTH]) 
      this->in[SOUTH]->lights[LEFT] = AMBER;
    break;
    
  case EASTWEST_AL_AMBER:
    if (this->in[EAST]) 
      this->in[EAST]->lights[LEFT] = AMBER;
    if (this->in[WEST]) 
      this->in[WEST]->lights[LEFT] = AMBER;
    break;  

  case NORTHSOUTH_R_AMBER:
    if (this->in[NORTH]) 
      this->in[NORTH]->lights[RIGHT] = AMBER;
    if (this->in[SOUTH]) 
      this->in[SOUTH]->lights[RIGHT] = AMBER;
    break;
    
  case EASTWEST_R_AMBER:
    if (this->in[EAST]) 
      this->in[EAST]->lights[RIGHT] = AMBER;
    if (this->in[WEST]) 
      this->in[WEST]->lights[RIGHT] = AMBER;
    break;
  }
    
}


void intersection::write_state(FILE* output)
{
  fprintf(output, "Coordinates:%2d %2d\n", x, y);
}

