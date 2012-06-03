
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
  FILE *output = fopen ("../learned_values.txt","w");
  int state_no = 0;
  for (int i = 0; i < nodec; i++) {
    fprintf (output,"Node: %d\n", i);
    
    for (int p = 0; p < 4; p++)
      {
	for (int t1 = 0; t1 < 10; t1++)
	  {
	    for (int t2 = 0; t2 < 10; t2++)
	      {
		for (int t3 = 0; t3 < 10; t3++)
		  {
		    for (int t4 = 0; t4 < 10; t4++)
		      {
			int curr_state[5] = {p, t4, t3, t2, t1};
			for (int action = 0; action < 4; action++)
			  {
			    fprintf (output, "State: %d => %d %d %d %d %d, Action: %d\t", state_no, t1, t2, t3, t4, p, action);
			    fprintf (output, "%f\n", *(nodes[i]->get_q_entry (curr_state,action)));		   
			  }
			state_no++;
		      }
		  }
	      }
	  }
      }
  }

  fclose (output);
}
