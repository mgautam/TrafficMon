#include "road.h"
#include "intersection.h"
#include "car.h"

#include <cstring>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define MAX_DEGREE 4

road::road(intersection* init, intersection* final)
{
  this->init = init;
  this->final = final;
  this->length = (final->x - init->x) + (final->y - init->y); //manhattan 
  if (this->length < 0) this->length *= -1;
  this->length -= 1;

  this->cars = new car*[length + 2] + 2;
  memset(this->cars, 0, length*sizeof(car*));

  this->lights[LEFT] = RED;
  this->lights[RIGHT] = RED;

  if (this->final->x < this->init->x)
    {
      this->compass = WEST;
      init->out[WEST] = this;// out to the west
      final->in[EAST] = this;// in from the east
    }
  else if (this->final->x > this->init->x)
    {
      this->compass = EAST;
      init->out[EAST] = this;
      final->in[WEST] = this;
    }
  else if (this->final->y < this->init->y)
    {
      this->compass = SOUTH;
      init->out[SOUTH] = this;
      final->in[NORTH] = this;
    }
  else if (this->final->y > this->init->y)
    {
      this->compass = NORTH;
      init->out[NORTH] = this;
      final->in[SOUTH] = this;
    }
  else
    {
      printf ("Error in Road Endpoints! They are invalid\n");
      exit (-1);
    }

  init->in_count++;
  final->out_count++;
}

road* road::get_left()
{
  return final->out[(compass+3)%4];
}

road* road::get_right()
{
  return final->out[(compass+1)%4];

}
road* road::get_ahead()
{
  return final->out[(compass+2)%4];
}

void road::write_state(FILE* output)
{
  fprintf(output, "Coordinates: %2d %2d %2d %2d   ", this->init->x, this->init->y, this->final->x, this->final->y);

  fprintf (output, "LEFT Light:%d ",lights[LEFT]);
  fprintf (output, "RIGHT Light:%d ",lights[RIGHT]);

  fprintf (output,"\n");
}


bool road::car_can_move_to(int new_pos)
{
  return true;
}

