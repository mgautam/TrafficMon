#include "common.h"
#include "intersection.h"

#include "car.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


//constructor
car::car (road* init_road, int next_turn) {
  this->setCar (init_road, next_turn, init_road->length - 1);
}

car::car (road* init_road, int next_turn, int position) {
  this->setCar (init_road, next_turn, position);
}

void car::setCar (road* init_road, int next_turn, int position)
{
  this->curr_road = init_road;
  this->position = position;
  init_road->cars[this->position] = this;
  this->color.r = ((float)rand()/(float)RAND_MAX);
  this->color.g = ((float)rand()/(float)RAND_MAX);
  this->color.b = ((float)rand()/(float)RAND_MAX);

  this->turn = next_turn;
}

void car::write_state(FILE* output)
{
  fprintf (output, "Position:%d ",this->position);
  // fprintf(output, "%d %d\n", this->curr_road->init->x + this->displacement_x, this->curr_road->init->y + this->displacement_y);
  fprintf (output, "Compass: %d ",this->curr_road->compass);
  // fprintf (output,"%f %f %f", this->color.r, this->color.g, this->color.b);
  fprintf (output, "Next Turn: %d ",this->turn);
  fprintf (output, "\n");
}

bool car::can_move()
{
  if (this->position == 0) // enter the intersection
    {
      if (this->turn == LEFT || this->turn == AHEAD)
	{
	  return (this->curr_road->lights[LEFT] == GREEN);
	}
      else // this->turn == RIGHT
	{
	  return (this->curr_road->lights[RIGHT] == GREEN);
	}
    }
  else if (this->position < 0) //in the intersection
    {
      return true;
    }
  else
    {
      if (this->curr_road->cars[(int)this->position-1]) // if a car exists just before this car,
	{
	  return this->curr_road->cars[(int)this->position-1]->can_move(); // if the previous car can move,
	}
      else //if there are no cars in the front.
	{
	  return true; 
	}
    }
}

void car::move()
{
  printf("position: %d\n", this->position);

  if (!can_move())
    {
      this->wait++;
      return;
    }

  printf("wait time: %d\n", this->wait);
  //cars can move

  road* next_road = NULL;
  this->wait = 0;

  if (this->position == 0)
    {
      this->curr_road->cars[this->position] = 0;
      this->curr_road->cars[--this->position] = this;
    }

  else if (this->position == -1)
    {
      if (this->turn == AHEAD || this->turn == RIGHT)
	{
	  this->curr_road->cars[this->position] = 0;
	  this->curr_road->cars[--this->position] = this;
	}
      else if (this->turn == LEFT)
	{
	  if ((next_road = this->curr_road->get_left())) 
	    {
	      curr_road->cars[this->position] = 0;
	      this->position = next_road->length - 1; //last position
	      next_road->cars[this->position] = this;
	      this->curr_road = next_road;
	    }
	}
    }
  else if (this->position == -2)
    {
      if (this->turn == AHEAD)
	next_road = this->curr_road->get_ahead();
      else if (this->turn == RIGHT)
	next_road = this->curr_road->get_right();
      
      if (next_road)
	{
	  curr_road->cars[this->position] = 0;
	  this->position = next_road->length - 1;
	  next_road->cars[this->position] = this;
	  this->curr_road = next_road;
	}
    }
  else
    {
      this->curr_road->cars[this->position] = 0;
      this->curr_road->cars[--this->position] = this;
    }

}

void car::make_turn()
{
}



