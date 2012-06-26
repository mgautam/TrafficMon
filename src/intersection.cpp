#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;
#include <cstring>


#include "config.h"
#include "intersection.h"
#include "car.h"
#include <math.h>



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
  //num_state_attribute_blocks = 3;
  
  attribute_block_length[0] = 1; // Traffic light pattern
  attributes_block_range[0] = NUM_TRAFFIC_PATTERNS;

  attribute_block_length[1] = NUM_LANES_PER_ROAD*MAX_DEGREE; // distance to Nearest car from Traffic Light
  attributes_block_range[1] = MAX_SLOTS_TO_CHECK;

  //  attribute_block_length[2] = NUM_LANES_PER_ROAD*MAX_DEGREE; // Number of cars on road
  //attributes_block_range[2] = MAX_SLOTS_TO_CHECK;

  number_of_actions_per_state = NUM_TRAFFIC_PATTERNS;
  /**** Configure this block to add different features ****/

  state_vector_size = 0;
  for (int i = 0; i < num_state_attribute_blocks; i++)
    state_vector_size += attribute_block_length[i];

  state_space_size = 1;
  for (int i = 0; i < num_state_attribute_blocks; i++)
    state_space_size *= pow (attributes_block_range[i],attribute_block_length[i]);
  
  
  long long int q_table_size = state_space_size * number_of_actions_per_state;
  
  printf ("state_vector_size: %d       \
state_space_size: %d		       \
q_table_size:%lld \n",
          state_vector_size,
          state_space_size,
          q_table_size);

  //printf ("%lld\n",q_table_size);
  //  this->q_table = new float [q_table_size];
  q_table = (float *) malloc (q_table_size * sizeof (float));

  /*  for (long long i = 0; i < q_table_size; i++)
    {
      q_table[i] = (float)rand()/(float)RAND_MAX; //Random Initialization of Q_table
      }*/
   memset (q_table, 0, q_table_size * sizeof (float));//Zero Initialization of Q_table

  
  this->curr_state = new int[state_vector_size]; //curr_state
  this->prev_state = new int[state_vector_size]; //prev_state
  
  memset (this->in, 0, MAX_DEGREE*sizeof(road*));
  memset (this->out, 0, MAX_DEGREE*sizeof(road*));//not necessary

  best_action = 1;
  traffic_pattern_id = 0;
}

