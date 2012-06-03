#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "config.h"
#include "intersection.h"
#include "car.h"



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
  this->wait = 0;
}

void car::write_state(FILE* output)
{
  fprintf (output, "Position:%d ",this->position);
  // fprintf(output, "%d %d\n", this->curr_road->init->x + this->displacement_x, this->curr_road->init->y + this->displacement_y);
  fprintf (output, "Compass: %d ",this->curr_road->compass);
  // fprintf (output,"%f %f %f", this->color.r, this->color.g, this->color.b);
  //fprintf (output, "Next Turn: %d ",this->turn);
  fprintf (output, "\n");
}

int car::move()
{
  //printf ("pOSITION: %d\n",position);
  road* next_road = curr_road->get_next(turn);
  printf ("Car: %p :",this);
  if (position > 0 && curr_road->cars[position-1] == 0)//move forward on the road
    {
      printf ("Move Forward\n");
      curr_road->cars[position] = 0;
      curr_road->cars[--position] = this;
      wait = 0;
    }
  else if (position == 0 &&
	   ((next_road && next_road->cars[next_road->length - 1] == 0) || !next_road) )//enter intersection
    {
      printf ("position 0 nextroad empty\n");
      if ( (turn == AHEAD && curr_road->lights[LEFT] == GREEN) ||  curr_road->lights[turn] == GREEN) {
	curr_road->cars[position] = 0;
	curr_road->cars[--position] = this;
	wait = 0;
	//printf ("TURN:%d next_road:%p\n",turn,next_road);
      }
    }
  else if (position == -1 && turn != LEFT)//move forward in the intersection
    {
      printf ("pos-1 turn!=LEFT\n");
      curr_road->cars[position] = 0;
      curr_road->cars[--position] = this;
      wait = 0;
    }
  else if (position == -1 && turn == LEFT)//turn into left road
    {
      printf ("pos-1 LEFT\n");
      curr_road->cars[position] = 0;

      wait = 0;
      if (!next_road)
	{
	  //escape the city
	  escape_city();
	  return wait;
	}
     
      position = next_road->length - 1;
      next_road->cars[position] = this;
      curr_road = next_road;


      // setting the next turn to be random
      this->turn = (float)rand ()/(float)RAND_MAX * 3;
    }
  else if (position == -2) //turn != LEFT //turn into right road or move onto road ahead
    {
      printf ("pos-2 \n");
      curr_road->cars[position] = 0;

      wait = 0;
      if (!next_road)
	{
	  //escape the city
	  escape_city();
	  return wait;
	}

      position = next_road->length - 1;
      next_road->cars[position] = this;
      curr_road = next_road;

      // setting the next turn to be random
      this->turn = (float)rand ()/(float)RAND_MAX * 3;
    }
  else
    {
      printf ("Wait!\n");
    }

  moved = true;

  return wait;
}

void car::sense()
{
  road* next_road = curr_road->get_next(turn);

  if (position > 0 && curr_road->cars[position-1] == 0)//move forward on the road
    {
      sensed = true;
    }
  else if (position == 0 && 
	   ((next_road && next_road->cars[next_road->length - 1] == 0) || !next_road) )//enter intersection
    {
      if ((turn == AHEAD && curr_road->lights[LEFT] == GREEN) ||  curr_road->lights[turn] == GREEN) {
	wait = 0;
	sensed = true;
      }
      else {
	wait++;
	sensed = false;
      }
      //printf ("Worst Place: %d %p\n",turn,next_road);
      /*if ( turn == AHEAD && curr_road->lights[LEFT] == GREEN )
	sensed = true;
      else if (curr_road->lights[turn] == GREEN)
	sensed = true;
      else
      	sensed = true;
      */

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
      wait++;
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



