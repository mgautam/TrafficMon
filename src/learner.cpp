#include "learner.h"
#include "car.h"

learner::learner (void) {
  initial_intersection = NULL;
  intc = 0;
}

learner::learner (intersection* node) {
  initial_intersection = new intersection_list;
  initial_intersection->node = node;
  initial_intersection->next_node = NULL;
  last_node_ptr = initial_intersection;
  intc = 1;
  
}

void learner::addnode (intersection* node) {
  last_node_ptr->next_node = new intersection_list;
  last_node_ptr = last_node_ptr->next_node;
  
  last_node_ptr->node = node;
  last_node_ptr->next_node = NULL;
  intc++;
}

int learner::evaluate (intersection *node) {
  int total = 0;
  for (int roadIndex = 0; roadIndex < MAX_DEGREE; roadIndex++) {
    road *curr_road = node->in[roadIndex];
    if (curr_road) {
      for (int position = 0; position < curr_road->length; position++) {
	if (curr_road->cars[position])
	  if (curr_road->cars[position]->wait > 0) total++;
	  //total += curr_road->cars[position]->wait;
      }
    }
  }
  return total;
}

int learner::evaluate (void) {
  intersection_list *curr_node_ptr = initial_intersection;
  int total = 0;
  while (curr_node_ptr != NULL) {
    if (curr_node_ptr) {
      total += this->evaluate (curr_node_ptr->node);
      curr_node_ptr = curr_node_ptr->next_node;
    }
  }
  return total;
}

int learner::evaluate (intersection** nodes, int intc) {
  int total = 0;
  for (int i = 0; i < intc; i++) 
    //  int i = 0;
  {
    if (nodes[i])
      total += this->evaluate (nodes[i]);
    //    printf ("%d\n",total);
  }
  return total;
}
