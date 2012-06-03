#ifndef LEARNER_H
#define LEARNER_H 1

#include "intersection.h"
#include "world.h"
#include "painter.h"

class learner {
 public:
  learner (world* sim, painter* ppainter);

  // Actuate
  void naiveControl (world* simulation);
  void learn ();
  void comply ();

  // Measure Performance
  int evaluate (intersection *node);
  int evaluate (void);
  int evaluate (intersection **nodes, int intc);

  // Show Results
  void print_to_file (void);

  //vars
  intersection** nodes;
  int nodec;

  world* sim;
  painter* ppainter;

};

#endif
