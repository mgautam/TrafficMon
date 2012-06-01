#include "config.h"
#include "learner.h"
#include "car.h"
#include "math.h"
#include "assert.h"


learner::learner (void) {
  intc = 0;
}

learner::learner (intersection** nodes, int nodec) {
  this->nodes = nodes;
  this->nodec = nodec;
  this->q_table = new int[4*((int)pow(10, nodec*MAX_DEGREE))];
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
	  state[4 + i*MAX_DEGREE + j] = curr_road->length;
	  if (curr_road)
	    {
	      for (int k = 0; k < curr_road->length; k++)
		{
		  if (curr_road->cars[k])
		    state[4 + i*MAX_DEGREE + j] = k;
		}
	    }
	}
    }
}

int* learner::get_q(int* action)
{
  int index = 0;

  for (int i = 0; i < nodec; i++)
    {
      index += ((int) pow(4, i))*state[i];
    }

  for (int i = 0; i < nodec; i++)
    {
      for (int j = 0; j < MAX_DEGREE; j++)
	{
	  index += (int) (pow(4, MAX_DEGREE)*pow(NUM_SLOTS_IN_ROAD, i*MAX_DEGREE + j));
	}
    }

  for (int i = 0; i < nodec; i++)
    {
      index += (int) pow(4, MAX_DEGREE)*pow(NUM_SLOTS_IN_ROAD, nodec*MAX_DEGREE)*pow(4, i)*action[i];
    }


  return &(q_table[index]);

}

void learner::learn (int* action)
{
  sense();



}

int* learner::get_state (intersection *node)
{

  //state space is:
  //[C_L, C_R, L_L1, L_L2, L_R1,  L_R2]

  int* ret_state = new int[3*MAX_DEGREE];

  /*
  int ret_state[3*MAX_DEGREE];

  for (int roadIndex = 0; roadIndex < MAX_DEGREE; roadIndex++) {
    road *curr_road = node->in[roadIndex];
    if (curr_road)
      {
	ret_state[roadIndex] = curr_road->length-1;
	for (int position = 0; position < curr_road->length; position++) {
	  if (curr_road->cars[position])
	    ret_state[roadIndex] = position;
	}
      }
    else {
      ret_state[roadIndex] = curr_road->length;
    }
  }

  for (int roadIndex = 0; roadIndex < MAX_DEGREE; roadIndex++) {
    road *curr_road = node->in[roadIndex];
    if (curr_road) {
      ret_state[2*MAX_DEGREE+2*roadIndex] = curr_road->lights[0];
      ret_state[2*MAX_DEGREE+2*roadIndex+1] = curr_road->lights[1];
    }
  }
  return ret_state;
}


  */
  return new int;
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



int learner::evaluate (intersection *node) {
  assert (node != 0);
  int total = 0;
  for (int roadIndex = 0; roadIndex < MAX_DEGREE; roadIndex++) {
    road *curr_road = node->in[roadIndex];
    if (curr_road) {
      for (int position = 0; position < curr_road->length; position++) {
	if (curr_road->cars[position])
	  if (curr_road->cars[position]->wait > 0) 
	    total++;
	// total += curr_road->cars[position]->wait;
	//total += curr_road->cars[position]->wait;
      }
    }
  }
  return total;
}

int learner::evaluate (void) {
}


int learner::evaluate (intersection** nodes, int intc) {

}
