#include <stdlib.h>

#include "config.h"
#include "intersection.h"
#include "car.h"


int get_lane_index (int turn) {
  int laneIndex;
  switch (turn) {
  case UTURN: laneIndex = 0;
    break;

  case RIGHT: laneIndex = 0;
    break;

  case AHEAD: laneIndex = 1;
    break;

  case LEFT:  laneIndex = 1;
    break;

  default: printf ("Invalid Turn in get_lane_index\n");
    exit (-1);
    break;
  }

  return laneIndex;
}



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

  this->color.r = ((float)rand()/(float)RAND_MAX);
  this->color.g = ((float)rand()/(float)RAND_MAX);
  this->color.b = ((float)rand()/(float)RAND_MAX);

  this->turn = next_turn;

  this->nextlaneIndex = get_lane_index (this->turn);
  this->currlaneIndex = this->nextlaneIndex;

  init_road->cars[this->currlaneIndex][this->position] = this;

  this->moved = false;
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
  //printf ("Car: %p :",this);

  if ( position == curr_road->length-1 && !next_road) {
    turn = UTURN;
    next_road = curr_road->get_next(UTURN);
    nextlaneIndex = get_lane_index (UTURN);
  }

  if (position >= 1 
      && curr_road->cars[nextlaneIndex][position-1] == 0)//switch to next lane
    {
      //printf ("Switch Lane\n");
      curr_road->cars[currlaneIndex][position] = 0;
      curr_road->cars[nextlaneIndex][--position] = this;
      currlaneIndex = nextlaneIndex;
      
      //nextlaneIndex = ++nextlaneIndex % 2;// Just of debugging
      wait = 0;
    }
  else if (position > 1 
	   && curr_road->cars[currlaneIndex][position-1] == 0)//move forward in the same lane
    {
      //printf ("Move Forward\n");
      curr_road->cars[currlaneIndex][position] = 0;
      curr_road->cars[currlaneIndex][--position] = this;
      wait = 0;
    }  
  else if (position == 0 &&
	   ((next_road && next_road->cars[currlaneIndex][next_road->length - 1] == 0) || !next_road) )//enter intersection
    {
      //printf ("position 0 nextroad empty\n");

      // Because sensed is set randomly, no more randomness is needed
      if ((turn == AHEAD && curr_road->lights[LEFT] != RED) ||  
	  (turn == UTURN && curr_road->lights[RIGHT] != RED) ||  
	  (turn == RIGHT && curr_road->lights[RIGHT] != RED) ||
	  (turn == LEFT && curr_road->lights[LEFT] != RED))	{
	curr_road->cars[currlaneIndex][position] = 0;
	curr_road->cars[currlaneIndex][--position] = this;
	wait = 0;
	//printf ("TURN:%d next_road:%p\n",turn,next_road);
      }
    }
  else if (position == -1 && turn != LEFT)//move forward in the intersection
    {
      //printf ("pos-1 turn!=LEFT\n");
      curr_road->cars[currlaneIndex][position] = 0;
      curr_road->cars[currlaneIndex][--position] = this;
      wait = 0;
    }
  else if (position == -1 && turn == LEFT)//turn into left road
    {
      //printf ("pos-1 LEFT\n");
      curr_road->cars[currlaneIndex][position] = 0;

      wait = 0;


      if (!next_road)
	{
	  //escape the city
	  escape_city();
	  return wait;
	}
     
      position = next_road->length - 1;
      next_road->cars[currlaneIndex][position] = this;
      curr_road = next_road;
      
      // Check Road pattern distribution //always left // setting the next turn to be random
      this->turn = curr_road->get_random_turn_from_cdf ();//LEFT;//(float)rand ()/(float)RAND_MAX * 3;
      nextlaneIndex = get_lane_index (this->turn);
    }
  else if (position == -2) //turn != LEFT //turn into right/ahead/uturn
    {
      //printf ("pos-2 \n");
      curr_road->cars[currlaneIndex][position] = 0;

      wait = 0;
      if (!next_road)
	{
	  //escape the city
	  escape_city();
	  return wait;
	}

      position = next_road->length - 1;
      next_road->cars[currlaneIndex][position] = this;
      curr_road = next_road;

      //Check Road pattern distribution //always right // setting the next turn to be random
      this->turn = curr_road->get_random_turn_from_cdf ();//RIGHT;//(float)rand ()/(float)RAND_MAX * 3;
      nextlaneIndex = get_lane_index (this->turn);
    }
  else
    {
      //printf ("Wait!\n");
    }

  moved = true;

  return wait;
}

void car::sense()
{
  road* next_road = curr_road->get_next(turn);
  if ( position == curr_road->length-1 && !next_road) {
    turn = UTURN;
    next_road = curr_road->get_next(UTURN);
    nextlaneIndex = get_lane_index (UTURN);
  }



  if (position >= 1 && curr_road->cars[nextlaneIndex][position-1] == 0)//move forward on the road
    {
      sensed = true;
    }
  else if (position > 1 && curr_road->cars[currlaneIndex][position-1] == 0)//move forward on the road
    {
      sensed = true;
    }
  else if (position == 0 
	   && ((next_road && next_road->cars[currlaneIndex][next_road->length - 1] == 0)
	       || !next_road) )//enter intersection
    {
      if ((turn == AHEAD && curr_road->lights[LEFT] == GREEN) ||  
	  (turn == UTURN && curr_road->lights[RIGHT] == GREEN) ||  
	  (turn == RIGHT && curr_road->lights[RIGHT] == GREEN) ||
	  (turn == LEFT && curr_road->lights[LEFT] == GREEN))	{
	wait = 0;
	sensed = true;
      }
      else if ( ((turn == AHEAD && curr_road->lights[LEFT] == AMBER) ||  
		 (turn == UTURN && curr_road->lights[RIGHT] == AMBER) ||  
		 (turn == RIGHT && curr_road->lights[RIGHT] == AMBER) ||
		 (turn == LEFT && curr_road->lights[LEFT] == AMBER))
		&& (rand () > RAND_MAX / 2) 
		) {
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
  else if (position == 0 
	   && (next_road && next_road->cars[currlaneIndex][next_road->length - 1] != 0)
	   && wait > MAX_WAIT_AT_SIGNAL  )//traffic jam so change turn
    {
      turn = (++turn)%4;
      wait++;
      sensed = false;
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

  curr_road->cars[currlaneIndex][position] = 0;
  position = next_road->length - 1 - 2;
  next_road->cars[currlaneIndex][position] = this;
  curr_road = next_road;
}



