#include "common.h"
#include "intersection.h"

#include "car.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


//constructor
car::car (road* init_road, int next_turn) {
  this->setCar (init_road, next_turn, (float)init_road->length - MIN_INTER_CAR_SPACE);//
}

car::car (road* init_road, int next_turn, float _position) {
  this->setCar (init_road, next_turn, _position);
}

void car::setCar (road* init_road, int next_turn, float _position)
{
  this->curr_road = init_road;
  this->position = _position;
  init_road->cars[(int)this->position] = this;
  this->color.r = ((float)rand()/(float)RAND_MAX);
  this->color.g = ((float)rand()/(float)RAND_MAX);
  this->color.b = ((float)rand()/(float)RAND_MAX);

  this->turn = next_turn;
}

void car::write_state(FILE* output)
{
  fprintf (output, "Position:%f ",this->position);
  // fprintf(output, "%d %d\n", this->curr_road->init->x + this->displacement_x, this->curr_road->init->y + this->displacement_y);
  fprintf (output, "Compass: %d ",this->curr_road->compass);
  // fprintf (output,"%f %f %f", this->color.r, this->color.g, this->color.b);
  fprintf (output, "Next Turn: %d ",this->turn);
  fprintf (output, "\n");
}

bool car::can_move()
{
  if (this->position <= STEP_SIZE)//A very small number close to 0
    {
      if (this->turn == LEFT || this->turn == AHEAD)
	return (this->curr_road->lights[LEFT] == GREEN);
      else // this->turn == RIGHT
	return (this->curr_road->lights[RIGHT] == GREEN);
    }
  else if (this->position < 1) {
    return true; // Make the car move until the end of the road
  }
  else if (this->position < this->curr_road->length)
    {
      if (this->curr_road->cars[(int)this->position-1]) // if a car exists just before this car,
      {
	if (this->curr_road->cars[(int)this->position-1]->can_move()) // if the previous car can move,
	  return true;
	else if ((this->position - this->curr_road->cars[(int)this->position-1]->position)  > MIN_INTER_CAR_SPACE) 
	  // or if the gap between this car and previous car is more than minimum distance
	  return true;
	else 
	  return false;
      }
      else
	return true;
   }
  else
    {
      printf ("Error in can move routine in car.h");
      exit (-1);
    }
}

void car::move()
{
  // If the car is at position 0, it has to make turn
  if (this->position <= STEP_SIZE)
    return;

  if (!can_move())
    {
      this->wait++;
    }
  else
    {
      this->wait = 0;
      this->curr_road->cars[(int)this->position] = 0;
      this->position -= STEP_SIZE;
      this->curr_road->cars[(int)this->position] = this;
    }
}

void car::make_turn()
{
  // If the car is not at position 0, it has to move
   if (this->position > STEP_SIZE) {
    return;
   }

  if (!can_move())
    {
      this->wait++;
    }
  else
    {
      this->wait = 0;

      this->curr_road->cars[(int)this->position] = 0;

      road *next_road;

      if (this->turn == LEFT)
	next_road = this->curr_road->get_left();
      else if (this->turn == RIGHT)
	next_road = this->curr_road->get_right();
      else if (this->turn == AHEAD)
	next_road = this->curr_road->get_ahead();

      if (next_road) {
	curr_road->cars[(int)this->position] = 0;
	this->position = next_road->length-MIN_INTER_CAR_SPACE;//why doesn't it work when MIN_INTER_CAR_SPACE is 0.5
	next_road->cars[(int)this->position] = this;
	this->curr_road = next_road;
      }
      else {
	// Trash the car
	//	car *car_ptr = curr_road->cars[(int)this->position];
	delete curr_road->cars[(int)this->position];
	curr_road->cars[(int)this->position] = 0;
	//	delete car_ptr;

	this->position = -1;
	this->curr_road = 0;	
      }
    }
}



