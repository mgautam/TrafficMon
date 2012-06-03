
#include "config.h"
#include "learner.h"
#include "car.h"
#include "math.h"
#include "assert.h"


learner::learner (world* sim, painter* ppainter) {
  this->sim = sim;
  this->nodes = sim->intersections;
  this->nodec = sim->intc;
  this->ppainter = ppainter;
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



void learner::learn ()
{
  for (int i = 0; i < nodec; i++)
    {
      nodes[i]->sense_state();
      nodes[i]->select_action();
      nodes[i]->apply_action();
      nodes[i]->get_reward();
      nodes[i]->update_q_entry();
    }

  //sim->updateWorld();
}



void learner::comply () {
  for (int i = 0; i < nodec; i++) {
    nodes[i]->sense_state();
    nodes[i]->select_learned_action ();
    nodes[i]->apply_action ();
  } 
  printf ("Called!\n");
  //sim->updateWorld ();
}

int learner::evaluate(intersection** nodes, int nodec)
{
  return 0;
}

void learner::print_to_file (void) {

}
