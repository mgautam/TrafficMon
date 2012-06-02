
#include "config.h"
#include "learner.h"
#include "car.h"
#include "math.h"
#include "assert.h"

static int num_state_attribute_blocks = 2;
static int state_space_size_per_node = 1;
static int *attribute_block_length;
static int *attributes_block_range;
static int number_of_actions_per_global_state;
static int per_node_state_vector_size;
static long long global_state_vector_size;

learner::learner (world* sim, painter* ppainter) {
  this->sim = sim;
  this->nodes = sim->intersections;
  this->nodec = sim->intc;
  this->ppainter = ppainter;

  attribute_block_length = new int[num_state_attribute_blocks];
  attributes_block_range = new int[num_state_attribute_blocks];

  /**** Configure this block to add different features ****/
  attribute_block_length[0] = 1;
  attributes_block_range[0] = NUM_TRAFFIC_PATTERNS;

  attribute_block_length[1] = MAX_DEGREE;
  attributes_block_range[1] = 10;

  number_of_actions_per_global_state = pow (NUM_TRAFFIC_PATTERNS , nodec);
  /**** Configure this block to add different features ****/

  per_node_state_vector_size = 0;
  for (int i = 0; i < num_state_attribute_blocks; i++)
    per_node_state_vector_size += attribute_block_length[i];
  global_state_vector_size = nodec*per_node_state_vector_size;

  for (int i = 0; i < num_state_attribute_blocks; i++)
    state_space_size_per_node *= pow (attributes_block_range[i],attribute_block_length[i]);
  int global_state_space_size = pow (state_space_size_per_node,nodec);



  long long int q_table_size = global_state_space_size * number_of_actions_per_global_state;
  this->q_table = new long double [q_table_size];
  this->state = new int[global_state_vector_size];
  //  this->actions = new int[nodec];

  printf ("per_node_state_vector_size: %d	\
state_space_size_per_node: %d		  \
global_state_vector_size: %lld		  \
global_state_space_size: %d		  \
q_table_size:%lld \n",
	  per_node_state_vector_size,
	  state_space_size_per_node,
	  global_state_vector_size,
	  global_state_space_size,
	  q_table_size);
}


 int* curr_stateg = NULL;
 int* next_stateg = NULL;
 int curr_actiong = 0;
 int curr_rewardg = 0;

void learner::glLearn ()
{
  for (int i = 0; i < nodec; i++)
    {
      nodes[i]->sense_state();
      nodes[i]->select_action();
      nodes[i]->apply_action();
      nodes[i]->get_reward();
      nodes[i]->update_q_entry();
    }

  sim->updateWorld();
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
       sim->intersections[i]->controlLights (((int)x)%4);//EASTWEST_RIGHT);x++%4
       }
     x++;
   } 
   TrafficPhase++;
  
}

int learner::evaluate(intersection** nodes, int nodec)
{
  return 0;
}
