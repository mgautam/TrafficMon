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
  this->setCar (init_road, next_turn, 0);
}

car::car (road* init_road, int next_turn, int laneIndex) {
  this->setCar (init_road, next_turn, laneIndex);
}

void car::setCar (road* init_road, int next_turn, int laneIndex)
{
  this->curr_road = init_road;
  this->position = init_road->length - 1;
  this->curr_lane = laneIndex;//0; //(float)rand()/(float)RAND_MAX * curr->road->numlanes;
  init_road->cars[this->position][curr_lane] = this;
  this->color.r = ((float)rand()/(float)RAND_MAX);
  this->color.g = ((float)rand()/(float)RAND_MAX);
  this->color.b = ((float)rand()/(float)RAND_MAX);

  this->turn = next_turn;
  this->next_lane = curr_lane;//(float)rand()/(float)RAND_MAX * curr_road->numlanes;
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

  if (position > 0 &&  curr_road->cars[position-1][next_lane] == 0)//move forward on the road
    {
      curr_road->cars[position][curr_lane] = 0;
      curr_road->cars[--position][next_lane] = this;
      curr_lane = next_lane;
      wait = 0;
    }
  else if (position == 0 && //curr_road->lights[turn] == GREEN &&
	   ((next_road && next_road->cars[next_road->length - 1][next_lane] == 0) || !next_road) )//enter intersection
    { // Assumption is no lane change during turns
      if ( turn == AHEAD && curr_road->lights[LEFT] == GREEN ||
	   curr_road->lights[turn] == GREEN) {
	curr_road->cars[position][curr_lane] = 0;
	curr_road->cars[--position][next_lane] = this;
	wait = 0;
	//printf ("TURN:%d next_road:%p\n",turn,next_road);
      }
    }
  else if (position == -1 && turn != LEFT)//move forward in the intersection
    {
      curr_road->cars[position][curr_lane] = 0;
      curr_road->cars[--position][curr_lane] = this; // Assumption is no lane change during turns
      //curr_lane = next_lane;
      wait = 0;
    }
  else if (position == -1 && turn == LEFT)//turn into left road
    {
      curr_road->cars[position][curr_lane] = 0;

      if (!next_road)
	{
	  //escape the city
	  escape_city();
	  return accrued_wait;
	}

      position = next_road->length - 1;
      next_road->cars[position][curr_lane] = this; // Assumption is no lane change during turns
      curr_road = next_road;
      wait = 0;

      // setting the next turn to be random
      this->turn = (float)rand ()/(float)RAND_MAX * 3;
    }
  else if (position == -2) //turn != LEFT //turn into right road or move onto road ahead
    {
      curr_road->cars[position][curr_lane] = 0;

      if (!next_road)
	{
	  //escape the city
	  escape_city();
	  return accrued_wait;
	}

      position = next_road->length - 1;
      next_road->cars[position][curr_lane] = this;
      curr_road = next_road;
      wait = 0;

      // setting the next turn to be random
      this->turn = (float)rand ()/(float)RAND_MAX * 3;
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
  //next_lane = curr_lane;//0; //(float)rand()/(float)RAND_MAX * curr->road->numlanes;

  if (position > 0 && curr_road->cars[position-1][next_lane] == 0)//move forward on the road
    {
      sensed = true;
    }
  else if (position == 0 
	   //&& curr_road->lights[turn] == GREEN
	   )//enter intersection	
    {
      //printf ("Worst Place: %d %p\n",turn,next_road);
      if ( turn == AHEAD && curr_road->lights[LEFT] == GREEN || curr_road->lights[turn] == GREEN ) {
	next_lane = curr_lane; // Assuming no lane change if turning
	if (!next_road)
	  sensed = true;
	else if (next_road && next_lane < next_road->numlanes && next_road->cars[next_road->length - 1][next_lane] == 0) {
	  sensed = true;
	}
	else if (next_road && curr_lane > next_road->numlanes && next_road->cars[next_road->length - 1][curr_lane] == 0) {
	  printf ("Transit\n");
	  next_lane = next_road->numlanes - 1;
	  sensed = true;
	}
      }
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


/*
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



*/
