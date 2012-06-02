
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

void learner::glLearn () {

  for (int i = 0; i < nodec; i++)
    {
      // curr_stateg = sense_state();
      // printf ("Current State: ");
      // for (int i = 0; i < global_state_vector_size; i++)
      // 	printf ("%d ",curr_stateg[i]);
      // printf ("\n");

      // curr_actiong = select_action(curr_stateg);
      // printf ("Action: %d\n",curr_actiong);
      // apply_action(curr_actiong);
      // curr_rewardg = get_reward();// This is the reward for moving between states and not the reward of the state
      // next_stateg = sense_state();
  
      // *get_q_entry(curr_stateg, curr_actiong) = curr_rewardg + 0.9 * *get_max_q_entry(next_stateg);
      // curr_stateg = next_stateg;
    }
}


void learner::learn ()
{
  // int* curr_state = NULL;
  // int* next_state = NULL;
  // int curr_action = 0;
  // int curr_reward = 0;

  // curr_state = sense_state();

  for (int i = 0; i < nodec; i++)
    {
      nodes[i]->sense_state();
      nodes[i]->select_action();
      nodes[i]->apply_action();
      nodes[i]->get_reward();
      nodes[i]->update_q_entry();
    }

  sim->updateWorld();

  // Shouldn't this be put just after apply_action?
  //if (ppainter)
  //ppainter->draw();
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
