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
    this->length = (final->x - init->x) + (final->y - init->y); //manhattan 
    if (this->length < 0) this->length *= -1;
    this->length -= 1;

    car* cars[length];
    this->cars = cars;


    init->out[init->in_count++] = this;
    final->in[final->out_count++] = this;

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

    float lenBWlights = 0.05;
    float roadSideOffset = 0.5;
    float intersectionOffset = 0.5;

    switch (compass) {

      case NORTH:
	glVertex2f (((float)this->init->x - lenBWlights)*scale, ((float)this->init->y + intersectionOffset)*scale);
	glVertex2f (((float)this->init->x - lenBWlights)*scale, ((float)this->final->y - intersectionOffset)*scale);
	glVertex2f (((float)this->init->x - roadSideOffset)*scale, ((float)this->final->y - intersectionOffset)*scale);
	glVertex2f (((float)this->init->x - roadSideOffset)*scale, ((float)this->init->y + intersectionOffset)*scale);
      break;

      case EAST:
	glVertex2f (((float)this->init->x + intersectionOffset)*scale,((float)this->init->y + roadSideOffset)*scale);
	glVertex2f (((float)this->final->x - intersectionOffset)*scale,((float)this->init->y + roadSideOffset)*scale);
	glVertex2f (((float)this->final->x - intersectionOffset)*scale,((float)this->init->y + lenBWlights)*scale);
	glVertex2f (((float)this->init->x + intersectionOffset)*scale,((float)this->init->y + lenBWlights)*scale);
      break;

      case SOUTH:
	glVertex2f (((float)this->init->x + roadSideOffset)*scale, ((float)this->init->y - intersectionOffset)*scale);
	glVertex2f (((float)this->init->x + roadSideOffset)*scale, ((float)this->final->y + intersectionOffset)*scale);
	glVertex2f (((float)this->init->x + lenBWlights)*scale, ((float)this->final->y + intersectionOffset)*scale);
	glVertex2f (((float)this->init->x + lenBWlights)*scale, ((float)this->init->y - intersectionOffset)*scale);
      break;

      case WEST:
	glVertex2f (((float)this->init->x - intersectionOffset)*scale,((float)this->init->y - lenBWlights)*scale);
	glVertex2f (((float)this->final->x + intersectionOffset)*scale,((float)this->init->y - lenBWlights)*scale);
	glVertex2f (((float)this->final->x + intersectionOffset)*scale,((float)this->init->y - roadSideOffset)*scale);
	glVertex2f (((float)this->init->x - intersectionOffset)*scale,((float)this->init->y - roadSideOffset)*scale);
      break;

      default:
      printf ("Error in Road Endpoints! They are invalid\n");
      exit (-1);
    }

    glEnd ();
  }

  void viewLights (float scale) {
    float lenBWlights = 0.05;
    float roadSideOffset = 0.5;
    float intersectionOffset = 0.5;
    float LightSize = 0.5;


    for ( int i = 0; i <=1; i++) {
    lights[i] = i;
    if (lights[i] == 0) glColor3f (1.0f, 0.0f, 0.0f);
    else if (lights[i] == 1) glColor3f (1.0f, 1.0f, 0.0f);
    else glColor3f (0.0f, 1.0f, 0.0f);

    glBegin (GL_QUADS);

    switch (compass) {

      case NORTH:
	glVertex2f (((float)this->init->x - roadSideOffset)*scale, 
		    ((float)this->final->y - (intersectionOffset + (lights[i]+1)*LightSize))*scale);
	glVertex2f (((float)this->init->x - roadSideOffset)*scale, 
		    ((float)this->final->y - (intersectionOffset + lights[i] * LightSize))*scale);
	glVertex2f (((float)this->init->x - (2*(i+1)*roadSideOffset - lenBWlights))*scale, 
		    ((float)this->final->y - (intersectionOffset + lights[i] * LightSize))*scale);
	glVertex2f (((float)this->init->x - (2*(i+1)*roadSideOffset - lenBWlights))*scale, 
		    ((float)this->final->y - (intersectionOffset + (lights[i]+1)*LightSize))*scale);
      break;

      case EAST:
	glVertex2f (((float)this->final->x - (intersectionOffset + (lights[i]+1)*LightSize))*scale,
		    ((float)this->init->y + roadSideOffset)*scale);
	glVertex2f (((float)this->final->x - (intersectionOffset + lights[i] * LightSize))*scale,
		    ((float)this->init->y + roadSideOffset)*scale);
	glVertex2f (((float)this->final->x - (intersectionOffset + lights[i] * LightSize))*scale,
		    ((float)this->init->y + (2*(i+1)*roadSideOffset - lenBWlights))*scale);
	glVertex2f (((float)this->final->x - (intersectionOffset + (lights[i]+1) * LightSize))*scale,
		    ((float)this->init->y + (2*(i+1)*roadSideOffset - lenBWlights))*scale);
      break;

      case SOUTH:
	glVertex2f (((float)this->init->x + roadSideOffset)*scale, 
		    ((float)this->final->y + (intersectionOffset + (lights[i]+1)*LightSize))*scale);
	glVertex2f (((float)this->init->x + roadSideOffset)*scale, 
		    ((float)this->final->y + (intersectionOffset + lights[i] * LightSize))*scale);
	glVertex2f (((float)this->init->x + (2*(i+1)*roadSideOffset - lenBWlights))*scale, 
		    ((float)this->final->y + (intersectionOffset + lights[i] * LightSize))*scale);
	glVertex2f (((float)this->init->x + (2*(i+1)*roadSideOffset - lenBWlights))*scale, 
		    ((float)this->final->y + (intersectionOffset + (lights[i]+1)*LightSize))*scale);
      break;

      case WEST:
	glVertex2f (((float)this->final->x + (intersectionOffset + (lights[i]+1)*LightSize))*scale,
		    ((float)this->init->y - roadSideOffset)*scale);
	glVertex2f (((float)this->final->x + (intersectionOffset + lights[i] * LightSize))*scale,
		    ((float)this->init->y - roadSideOffset)*scale);
	glVertex2f (((float)this->final->x + (intersectionOffset + lights[i] * LightSize))*scale,
		    ((float)this->init->y - (2*(i+1)*roadSideOffset - lenBWlights))*scale);
	glVertex2f (((float)this->final->x + (intersectionOffset + (lights[i]+1) * LightSize))*scale,
		    ((float)this->init->y - (2*(i+1)*roadSideOffset - lenBWlights))*scale);
      break;

      default:
      printf ("Error in Road Endpoints! They are invalid\n");
      exit (-1);
    }

    glEnd ();
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

  int lights[2];

  int compass; // The direction this road points in World View
};

#endif


