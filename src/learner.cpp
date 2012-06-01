#include "config.h"
#include "learner.h"
#include "car.h"
#include "math.h"
#include "assert.h"

#define MAX_SLOTS_TO_CHECK 10

learner::learner (void) {
  intc = 0;
}

static int num_state_attribute_blocks = 2;
static int state_space_size_per_node = 1;
static int *attribute_block_length;
static int *attributes_range;
static int number_of_actions_per_state;

learner::learner (world* sim) {
  this->nodes = sim->intersections;
  this->nodec = sim->intc;


  attribute_block_length = new int[num_state_attribute_blocks];
  attributes_range = new int[num_state_attribute_blocks];

  attribute_block_length[] = {1,MAX_DEGREE};
  attributes_range[] = {NUM_TRAFFIC_PATTERNS, MAX_SLOTS_TO_CHECK};
  

  for (int i = 0, i < num_state_attribute_blocks, i++)
    state_space_size_per_node *= pow (attributes_range[i],attribute_block_length[i]);
  int global_state_space_size = pow (state_space_size_per_node,nodec);

  number_of_actions_per_state = pow (NUM_TRAFFIC_PATTERNS , nodec);

  int Qtable_size = global_state_space_size * number_of_actions_per_state;
  this->q_table = new float [Qtable_size];
}


void learner::act(int* action)
{
  for (int i = 0; i < nodec; i++)
    {
      nodes[i]->controlLights(action[i]);
    }
}

void learner::sense()
{
  for (int i = 0; i < nodec; i++)
    {
      state[i] = nodes[i]->pattern_id;
    }
  
  for (int i = 0; i < nodec; i++)
    {
      for (int j = 0; j < MAX_DEGREE; j++)
	{
	  road* curr_road = nodes[i]->in[j];
	  state[nodec + i*MAX_DEGREE + j] = MAX_SLOTS_TO_CHECK;
	  if (curr_road)
	    {
	      for (int k = 0; k < MAX_SLOTS_TO_CHECK; k++)
		{
		  if (curr_road->cars[k]) {
		    state[nodec + i*MAX_DEGREE + j] = k;
		    break;
		  }
		}
	    }
	}
    }
}

int* learner::get_q_entry(int *state, int action)
{
  int global_State = 0;
  for (int i = 0; i < nodec; i++) {
    int currNodeState = 0;
      for (int j = 0; j < num_state_attribute_blocks ; j++) {
	// Within each node
	if (j)
	currNodeState *= pow (attribute_block_range[j-1], attribute_block_length[j-1]);
	for (int k = 0; k < attribute_block_length[j]; k++) {
	  // We are evaluating state within attribute block
	  currNodeState += pow (attribute_block_range[j],attribute_block_length[j]-k) * state[i*nodec+j*attribute_block_length[j]+k];
	}
      }
      global_State += pow (state_space_size_per_node, i) * currNodeState;
  }
  
  return &(q_table[global_State * number_of_actions + action]);
}

void learner::learn (int* action)
{
  sense();
  int* entry = get_q_entry(action);

  int min_q_prev = 0;

  for (int i = 0; i < nodec*NUM_TRAFFIC_PATTERNS; i++)
    {
      for (int j = 0; j < nodec; j++) {
	action[j] = i/((int)pow(NUM_TRAFFIC_PATTERNS,j))%NUM_TRAFFIC_PATTERNS;
      }
      if (*get_q_entry(action) < min_q_prev)
	min_q_prev = *get_q_entry(action);
    }
    
  *entry = get_reward() + 0.9*min_q_prev;
}

int learner::get_reward () {
  int total = 0;
  for (int i = 0; i < nodec; i++) {
    for (int roadIndex = 0; roadIndex < MAX_DEGREE; roadIndex++) {
      road *curr_road = nodes[i]->in[roadIndex];
      if (curr_road) {
	for (int position = 0; position < curr_road->length; position++) {
	  if (curr_road->cars[position] && curr_road->cars[position]->wait > 0)
	    total++;
	}
      }
    }
  }
  return total;
}

float x = 0;
static int TrafficPhase = 0;
void learner::naiveControl (world *sim) {
  //Put traffic light changing code at the end of the routine 
  //   if you want to give a 1 time unit window for the car to move
  //      after GREEN light is signalled
  
   if (TrafficPhase % MIN_TL_SWITCH_INTERVAL == 0) {
     for (int i = 0; i < sim->intc; i++)
       {
       if (!sim->intersections[i])
   	continue;
       sim->intersections[i]->controlLights (((int)x)%4*2);//EASTWEST_RIGHT);x++%4
       }
     x++;
   } 
   TrafficPhase++;
  
}

int learner::evaluate(intersection** nodes, int nodec)
{
}
