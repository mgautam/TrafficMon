#ifndef INTERSECTION_H
#define INTERSECTION_H 1

#define MAX_DEGREE 4

#include <stdio.h>
#include <iostream>
using namespace std;

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

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
  void apply_action();
  void get_reward();
  void update_q_entry();
//variables
  int x;
  int y;
  
  int in_count;
  int out_count;

  int length; //2 means the intersection is 2 x 2.

  road* in[MAX_DEGREE];
  road* out[MAX_DEGREE];

  int pattern_id;
  int* states;
  float reward;
  int action;

};

#endif
