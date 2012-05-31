#ifndef LEARNER_H
#define LEARNER_H 1

#include "intersection.h"

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
  learner (intersection* node);
  
  void addnode (intersection *node);
  int evaluate (intersection *node);
  int evaluate (void);
  int evaluate (intersection **nodes, int intc);

  //vars
  intersection_list *initial_intersection;
  intersection_list *last_node_ptr; // for inserting new nodes without hassle
  int intc;
};

#endif
