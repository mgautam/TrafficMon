#include <cstring>

#include "config.h"
#include "bigbang.h"

void bigbang::create_world(world** sim)
{
  
  intersection** intersections = new intersection*[5];
  intersections[0] =   new intersection (-NUM_SLOTS_IN_ROAD,0); // left
  intersections[1] =   new intersection (0,-NUM_SLOTS_IN_ROAD); // bottom
  intersections[2] =   new intersection (0,0); // center
  intersections[3] =   new intersection (0,NUM_SLOTS_IN_ROAD); // top
  intersections[4] =   new intersection (NUM_SLOTS_IN_ROAD,0); // right
  int intersection_count = 5;


  road** roads = new road*[8];
  roads[0] = new road (intersections[1],intersections[2]);//NORTH
  roads[1] = new road (intersections[0],intersections[2]);//EAST
  roads[2] = new road (intersections[2],intersections[3]);//SOUTH
  roads[3] = new road (intersections[2],intersections[4]);//WEST
  roads[4] = new road (intersections[2],intersections[1]);
  roads[5] = new road (intersections[2],intersections[0]);
  roads[6] = new road (intersections[3],intersections[2]);
  roads[7] = new road (intersections[4],intersections[2]);
  int road_count = 8;
     
  car** cars = new car*[10];
  cars[0] = new car(roads[0], AHEAD, roads[0]->length - 1); 
  cars[1] = new car(roads[0], RIGHT, (float)roads[0]->length - 3);
  cars[2] = new car(roads[0], LEFT, (float)roads[0]->length - 5);
  cars[3] = new car(roads[0], LEFT, (float)roads[0]->length - 7);

  cars[5] = new car(roads[1], AHEAD, (float)roads[1]->length - 1);
  cars[6] = new car(roads[1], RIGHT, (float)roads[1]->length - 3);
  cars[7] = new car(roads[1], LEFT, (float)roads[1]->length - 5);
  cars[8] = new car(roads[1], LEFT, (float)roads[1]->length - 7);


  
  /*
  int road_count = 2*2*(SQUARE_SIZE+1)*SQUARE_SIZE;
  int intersection_count = (SQUARE_SIZE+2)*(SQUARE_SIZE+2);
  intersection** intersections = new intersection*[intersection_count];
  road** roads = new road*[road_count];
  
  memset(intersections, 0, sizeof(intersection*)*intersection_count);
  for (int y = 0; y < SQUARE_SIZE+2; y++)
    {
      for (int x = 0; x < SQUARE_SIZE+2; x++)
	{
	  intersections[y*(SQUARE_SIZE+2)+x] = 0;
	  if ((x == 0 || x == SQUARE_SIZE + 1) && (y == 0 || y == SQUARE_SIZE + 1))
	    continue;

	  // Add 2 unit to the length of road in order to include size of intersection
	  // intersections[y*SQUARE_SIZE+x] = new intersection((2*x - (SQUARE_SIZE-1))*(NUM_SLOTS_IN_ROAD+1), (2*y - (SQUARE_SIZE-1))*(NUM_SLOTS_IN_ROAD+1));
	  intersections[y*(SQUARE_SIZE+2)+x] = new intersection((NUM_SLOTS_IN_ROAD+2)*x, (NUM_SLOTS_IN_ROAD+2)*y);
	}
    }

  for (int y = 0; y < SQUARE_SIZE; y++)
    {
      for (int x = 0; x < SQUARE_SIZE + 1; x++)
	{
	  roads[y*(SQUARE_SIZE+1)+x] = new road(intersections[(y+1)*(SQUARE_SIZE+2)+x], intersections[(y+1)*(SQUARE_SIZE+2) + (x+1)]);
	  roads[road_count/4 + y*(SQUARE_SIZE+1)+x] = new road(intersections[(y+1)*(SQUARE_SIZE+2) + (x+1)], intersections[(y+1)*(SQUARE_SIZE+2) + x]);
	}
    }

  for (int x = 0; x < SQUARE_SIZE; x++)
    {
      for (int y = 0; y < SQUARE_SIZE + 1; y++)
	{
	  roads[road_count/2 + x*(SQUARE_SIZE+1)+y] = new road(intersections[y*(SQUARE_SIZE+2)+(x+1)], intersections[(y+1)*(SQUARE_SIZE+2) + (x+1)]);
	  roads[road_count/4*3 + x*(SQUARE_SIZE+1)+y] = new road(intersections[(y+1)*(SQUARE_SIZE+2)+(x+1)], intersections[y*(SQUARE_SIZE+2) + (x+1)]);
	}
    }
    
  car** cars = new car*[10];
  cars[0] = new car(roads[0], AHEAD, roads[0]->length - 1); 
  cars[1] = new car(roads[0], RIGHT, (float)roads[0]->length - 3);
  cars[2] = new car(roads[0], LEFT, (float)roads[0]->length - 5);
  cars[3] = new car(roads[0], LEFT, (float)roads[0]->length - 7);

  cars[5] = new car(roads[1], AHEAD, (float)roads[1]->length - 1);
  cars[6] = new car(roads[1], RIGHT, (float)roads[1]->length - 3);
  cars[7] = new car(roads[1], LEFT, (float)roads[1]->length - 5);
  cars[8] = new car(roads[1], LEFT, (float)roads[1]->length - 7);
 
  
  new car(roads[20], LEFT, (float)roads[20]->length - 1);
  new car(roads[20], RIGHT, (float)roads[20]->length - 3);
  new car(roads[20], AHEAD, (float)roads[20]->length - 5);
  new car(roads[20], AHEAD, (float)roads[20]->length - 7);

  new car(roads[23], LEFT, (float)roads[23]->length - 1);
  new car(roads[23], RIGHT, (float)roads[23]->length - 3);
  new car(roads[23], AHEAD, (float)roads[23]->length - 5);
  new car(roads[23], AHEAD, (float)roads[23]->length - 7);
*/
  // new car(roads[0], RIGHT, (float)roads[0]->length - 9);
  // new car(roads[1], RIGHT, (float)roads[1]->length - 9);
  // new car(roads[2], RIGHT, (float)roads[2]->length - 9);
  // new car(roads[3], RIGHT, (float)roads[3]->length - 9);
  // new car(roads[4], RIGHT, (float)roads[4]->length - 9);



  *sim = new world ( intersection_count, intersections, road_count, roads);

}


