#include <cstring>

#include "config.h"
#include "factory.h"

void factory::create_world(world** sim)
{
  /*
  intersection** intersections = new intersection*[5];
  intersections[0] =   new intersection (-NUM_SLOTS_IN_ROAD-2,0); // left
  intersections[1] =   new intersection (0,-NUM_SLOTS_IN_ROAD-2,0); // bottom
  intersections[2] =   new intersection (0,0); // center
  intersections[3] =   new intersection (0,NUM_SLOTS_IN_ROAD+2); // top
  //intersections[4] =   new intersection (NUM_SLOTS_IN_ROAD+2,0); // right
  int intersection_count = 4;


  road** roads = new road*[6];
  roads[0] = new road (intersections[1],intersections[2]);//NORTH
  roads[1] = new road (intersections[0],intersections[2]);//EAST
  roads[2] = new road (intersections[2],intersections[3]);//SOUTH
  //roads[3] = new road (intersections[2],intersections[4]);//WEST
  //roads[4] = new road (intersections[2],intersections[1]);
  //roads[5] = new road (intersections[2],intersections[0]);
  int road_count = 3;
       */



  
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

  cars[5] = new car(roads[3], AHEAD, (float)roads[0]->length - 1);
  cars[6] = new car(roads[3], RIGHT, (float)roads[0]->length - 3);
  cars[7] = new car(roads[3], LEFT, (float)roads[0]->length - 5);
  cars[8] = new car(roads[3], LEFT, (float)roads[0]->length - 7);
 
  new car(roads[20], LEFT, (float)roads[0]->length - 1);
  new car(roads[20], RIGHT, (float)roads[0]->length - 3);
  new car(roads[20], AHEAD, (float)roads[0]->length - 5);
  new car(roads[20], AHEAD, (float)roads[0]->length - 7);

  new car(roads[23], LEFT, (float)roads[0]->length - 1);
  new car(roads[23], RIGHT, (float)roads[0]->length - 3);
  new car(roads[23], AHEAD, (float)roads[0]->length - 5);
  new car(roads[23], AHEAD, (float)roads[0]->length - 7);

  // new car(roads[3], RIGHT, (float)roads[0]->length - 9);
  // new car(roads[3], RIGHT, (float)roads[0]->length - 9);
  // new car(roads[3], RIGHT, (float)roads[0]->length - 9);
  // new car(roads[3], RIGHT, (float)roads[0]->length - 9);
  // new car(roads[3], RIGHT, (float)roads[0]->length - 9);







  *sim = new world ( intersection_count, intersections, road_count, roads);

}