void intersection::sense_state ()
{
  prev_wait = curr_wait;
  memcpy(prev_state, curr_state, sizeof(int)*state_vector_size);
  // prev_state = curr_state;
  
  curr_wait = get_wait ();
  curr_state[0] = traffic_pattern_id;
  
  for (int r = 0; r < MAX_DEGREE; r++)
    {
      for (int l = 0; l < NUM_LANES_PER_ROAD; l++) {
	road* curr_road = in[r];
	
	if (curr_road)
	  {
	    curr_state[1 + r] = MAX_SLOTS_TO_CHECK-1;
	    for (int k = 0; k < MAX_SLOTS_TO_CHECK; k++)
	      {
		if (curr_road->cars[k]) {
		  curr_state[1 + r*NUM_LANES_PER_ROAD+l] = k;
		  break;
		}
	      }
	    
	    /*
	    curr_state[1+MAX_DEGREE+r] = 0;
	    for (int k = 0; k < MAX_SLOTS_TO_CHECK; k++) {
	      if (curr_road->cars[k]) {
		curr_state[1+MAX_DEGREE*NUM_LANES_PER_ROAD+r*NUM_LANES_PER_ROAD+l]++;
	      }
	    }
	    */
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
      total_kq += pow(0.3, *get_q_entry(curr_state, action));//is this the right probability distribution?
      //It makes higher rewards to have lower kq
      cum_kq[action] = total_kq;
      //printf (" %f ",total_kq);
    } 

  float r = (float)rand()/(float)RAND_MAX*total_kq;
  //printf ("\t%f :",r);
  for (int action = 0; action < NUM_TRAFFIC_PATTERNS; action++)
    {
      if (cum_kq[action] >= r)// Is this less than or greater than?
	{
	  this->action = action;
	  //printf ("%d\n",this->action);
	  return;
	}
    }
}

void intersection::select_learned_action () {
  // If all states are of equal importance (value) then retain the previous best action
  // int best_action = best_action;//i.e. initialize with previous best action
  for (int action = 0; action < NUM_TRAFFIC_PATTERNS; action++) {
    if ( *get_q_entry (curr_state,best_action) < *get_q_entry (curr_state,action) )
      best_action = action;
  }
  this->action = best_action;
}

void intersection::apply_action()
{
  controlLights(this->action);
}

int intersection::get_wait () {
  int total = 0;
  for (int roadIndex = 0; roadIndex < MAX_DEGREE; roadIndex++) {
    road *curr_road = in[roadIndex];
    if (curr_road) {
      for (int laneIndex = 0; laneIndex < curr_road->numlanes; laneIndex++) {
	// We check whole road not just MAX_SLOTS_TO_CHECK
	for (int position = 0; position < curr_road->length; position++) {
	  if (curr_road->cars[laneIndex][position]) {
	    //printf ("Car %p: Position:%d Wait%d\n",curr_road->cars[position],position, curr_road->cars[position]->wait);
	    if (curr_road->cars[laneIndex][position]->wait > 0) {
	      total++;
	      //printf ("%d\n",total);
	    }
	  }
	}
      }
    }
  }

  //reward = -(float) total; // because they are all costs & not rewards
  //printf ("%f\n",reward);
  return total;
}

void intersection::get_reward (void) {

    // reward = (float) 1.0/( 1.0 + exp(- (prev_wait - curr_wait) ));//logistic function

  reward = (float) ( prev_wait - curr_wait);//  + 0.5* (prev_state[0] == curr_state[0]);

  //  printf ("%d -> %f\n",action,reward);
}

float* intersection::get_q_entry(int* state, int action)
{
  int currStateIndex = 0;
  int blockIndex = 0;
  int blockMultiplier = state_space_size;
  int subStateIndex = 0; // Part of State Index within each block

  for (int j = 0; j < num_state_attribute_blocks ; j++) {
    // Within each node
    blockMultiplier /= pow (attributes_block_range[j], attribute_block_length[j]);
    subStateIndex = 0;

    for (int k = 0; k < attribute_block_length[j]; k++) {
      // We are evaluating state within attribute block
      subStateIndex += pow (attributes_block_range[j],attribute_block_length[j]-1-k) * state[blockIndex+k];
    }
    if (j != num_state_attribute_blocks-1)
      currStateIndex += subStateIndex * blockMultiplier;
    blockIndex += attribute_block_length[j];
  }
  //printf (" %d\n",currStateIndex);// * number_of_actions_per_state + action);
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
  float alpha = LEARNING_RATE;
  float prev_Qvalue =   *get_q_entry(prev_state, action);
  float curr_Qvalue = reward + 0.9 * *get_max_q_entry(curr_state);
  *get_q_entry(prev_state, action) = (1 - alpha) * prev_Qvalue + alpha * curr_Qvalue;
  // previous = current
}

void intersection::controlLights (int PatternID) {

  for (int direction = 0; direction < 4; direction++)
    if (this->in[direction]) {
      this->in[direction]->lights[LEFT] = RED;
      this->in[direction]->lights[RIGHT] = RED;
    }

  if ( isAMBER < MIN_TL_AR_SWITCH_INTERVAL ) // AMBER_STATES
  {
    this->traffic_pattern_id = -1; // ALL_RED STATE
    isAMBER++;
  }

  // this->traffic_pattern_id => previous_pattern
  else if (
      this->traffic_pattern_id !=  PatternID // GREEN to RED Switch
      //&& this->traffic_pattern_id != -1
      ) {
    this->traffic_pattern_id += 4; // AMBER_STATES
    isAMBER = 0;
  }
  

  switch (traffic_pattern_id) {
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

  default:break; // Do Nothing
  }

  this->traffic_pattern_id = PatternID;    

}


void intersection::write_state(FILE* output)
{
  fprintf (output, "Previous State: ");
  for (int i = 0; i < state_vector_size; i++)
    fprintf (output, " %d", prev_state[i]);
  fprintf (output, "\n");

  fprintf (output, "Current State: ");
  for (int i = 0; i < state_vector_size; i++)
    fprintf (output, " %d", curr_state[i]);
  fprintf (output,"\n");

  fprintf (output, "Action: %d\n", action);

  fprintf(output, "Coordinates:%2d %2d\n", x, y);
}

