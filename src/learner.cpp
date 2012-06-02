
#include "config.h"
#include "learner.h"
#include "car.h"
#include "math.h"
#include "assert.h"

#define MAX_SLOTS_TO_CHECK 10

static int num_state_attribute_blocks = 2;
static int state_space_size_per_node = 1;
static int *attribute_block_length;
static int *attributes_block_range;
static int number_of_actions_per_state;

learner::learner (world* sim, painter* ppainter) {
  this->sim = sim;
  this->nodes = sim->intersections;
  this->nodec = sim->intc;
  this->ppainter = ppainter;

  attribute_block_length = new int[num_state_attribute_blocks];
  attributes_block_range = new int[num_state_attribute_blocks];

  attribute_block_length[0] = 1;
  attributes_block_range[0] = NUM_TRAFFIC_PATTERNS;

  attribute_block_length[1] = MAX_DEGREE;
  attributes_block_range[1] = MAX_SLOTS_TO_CHECK;

  for (int i = 0; i < num_state_attribute_blocks; i++)
    state_space_size_per_node *= pow (attributes_block_range[i],attribute_block_length[i]);
  int global_state_space_size = pow (state_space_size_per_node,nodec);

  number_of_actions_per_state = pow (NUM_TRAFFIC_PATTERNS , nodec);

  int q_table_size = global_state_space_size * number_of_actions_per_state;
  this->q_table = new float [q_table_size];
  this->state = new int[nodec + nodec*MAX_DEGREE];
  this->actions = new int[nodec];
}


void learner::act(int* action)
{
  for (int i = 0; i < nodec; i++)
    {
      nodes[i]->controlLights(action[i]);
    }
}

int* learner::sense_state()
{
  for (int i = 0; i < nodec; i++)
    {
      state[i] = nodes[i]->pattern_id;
      for (int j = 1; j < 1+MAX_DEGREE; j++)
	{
	  road* curr_road = nodes[i]->in[j-1];
	  state[i*(1+MAX_DEGREE) + j] = MAX_SLOTS_TO_CHECK;
	  if (curr_road)
	    {
	      for (int k = 0; k < MAX_SLOTS_TO_CHECK; k++)
		{
		  if (curr_road->cars[k]) {
		    state[ i*(1+MAX_DEGREE) + j] = k;
		    break;
		  }
		}
	    }
	}
    }
  return state;
}


float* learner::get_q_entry(int *state, int action)
{
  int global_State = 0;
  for (int i = 0; i < nodec; i++) {
    int currNodeState = 0;
      for (int j = 0; j < num_state_attribute_blocks ; j++) {
	// Within each node
	if (j)
	currNodeState *= pow (attributes_block_range[j-1], attribute_block_length[j-1]);
	for (int k = 0; k < attribute_block_length[j]; k++) {
	  // We are evaluating state within attribute block
	  currNodeState += pow (attributes_block_range[j],attribute_block_length[j]-k) * state[i*nodec+j*attribute_block_length[j]+k];
	}
      }
      global_State += pow (state_space_size_per_node, i) * currNodeState;
  }
  
  return &(q_table[global_State * number_of_actions_per_state + action]);

}

int learner::select_action(int* curr_state)
{
  float total_kq = 0;
  float cum_kq[nodec*NUM_TRAFFIC_PATTERNS];

  for (int action = 0; action < nodec*NUM_TRAFFIC_PATTERNS; action++)
    {
      //      action_to_actions(action, actions);
      total_kq += pow(0.1, *get_q_entry(curr_state, action));
      cum_kq[action] = total_kq;
    }  

  float r = (float)rand()/(float)RAND_MAX*total_kq;

  for (int action = 0; action < nodec*NUM_TRAFFIC_PATTERNS; action++)
    {
      if (cum_kq[action] >= r)
	  return action;
    }
  return 0;
}

float* learner::get_max_q_entry(int* next_state)
{
  float* max_q_entry = 0;

  for (int action = 0; action < nodec*NUM_TRAFFIC_PATTERNS; action++)
    {
      //      action_to_actions(action, actions);
      float* q_entry = get_q_entry(next_state, action);
      if (!max_q_entry || *q_entry > *max_q_entry)
	max_q_entry = q_entry;
    }
  return max_q_entry;
}

void learner::action_to_actions(int action, int* actions)
{
  for (int i = 0; i < nodec; i++)
    {
      actions[i] = action/((int) pow(nodec, i))%nodec;
    }
}

void learner::apply_action(int* actions)
{
  for (int i = 0; i < nodec; i++)
    {
      nodes[i]->controlLights(actions[i]);
    }

  sim->updateWorld();

}


void learner::learn ()
{
  int* curr_state = NULL;
  int* next_state = NULL;
  int curr_action = 0;
  int curr_reward = 0;
  int* curr_actions = new int[nodec];

  curr_state = sense_state();

  while (true)
    {
      curr_action = select_action(curr_state);
      action_to_actions(curr_action, curr_actions);
      apply_action(curr_actions);
      curr_reward = get_reward();
      next_state = sense_state();
      
      *get_q_entry(curr_state, curr_action) = curr_reward + 0.9 * *get_max_q_entry(next_state);
      curr_state = next_state;

      if (ppainter)
	ppainter->draw();

    }
}

float learner::get_reward () {
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
  return -(float) total;
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
  return 0;
}
