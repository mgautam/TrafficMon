#include "common.h"
#include "intersection.h"

#include "car.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif




  //constructor
car::car (road* init_road)
  {
    this->curr_road = init_road;
    this->position = init_road->length-1;
    init_road->cars[this->position] = this;
    this->color.r = ((float)rand()/(float)RAND_MAX);
    this->color.g = ((float)rand()/(float)RAND_MAX);
    this->color.b = ((float)rand()/(float)RAND_MAX);

    this->turn =LEFT;
  }

  void car::write_state(FILE* output)
  {
    fprintf (output, "Position:%d ",this->position);
    // fprintf(output, "%d %d\n", this->curr_road->init->x + this->displacement_x, this->curr_road->init->y + this->displacement_y);
    fprintf (output, "Compass: %d ",this->curr_road->compass);
    // fprintf (output,"%f %f %f", this->color.r, this->color.g, this->color.b);
    fprintf (output, "Next Turn: %d ",this->turn);
    fprintf (output, "\n");
  }

  bool car::can_move()
  {
    if (this->position == 0)
      {
	if (this->turn == LEFT || this->turn == AHEAD)
	  return (this->curr_road->lights[LEFT] == GREEN);
	else // this->turn == RIGHT
	  return (this->curr_road->lights[RIGHT] == GREEN);
      }
    else if (this->position < this->curr_road->length)
      {
	return !this->curr_road->cars[this->position-1] ||
	  this->curr_road->cars[this->position-1]->can_move();
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
    if (!this->position)
      return;

    if (!can_move())
      {
	this->wait++;
      }
    else
      {
	this->wait = 0;
	this->curr_road->cars[this->position] = 0;
	this->curr_road->cars[--this->position] = this;
      }
  }

  void car::make_turn()
  {
    // If the car is not at position 0, it has to move
    if (this->position)
      return;

    if (!can_move())
      {
	this->wait++;
      }
    else
      {
	this->wait = 0;

	this->curr_road->cars[this->position] = 0;

	road *next_road;

	if (this->turn == LEFT)
	    next_road = this->curr_road->get_left();
	else if (this->turn == RIGHT)
	    next_road = this->curr_road->get_right();
	else if (this->turn == AHEAD)
	    next_road = this->curr_road->get_ahead();

	if (next_road) {
	  curr_road->cars[position] = 0;
	  this->position = next_road->length-1;
	  next_road->cars[this->position] = this;
	}
	else {
	  curr_road->cars[position] = 0;
	  this->position = -1;
	}
      }
  }

  void car::viewCar (float scale) {

    // The first 0.5 is to include the offset of intersection to road
    // -1 coz position starts from 1 not 0. 
    // The last 0.5 as offset for between car padding
    float roadOffset =  (0.5 + (float)this->position+ 0.5);
    float halfCarLen = 0.25;

    float lenBWlanes = 0.05;

    glColor3f (this->color.r,this->color.g,this->color.b);
    glBegin (GL_QUADS);

    switch (this->curr_road->compass) {
      
      case NORTH:
	glVertex2f (((float)this->curr_road->final->x - lenBWlanes)*scale, ((float)this->curr_road->final->y - (roadOffset + halfCarLen))*scale);
	glVertex2f (((float)this->curr_road->final->x - lenBWlanes)*scale, ((float)this->curr_road->final->y - (roadOffset - halfCarLen))*scale);
	glVertex2f (((float)this->curr_road->final->x - 0.5)*scale, ((float)this->curr_road->final->y - (roadOffset - halfCarLen))*scale);
	glVertex2f (((float)this->curr_road->final->x - 0.5)*scale, ((float)this->curr_road->final->y - (roadOffset + halfCarLen))*scale);
	break;
    
      case EAST:
	glVertex2f (((float)this->curr_road->final->x - (roadOffset + halfCarLen))*scale ,((float)this->curr_road->final->y + lenBWlanes)*scale);
	glVertex2f (((float)this->curr_road->final->x - (roadOffset - halfCarLen))*scale ,((float)this->curr_road->final->y + lenBWlanes)*scale);
	glVertex2f (((float)this->curr_road->final->x - (roadOffset - halfCarLen))*scale ,((float)this->curr_road->final->y + 0.5)*scale);
	glVertex2f (((float)this->curr_road->final->x - (roadOffset + halfCarLen))*scale ,((float)this->curr_road->final->y + 0.5)*scale);
	break;

      case SOUTH:
	glVertex2f (((float)this->curr_road->final->x + lenBWlanes)*scale, ((float)this->curr_road->final->y + (roadOffset + halfCarLen))*scale);
	glVertex2f (((float)this->curr_road->final->x + lenBWlanes)*scale, ((float)this->curr_road->final->y + (roadOffset - halfCarLen))*scale);
	glVertex2f (((float)this->curr_road->final->x + 0.5)*scale, ((float)this->curr_road->final->y + (roadOffset - halfCarLen))*scale);
	glVertex2f (((float)this->curr_road->final->x + 0.5)*scale, ((float)this->curr_road->final->y + (roadOffset + halfCarLen))*scale);
	break;

      case WEST:
	glVertex2f (((float)this->curr_road->final->x + (roadOffset + halfCarLen))*scale ,((float)this->curr_road->final->y - lenBWlanes)*scale);
	glVertex2f (((float)this->curr_road->final->x + (roadOffset - halfCarLen))*scale ,((float)this->curr_road->final->y - lenBWlanes)*scale);
	glVertex2f (((float)this->curr_road->final->x + (roadOffset - halfCarLen))*scale ,((float)this->curr_road->final->y - 0.5)*scale);
	glVertex2f (((float)this->curr_road->final->x + (roadOffset + halfCarLen))*scale ,((float)this->curr_road->final->y - 0.5)*scale);
	break;
    
      default:
	printf ("Error in Road Endpoints! They are invalid\n");
	exit (-1);
    }
    glEnd ();
  }


