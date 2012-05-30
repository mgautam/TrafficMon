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
  this->moved = true;
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

int car::move()
{

  road* next_road = curr_road->get_next(turn);
  int accrued_wait = wait;

  if (position > 0 && curr_road->cars[position-1] == 0)//move forward on the road
    {
      curr_road->cars[position] = 0;
      curr_road->cars[--position] = this;
      wait = 0;
    }
  else if (position == 0 && curr_road->lights[turn] == GREEN &&
	   ((next_road && next_road->cars[next_road->length - 1] == 0) || !next_road) )//enter intersection
    {
      curr_road->cars[position] = 0;
      curr_road->cars[--position] = this;
      wait = 0;
    }
  else if (position == -1 && turn != LEFT)//move forward in the intersection
    {
      curr_road->cars[position] = 0;
      curr_road->cars[--position] = this;
      wait = 0;
    }
  else if (position == -1 && turn == LEFT)//turn into left road
    {
      curr_road->cars[position] = 0;

      if (!next_road)
	{
	  //escape the city
	  escape_city();
	  return accrued_wait;
	}

      position = next_road->length - 1;
      next_road->cars[position] = this;
      curr_road = next_road;
      wait = 0;
    }
  else if (position == -2) //turn != LEFT //turn into right road or move onto road ahead
    {
      curr_road->cars[position] = 0;

      if (!next_road)
	{
	  //escape the city
	  escape_city();
	  return accrued_wait;
	}

      position = next_road->length - 1;
      next_road->cars[position] = this;
      curr_road = next_road;
      wait = 0;
    }
  else
    {
      wait++;
      accrued_wait = -1;
    }

  moved = true;

  return accrued_wait;
}

void car::sense()
{
  road* next_road = curr_road->get_next(turn);

  if (position > 0 && curr_road->cars[position-1] == 0)//move forward on the road
    {
      sensed = true;
    }
  else if (position == 0 && curr_road->lights[turn] == GREEN &&
	   ((next_road && next_road->cars[next_road->length - 1] == 0) || !next_road) )//enter intersection
    {
      sensed = true;
    }
  else if (position == -1 && turn != LEFT)//move forward in the intersection
    {
      sensed = true;
    }
  else if (position == -1 && turn == LEFT)//turn into left road
    {
      sensed = true;
    }
  else if (position == -2) //turn != LEFT //turn into right road or move onto road ahead
    {
      sensed = true;
    }
  else
    {
      sensed = false;
    }
}


void car::escape_city()
{
  delete this;
}



void car::make_turn()
{
  road* next_road;

  if (position == -2) //turn != LEFT
    {
      next_road = curr_road->get_next(turn);
    }
  else if (position == -1 && turn == LEFT)
    {
      next_road = curr_road->get_next(turn);
    }

  curr_road->cars[position] = 0;
  position = next_road->length - 1 - 2;
  next_road->cars[position] = this;
  curr_road = next_road;
}



