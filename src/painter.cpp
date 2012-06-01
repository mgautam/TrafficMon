#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "config.h"
#include "input_device_handler.h"
#include "painter.h"

void (*painter::display) (void) = NULL;
void (*painter::timerCallback) (int) = NULL;

painter::painter(world* _simulation, void (*_display) (void), void (*_timerCallback) (int), int argc, char** argv) {

  this->simulation = _simulation;
  this->display = _display;
  this->timerCallback = _timerCallback;

  // Calculating world scale to map to our windowed perception of the world
  int minWorldX = 0;
  int minWorldY = 0;
  int maxWorldX = 0;
  int maxWorldY = 0;
  for (int i = 0; i < _simulation->intc; i++) {
    if (!_simulation->intersections[i])
      continue;

    if (maxWorldX < _simulation->intersections[i]->x)
      maxWorldX = _simulation->intersections[i]->x;
    if (minWorldX > _simulation->intersections[i]->x)
      minWorldX = _simulation->intersections[i]->x;

    if (maxWorldY < _simulation->intersections[i]->y)
      maxWorldY = _simulation->intersections[i]->y;
    if (minWorldY > _simulation->intersections[i]->y)
      minWorldY = _simulation->intersections[i]->y;
  }

  this->scale = (maxWorldX - minWorldX) > (maxWorldY - minWorldY)?
    1.5/((float)(maxWorldX - minWorldX + MARGIN_PADDING))  :  1.5/((float)(maxWorldY - minWorldY + MARGIN_PADDING));
  //printf ("minX = %d, minY = %d \t maxX = %d, maxY = %d", minWorldX, minWorldY, maxWorldX, maxWorldY);
  //Calculating scale ends here
  
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

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
 
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();

  glScalef (0.125f,0.125f,1.0f);
  glTranslatef (-7.5,-7.5,0);
  
  for (int i = 0; i < simulation->intc; i++)
    draw(simulation->intersections[i]);
  
  for (int i = 0; i < simulation->roadc; i++) {
    draw(simulation->roads[i]);
    drawLights(simulation->roads[i]);

    for (int j = -2; j < simulation->roads[i]->length; j++)
      {
	if (simulation->roads[i]->cars[j])
	  draw(simulation->roads[i]->cars[j]);
      }
  }
  
  glutSwapBuffers ();
}





