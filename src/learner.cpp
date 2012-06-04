
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
	 if (sim->intersections[i]) {
	   sim->intersections[i]->controlLights (((int)x)%4);//EASTWEST_RIGHT);x++%4
	 }
       }
     x++;
   } 
   TrafficPhase++;
   
   sim->updateWorld ();
}



void learner::learn ()
{
  printf ("\ntime %lld:\n",sim->timestamp);
  for (int i = 0; i < nodec; i++)
    {
      if (nodes[i]) {
	nodes[i]->sense_state();
	nodes[i]->select_action();
	nodes[i]->apply_action();
      }
    }

  sim->updateWorld();//cars move here cars = clients
  
  for (int i = 0; i < nodec; i++)
    {
      if (nodes[i]) {
	nodes[i]->get_reward();
	if (i == 4) {
	  //	  printf ("Node %d: \n");
	  for (int k = 0; k < 4; k++) {
	    printf ("\t");
	    for (int j = 0; j < 5; j++)
	      printf ("%d ", nodes[i]->prev_state[j]);
	    printf ("%d : ",k);
	    printf ("%f\n", *nodes[i]->get_q_entry (nodes[i]->prev_state,k));
	  }
	}
	nodes[i]->sense_state();
	nodes[i]->update_q_entry();
      }
    }
  sim->incr_timestamp();
}



void learner::comply () {
  for (int i = 0; i < nodec; i++) {
    if (nodes[i]) {
      nodes[i]->sense_state();
      nodes[i]->select_learned_action ();
      nodes[i]->apply_action ();
    }
  } 
  printf ("Called!\n");
  sim->updateWorld ();
}

int learner::evaluate(intersection** nodes, int nodec)
{
  return 0;
}

void learner::print_to_file (void) {
  /*  FILE *output = fopen ("../learned_values.txt","w");
  int *curr_state;
  
  for (int i = 0; i < nodec; i++) {
    if (nodes[i]) {
      fprintf (output,"Node: %d\n", i);
      curr_state = new int [nodes[i]->state_vector_size];
      
      for (int j = 0; j < nodes[i]->state_space_size; j++) {
	
	int stateIndex = j;
	int blockIndex = nodes[i]->state_vector_size-1;
	for (int k = nodes[i]->num_state_attribute_blocks-1; k >= 0; k--) {
	  for (int l = nodes[i]->attribute_block_length[k]-1; l >= 0; l--) {
	    curr_state[blockIndex+l] = stateIndex % nodes[i]->attributes_block_range[k];
	    stateIndex -= curr_state[blockIndex+l];
	    stateIndex /= nodes[i]->attributes_block_range[k];
	  }
	  blockIndex -= nodes[i]->attribute_block_length[k];
	}
	
	for (int action = 0; action < nodes[i]->number_of_actions_per_state; action++) {
	  fprintf (output, "State: %d\t",j);
	  for (int m = 0; m < nodes[i]->state_vector_size; m++)
	    fprintf (output, "%d ", curr_state[m]);
	  fprintf (output, "Action: %d\t", action);
	  curr_state[0] = 0;
	  fprintf (output, "%f", *(nodes[i]->get_q_entry (curr_state,action)) );
	  fprintf (output,"\n");
	}


      }
      

      fprintf (output,"\n\n");
      delete curr_state;
    }
  }
  
  fclose (output);
}
  */
  FILE *output = fopen ("../learned_values.txt","w");
  
  for (int i = 0; i < nodec; i++) {
    if (nodes[i]) {
    fprintf (output,"Node: %d\n", i);
    int state_no = 0;  
    for (int p = 0; p < 4; p++)
      {
	for (int t1 = 0; t1 < 10 ; t1++)
	  {
	    for (int t2 = 0; t2 < 10 ; t2++)
	      {
		for (int t3 = 0; t3 < 10 ; t3++)
		  {
		    for (int t4 = 0; t4 < 10;  t4++)
		      {
			int curr_state[5] = {p, t4, t3, t2, t1};
			for (int action = 0; action < 4; action++)
			  {
			    fprintf (output, "State: %d => %d %d %d %d %d, Action: %d\t", state_no, p, t4, t3, t2, t1, action);
			    fprintf (output, "%f\n", *(nodes[i]->get_q_entry (curr_state,action)));		   
			  }
			state_no++;
		      }
		  }
	      }
	  }
      }
    }
  }

  fclose (output);
  }
