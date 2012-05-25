#ifndef CAR_H
#define CAR_H 1



#include "common.h"
#include "intersection.h"
#include "road.h"
#include <stdio.h>
#include <iostream>
using namespace std;

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

class car
{
 public:


  //constructor
  car(road* init_road)
  {
    this->curr_road = init_road;
    this->position = init_road->length;
  }

  void write_state(FILE* output)
  {
    fprintf(output, "%d %d\n", curr_road->init->x + displacement_x, curr_road->init->y + displacement_y);
  }

  bool can_move()
  {
    if (position == 0)
      {
	if (turn == LEFT || turn == AHEAD)
	  return this->curr_road->lights_1;
	else // turn == RIGHT
	  return this->curr_road->lights_2;
      }

    else
      {
	return (curr_road->cars[position-1]->can_move());
      }
  }

  void move()
  {
    if (can_move())
      {
	this->wait = 0;
	this->position--;
	curr_road->cars[position] = 0;
	curr_road->cars[position-1] = this;
      }
    else
      {
	this->wait++;
      }
  }

  void viewCar (float scale) {
    if (curr_road->init->x == curr_road->final->x && curr_road->init->y != curr_road->final->y) {

      bool directionY = (curr_road->init->y < curr_road->final->y);

      float Yoffset;
      if (directionY) Yoffset = 0.5;
      else Yoffset = -0.5;

      glColor3f (0.0f, 0.0f, 1.0f);
      glBegin (GL_QUADS);
      glVertex3f (((float)curr_road->init->x + 0.5)/scale, ((float)curr_road->init->y + Yoffset)/scale, 0.0f);
      glVertex3f (((float)curr_road->init->x + 0.5)/scale, ((float)curr_road->final->y - Yoffset)/scale, 0.0f);
      glVertex3f (((float)curr_road->init->x - 0.5)/scale, ((float)curr_road->final->y - Yoffset)/scale, 0.0f);
      glVertex3f (((float)curr_road->init->x - 0.5)/scale, ((float)curr_road->init->y + Yoffset)/scale, 0.0f);
      glEnd ();
    }
    else if (curr_road->init->x != curr_road->final->x && curr_road->init->y == curr_road->final->y) {
        bool directionX = (curr_road->init->x < curr_road->final->x);
	float Xoffset;
	if (directionX) Xoffset = 0.5;
	else Xoffset = -0.5;
	
	glColor3f (0.0f, 0.0f, 1.0f);
	glBegin (GL_QUADS);
	glVertex3f (((float)curr_road->init->x + Xoffset)/scale,((float)curr_road->init->y + 0.5)/scale, 0.0f);
	glVertex3f (((float)curr_road->final->x - Xoffset)/scale,((float)curr_road->init->y + 0.5)/scale, 0.0f);
	glVertex3f (((float)curr_road->final->x - Xoffset)/scale,((float)curr_road->init->y - 0.5)/scale, 0.0f);
	glVertex3f (((float)curr_road->init->x + Xoffset)/scale,((float)curr_road->init->y - 0.5)/scale, 0.0f);
	glEnd ();
    }
    else {
      printf ("Error in Road Endpoints! They are invalid\n");
      exit (-1);
    }

  }


  //variables
  int turn;
  int wait;
  int position;
  int displacement_x;
  int displacement_y;
  road* curr_road;

  void enter_road(int new_road, int new_intention);

  
};







// car::car(int road, int intention, int length)
// {
//   this->road = road;
//   this->intention = intention;
//   this->length = length;
// }

// void car::enter_road(int new_road, int new_intention)
// {
//   this->road = new_road;
//   this->turn = turn;
// }



#endif
