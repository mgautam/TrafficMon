#ifndef LEARNER_H
#define LEARNER_H 1

#include "intersection.h"
#include "world.h"

class learner {
 public:
  learner (void);
  learner (world* sim);

  // Sense Enviroment
  int* get_state (intersection* node);
  int* lights_state (intersection* node);


  void act(int* action);
  void sense();
  void learn(int* action);

  // Actuate
  void naiveControl (world* simulation);

  // Measure Performance
  int evaluate (intersection *node);
  int evaluate (void);
  int evaluate (intersection **nodes, int intc);
  int* get_q_entry(int* action);
  int get_reward();

  //vars
  intersection** nodes;
  int nodec;

  intersection_list *last_node_ptr; // for inserting new nodes without hassle
  int intc;

  int* q_table;
  int* state;
  int* action;
};

#endif
