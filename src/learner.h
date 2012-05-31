#ifndef LEARNER_H
#define LEARNER_H 1

#include "intersection.h"

struct {
  intersection *node;
  intersection_list *next_node;
} intersection_list;

class leaner {
 public:
  learner (void);
  learner (intersection* node);
  
  void addnode (intersection *node);
  int evaluate (intersection *node);
  int evaluate (void);

  //vars
  intersection_list *initial_intersection;
  intersection_list *last_node_ptr; // for inserting new nodes without hassle
  int intc;
};

#endif
