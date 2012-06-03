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

  attribute_block_length = new int[num_state_attribute_blocks];
  attributes_block_range = new int[num_state_attribute_blocks];

  /**** Configure this block to add different features ****/
  num_state_attribute_blocks = 2;
  
  attribute_block_length[0] = 1;
  attributes_block_range[0] = NUM_TRAFFIC_PATTERNS;

  attribute_block_length[1] = MAX_DEGREE;
  attributes_block_range[1] = MAX_SLOTS_TO_CHECK;

  number_of_actions_per_state = NUM_TRAFFIC_PATTERNS;
  /**** Configure this block to add different features ****/

  state_vector_size = 0;
  for (int i = 0; i < num_state_attribute_blocks; i++)
    state_vector_size += attribute_block_length[i];

  state_space_size = 1;
  for (int i = 0; i < num_state_attribute_blocks; i++)
    state_space_size *= pow (attributes_block_range[i],attribute_block_length[i]);
  
  
  long long int q_table_size = state_space_size * number_of_actions_per_state;
  //  this->q_table = new float [q_table_size];
  q_table = (float *) malloc (q_table_size * sizeof (float));

  
  printf ("state_vector_size: %d       \
state_space_size: %d		       \
q_table_size:%lld \n",
          state_vector_size,
          state_space_size,
          q_table_size);

  this->states = new int*[2];
  this->states[0] = new int[state_vector_size]; //curr_state
  this->states[1] = new int[state_vector_size]; //next_state

  memset (this->in, 0, MAX_DEGREE*sizeof(road*));
  memset (this->out, 0, MAX_DEGREE*sizeof(road*));//not necessary
}

void intersection::sense_state ()
{
  // What's happening here?
  memcpy(states[1], states[0], sizeof(int)*state_vector_size);
  states[0][0] = pattern_id;

  for (int j = 0; j < MAX_DEGREE; j++)
    {
      road* curr_road = in[j];
      states[0][1 + j] = MAX_SLOTS_TO_CHECK;
      if (curr_road)
	{
	  for (int k = 0; k < MAX_SLOTS_TO_CHECK; k++)
	    {
	      if (curr_road->cars[k]) {
		states[0][1 + j] = k;
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
      total_kq += pow(0.1, *get_q_entry(states[0], action));//is this the right probability distribution?
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

void intersection::select_learned_action () {
  int best_action = 0;
  for (int action = 0; action < NUM_TRAFFIC_PATTERNS; action++) {
    if ( *get_q_entry (states[0],best_action) < *get_q_entry (states[0],action) )
      best_action = action;
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

  reward = -(float) total; // because they are all costs & not rewards
}

float* intersection::get_q_entry(int* state, int action)
{
  int currStateIndex = 0;
  int blockIndex = 0;
  int blockMultiplier = state_space_size;

  for (int j = 0; j < num_state_attribute_blocks ; j++) {
    // Within each node
    blockMultiplier /= pow (attributes_block_range[j], attribute_block_length[j]);

    for (int k = 0; k < attribute_block_length[j]; k++) {
      // We are evaluating state within attribute block
      currStateIndex += pow (attributes_block_range[j],attribute_block_length[j]-1-k) * state[blockIndex+k];
    }

    currStateIndex *= blockMultiplier;
    blockIndex += attribute_block_length[j];
  }
   
  return &(q_table[currStateIndex * number_of_actions_per_state + action]);

 }

float* intersection::get_max_q_entry (int *state) {
  float *MaxQEntrySoFar = 0;
  for (int i = 0; i < number_of_actions_per_state; i++) {
    if ( !MaxQEntrySoFar || *(MaxQEntrySoFar) < *get_q_entry (state, i) )
      MaxQEntrySoFar = get_q_entry (state, i);
  }
  return MaxQEntrySoFar;
}


void intersection::update_q_entry()
{
  *get_q_entry(states[0], action) = reward + 0.9 * *get_max_q_entry(states[1]);
  // curr_state = next_state;
     states[0] = states[1];
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
  fprintf (output, "Current State: ");
  for (int i = 0; i < state_vector_size; i++)
    fprintf (output, " %d", states[0][i]);
  fprintf (output,"\n");

  fprintf (output, "Next State: ");
  for (int i = 0; i < state_vector_size; i++)
    fprintf (output, " %d", states[1][i]);
  fprintf (output, "\n");

  fprintf (output, "Action: %d\n", action);

  fprintf(output, "Coordinates:%2d %2d\n", x, y);
}