void bigbang::destroy_world (world* sim) {
  int temp_intc = sim->intc;
  int temp_roadc = sim->roadc;

  sim->intc = 0;
  sim->roadc = 0;

  delete sim->next_spawn_time;
  delete sim->num_spawn;

  delete sim->road_intersection_relations[0];
  delete sim->road_intersection_relations[1];

  for (int i = 0; i < temp_intc; i++) {
    if (!sim->intersections[i])
      continue;

    delete sim->intersections[i]->attribute_block_length;
    delete sim->intersections[i]->attributes_block_range;
    delete sim->intersections[i]->curr_state;
    delete sim->intersections[i]->prev_state;
    delete sim->intersections[i]->q_table;
    
    delete sim->intersections[i];
    sim->intersections[i] = 0;
  }
  delete sim->intersections;

  for (int i = 0; i < temp_roadc; i++) {
    for (int j = -2; j < sim->roads[i]->length; j++)
      {
	if (sim->roads[i]->cars[j])
	  delete sim->roads[i]->cars[j];
      }
    //    delete sim->roads[i]->cars;
    delete sim->roads[i];
  }
  delete sim->roads;
}



void bigbang::save_world (world* sim) {
  FILE *output = fopen ("../world_state","wb");
  
  // World Variables
  //  fprintf (output,"%d",sim->intc);
  fwrite (&sim->intc, sizeof (int), 1, output);
  fwrite (&sim->roadc, sizeof (int), 1, output);
  fwrite (&sim->timestamp, sizeof (long long), 1, output);
  
  fwrite (sim->next_spawn_time,sizeof (long long),sim->roadc,output);
  fwrite (sim->num_spawn,sizeof (int),sim->roadc,output);  

  fwrite (sim->road_intersection_relations[0],sizeof (int), sim->roadc, output);
  fwrite (sim->road_intersection_relations[1],sizeof (int), sim->roadc, output);

  // Intersection Variables
  int zero = 0, ones = 127;
  for (int i = 0; i < sim->intc; i++) {
    if (!sim->intersections[i]) {
      fwrite (&zero, sizeof (int), 1, output);
      continue;
    }
    else 
      fwrite (&ones, sizeof (int), 1, output);

    fwrite (&sim->intersections[i]->x, sizeof (int), 1, output);
    fwrite (&sim->intersections[i]->y, sizeof (int), 1, output);
    
    //fwrite (&sim->intersections[i]->in_count, sizeof (int), 1, output);
    //fwrite (&sim->intersections[i]->out_count, sizeof (int), 1, output);

    fwrite (&sim->intersections[i]->traffic_pattern_id, sizeof (int), 1, output);

    fwrite (&sim->intersections[i]->num_state_attribute_blocks, sizeof (int), 1, output);
    fwrite (sim->intersections[i]->attribute_block_length, sizeof (int), sim->intersections[i]->num_state_attribute_blocks, output);
    fwrite (sim->intersections[i]->attributes_block_range, sizeof (int), sim->intersections[i]->num_state_attribute_blocks, output);

    fwrite (&sim->intersections[i]->state_vector_size, sizeof (int), 1, output);
    fwrite (&sim->intersections[i]->state_space_size, sizeof (int), 1, output);

    fwrite (&sim->intersections[i]->number_of_actions_per_state, sizeof (int), 1, output);

    fwrite (&sim->intersections[i]->curr_wait, sizeof (int), 1, output);
    fwrite (&sim->intersections[i]->prev_wait, sizeof (int), 1, output);

    fwrite (&sim->intersections[i]->reward, sizeof (float), 1, output);
    fwrite (&sim->intersections[i]->action, sizeof (int), 1, output);
    fwrite (&sim->intersections[i]->best_action, sizeof (int), 1, output);

    fwrite (sim->intersections[i]->curr_state, sizeof (int), sim->intersections[i]->state_vector_size, output);
    fwrite (sim->intersections[i]->prev_state, sizeof (int), sim->intersections[i]->state_vector_size, output);

    fwrite (sim->intersections[i]->q_table, sizeof (float), sim->intersections[i]->state_space_size*sim->intersections[i]->number_of_actions_per_state, output);
  }
  
  // Road Variables
  for (int i = 0; i < sim->roadc; i++) {
    //fwrite (&sim->roads[i]->length, sizeof (int), 1, output);
    fwrite (sim->roads[i]->lights, sizeof (int), 2, output);
    //fwrite (&sim->roads[i]->compass, sizeof (int), 1, output);
    fwrite (sim->roads[i]->traffic_pattern_cdf, sizeof (float), 4, output);

    int syncInt = -127;
    for (int j = -2; j < sim->roads[i]->length; j++) {
      if (sim->roads[i]->cars[j]) {
	//fputc ('t',output); // sync info
	fwrite (&syncInt, sizeof (int), 1, output);// sync info

	fwrite (&sim->roads[i]->cars[j]->position, sizeof (int), 1, output);
	fwrite (&sim->roads[i]->cars[j]->turn, sizeof (int), 1, output);
	fwrite (&sim->roads[i]->cars[j]->wait, sizeof (int), 1, output);

	fwrite (&sim->roads[i]->cars[j]->color.r, sizeof (float), 1, output);
	fwrite (&sim->roads[i]->cars[j]->color.g, sizeof (float), 1, output);
	fwrite (&sim->roads[i]->cars[j]->color.b, sizeof (float), 1, output);
	
	fwrite (&sim->roads[i]->cars[j]->moved, sizeof (bool), 1, output);
	fwrite (&sim->roads[i]->cars[j]->sensed, sizeof (bool), 1, output);
      }
    }
    //fputc ('f',output); // sync Info
    syncInt = -128;
    fwrite (&syncInt, sizeof (int), 1, output);// sync info
  }

  fclose (output);
}

