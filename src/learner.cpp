
#include "config.h"
#include "learner.h"
#include "car.h"
#include "math.h"
#include "assert.h"


learner::learner (world* sim) {
  this->sim = sim;
  this->nodes = sim->intersections;
  this->nodec = &sim->intc;
  //this->ppainter = ppainter;
}



static int TrafficPhase = 0;
void learner::naiveControl (world *sim) {
  //Put traffic light changing code at the end of the routine 
  //   if you want to give a 1 time unit window for the car to move
  //      after GREEN light is signalled
  
  for (int i = 0; i < sim->intc; i++)
    {
      if (sim->intersections[i]) {
	sim->intersections[i]->controlLights (((int)TrafficPhase)%4);//EASTWEST_RIGHT);x++%4
      }
    }


   if (sim->timestamp % MIN_TL_SWITCH_INTERVAL == 0) {
     TrafficPhase++;
   }
   
   sim->updateWorld ();
   sim->incr_timestamp();
   this->evaluate ();
}



void learner::learn (bool fullSpeed)
{
  if ( !( sim->timestamp % (fullSpeed?10000:100) ) )
    printf ("time %lld\n",sim->timestamp);



    for (int i = 0; i < *nodec; i++)
      {
	if (nodes[i]) {
	  if ( sim->timestamp % MIN_TL_SWITCH_INTERVAL == 0) {
	    //printf ("Second Sense:\n ");
	    nodes[i]->sense_state();
	    nodes[i]->get_reward();
	    nodes[i]->update_q_entry();
	    //printf ("First Sense:\n");
	    //nodes[i]->sense_state();
	    nodes[i]->select_action();
	    nodes[i]->total_waiting_cars = 0;
	  }
	    nodes[i]->apply_action();
	}
      }

  sim->updateWorld();//cars move here cars = clients
  /*
  if ( sim->timestamp % MIN_TL_SWITCH_INTERVAL == 0) {
    for (int i = 0; i < *nodec; i++)
      {
	if (nodes[i]) {
	  //printf ("Second Sense:\n ");
	  nodes[i]->sense_state();
	  nodes[i]->get_reward();
	  // for (int j = 1; j < 5; j++)
	  //    printf ("%d ", nodes[i]->prev_state[j]);
	  // if (i == 4) {
	  //   printf ("Node %d: \n",i);
	  //   for (int k = 0; k < 4; k++) {
	  //   printf ("\t");
	  //   for (int j = 0; j < 5; j++)
	  //   printf ("%d ", nodes[i]->prev_state[j]);
	  //   printf ("%d : ",k);
	  //   printf ("%i %f\n", nodes[i]->get_q_entry (nodes[i]->prev_state,k) - nodes[i]->q_table, *nodes[i]->get_q_entry (nodes[i]->prev_state,k));
	  //    }
	  //}
	  nodes[i]->update_q_entry();
	}
      }
  }
  */
  sim->incr_timestamp();
}

static int performance = 0;
int learner::evaluate(void)//intersection** nodes, int *nodec)
{
  for (int i = 0; i < *nodec; i++)
    if (nodes[i]) {
      for (int r = 0; r < MAX_DEGREE; r++){ // not in_count
	if ( nodes[i]->in[r] )
	  for (int l = 0; l < nodes[i]->in[r]->numlanes; l++) {
	    for (int s = 0; s < nodes[i]->in[r]->length;s++) // We check whole road not just MAX_SLOTS_TO_CHECK
	      if (nodes[i]->in[r]->cars[l][s] && nodes[i]->in[r]->cars[l][s]->wait > 0) {
		performance--;
	    }
	  }
      }
    }
  return performance;
}

void learner::displayPerformance (int timeInterval) {
  printf (">> Performance in last interval of %d time units is %d << \n",timeInterval,
	  performance);
  performance = 0;
}

void learner::comply () {
  if ( sim->timestamp % MIN_TL_SWITCH_INTERVAL == 0) {
    for (int i = 0; i < *nodec; i++) {
      if (nodes[i]) {
	nodes[i]->sense_state();
	nodes[i]->select_learned_action ();
	nodes[i]->apply_action ();
      }
    } 
  }
  // printf ("Called!\n");
  sim->updateWorld ();
  sim->incr_timestamp();
}

void learner::print_to_file (void) {
  FILE *output = fopen ("../learned_values.txt","w");
  int *curr_state;
  
  for (int i = 0; i < *nodec; i++) {
    if (nodes[i]) {
      fprintf (output,"Node: %d\n", i);
      curr_state = new int [nodes[i]->state_vector_size];
      
      for (int j = 0; j < nodes[i]->state_space_size; j++) {
	
	int stateIndex = j;
	int blockIndex = nodes[i]->state_vector_size;
	for (int k = nodes[i]->num_state_attribute_blocks-1; k >= 0; k--) {
	  blockIndex -= nodes[i]->attribute_block_length[k];
	  for (int l = nodes[i]->attribute_block_length[k]-1; l >= 0; l--) {
	    curr_state[blockIndex+l] = stateIndex % nodes[i]->attributes_block_range[k];
	    stateIndex -= curr_state[blockIndex+l];
	    stateIndex /= nodes[i]->attributes_block_range[k];
	  }
	}
	
	for (int action = 0; action < nodes[i]->number_of_actions_per_state; action++) {
	  fprintf (output, "State: %d\t",j);
	  for (int m = 0; m < nodes[i]->state_vector_size; m++)
	    fprintf (output, "%d ", curr_state[m]);
	  fprintf (output, "Action: %d\t", action);
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

