#include "learner.h"

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

learner::addnode (intersection* node) {
  last_node_ptr->next_node = new intersection_list;
  last_node_ptr = last_node_ptr->next_node;
  
  last_node_ptr->node = node;
  last_ptr->next_node = NULL;
  intc++;
}

int learner::evaluate (intersection *node) {
  int total = 0;
  for (int roadIndex = 0; roadIndex < MAX_DEGREE; roadIndex++) {
    road *curr_road = node->in[roadIndex];
    for (int position = 0; position < curr_road->length; position++) {
      total += curr_road->cars[position]->wait;
    }
  }
  return total;
}

int learner::evaluate (void) {
  intersection_list *curr_node_ptr = initial_intersection;
  int total = 0;
  while (curr_node_ptr != NULL) {
    total += this->evaluate (curr_node_ptr->node);
    curr_node_ptr = curr_node_ptr->next_node;
  }
}