void bigbang::recreate_world (world* sim) {
  FILE *input = fopen ("../world_state","rb");

  int temp_intc, temp_roadc;

  // World Variables%
  //fscanf (input,"%d",&temp_intc);
  fread (&temp_intc, sizeof (int), 1, input);
  fread (&temp_roadc, sizeof (int), 1, input);
  fread (&sim->timestamp, sizeof (long long), 1, input);

  sim->next_spawn_time = new long long [temp_roadc];
  fread (sim->next_spawn_time,sizeof (long long),temp_roadc,input);
  sim->num_spawn = new int[temp_roadc];
  fread (sim->num_spawn,sizeof (int),temp_roadc,input);  

  sim->road_intersection_relations[0] = new int [temp_roadc];
  fread (sim->road_intersection_relations[0],sizeof (int), temp_roadc, input);
  sim->road_intersection_relations[1] = new int [temp_roadc];
  fread (sim->road_intersection_relations[1],sizeof (int), temp_roadc, input);

  // Intersection Variables
  sim->intersections = new intersection*[temp_intc];

  int x,y;
  int isOnes;
  for (int i = 0; i < temp_intc; i++) {
    printf ("%d: ",i);

    fread (&isOnes, sizeof (int), 1, input);
    if (isOnes) {
      fread (&x, sizeof (int), 1, input);
      fread (&y, sizeof (int), 1, input);
      
      sim->intersections[i] = new intersection (x,y);
      
      //fread (&sim->intersections[i]->in_count, sizeof (int), 1, input);
      //fread (&sim->intersections[i]->out_count, sizeof (int), 1, input);
      
      fread (&sim->intersections[i]->traffic_pattern_id, sizeof (int), 1, input);
      
      fread (&sim->intersections[i]->num_state_attribute_blocks, sizeof (int), 1, input);
      
      sim->intersections[i]->attribute_block_length = new int [sim->intersections[i]->num_state_attribute_blocks];
      fread (sim->intersections[i]->attribute_block_length, sizeof (int), sim->intersections[i]->num_state_attribute_blocks, input);
      sim->intersections[i]->attributes_block_range = new int [sim->intersections[i]->num_state_attribute_blocks];
      fread (sim->intersections[i]->attributes_block_range, sizeof (int), sim->intersections[i]->num_state_attribute_blocks, input);
      
      fread (&sim->intersections[i]->state_vector_size, sizeof (int), 1, input);
      fread (&sim->intersections[i]->state_space_size, sizeof (int), 1, input);
      
      fread (&sim->intersections[i]->number_of_actions_per_state, sizeof (int), 1, input);
      
      fread (&sim->intersections[i]->curr_wait, sizeof (int), 1, input);
      fread (&sim->intersections[i]->prev_wait, sizeof (int), 1, input);
      
      fread (&sim->intersections[i]->reward, sizeof (float), 1, input);
      fread (&sim->intersections[i]->action, sizeof (int), 1, input);
      fread (&sim->intersections[i]->best_action, sizeof (int), 1, input);
      
      if (sim->intersections[i]->curr_state)  delete sim->intersections[i]->curr_state;
      sim->intersections[i]->curr_state = new int [sim->intersections[i]->state_vector_size];
      fread (sim->intersections[i]->curr_state, sizeof (int), sim->intersections[i]->state_vector_size, input);
      if (sim->intersections[i]->prev_state)  delete sim->intersections[i]->prev_state;
      sim->intersections[i]->prev_state = new int [sim->intersections[i]->state_vector_size];
      fread (sim->intersections[i]->prev_state, sizeof (int), sim->intersections[i]->state_vector_size, input);
      
      sim->intersections[i]->q_table = new float[sim->intersections[i]->state_space_size*sim->intersections[i]->number_of_actions_per_state];
      fread (sim->intersections[i]->q_table, sizeof (float), sim->intersections[i]->state_space_size*sim->intersections[i]->number_of_actions_per_state, input);
    }
    else {
      printf ("%d\n",0);
      sim->intersections[i] = 0;
    }
  }
  
  // Road Variables
  sim->roads = new road*[temp_roadc];
  int init_index, final_index;
  for (int r = 0; r < temp_roadc; r++) {
    init_index = sim->road_intersection_relations[0][r];
    final_index = sim->road_intersection_relations[1][r];
    sim->roads[r] = new road ( sim->intersections[init_index], sim->intersections[final_index] );
  }

  for (int i = 0; i < temp_roadc; i++) {
    //    fscanf (input, "%d", &sim->roads[i]->length);//Could be removed
    fread (sim->roads[i]->lights, sizeof (int), 2, input);
    //    fscanf (input, "%d", &sim->roads[i]->compass);//Could be removed
    fread  (sim->roads[i]->traffic_pattern_cdf, sizeof (float), 4, input);

    sim->roads[i]->cars = new car*[sim->roads[i]->length+2];
    sim->roads[i]->cars = sim->roads[i]->cars + 2;

    int position,tempTurn;
    int syncInt;
    fread (&syncInt, sizeof (int), 1, input);
    while (//fgetc (input) == 't'
	   syncInt == -127
	   ) {
      fread (&position, sizeof (int), 1, input); // position
      fread (&tempTurn, sizeof (int), 1, input);
      sim->roads[i]->cars[position] = new car (sim->roads[i], tempTurn, position);

      fread (&sim->roads[i]->cars[position]->wait, sizeof (int), 1, input);

      fread (&sim->roads[i]->cars[position]->color.r, sizeof (float), 1, input);
      fread (&sim->roads[i]->cars[position]->color.g, sizeof (float), 1, input);
      fread (&sim->roads[i]->cars[position]->color.b, sizeof (float), 1, input);
      
      fread (&sim->roads[i]->cars[position]->moved, sizeof (bool), 1, input );
      fread (&sim->roads[i]->cars[position]->sensed, sizeof (bool), 1, input);

      fread (&syncInt, sizeof (int) , 1, input);
    }

  }

  sim->intc = temp_intc;
  sim->roadc = temp_roadc;


}

