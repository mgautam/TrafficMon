#ifndef ROAD_H
#define ROAD_H 1


//#include "car.h"
#include "intersection.h"
#include <iostream>

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
  };

  void write_state(FILE* output)
  {
    fprintf(output, "%d %d %d %d\n", this->init->x, this->init->y, this->final->x, this->final->y);
  }


  void viewRoad (float scale) {

    if (this->init->x == this->final->x && this->init->y != this->final->y) {

      bool directionY = (this->init->y < this->final->y);

      float Yoffset;
      if (directionY) Yoffset = 0.5;
      else Yoffset = -0.5;

      glColor3f (0.0f, 0.0f, 1.0f);
      glBegin (GL_QUADS);
      glVertex3f (((float)this->init->x + 0.5)/scale, ((float)this->init->y + Yoffset)/scale, 0.0f);
      glVertex3f (((float)this->init->x + 0.5)/scale, ((float)this->final->y - Yoffset)/scale, 0.0f);
      glVertex3f (((float)this->init->x - 0.5)/scale, ((float)this->final->y - Yoffset)/scale, 0.0f);
      glVertex3f (((float)this->init->x - 0.5)/scale, ((float)this->init->y + Yoffset)/scale, 0.0f);
      glEnd ();
    }
    else if (this->init->x != this->final->x && this->init->y == this->final->y) {
        bool directionX = (this->init->x < this->final->x);
	float Xoffset;
	if (directionX) Xoffset = 0.5;
	else Xoffset = -0.5;
	
	glColor3f (0.0f, 0.0f, 1.0f);
	glBegin (GL_QUADS);
	glVertex3f (((float)this->init->x + Xoffset)/scale,((float)this->init->y + 0.5)/scale, 0.0f);
	glVertex3f (((float)this->final->x - Xoffset)/scale,((float)this->init->y + 0.5)/scale, 0.0f);
	glVertex3f (((float)this->final->x - Xoffset)/scale,((float)this->init->y - 0.5)/scale, 0.0f);
	glVertex3f (((float)this->init->x + Xoffset)/scale,((float)this->init->y - 0.5)/scale, 0.0f);
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
};

#endif


