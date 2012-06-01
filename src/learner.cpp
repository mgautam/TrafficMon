#include "config.h"
#include "learner.h"
#include "car.h"
#include "math.h"
#include "assert.h"


learner::learner (void) {
  intc = 0;
}

learner::learner (world* sim) {
  this->sim = sim;
  this->nodes = sim->intersections;
  this->nodec = sim->intc;
  this->q_table = new float[NUM_TRAFFIC_PATTERNS*((int)pow(10, nodec*MAX_DEGREE))];
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
    }
  
  for (int i = 0; i < nodec; i++)
    {
      for (int j = 0; j < MAX_DEGREE; j++)
	{
	  road* curr_road = nodes[i]->in[j];
	  state[nodec + i*MAX_DEGREE + j] = NUM_SLOTS_IN_ROAD;
	  if (curr_road)
	    {
	      for (int k = 0; k < curr_road->length; k++)
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

float* learner::get_q_entry(int* state, int* actions)
{
  return &(q_table[0]);

}

int learner::select_action(int* curr_state)
{
  float total_kq = 0;
  float cum_kq[nodec*NUM_TRAFFIC_PATTERNS];

  for (int action = 0; action < nodec*NUM_TRAFFIC_PATTERNS; action++)
    {
      action_to_actions(action, actions);
      total_kq += pow(0.1, *get_q_entry(curr_state, actions));
      cum_kq[action] = total_kq;
    }  

  float r = (float)rand()/(float)RAND_MAX*total_kq;

  for (int action = 0; action < nodec*NUM_TRAFFIC_PATTERNS; action++)
    {
      if (cum_kq[action] >= r)
	  return action;
    }
}

float* learner::get_max_q_entry(int* next_state)
{
  float* max_q_entry = 0;

  for (int action = 0; action < nodec*NUM_TRAFFIC_PATTERNS; action++)
    {
      action_to_actions(action, actions);
      float* q_entry = get_q_entry(next_state, actions);
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
      
      *get_q_entry(curr_state, curr_actions) = curr_reward + 0.9 * *get_max_q_entry(next_state);
      curr_state = next_state;
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
       sim->intersections[i]->controlLights (((int)x)%8*2);//EASTWEST_RIGHT);x++%4
       }
     x++;
   } 
   TrafficPhase++;
  
}

int learner::evaluate(intersection** nodes, int nodec)
{
}
