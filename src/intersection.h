#ifndef INTERSECTION_H
#define INTERSECTION_H 1

#define MAX_DEGREE 4

#include <stdio.h>
#include <iostream>
using namespace std;

#include "road.h"
#include "config.h"

class intersection
{
 public:
  intersection(int x, int y);
  void write_state(FILE* output);
  void controlLights (int PatternID);

  void sense_state();
  void select_action();
  float* get_q_entry(int* state, int action);
  float* get_max_q_entry (int *state);
  void apply_action();
  void get_reward();
  int get_wait ();
  void update_q_entry();

  void select_learned_action (void);
//variables
  int x, y;

  int in_count; // Number of incoming roads
  int out_count; // Number of outgoing roads
  road* in[MAX_DEGREE];
  road* out[MAX_DEGREE];

  int traffic_pattern_id;

  int num_state_attribute_blocks;
  int *attribute_block_length;
  int *attributes_block_range;

  int state_vector_size;
  int state_space_size;

  int number_of_actions_per_state;

  int curr_wait;
  int prev_wait;

  float reward;
  int action;
  int best_action;


  int* curr_state;
  int* prev_state;
  float *q_table;

};

#endif
