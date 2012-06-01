#ifndef LEARNER_H
#define LEARNER_H 1

#include "intersection.h"
#include "world.h"

//typedef struct intersection_list intersection_list;
//typedef struct intersection_list *intersection_list_ptr;

//struct intersection_list;

struct intersection_list {
  intersection *node;
  intersection_list *next_node;
};


class learner {
 public:
  learner (void);
  learner (world* sim);

  void addnode (intersection *node);

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
  float* get_q_entry(int* state, int* action);
  float* get_max_q_entry(int* next_state);
  float get_reward();

  //vars
  intersection** nodes;
  int nodec;

  intersection_list *last_node_ptr; // for inserting new nodes without hassle
  int intc;

  void learn();
  int* sense_state();
  int select_action(int* curr_state);
  void apply_action(int* actions);
  void action_to_actions(int action, int* actions);
  float* q_table;
  int* state;
  int* actions;
  world* sim;
};

#endif
