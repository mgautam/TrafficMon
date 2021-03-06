#include <cstring>
#include <stdlib.h>

#include "config.h"
#include "road.h"
#include "intersection.h"
#include "car.h"

#define MAX_DEGREE 4

road::road(intersection* init, intersection* final)
{
  this->init = init;
  this->final = final;
  this->length = (final->x - init->x) + (final->y - init->y); //manhattan 
  if (this->length < 0) this->length *= -1;
  // length = length - 2;

  numlanes = NUM_LANES_PER_ROAD;

  this->cars = new car**[numlanes];
  
  for (int l = 0; l < numlanes; l++) {
    this->cars[l] = new car*[length + 2];
    memset(this->cars[l], 0, (length + 2)*sizeof(car*));

    // For future references to be between -2 and (road length)
    this->cars[l] = this->cars[l] + 2;
  }

  this->lights[LEFT] = RED;
  this->lights[RIGHT] = RED;

  if (this->final->x < this->init->x)
    {
      this->compass = WEST;
      init->out[WEST] = this;// out to the west
      final->in[EAST] = this;// in from the east
    }
  else if (this->final->x > this->init->x)
    {
      this->compass = EAST;
      init->out[EAST] = this;
      final->in[WEST] = this;
    }
  else if (this->final->y < this->init->y)
    {
      this->compass = SOUTH;
      init->out[SOUTH] = this;
      final->in[NORTH] = this;
    }
  else if (this->final->y > this->init->y)
    {
      this->compass = NORTH;
      init->out[NORTH] = this;
      final->in[SOUTH] = this;
    }
  else
    {
      printf ("Error in Road Endpoints! They are invalid\n");
      exit (-1);
    }

  init->in_count++;
  final->out_count++;

  // Uniform Distribution
  float pdf[4] = {0.0};
  // pdf[RIGHT] = 1.0;
  pdf[AHEAD] = 1.0;
  set_cdf_turn_patterns (pdf);

}

void road::set_cdf_turn_patterns (float *pdf) {
  float Norm = 0;
  for (int i = 0; i < 4; i++) {
    Norm += pdf[i];
  }
  if (Norm == 0)
    Norm = 1.0;

  // Normalization & Cumulation
  for (int i = 0; i < 4; i++) {
      traffic_pattern_cdf[i] = pdf[i] / Norm;
      if (i != 0) 
	traffic_pattern_cdf[i] += traffic_pattern_cdf[i-1];
      //printf ("%f\t", traffic_pattern_cdf[i]);
  }
  //printf ("\n");

}

road* road::get_next(int turn)
{
  switch (turn)
    {
    case LEFT:
      return final->out[(compass+3)%4];

    case RIGHT:
      return final->out[(compass+1)%4];

    case AHEAD:
      return final->out[(compass+0)%4];
    
    case UTURN:
      return final->out[(compass+2)%4];

    default:
      printf("error in get_next\n");
      exit(-1);
    }
}

void road::write_state(FILE* output)
{
  fprintf(output, "Coordinates: %2d %2d %2d %2d   ", this->init->x, this->init->y, this->final->x, this->final->y);

  fprintf (output, "LEFT Light:%d ",lights[LEFT]);
  fprintf (output, "RIGHT Light:%d ",lights[RIGHT]);

  fprintf (output,"\n");
}


bool road::car_can_move_to(int new_pos)
{
  return true;
}

int road::get_random_turn_from_cdf () {
  float seed = (float)rand ()/(float)RAND_MAX;
  //printf ("%f",seed);
  for (int turn = 0; turn < 4; turn++)
    if (seed < traffic_pattern_cdf[turn]) {
      //printf (" => %d\n",turn);
      return turn;
    }

  return 0;
}
