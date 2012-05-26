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
    if (this->length < 0) this->length *= -1;

    car* cars[length];
    this->cars = cars;


    init->out[init->in_count] = this;
    final->in[final->out_count] = this;

    if (this->init->x == this->final->x && this->init->y != this->final->y)
      compass = (this->init->y < this->final->y) ? NORTH : SOUTH;
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
    glColor3f (0.0f, 0.0f, 1.0f);
    glBegin (GL_QUADS);

    float lenBWlanes = 0.05;

    switch (compass) {

      case NORTH:
	glVertex2f (((float)this->init->x - lenBWlanes)*scale, ((float)this->init->y + 0.5)*scale);
	glVertex2f (((float)this->init->x - lenBWlanes)*scale, ((float)this->final->y - 0.5)*scale);
	glVertex2f (((float)this->init->x - 0.5)*scale, ((float)this->final->y - 0.5)*scale);
	glVertex2f (((float)this->init->x - 0.5)*scale, ((float)this->init->y + 0.5)*scale);
      break;

      case EAST:
	glVertex2f (((float)this->init->x + 0.5)*scale,((float)this->init->y + 0.5)*scale);
	glVertex2f (((float)this->final->x - 0.5)*scale,((float)this->init->y + 0.5)*scale);
	glVertex2f (((float)this->final->x - 0.5)*scale,((float)this->init->y + lenBWlanes)*scale);
	glVertex2f (((float)this->init->x + 0.5)*scale,((float)this->init->y + lenBWlanes)*scale);
      break;

      case SOUTH:
	glVertex2f (((float)this->init->x + 0.5)*scale, ((float)this->init->y - 0.5)*scale);
	glVertex2f (((float)this->init->x + 0.5)*scale, ((float)this->final->y + 0.5)*scale);
	glVertex2f (((float)this->init->x + lenBWlanes)*scale, ((float)this->final->y + 0.5)*scale);
	glVertex2f (((float)this->init->x + lenBWlanes)*scale, ((float)this->init->y - 0.5)*scale);
      break;

      case WEST:
	glVertex2f (((float)this->init->x - 0.5)*scale,((float)this->init->y - lenBWlanes)*scale);
	glVertex2f (((float)this->final->x + 0.5)*scale,((float)this->init->y - lenBWlanes)*scale);
	glVertex2f (((float)this->final->x + 0.5)*scale,((float)this->init->y - 0.5)*scale);
	glVertex2f (((float)this->init->x - 0.5)*scale,((float)this->init->y - 0.5)*scale);
      break;

      default:
      printf ("Error in Road Endpoints! They are invalid\n");
      exit (-1);
    }

    glEnd ();
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

  int compass; // The direction this road points in World View
};

#endif


