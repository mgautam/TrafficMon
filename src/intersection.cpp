#include <stdio.h>
#include <iostream>
using namespace std;
#include <cstring>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "config.h"
#include "intersection.h"
#include "car.h"
#include <math.h>

#define MAX_SLOTS_TO_CHECK 10

//constructor
intersection::intersection(int x, int y)
{
  this->x = x;
  this->y = y;

  this->in_count = 0;
  this->out_count = 0;

  this->states = new int[2*(MAX_DEGREE + 1)]; //traffic pattern, car counts

  memset (this->in, 0, MAX_DEGREE*sizeof(road*));
  memset (this->out, 0, MAX_DEGREE*sizeof(road*));//not necessary
}

void intersection::sense_state ()
{
  memcpy(states + (MAX_DEGREE + 1), states, sizeof(int)*(MAX_DEGREE + 1));
  states[0] = pattern_id;

  for (int j = 0; j < MAX_DEGREE; j++)
    {
      road* curr_road = in[j];
      states[1 + j] = MAX_SLOTS_TO_CHECK;
      if (curr_road)
	{
	  for (int k = 0; k < MAX_SLOTS_TO_CHECK; k++)
	    {
	      if (curr_road->cars[k]) {
		states[1 + j] = k;
		break;
	      }
	    }
	}
    }
}

void intersection::select_action()
{
  float total_kq = 0;
  float cum_kq[NUM_TRAFFIC_PATTERNS];

  for (int action = 0; action < NUM_TRAFFIC_PATTERNS; action++)
    {
      total_kq += pow(0.1, *get_q_entry(states + 0, action));//is this the right probability distribution?
      //It makes higher rewards to have lower kq
      cum_kq[action] = total_kq;
    }  

  float r = (float)rand()/(float)RAND_MAX*total_kq;

  for (int action = 0; action < NUM_TRAFFIC_PATTERNS; action++)
    {
      if (cum_kq[action] >= r)// Is this less than or greater than?
	{
	  this->action = action;
	  return;
	}
    }
}

void intersection::apply_action()
{
  controlLights(action);
}

void intersection::get_reward () {
  int total = 0;
  for (int roadIndex = 0; roadIndex < MAX_DEGREE; roadIndex++) {
    road *curr_road = in[roadIndex];
    if (curr_road) {
      for (int position = 0; position < curr_road->length; position++) {
	if (curr_road->cars[position] && curr_road->cars[position]->wait > 0)
	  total++;
      }
    }
  }

  reward = -(float) total;
}

float* intersection::get_q_entry(int* state, int action)
{
  return 0;
}

void intersection::update_q_entry()
{
  // *get_q_entry(curr_state, curr_action) = curr_reward + 0.9 * *get_max_q_entry(next_state);
  // curr_state = next_state;
}

void intersection::controlLights (int PatternID) {

  this->pattern_id = PatternID;

  for (int direction = 0; direction < 4; direction++)
    if (this->in[direction]) {
      this->in[direction]->lights[LEFT] = RED;
      this->in[direction]->lights[RIGHT] = RED;
    }

  switch (PatternID) {
  case NORTHSOUTH_AHEADLEFT:
    if (this->in[NORTH]) 
      this->in[NORTH]->lights[LEFT] = GREEN;
    if (this->in[SOUTH]) 
      this->in[SOUTH]->lights[LEFT] = GREEN;
    break;

  case EASTWEST_AHEADLEFT:
    if (this->in[EAST]) 
      this->in[EAST]->lights[LEFT] = GREEN;
    if (this->in[WEST]) 
      this->in[WEST]->lights[LEFT] = GREEN;
    break;

  case NORTHSOUTH_RIGHT:
    if (this->in[NORTH]) 
      this->in[NORTH]->lights[RIGHT] = GREEN;
    if (this->in[SOUTH]) 
      this->in[SOUTH]->lights[RIGHT] = GREEN;
    break;

  case EASTWEST_RIGHT:
    if (this->in[EAST]) 
      this->in[EAST]->lights[RIGHT] = GREEN;
    if (this->in[WEST]) 
      this->in[WEST]->lights[RIGHT] = GREEN;
    break;

  case NORTHSOUTH_AL_AMBER:
    if (this->in[NORTH]) 
      this->in[NORTH]->lights[LEFT] = AMBER;
    if (this->in[SOUTH]) 
      this->in[SOUTH]->lights[LEFT] = AMBER;
    break;
    
  case EASTWEST_AL_AMBER:
    if (this->in[EAST]) 
      this->in[EAST]->lights[LEFT] = AMBER;
    if (this->in[WEST]) 
      this->in[WEST]->lights[LEFT] = AMBER;
    break;  

  case NORTHSOUTH_R_AMBER:
    if (this->in[NORTH]) 
      this->in[NORTH]->lights[RIGHT] = AMBER;
    if (this->in[SOUTH]) 
      this->in[SOUTH]->lights[RIGHT] = AMBER;
    break;
    
  case EASTWEST_R_AMBER:
    if (this->in[EAST]) 
      this->in[EAST]->lights[RIGHT] = AMBER;
    if (this->in[WEST]) 
      this->in[WEST]->lights[RIGHT] = AMBER;
    break;
  }
    
}


void intersection::write_state(FILE* output)
{
  fprintf(output, "Coordinates:%2d %2d\n", x, y);
}

