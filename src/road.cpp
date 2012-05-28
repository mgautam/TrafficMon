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

    this->cars = new car*[length];
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


void road::viewRoad (float scale) {
    glColor3f (0.0f, 0.0f, 1.0f);
    glBegin (GL_QUADS);

    float lenBWlights = 0.05;
    float roadSideOffset = 0.5;
    float intersectionOffset = 0.5;

    switch (compass) {

      case NORTH:
	glVertex2f (((float)this->init->x - lenBWlights)*scale, ((float)this->init->y + intersectionOffset)*scale);
	glVertex2f (((float)this->init->x - lenBWlights)*scale, ((float)this->final->y - intersectionOffset)*scale);
	glVertex2f (((float)this->init->x - roadSideOffset)*scale, ((float)this->final->y - intersectionOffset)*scale);
	glVertex2f (((float)this->init->x - roadSideOffset)*scale, ((float)this->init->y + intersectionOffset)*scale);
      break;

      case EAST:
	glVertex2f (((float)this->init->x + intersectionOffset)*scale,((float)this->init->y + roadSideOffset)*scale);
	glVertex2f (((float)this->final->x - intersectionOffset)*scale,((float)this->init->y + roadSideOffset)*scale);
	glVertex2f (((float)this->final->x - intersectionOffset)*scale,((float)this->init->y + lenBWlights)*scale);
	glVertex2f (((float)this->init->x + intersectionOffset)*scale,((float)this->init->y + lenBWlights)*scale);
      break;

      case SOUTH:
	glVertex2f (((float)this->init->x + roadSideOffset)*scale, ((float)this->init->y - intersectionOffset)*scale);
	glVertex2f (((float)this->init->x + roadSideOffset)*scale, ((float)this->final->y + intersectionOffset)*scale);
	glVertex2f (((float)this->init->x + lenBWlights)*scale, ((float)this->final->y + intersectionOffset)*scale);
	glVertex2f (((float)this->init->x + lenBWlights)*scale, ((float)this->init->y - intersectionOffset)*scale);
      break;

      case WEST:
	glVertex2f (((float)this->init->x - intersectionOffset)*scale,((float)this->init->y - lenBWlights)*scale);
	glVertex2f (((float)this->final->x + intersectionOffset)*scale,((float)this->init->y - lenBWlights)*scale);
	glVertex2f (((float)this->final->x + intersectionOffset)*scale,((float)this->init->y - roadSideOffset)*scale);
	glVertex2f (((float)this->init->x - intersectionOffset)*scale,((float)this->init->y - roadSideOffset)*scale);
      break;

      default:
      printf ("Error in Road Endpoints! They are invalid\n");
      exit (-1);
    }

    glEnd ();
  }

void road::viewCars (float scale) {
  for (int i = 0; i < length; i++)
    {
      if (cars[i])
	cars[i]->viewCar(scale);
    }
}

void road::viewLights (float scale) {
    float lenBWlights = 0.05;
    float roadSideOffset = 0.5;
    float intersectionOffset = 0.5;
    float LightSize = 0.5;

    //lights[LEFT][GREEN] = true; // Debugging purpose only
    //lights[RIGHT][RED] = true; // Debugging purpose only


    for ( int i = 0; i <=1; i++) {

      for (int j = 0; j < 3; j++) {
	
	glColor3f (1.0f*(float)(j<=AMBER),1.0f*(float)(j>=AMBER),0.0f);
	//glColor3i (255,0,0);

	if (this->lights[i] == j) glBegin (GL_QUADS);
	else glBegin (GL_LINE_LOOP);

	switch (compass) {

	case NORTH:
	  glVertex2f (((float)this->init->x - (roadSideOffset + i*LightSize))*scale, 
		      ((float)this->final->y - (intersectionOffset + (j+1)*LightSize))*scale);
	  glVertex2f (((float)this->init->x - (roadSideOffset + i*LightSize))*scale, 
		      ((float)this->final->y - (intersectionOffset + j * LightSize))*scale);
	  glVertex2f (((float)this->init->x - (roadSideOffset + (i+1)*LightSize - lenBWlights))*scale, 
		      ((float)this->final->y - (intersectionOffset + j * LightSize))*scale);
	  glVertex2f (((float)this->init->x - (roadSideOffset + (i+1)*LightSize - lenBWlights))*scale, 
		      ((float)this->final->y - (intersectionOffset + (j+1)*LightSize))*scale);
	  break;

	case EAST:
	  glVertex2f (((float)this->final->x - (intersectionOffset + (j+1)*LightSize))*scale,
		      ((float)this->init->y + (roadSideOffset + i*LightSize))*scale);
	  glVertex2f (((float)this->final->x - (intersectionOffset + j * LightSize))*scale,
		      ((float)this->init->y + (roadSideOffset + i*LightSize))*scale);
	  glVertex2f (((float)this->final->x - (intersectionOffset + j * LightSize))*scale,
		      ((float)this->init->y + (roadSideOffset + (i+1)*LightSize - lenBWlights))*scale);
	  glVertex2f (((float)this->final->x - (intersectionOffset + (j+1) * LightSize))*scale,
		      ((float)this->init->y + (roadSideOffset + (i+1)*LightSize - lenBWlights))*scale);
	  break;

	case SOUTH:
	  glVertex2f (((float)this->init->x + (roadSideOffset + i*LightSize))*scale, 
		      ((float)this->final->y + (intersectionOffset + (j+1)*LightSize))*scale);
	  glVertex2f (((float)this->init->x + (roadSideOffset + i*LightSize))*scale, 
		      ((float)this->final->y + (intersectionOffset + j * LightSize))*scale);
	  glVertex2f (((float)this->init->x + (roadSideOffset + (i+1)*LightSize - lenBWlights))*scale, 
		      ((float)this->final->y + (intersectionOffset + j * LightSize))*scale);
	  glVertex2f (((float)this->init->x + (roadSideOffset + (i+1)*LightSize - lenBWlights))*scale, 
		      ((float)this->final->y + (intersectionOffset + (j+1)*LightSize))*scale);
	  break;

	case WEST:
	  glVertex2f (((float)this->final->x + (intersectionOffset + (j+1)*LightSize))*scale,
		      ((float)this->init->y - (roadSideOffset + i*LightSize))*scale);
	  glVertex2f (((float)this->final->x + (intersectionOffset + j * LightSize))*scale,
		      ((float)this->init->y - (roadSideOffset + i*LightSize))*scale);
	  glVertex2f (((float)this->final->x + (intersectionOffset + j * LightSize))*scale,
		      ((float)this->init->y - (roadSideOffset + (i+1)*LightSize - lenBWlights))*scale);
	  glVertex2f (((float)this->final->x + (intersectionOffset + (j+1) * LightSize))*scale,
		      ((float)this->init->y - (roadSideOffset + (i+1)*LightSize - lenBWlights))*scale);
	  break;

	default:
	  printf ("Error in Road Endpoints! They are invalid\n");
	  exit (-1);
	}

	glEnd ();
      }
    }
  }


bool road::car_can_move_to(int new_pos)
  {
    return true;
  }

