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
    fprintf (output, "Position:%d\n",this->position);
    fprintf(output, "%d %d\n", curr_road->init->x + displacement_x, curr_road->init->y + displacement_y);
    fprintf (output, "Compass: %d\n",this->curr_road->compass);
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

    // The first 0.5 is to include the offset of intersection to road
    // -1 coz position starts from 1 not 0. 
    // The last 0.5 as offset for between car padding
    float roadOffset =  (0.5 + (float)this->position - 1 + 0.5);
    float halfCarLen = 0.25;

    float lenBWlanes = 0.05;

    glColor3f (0.0f,0.25*(curr_road->compass+1), 0.0f);//(0.0f, 1.0f, 0.0f);
    glBegin (GL_QUADS);

    switch (curr_road->compass) {
      
      case NORTH:
	glVertex2f (((float)curr_road->final->x - lenBWlanes)*scale, ((float)curr_road->final->y - (roadOffset + halfCarLen))*scale);
	glVertex2f (((float)curr_road->final->x - lenBWlanes)*scale, ((float)curr_road->final->y - (roadOffset - halfCarLen))*scale);
	glVertex2f (((float)curr_road->final->x - 0.5)*scale, ((float)curr_road->final->y - (roadOffset - halfCarLen))*scale);
	glVertex2f (((float)curr_road->final->x - 0.5)*scale, ((float)curr_road->final->y - (roadOffset + halfCarLen))*scale);
	break;
    
      case EAST:
	glVertex2f (((float)curr_road->final->x - (roadOffset + halfCarLen))*scale ,((float)curr_road->final->y + lenBWlanes)*scale);
	glVertex2f (((float)curr_road->final->x - (roadOffset - halfCarLen))*scale ,((float)curr_road->final->y + lenBWlanes)*scale);
	glVertex2f (((float)curr_road->final->x - (roadOffset - halfCarLen))*scale ,((float)curr_road->final->y + 0.5)*scale);
	glVertex2f (((float)curr_road->final->x - (roadOffset + halfCarLen))*scale ,((float)curr_road->final->y + 0.5)*scale);
	break;

      case SOUTH:
	glVertex2f (((float)curr_road->final->x + lenBWlanes)*scale, ((float)curr_road->final->y + (roadOffset + halfCarLen))*scale);
	glVertex2f (((float)curr_road->final->x + lenBWlanes)*scale, ((float)curr_road->final->y + (roadOffset - halfCarLen))*scale);
	glVertex2f (((float)curr_road->final->x + 0.5)*scale, ((float)curr_road->final->y + (roadOffset - halfCarLen))*scale);
	glVertex2f (((float)curr_road->final->x + 0.5)*scale, ((float)curr_road->final->y + (roadOffset + halfCarLen))*scale);
	break;

      case WEST:
	glVertex2f (((float)curr_road->final->x + (roadOffset + halfCarLen))*scale ,((float)curr_road->final->y - lenBWlanes)*scale);
	glVertex2f (((float)curr_road->final->x + (roadOffset - halfCarLen))*scale ,((float)curr_road->final->y - lenBWlanes)*scale);
	glVertex2f (((float)curr_road->final->x + (roadOffset - halfCarLen))*scale ,((float)curr_road->final->y - 0.5)*scale);
	glVertex2f (((float)curr_road->final->x + (roadOffset + halfCarLen))*scale ,((float)curr_road->final->y - 0.5)*scale);
	break;
    
      default:
	printf ("Error in Road Endpoints! They are invalid\n");
	exit (-1);
    }
    glEnd ();

    //this->position--;
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