void painter::draw (car* curr_car) {

  if (!curr_car)
    return;

  // The first 0.5 is to include the offset of intersection to road
  // -1 coz position starts from 1 not 0. 
  // The last 0.5 as offset for between car padding
  //  float carOffset_len =  (INTERSECTION_SIZE/2 + 7.5*(float)curr_car->position/(float)(CAR_LENGTH+MIN_INTER_CAR_SPACE) + HCAR_LENGTH);//*(CAR_LENGTH+MIN_INTER_CAR_SPACE)
  float carOffset_len =  (INTERSECTION_SIZE/2 +  ROAD_SCALE*curr_car->position + HCAR_LENGTH);//*(CAR_LENGTH+MIN_INTER_CAR_SPACE)
  // offset of car along the length of the road from final intersection
  float carOffset_wid = ROAD_WIDTH/2 + HLEN_BW_LANES;

  glColor3f (curr_car->color.r,curr_car->color.g,curr_car->color.b);
  glBegin (GL_QUADS);

  switch (curr_car->curr_road->compass) {
      
  case NORTH:
    glVertex2f (((float)curr_car->curr_road->final->x*ROAD_SCALE - (carOffset_wid-HCAR_WIDTH))*scale, ((float)curr_car->curr_road->final->y*ROAD_SCALE - (carOffset_len + HCAR_LENGTH))*scale);
    glVertex2f (((float)curr_car->curr_road->final->x*ROAD_SCALE - (carOffset_wid-HCAR_WIDTH))*scale, ((float)curr_car->curr_road->final->y*ROAD_SCALE - (carOffset_len - HCAR_LENGTH))*scale);
    glVertex2f (((float)curr_car->curr_road->final->x*ROAD_SCALE - (carOffset_wid+HCAR_WIDTH))*scale, ((float)curr_car->curr_road->final->y*ROAD_SCALE - (carOffset_len - HCAR_LENGTH))*scale);
    glVertex2f (((float)curr_car->curr_road->final->x*ROAD_SCALE - (carOffset_wid+HCAR_WIDTH))*scale, ((float)curr_car->curr_road->final->y*ROAD_SCALE - (carOffset_len + HCAR_LENGTH))*scale);
    break;
    
  case EAST:
    glVertex2f (((float)curr_car->curr_road->final->x*ROAD_SCALE - (carOffset_len + HCAR_LENGTH))*scale ,((float)curr_car->curr_road->final->y*ROAD_SCALE + (carOffset_wid-HCAR_WIDTH))*scale);
    glVertex2f (((float)curr_car->curr_road->final->x*ROAD_SCALE - (carOffset_len - HCAR_LENGTH))*scale ,((float)curr_car->curr_road->final->y*ROAD_SCALE + (carOffset_wid-HCAR_WIDTH))*scale);
    glVertex2f (((float)curr_car->curr_road->final->x*ROAD_SCALE - (carOffset_len - HCAR_LENGTH))*scale ,((float)curr_car->curr_road->final->y*ROAD_SCALE + (carOffset_wid+HCAR_WIDTH))*scale);
    glVertex2f (((float)curr_car->curr_road->final->x*ROAD_SCALE - (carOffset_len + HCAR_LENGTH))*scale ,((float)curr_car->curr_road->final->y*ROAD_SCALE + (carOffset_wid+HCAR_WIDTH))*scale);
    break;

  case SOUTH:
    glVertex2f (((float)curr_car->curr_road->final->x*ROAD_SCALE + (carOffset_wid-HCAR_WIDTH))*scale, ((float)curr_car->curr_road->final->y*ROAD_SCALE + (carOffset_len + HCAR_LENGTH))*scale);
    glVertex2f (((float)curr_car->curr_road->final->x*ROAD_SCALE + (carOffset_wid-HCAR_WIDTH))*scale, ((float)curr_car->curr_road->final->y*ROAD_SCALE + (carOffset_len - HCAR_LENGTH))*scale);
    glVertex2f (((float)curr_car->curr_road->final->x*ROAD_SCALE + (carOffset_wid+HCAR_WIDTH))*scale, ((float)curr_car->curr_road->final->y*ROAD_SCALE + (carOffset_len - HCAR_LENGTH))*scale);
    glVertex2f (((float)curr_car->curr_road->final->x*ROAD_SCALE + (carOffset_wid+HCAR_WIDTH))*scale, ((float)curr_car->curr_road->final->y*ROAD_SCALE + (carOffset_len + HCAR_LENGTH))*scale);
    break;

  case WEST:
    glVertex2f (((float)curr_car->curr_road->final->x*ROAD_SCALE + (carOffset_len + HCAR_LENGTH))*scale ,((float)curr_car->curr_road->final->y*ROAD_SCALE - (carOffset_wid-HCAR_WIDTH))*scale);
    glVertex2f (((float)curr_car->curr_road->final->x*ROAD_SCALE + (carOffset_len - HCAR_LENGTH))*scale ,((float)curr_car->curr_road->final->y*ROAD_SCALE - (carOffset_wid-HCAR_WIDTH))*scale);
    glVertex2f (((float)curr_car->curr_road->final->x*ROAD_SCALE + (carOffset_len - HCAR_LENGTH))*scale ,((float)curr_car->curr_road->final->y*ROAD_SCALE - (carOffset_wid+HCAR_WIDTH))*scale);
    glVertex2f (((float)curr_car->curr_road->final->x*ROAD_SCALE + (carOffset_len + HCAR_LENGTH))*scale ,((float)curr_car->curr_road->final->y*ROAD_SCALE - (carOffset_wid+HCAR_WIDTH))*scale);
    break;
    
  default:
    printf ("Error in Road Endpoints! They are invalid\n");
    exit (-1);
  }
  glEnd ();
}


void painter::draw (intersection* curr_intersection) {


   if (!curr_intersection)
      return;

   glColor3f (0.5f, 0.5f, 0.5f);
   glBegin (GL_QUADS);
   glVertex2f (((float) curr_intersection->x*ROAD_SCALE - INTERSECTION_SIZE/2.0) * scale, ((float)curr_intersection->y*ROAD_SCALE + INTERSECTION_SIZE/2.0)* scale);
   glVertex2f (((float) curr_intersection->x*ROAD_SCALE + INTERSECTION_SIZE/2.0) * scale, ((float)curr_intersection->y*ROAD_SCALE + INTERSECTION_SIZE/2.0)* scale);
   glVertex2f (((float) curr_intersection->x*ROAD_SCALE + INTERSECTION_SIZE/2.0) * scale, ((float)curr_intersection->y*ROAD_SCALE - INTERSECTION_SIZE/2.0)* scale);
   glVertex2f (((float) curr_intersection->x*ROAD_SCALE - INTERSECTION_SIZE/2.0) * scale, ((float)curr_intersection->y*ROAD_SCALE - INTERSECTION_SIZE/2.0)* scale);
   glEnd ();

}

