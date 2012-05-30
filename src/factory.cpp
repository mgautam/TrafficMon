#include "factory.h"

void factory::create_world(world** sim)
{
  int square_size = 3;
  int road_length = 4;
  int road_count = 2*2*(square_size-1)*square_size;

  intersection** intersections = new intersection*[square_size*square_size+4];
  road** roads = new road*[road_count+4];

  for (int y = 0; y < square_size; y++)
    {
      for (int x = 0; x < square_size; x++)
	{
	  // Add 2 unit to the length of road in order to include size of intersection
	  // intersections[y*square_size+x] = new intersection((2*x - (square_size-1))*(road_length+1), (2*y - (square_size-1))*(road_length+1));
	  intersections[y*square_size+x] = new intersection((road_length+2)*x, (road_length+2)*y);
	}
    }

  for (int y = 0; y < square_size; y++)
    {
      for (int x = 0; x < square_size - 1; x++)
	{
	  roads[y*(square_size-1)+x] = new road(intersections[y*square_size+x], intersections[y*square_size + (x+1)]);
	  roads[road_count/4 + y*(square_size-1)+x] = new road(intersections[y*square_size + (x+1)], intersections[y*square_size + x]);
	}
    }

  for (int x = 0; x < square_size; x++)
    {
      for (int y = 0; y < square_size - 1; y++)
	{
	  roads[road_count/2 + x*(square_size-1)+y] = new road(intersections[y*square_size+x], intersections[(y+1)*square_size + x]);
	  roads[road_count/4*3 + x*(square_size-1)+y] = new road(intersections[(y+1)*square_size+x], intersections[y*square_size + x]);
	}
    }

  car** cars = new car*[5];
  cars[0] = new car(roads[0], LEFT, roads[0]->length - 1); 
  cars[1] = new car(roads[0], LEFT, (float)roads[0]->length - 2);
  cars[2] = new car(roads[road_count/4+0], RIGHT, (float)roads[0]->length - 2); 
  // cars[2] = new car(roads[1*5+3], LEFT, (float)roads[1*5+2]->length - 3.3); 
  // cars[3] = new car(roads[4*5+3], LEFT, (float)roads[4*5+2]->length - 4.4);  
  // cars[4] = new car(roads[3*5+3], LEFT, (float)roads[3*5+2]->length - 5.5);
  

  //*sim = new world ( square_size*square_size, intersections, road_count, roads, 1, cars);
  *sim = new world ( square_size*square_size, intersections, road_count, roads);

}