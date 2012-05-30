#include "common.h"
#include "visualize.h"
#include "painter.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

void (*painter::display) (void) = NULL;
void (*painter::timerCallback) (int) = NULL;

painter::painter(world* _simulation, void (*_display) (void), void (*_timerCallback) (int), int argc, char** argv) {

  this->simulation = _simulation;
  this->display = _display;
  this->timerCallback = _timerCallback;
  
  glutInit (&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize (WINDOW_WIDTH,WINDOW_HEIGHT);

  glutCreateWindow ("World View");
  glDisable (GL_DEPTH_TEST);
  glutDisplayFunc (display);
  glutKeyboardFunc (handleKeyPress);
  glutReshapeFunc (handleResize);

  //  glClearColor (1.0f,1.0f,1.0f,0.0f);

  glMatrixMode (GL_PROJECTION);
  glPushMatrix ();

  glutTimerFunc (simulation_interval, timerCallback, 0);
}

void painter::animate()
{ 
  glutMainLoop();
}

void painter::draw () {

  float scale = simulation->scale;
  
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
 
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();

  glScalef (0.9f,0.9f,1.0f);
  glTranslatef (-0.6875,-0.6875,0);//(float)(simulation->maxWorldX-simulation->minWorldX),
  //		 (float)(simulation->maxWorldY-simulation->minWorldY),
  //		 0);
  
  for (int i = 0; i < simulation->intc; i++)
    draw(simulation->intersections[i], scale);
  
  for (int i = 0; i < simulation->roadc; i++) {
    draw(simulation->roads[i], scale);
    drawLights(simulation->roads[i], scale);

    for (int j = -2; j < simulation->roads[i]->length; j++)
      {
	if (simulation->roads[i]->cars[j])
	  draw(simulation->roads[i]->cars[j], scale);
      }
  }
  
  // for (int i = 0; i < simulation->carc; i++)
  //   simulation->cars[i]->viewCar (simulation->scale);
  
  glutSwapBuffers ();
}





void painter::draw (car* curr_car, float scale) {

  if (!curr_car)
    return;

  // The first 0.5 is to include the offset of intersection to road
  // -1 coz position starts from 1 not 0. 
  // The last 0.5 as offset for between car padding
  float carOffset =  (0.5 + curr_car->position + 1);


  glColor3f (curr_car->color.r,curr_car->color.g,curr_car->color.b);
  glBegin (GL_QUADS);

  switch (curr_car->curr_road->compass) {
      
  case NORTH:
    glVertex2f (((float)curr_car->curr_road->final->x - lenBWlanes)*scale, ((float)curr_car->curr_road->final->y - (carOffset + halfCarLen))*scale);
    glVertex2f (((float)curr_car->curr_road->final->x - lenBWlanes)*scale, ((float)curr_car->curr_road->final->y - (carOffset - halfCarLen))*scale);
    glVertex2f (((float)curr_car->curr_road->final->x - roadSideOffset)*scale, ((float)curr_car->curr_road->final->y - (carOffset - halfCarLen))*scale);
    glVertex2f (((float)curr_car->curr_road->final->x - roadSideOffset)*scale, ((float)curr_car->curr_road->final->y - (carOffset + halfCarLen))*scale);
    break;
    
  case EAST:
    glVertex2f (((float)curr_car->curr_road->final->x - (carOffset + halfCarLen))*scale ,((float)curr_car->curr_road->final->y + lenBWlanes)*scale);
    glVertex2f (((float)curr_car->curr_road->final->x - (carOffset - halfCarLen))*scale ,((float)curr_car->curr_road->final->y + lenBWlanes)*scale);
    glVertex2f (((float)curr_car->curr_road->final->x - (carOffset - halfCarLen))*scale ,((float)curr_car->curr_road->final->y + roadSideOffset)*scale);
    glVertex2f (((float)curr_car->curr_road->final->x - (carOffset + halfCarLen))*scale ,((float)curr_car->curr_road->final->y + roadSideOffset)*scale);
    break;

  case SOUTH:
    glVertex2f (((float)curr_car->curr_road->final->x + lenBWlanes)*scale, ((float)curr_car->curr_road->final->y + (carOffset + halfCarLen))*scale);
    glVertex2f (((float)curr_car->curr_road->final->x + lenBWlanes)*scale, ((float)curr_car->curr_road->final->y + (carOffset - halfCarLen))*scale);
    glVertex2f (((float)curr_car->curr_road->final->x + roadSideOffset)*scale, ((float)curr_car->curr_road->final->y + (carOffset - halfCarLen))*scale);
    glVertex2f (((float)curr_car->curr_road->final->x + roadSideOffset)*scale, ((float)curr_car->curr_road->final->y + (carOffset + halfCarLen))*scale);
    break;

  case WEST:
    glVertex2f (((float)curr_car->curr_road->final->x + (carOffset + halfCarLen))*scale ,((float)curr_car->curr_road->final->y - lenBWlanes)*scale);
    glVertex2f (((float)curr_car->curr_road->final->x + (carOffset - halfCarLen))*scale ,((float)curr_car->curr_road->final->y - lenBWlanes)*scale);
    glVertex2f (((float)curr_car->curr_road->final->x + (carOffset - halfCarLen))*scale ,((float)curr_car->curr_road->final->y - roadSideOffset)*scale);
    glVertex2f (((float)curr_car->curr_road->final->x + (carOffset + halfCarLen))*scale ,((float)curr_car->curr_road->final->y - roadSideOffset)*scale);
    break;
    
  default:
    printf ("Error in Road Endpoints! They are invalid\n");
    exit (-1);
  }
  glEnd ();
}


void painter::draw (intersection* curr_intersection, float scale) {


   if (!curr_intersection)
      return;

   glColor3f (0.5f, 0.5f, 0.5f);
   glBegin (GL_QUADS);
   glVertex2f (((float) curr_intersection->x - halfIntersectionLen) * scale, ((float)curr_intersection->y + halfIntersectionLen)* scale);
   glVertex2f (((float) curr_intersection->x + halfIntersectionLen) * scale, ((float)curr_intersection->y + halfIntersectionLen)* scale);
   glVertex2f (((float) curr_intersection->x + halfIntersectionLen) * scale, ((float)curr_intersection->y - halfIntersectionLen)* scale);
   glVertex2f (((float) curr_intersection->x - halfIntersectionLen) * scale, ((float)curr_intersection->y - halfIntersectionLen)* scale);
   glEnd ();

}

void painter::draw (road* curr_road, float scale) {

  if (!curr_road)
    return;

  glColor3f (0.5f, 0.5f, 0.5f);
  glBegin (GL_QUADS);

  switch (curr_road->compass) {

  case NORTH:
    glVertex2f (((float)curr_road->init->x - lenBWlanes)*scale, ((float)curr_road->init->y + halfIntersectionLen)*scale);
    glVertex2f (((float)curr_road->init->x - lenBWlanes)*scale, ((float)curr_road->final->y - halfIntersectionLen)*scale);
    glVertex2f (((float)curr_road->init->x - roadSideOffset)*scale, ((float)curr_road->final->y - halfIntersectionLen)*scale);
    glVertex2f (((float)curr_road->init->x - roadSideOffset)*scale, ((float)curr_road->init->y + halfIntersectionLen)*scale);
    break;

  case EAST:
    glVertex2f (((float)curr_road->init->x + halfIntersectionLen)*scale,((float)curr_road->init->y + roadSideOffset)*scale);
    glVertex2f (((float)curr_road->final->x - halfIntersectionLen)*scale,((float)curr_road->init->y + roadSideOffset)*scale);
    glVertex2f (((float)curr_road->final->x - halfIntersectionLen)*scale,((float)curr_road->init->y + lenBWlanes)*scale);
    glVertex2f (((float)curr_road->init->x + halfIntersectionLen)*scale,((float)curr_road->init->y + lenBWlanes)*scale);
    break;

  case SOUTH:
    glVertex2f (((float)curr_road->init->x + roadSideOffset)*scale, ((float)curr_road->init->y - halfIntersectionLen)*scale);
    glVertex2f (((float)curr_road->init->x + roadSideOffset)*scale, ((float)curr_road->final->y + halfIntersectionLen)*scale);
    glVertex2f (((float)curr_road->init->x + lenBWlanes)*scale, ((float)curr_road->final->y + halfIntersectionLen)*scale);
    glVertex2f (((float)curr_road->init->x + lenBWlanes)*scale, ((float)curr_road->init->y - halfIntersectionLen)*scale);
    break;

  case WEST:
    glVertex2f (((float)curr_road->init->x - halfIntersectionLen)*scale,((float)curr_road->init->y - lenBWlanes)*scale);
    glVertex2f (((float)curr_road->final->x + halfIntersectionLen)*scale,((float)curr_road->init->y - lenBWlanes)*scale);
    glVertex2f (((float)curr_road->final->x + halfIntersectionLen)*scale,((float)curr_road->init->y - roadSideOffset)*scale);
    glVertex2f (((float)curr_road->init->x - halfIntersectionLen)*scale,((float)curr_road->init->y - roadSideOffset)*scale);
    break;

  default:
    printf ("Error in Road Endpoints! They are invalid\n");
    exit (-1);
  }

  glEnd ();
}

void painter::drawLights (road* curr_road, float scale) {

  //lights[LEFT][GREEN] = true; // Debugging purpose only
  //lights[RIGHT][RED] = true; // Debugging purpose only


  for ( int i = 0; i <=1; i++) {

    for (int j = 0; j < 3; j++) {
	
      glColor3f (1.0f*(float)(j<=AMBER),1.0f*(float)(j>=AMBER),0.0f);
      //glColor3i (255,0,0);

      
      glBegin (GL_LINE_LOOP);

     switch (curr_road->compass) {

      case NORTH:
	glVertex2f (((float)curr_road->init->x - (roadSideOffset + i*LightSize))*scale, 
		    ((float)curr_road->final->y - (halfIntersectionLen + (j+1)*LightSize))*scale);
	glVertex2f (((float)curr_road->init->x - (roadSideOffset + i*LightSize))*scale, 
		    ((float)curr_road->final->y - (halfIntersectionLen + j * LightSize))*scale);
	glVertex2f (((float)curr_road->init->x - (roadSideOffset + (i+1)*LightSize - lenBWlights))*scale, 
		    ((float)curr_road->final->y - (halfIntersectionLen + j * LightSize))*scale);
	glVertex2f (((float)curr_road->init->x - (roadSideOffset + (i+1)*LightSize - lenBWlights))*scale, 
		    ((float)curr_road->final->y - (halfIntersectionLen + (j+1)*LightSize))*scale);
	break;

      case EAST:
	glVertex2f (((float)curr_road->final->x - (halfIntersectionLen + (j+1)*LightSize))*scale,
		    ((float)curr_road->init->y + (roadSideOffset + i*LightSize))*scale);
	glVertex2f (((float)curr_road->final->x - (halfIntersectionLen + j * LightSize))*scale,
		    ((float)curr_road->init->y + (roadSideOffset + i*LightSize))*scale);
	glVertex2f (((float)curr_road->final->x - (halfIntersectionLen + j * LightSize))*scale,
		    ((float)curr_road->init->y + (roadSideOffset + (i+1)*LightSize - lenBWlights))*scale);
	glVertex2f (((float)curr_road->final->x - (halfIntersectionLen + (j+1) * LightSize))*scale,
		    ((float)curr_road->init->y + (roadSideOffset + (i+1)*LightSize - lenBWlights))*scale);
	break;

      case SOUTH:
	glVertex2f (((float)curr_road->init->x + (roadSideOffset + i*LightSize))*scale, 
		    ((float)curr_road->final->y + (halfIntersectionLen + (j+1)*LightSize))*scale);
	glVertex2f (((float)curr_road->init->x + (roadSideOffset + i*LightSize))*scale, 
		    ((float)curr_road->final->y + (halfIntersectionLen + j * LightSize))*scale);
	glVertex2f (((float)curr_road->init->x + (roadSideOffset + (i+1)*LightSize - lenBWlights))*scale, 
		    ((float)curr_road->final->y + (halfIntersectionLen + j * LightSize))*scale);
	glVertex2f (((float)curr_road->init->x + (roadSideOffset + (i+1)*LightSize - lenBWlights))*scale, 
		    ((float)curr_road->final->y + (halfIntersectionLen + (j+1)*LightSize))*scale);
	break;

      case WEST:
	glVertex2f (((float)curr_road->final->x + (halfIntersectionLen + (j+1)*LightSize))*scale,
		    ((float)curr_road->init->y - (roadSideOffset + i*LightSize))*scale);
	glVertex2f (((float)curr_road->final->x + (halfIntersectionLen + j * LightSize))*scale,
		    ((float)curr_road->init->y - (roadSideOffset + i*LightSize))*scale);
	glVertex2f (((float)curr_road->final->x + (halfIntersectionLen + j * LightSize))*scale,
		    ((float)curr_road->init->y - (roadSideOffset + (i+1)*LightSize - lenBWlights))*scale);
	glVertex2f (((float)curr_road->final->x + (halfIntersectionLen + (j+1) * LightSize))*scale,
		    ((float)curr_road->init->y - (roadSideOffset + (i+1)*LightSize - lenBWlights))*scale);
	break;

      default:
	printf ("Error in Road Endpoints! They are invalid\n");
	exit (-1);
      }

      glEnd ();

    }

    
    int j = curr_road->lights[i];
     glColor3f (1.0f*(float)(j<=AMBER),1.0f*(float)(j>=AMBER),0.0f);
     glBegin (GL_QUADS);

     switch (curr_road->compass) {

      case NORTH:
	glVertex2f (((float)curr_road->init->x - (roadSideOffset + i*LightSize))*scale, 
		    ((float)curr_road->final->y - (halfIntersectionLen + (j+1)*LightSize))*scale);
	glVertex2f (((float)curr_road->init->x - (roadSideOffset + i*LightSize))*scale, 
		    ((float)curr_road->final->y - (halfIntersectionLen + j * LightSize))*scale);
	glVertex2f (((float)curr_road->init->x - (roadSideOffset + (i+1)*LightSize - lenBWlights))*scale, 
		    ((float)curr_road->final->y - (halfIntersectionLen + j * LightSize))*scale);
	glVertex2f (((float)curr_road->init->x - (roadSideOffset + (i+1)*LightSize - lenBWlights))*scale, 
		    ((float)curr_road->final->y - (halfIntersectionLen + (j+1)*LightSize))*scale);
	break;

      case EAST:
	glVertex2f (((float)curr_road->final->x - (halfIntersectionLen + (j+1)*LightSize))*scale,
		    ((float)curr_road->init->y + (roadSideOffset + i*LightSize))*scale);
	glVertex2f (((float)curr_road->final->x - (halfIntersectionLen + j * LightSize))*scale,
		    ((float)curr_road->init->y + (roadSideOffset + i*LightSize))*scale);
	glVertex2f (((float)curr_road->final->x - (halfIntersectionLen + j * LightSize))*scale,
		    ((float)curr_road->init->y + (roadSideOffset + (i+1)*LightSize - lenBWlights))*scale);
	glVertex2f (((float)curr_road->final->x - (halfIntersectionLen + (j+1) * LightSize))*scale,
		    ((float)curr_road->init->y + (roadSideOffset + (i+1)*LightSize - lenBWlights))*scale);
	break;

      case SOUTH:
	glVertex2f (((float)curr_road->init->x + (roadSideOffset + i*LightSize))*scale, 
		    ((float)curr_road->final->y + (halfIntersectionLen + (j+1)*LightSize))*scale);
	glVertex2f (((float)curr_road->init->x + (roadSideOffset + i*LightSize))*scale, 
		    ((float)curr_road->final->y + (halfIntersectionLen + j * LightSize))*scale);
	glVertex2f (((float)curr_road->init->x + (roadSideOffset + (i+1)*LightSize - lenBWlights))*scale, 
		    ((float)curr_road->final->y + (halfIntersectionLen + j * LightSize))*scale);
	glVertex2f (((float)curr_road->init->x + (roadSideOffset + (i+1)*LightSize - lenBWlights))*scale, 
		    ((float)curr_road->final->y + (halfIntersectionLen + (j+1)*LightSize))*scale);
	break;

      case WEST:
	glVertex2f (((float)curr_road->final->x + (halfIntersectionLen + (j+1)*LightSize))*scale,
		    ((float)curr_road->init->y - (roadSideOffset + i*LightSize))*scale);
	glVertex2f (((float)curr_road->final->x + (halfIntersectionLen + j * LightSize))*scale,
		    ((float)curr_road->init->y - (roadSideOffset + i*LightSize))*scale);
	glVertex2f (((float)curr_road->final->x + (halfIntersectionLen + j * LightSize))*scale,
		    ((float)curr_road->init->y - (roadSideOffset + (i+1)*LightSize - lenBWlights))*scale);
	glVertex2f (((float)curr_road->final->x + (halfIntersectionLen + (j+1) * LightSize))*scale,
		    ((float)curr_road->init->y - (roadSideOffset + (i+1)*LightSize - lenBWlights))*scale);
	break;

      default:
	printf ("Error in Road Endpoints! They are invalid\n");
	exit (-1);
      }

      glEnd ();
  }

}