void painter::draw (road* curr_road) {

  if (!curr_road)
    return;

  glColor3f (0.5f, 0.5f, 0.5f);
  glBegin (GL_QUADS);

  switch (curr_road->compass) {

  case NORTH:
    glVertex2f (((float)curr_road->init->x*ROAD_SCALE - HLEN_BW_LANES)*scale, ((float)curr_road->init->y*ROAD_SCALE + INTERSECTION_SIZE/2.0)*scale);
    glVertex2f (((float)curr_road->init->x*ROAD_SCALE - HLEN_BW_LANES)*scale, ((float)curr_road->final->y*ROAD_SCALE - INTERSECTION_SIZE/2.0)*scale);
    glVertex2f (((float)curr_road->init->x*ROAD_SCALE - (ROAD_WIDTH+HLEN_BW_LANES))*scale, ((float)curr_road->final->y*ROAD_SCALE - INTERSECTION_SIZE/2.0)*scale);
    glVertex2f (((float)curr_road->init->x*ROAD_SCALE - (ROAD_WIDTH+HLEN_BW_LANES))*scale, ((float)curr_road->init->y*ROAD_SCALE + INTERSECTION_SIZE/2.0)*scale);
    break;

  case EAST:
    glVertex2f (((float)curr_road->init->x*ROAD_SCALE + INTERSECTION_SIZE/2.0)*scale,((float)curr_road->init->y*ROAD_SCALE + (ROAD_WIDTH+HLEN_BW_LANES))*scale);
    glVertex2f (((float)curr_road->final->x*ROAD_SCALE - INTERSECTION_SIZE/2.0)*scale,((float)curr_road->init->y*ROAD_SCALE + (ROAD_WIDTH+HLEN_BW_LANES))*scale);
    glVertex2f (((float)curr_road->final->x*ROAD_SCALE - INTERSECTION_SIZE/2.0)*scale,((float)curr_road->init->y*ROAD_SCALE + HLEN_BW_LANES)*scale);
    glVertex2f (((float)curr_road->init->x*ROAD_SCALE + INTERSECTION_SIZE/2.0)*scale,((float)curr_road->init->y*ROAD_SCALE + HLEN_BW_LANES)*scale);
    break;

  case SOUTH:
    glVertex2f (((float)curr_road->init->x*ROAD_SCALE + (ROAD_WIDTH+HLEN_BW_LANES))*scale, ((float)curr_road->init->y*ROAD_SCALE - INTERSECTION_SIZE/2.0)*scale);
    glVertex2f (((float)curr_road->init->x*ROAD_SCALE + (ROAD_WIDTH+HLEN_BW_LANES))*scale, ((float)curr_road->final->y*ROAD_SCALE + INTERSECTION_SIZE/2.0)*scale);
    glVertex2f (((float)curr_road->init->x*ROAD_SCALE + HLEN_BW_LANES)*scale, ((float)curr_road->final->y*ROAD_SCALE + INTERSECTION_SIZE/2.0)*scale);
    glVertex2f (((float)curr_road->init->x*ROAD_SCALE + HLEN_BW_LANES)*scale, ((float)curr_road->init->y*ROAD_SCALE - INTERSECTION_SIZE/2.0)*scale);
    break;

  case WEST:
    glVertex2f (((float)curr_road->init->x*ROAD_SCALE - INTERSECTION_SIZE/2.0)*scale,((float)curr_road->init->y*ROAD_SCALE - HLEN_BW_LANES)*scale);
    glVertex2f (((float)curr_road->final->x*ROAD_SCALE + INTERSECTION_SIZE/2.0)*scale,((float)curr_road->init->y*ROAD_SCALE - HLEN_BW_LANES)*scale);
    glVertex2f (((float)curr_road->final->x*ROAD_SCALE + INTERSECTION_SIZE/2.0)*scale,((float)curr_road->init->y*ROAD_SCALE - (ROAD_WIDTH+HLEN_BW_LANES))*scale);
    glVertex2f (((float)curr_road->init->x*ROAD_SCALE - INTERSECTION_SIZE/2.0)*scale,((float)curr_road->init->y*ROAD_SCALE - (ROAD_WIDTH+HLEN_BW_LANES))*scale);
    break;

  default:
    printf ("Error in Road Endpoints! They are invalid\n");
    exit (-1);
  }

  glEnd ();
}

