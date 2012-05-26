#ifndef ROAD_H
#define ROAD_H 1


//#include "car.h"
#include "intersection.h"
#include <stdio.h>
#include <iostream>
using namespace std;

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

class car;

class road
{
 public:
  //constructor
  road(intersection* init, intersection* final)
  {
    this->init = init;
    this->final = final;
    this->length = (final->x - init->x) + (final->y - init->y) - 1; //manhattan 

    car* cars[length];
    this->cars = cars;


    init->out[init->in_count] = this;
    final->in[final->out_count] = this;

    if (this->init->x == this->final->x && this->init->y != this->final->y)
      compass = (this->init->y < this->final->y) ? SOUTH : NORTH;
    else if (this->init->x != this->final->x && this->init->y == this->final->y)
      compass = (this->init->x < this->final->x) ? EAST : WEST;
    else {
      printf ("Error in Road Endpoints! They are invalid\n");
      exit (-1);
    }
  }

  void write_state(FILE* output)
  {
    fprintf(output, "%d %d %d %d\n", this->init->x, this->init->y, this->final->x, this->final->y);
  }


  void viewRoad (float scale) {

    if (compass == NORTH || compass == SOUTH) {
      float Yoffset;
      if (compass == SOUTH) Yoffset = 0.5;
      else Yoffset = -0.5;

      glColor3f (0.0f, 0.0f, 1.0f);
      glBegin (GL_QUADS);
      glVertex3f (((float)this->init->x + 0.5)*scale, ((float)this->init->y + Yoffset)*scale, 0.0f);
      glVertex3f (((float)this->init->x + 0.5)*scale, ((float)this->final->y - Yoffset)*scale, 0.0f);
      glVertex3f (((float)this->init->x - 0.5)*scale, ((float)this->final->y - Yoffset)*scale, 0.0f);
      glVertex3f (((float)this->init->x - 0.5)*scale, ((float)this->init->y + Yoffset)*scale, 0.0f);
      glEnd ();
    }
    else if (compass == EAST || compass == WEST) {
	float Xoffset;
	if (compass == EAST) Xoffset = 0.5;
	else Xoffset = -0.5;
	
	glColor3f (0.0f, 0.0f, 1.0f);
	glBegin (GL_QUADS);
	glVertex3f (((float)this->init->x + Xoffset)*scale,((float)this->init->y + 0.5)*scale, 0.0f);
	glVertex3f (((float)this->final->x - Xoffset)*scale,((float)this->init->y + 0.5)*scale, 0.0f);
	glVertex3f (((float)this->final->x - Xoffset)*scale,((float)this->init->y - 0.5)*scale, 0.0f);
	glVertex3f (((float)this->init->x + Xoffset)*scale,((float)this->init->y - 0.5)*scale, 0.0f);
	glEnd ();
    }
    else {
      printf ("Error in Road Endpoints! They are invalid\n");
      exit (-1);
    }
  }


  bool car_can_move_to(int new_pos)
  {
    return true;
  }



  //variables
  int id;

  int x;
  int y;
  int length;

  car** cars;

  intersection* init;
  intersection* final;

  int lights_1;
  int lights_2;

  int compass; // The direction this road points.
};

#endif


