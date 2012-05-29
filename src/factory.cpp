#include "factory.h"

void factory::create_world(world** sim)
{
  int SQUARE_SIZE = 2;
  int ROAD_LENGTH = 5;
  int FOUR = 4;

  intersection** intersections = new intersection*[SQUARE_SIZE*SQUARE_SIZE];
  road** roads = new road*[FOUR*(SQUARE_SIZE-1)*SQUARE_SIZE];
  for (int y = 0; y < SQUARE_SIZE; y++) {

    for (int x = 0; x < SQUARE_SIZE; x++)
      //TODO: we need a way of instantiating these by reading a file
      // Add 1 unit to the length of road in order to include size of intersection
      intersections[y*SQUARE_SIZE+x] = new intersection (-SQUARE_SIZE*(ROAD_LENGTH+1)/4+(ROAD_LENGTH+1)*x,-SQUARE_SIZE*(ROAD_LENGTH+1)/4+(ROAD_LENGTH+1)*y);

    for (int x = 0; x < SQUARE_SIZE-1; x++) {
      // Away from min x
      roads[FOUR*(y*(SQUARE_SIZE-1)+x)+0] = new road (intersections[y*SQUARE_SIZE+x],intersections[y*SQUARE_SIZE+(x+1)]);
      // Towards min x
      roads[FOUR*(y*(SQUARE_SIZE-1)+x)+1] = new road (intersections[y*SQUARE_SIZE+(x+1)],intersections[y*SQUARE_SIZE+x]);
    }
 }
  
  for (int x = 0; x < SQUARE_SIZE; x++) {
    for (int y = 0; y < SQUARE_SIZE-1; y++) {
      // Away from min y
      roads[FOUR*(y*(SQUARE_SIZE-1)+x)+2] = new road (intersections[y*SQUARE_SIZE+x],intersections[(y+1)*SQUARE_SIZE+x]);
      // Towards min y
      roads[FOUR*(y*(SQUARE_SIZE-1)+x)+3] = new road (intersections[(y+1)*SQUARE_SIZE+x],intersections[y*SQUARE_SIZE+x]);
    }
  }
  
  car** cars = new car*[5];
  cars[0] = new car(roads[0], LEFT); 
  cars[1] = new car(roads[1], RIGHT); 
  cars[2] = new car(roads[2], LEFT); 
  cars[3] = new car(roads[3], LEFT); 
  cars[4] = new car(roads[4], RIGHT);

 *sim = new world ( SQUARE_SIZE*SQUARE_SIZE, intersections, FOUR*(SQUARE_SIZE-1)*SQUARE_SIZE, roads, 5, cars);
}