void painter::drawLights (road* curr_road) {

  //curr_road->lights[LEFT] = GREEN; // Debugging purpose only
  //curr_road->lights[RIGHT] = GREEN; // Debugging purpose only

  float light_left_offset;
  float light_right_offset;

  for ( int i = 0; i <=1; i++) {

    if (i == LEFT || i == AHEAD) {
      light_right_offset = 1*TL_LIGHT_SIZE;
      light_left_offset = (1+1)*TL_LIGHT_SIZE;
    }
    else if (i == RIGHT) {
      light_right_offset = 0*TL_LIGHT_SIZE;
      light_left_offset = (0+1)*TL_LIGHT_SIZE;
    }

    for (int j = 0; j < 3; j++) {
	
      glColor3f (1.0f*(float)(j<=AMBER),1.0f*(float)(j>=AMBER),0.0f);
      //glColor3i (255,0,0);

      
      glBegin (GL_LINE_LOOP);

     switch (curr_road->compass) {

      case NORTH:
	glVertex2f (((float)curr_road->init->x*ROAD_SCALE - ((ROAD_WIDTH+HLEN_BW_LANES) + light_right_offset))*scale, 
		    ((float)curr_road->final->y*ROAD_SCALE - (INTERSECTION_SIZE/2.0 + (j+1)*TL_LIGHT_SIZE))*scale);
	glVertex2f (((float)curr_road->init->x*ROAD_SCALE - ((ROAD_WIDTH+HLEN_BW_LANES) + light_right_offset))*scale, 
		    ((float)curr_road->final->y*ROAD_SCALE - (INTERSECTION_SIZE/2.0 + j * TL_LIGHT_SIZE))*scale);
	glVertex2f (((float)curr_road->init->x*ROAD_SCALE - ((ROAD_WIDTH+HLEN_BW_LANES) + light_left_offset - HLEN_BW_LIGHTS))*scale, 
		    ((float)curr_road->final->y*ROAD_SCALE - (INTERSECTION_SIZE/2.0 + j * TL_LIGHT_SIZE))*scale);
	glVertex2f (((float)curr_road->init->x*ROAD_SCALE - ((ROAD_WIDTH+HLEN_BW_LANES) + light_left_offset - HLEN_BW_LIGHTS))*scale, 
		    ((float)curr_road->final->y*ROAD_SCALE - (INTERSECTION_SIZE/2.0 + (j+1)*TL_LIGHT_SIZE))*scale);
	break;

      case EAST:
	glVertex2f (((float)curr_road->final->x*ROAD_SCALE - (INTERSECTION_SIZE/2.0 + (j+1)*TL_LIGHT_SIZE))*scale,
		    ((float)curr_road->init->y*ROAD_SCALE + ((ROAD_WIDTH+HLEN_BW_LANES) + light_right_offset))*scale);
	glVertex2f (((float)curr_road->final->x*ROAD_SCALE - (INTERSECTION_SIZE/2.0 + j * TL_LIGHT_SIZE))*scale,
		    ((float)curr_road->init->y*ROAD_SCALE + ((ROAD_WIDTH+HLEN_BW_LANES) + light_right_offset))*scale);
	glVertex2f (((float)curr_road->final->x*ROAD_SCALE - (INTERSECTION_SIZE/2.0 + j * TL_LIGHT_SIZE))*scale,
		    ((float)curr_road->init->y*ROAD_SCALE + ((ROAD_WIDTH+HLEN_BW_LANES) + light_left_offset - HLEN_BW_LIGHTS))*scale);
	glVertex2f (((float)curr_road->final->x*ROAD_SCALE - (INTERSECTION_SIZE/2.0 + (j+1) * TL_LIGHT_SIZE))*scale,
		    ((float)curr_road->init->y*ROAD_SCALE + ((ROAD_WIDTH+HLEN_BW_LANES) + light_left_offset - HLEN_BW_LIGHTS))*scale);
	break;

      case SOUTH:
	glVertex2f (((float)curr_road->init->x*ROAD_SCALE + ((ROAD_WIDTH+HLEN_BW_LANES) + light_right_offset))*scale, 
		    ((float)curr_road->final->y*ROAD_SCALE + (INTERSECTION_SIZE/2.0 + (j+1)*TL_LIGHT_SIZE))*scale);
	glVertex2f (((float)curr_road->init->x*ROAD_SCALE + ((ROAD_WIDTH+HLEN_BW_LANES) + light_right_offset))*scale, 
		    ((float)curr_road->final->y*ROAD_SCALE + (INTERSECTION_SIZE/2.0 + j * TL_LIGHT_SIZE))*scale);
	glVertex2f (((float)curr_road->init->x*ROAD_SCALE + ((ROAD_WIDTH+HLEN_BW_LANES) + light_left_offset - HLEN_BW_LIGHTS))*scale, 
		    ((float)curr_road->final->y*ROAD_SCALE + (INTERSECTION_SIZE/2.0 + j * TL_LIGHT_SIZE))*scale);
	glVertex2f (((float)curr_road->init->x*ROAD_SCALE + ((ROAD_WIDTH+HLEN_BW_LANES) + light_left_offset - HLEN_BW_LIGHTS))*scale, 
		    ((float)curr_road->final->y*ROAD_SCALE + (INTERSECTION_SIZE/2.0 + (j+1)*TL_LIGHT_SIZE))*scale);
	break;

      case WEST:
	glVertex2f (((float)curr_road->final->x*ROAD_SCALE + (INTERSECTION_SIZE/2.0 + (j+1)*TL_LIGHT_SIZE))*scale,
		    ((float)curr_road->init->y*ROAD_SCALE - ((ROAD_WIDTH+HLEN_BW_LANES) + light_right_offset))*scale);
	glVertex2f (((float)curr_road->final->x*ROAD_SCALE + (INTERSECTION_SIZE/2.0 + j * TL_LIGHT_SIZE))*scale,
		    ((float)curr_road->init->y*ROAD_SCALE - ((ROAD_WIDTH+HLEN_BW_LANES) + light_right_offset))*scale);
	glVertex2f (((float)curr_road->final->x*ROAD_SCALE + (INTERSECTION_SIZE/2.0 + j * TL_LIGHT_SIZE))*scale,
		    ((float)curr_road->init->y*ROAD_SCALE - ((ROAD_WIDTH+HLEN_BW_LANES) + light_left_offset - HLEN_BW_LIGHTS))*scale);
	glVertex2f (((float)curr_road->final->x*ROAD_SCALE + (INTERSECTION_SIZE/2.0 + (j+1) * TL_LIGHT_SIZE))*scale,
		    ((float)curr_road->init->y*ROAD_SCALE - ((ROAD_WIDTH+HLEN_BW_LANES) + light_left_offset - HLEN_BW_LIGHTS))*scale);
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
	glVertex2f (((float)curr_road->init->x*ROAD_SCALE - ((ROAD_WIDTH+HLEN_BW_LANES) + light_right_offset))*scale, 
		    ((float)curr_road->final->y*ROAD_SCALE - (INTERSECTION_SIZE/2.0 + (j+1)*TL_LIGHT_SIZE))*scale);
	glVertex2f (((float)curr_road->init->x*ROAD_SCALE - ((ROAD_WIDTH+HLEN_BW_LANES) + light_right_offset))*scale, 
		    ((float)curr_road->final->y*ROAD_SCALE - (INTERSECTION_SIZE/2.0 + j * TL_LIGHT_SIZE))*scale);
	glVertex2f (((float)curr_road->init->x*ROAD_SCALE - ((ROAD_WIDTH+HLEN_BW_LANES) + light_left_offset - HLEN_BW_LIGHTS))*scale, 
		    ((float)curr_road->final->y*ROAD_SCALE - (INTERSECTION_SIZE/2.0 + j * TL_LIGHT_SIZE))*scale);
	glVertex2f (((float)curr_road->init->x*ROAD_SCALE - ((ROAD_WIDTH+HLEN_BW_LANES) + light_left_offset - HLEN_BW_LIGHTS))*scale, 
		    ((float)curr_road->final->y*ROAD_SCALE - (INTERSECTION_SIZE/2.0 + (j+1)*TL_LIGHT_SIZE))*scale);
	break;

      case EAST:
	glVertex2f (((float)curr_road->final->x*ROAD_SCALE - (INTERSECTION_SIZE/2.0 + (j+1)*TL_LIGHT_SIZE))*scale,
		    ((float)curr_road->init->y*ROAD_SCALE + ((ROAD_WIDTH+HLEN_BW_LANES) + light_right_offset))*scale);
	glVertex2f (((float)curr_road->final->x*ROAD_SCALE - (INTERSECTION_SIZE/2.0 + j * TL_LIGHT_SIZE))*scale,
		    ((float)curr_road->init->y*ROAD_SCALE + ((ROAD_WIDTH+HLEN_BW_LANES) + light_right_offset))*scale);
	glVertex2f (((float)curr_road->final->x*ROAD_SCALE - (INTERSECTION_SIZE/2.0 + j * TL_LIGHT_SIZE))*scale,
		    ((float)curr_road->init->y*ROAD_SCALE + ((ROAD_WIDTH+HLEN_BW_LANES) + light_left_offset - HLEN_BW_LIGHTS))*scale);
	glVertex2f (((float)curr_road->final->x*ROAD_SCALE - (INTERSECTION_SIZE/2.0 + (j+1) * TL_LIGHT_SIZE))*scale,
		    ((float)curr_road->init->y*ROAD_SCALE + ((ROAD_WIDTH+HLEN_BW_LANES) + light_left_offset - HLEN_BW_LIGHTS))*scale);
	break;

      case SOUTH:
	glVertex2f (((float)curr_road->init->x*ROAD_SCALE + ((ROAD_WIDTH+HLEN_BW_LANES) + light_right_offset))*scale, 
		    ((float)curr_road->final->y*ROAD_SCALE + (INTERSECTION_SIZE/2.0 + (j+1)*TL_LIGHT_SIZE))*scale);
	glVertex2f (((float)curr_road->init->x*ROAD_SCALE + ((ROAD_WIDTH+HLEN_BW_LANES) + light_right_offset))*scale, 
		    ((float)curr_road->final->y*ROAD_SCALE + (INTERSECTION_SIZE/2.0 + j * TL_LIGHT_SIZE))*scale);
	glVertex2f (((float)curr_road->init->x*ROAD_SCALE + ((ROAD_WIDTH+HLEN_BW_LANES) + light_left_offset - HLEN_BW_LIGHTS))*scale, 
		    ((float)curr_road->final->y*ROAD_SCALE + (INTERSECTION_SIZE/2.0 + j * TL_LIGHT_SIZE))*scale);
	glVertex2f (((float)curr_road->init->x*ROAD_SCALE + ((ROAD_WIDTH+HLEN_BW_LANES) + light_left_offset - HLEN_BW_LIGHTS))*scale, 
		    ((float)curr_road->final->y*ROAD_SCALE + (INTERSECTION_SIZE/2.0 + (j+1)*TL_LIGHT_SIZE))*scale);
	break;

      case WEST:
	glVertex2f (((float)curr_road->final->x*ROAD_SCALE + (INTERSECTION_SIZE/2.0 + (j+1)*TL_LIGHT_SIZE))*scale,
		    ((float)curr_road->init->y*ROAD_SCALE - ((ROAD_WIDTH+HLEN_BW_LANES) + light_right_offset))*scale);
	glVertex2f (((float)curr_road->final->x*ROAD_SCALE + (INTERSECTION_SIZE/2.0 + j * TL_LIGHT_SIZE))*scale,
		    ((float)curr_road->init->y*ROAD_SCALE - ((ROAD_WIDTH+HLEN_BW_LANES) + light_right_offset))*scale);
	glVertex2f (((float)curr_road->final->x*ROAD_SCALE + (INTERSECTION_SIZE/2.0 + j * TL_LIGHT_SIZE))*scale,
		    ((float)curr_road->init->y*ROAD_SCALE - ((ROAD_WIDTH+HLEN_BW_LANES) + light_left_offset - HLEN_BW_LIGHTS))*scale);
	glVertex2f (((float)curr_road->final->x*ROAD_SCALE + (INTERSECTION_SIZE/2.0 + (j+1) * TL_LIGHT_SIZE))*scale,
		    ((float)curr_road->init->y*ROAD_SCALE - ((ROAD_WIDTH+HLEN_BW_LANES) + light_left_offset - HLEN_BW_LIGHTS))*scale);
	break;

      default:
	printf ("Error in Road Endpoints! They are invalid\n");
	exit (-1);
      }

      glEnd ();
  }

}
