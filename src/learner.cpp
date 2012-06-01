#include "config.h"
#include "learner.h"
#include "car.h"
#include "math.h"
#include "assert.h"


learner::learner (void) {
  intc = 0;
}

learner::learner (world* sim) {
  this->nodes = sim->intersections;
  this->nodec = sim->intc;
  this->q_table = new int[NUM_TRAFFIC_PATTERNS*((int)pow(10, nodec*MAX_DEGREE))];
  this->state = new int[nodec + nodec*MAX_DEGREE];
  this->action = new int[NUM_TRAFFIC_PATTERNS];
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

int* learner::get_q_entry(int* action)
{
  int index = 0;

  for (int i = 0; i < nodec; i++)
    {
      index += ((int) pow(NUM_TRAFFIC_PATTERNS, i))*state[i];
    }

  for (int i = 0; i < nodec; i++)
    {
      for (int j = 0; j < MAX_DEGREE; j++)
	{
	  index += (int) (pow(NUM_TRAFFIC_PATTERNS, nodec)*pow(NUM_SLOTS_IN_ROAD, i*MAX_DEGREE + j));
	}
    }

  for (int i = 0; i < nodec; i++)
    {
      index += (int) pow(NUM_TRAFFIC_PATTERNS, nodec)*pow(NUM_SLOTS_IN_ROAD, nodec*MAX_DEGREE)*pow(nodec, i)*action[i];
    }


  return &(q_table[index]);

}

void learner::learn (int* action)
{
  sense();
  int* entry = get_q_entry(action);

  int min_q_prev = 0;

  for (int i = 0; i < NUM_TRAFFIC_PATTERNS; i++)
    {
      if (*get_q_entry(i) < min_q_prev)
	min_q_prev = *get_q_entry(i);
    }
    
  *entry = 0.9*min_q-prev;
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
       sim->intersections[i]->controlLights (((int)x)%8*2);//EASTWEST_RIGHT);x++%4
       }
     x++;
   } 
   TrafficPhase++;
  
}
