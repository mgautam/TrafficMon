#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>
#include "config.h"
#include "input_device_handler.h"
#include "painter.h"


#ifdef OPENGL_MODE
/*
static void displayText (float x, float y, char *text) {

  glPushMatrix ();
  glScalef (0.125f,0.125f,1.0f);
  glColor3f(1.0f,1.0f,1.0f);

  while (*text) {
    glutStrokeCharacter(GLUT_STROKE_ROMAN, *text);
    text++;
  }
  glScalef (8.0f,8.0f,1.0f);  
  glPopMatrix ();

}
*/

painter::painter(world* _simulation, void (*display) (void), int argc, char** argv) {

  this->simulation = _simulation;

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

}

void painter::animate()
{ 
  glutMainLoop();
}

void painter::draw () {
  // Calculating world scale to map to our windowed perception of the world
  int minWorldX = 0;
  int minWorldY = 0;
  int maxWorldX = 0;
  int maxWorldY = 0;
  for (int i = 0; i < simulation->intc; i++) {
    if (!simulation->intersections[i])
      continue;

    if (maxWorldX < simulation->intersections[i]->x)
      maxWorldX = simulation->intersections[i]->x;
    if (minWorldX > simulation->intersections[i]->x)
      minWorldX = simulation->intersections[i]->x;

    if (maxWorldY < simulation->intersections[i]->y)
      maxWorldY = simulation->intersections[i]->y;
    if (minWorldY > simulation->intersections[i]->y)
      minWorldY = simulation->intersections[i]->y;
  }

  if (maxWorldX != minWorldX && maxWorldY != minWorldY)
    this->scale = (maxWorldX - minWorldX) > (maxWorldY - minWorldY)?
    1.5/((float)(maxWorldX - minWorldX + MARGIN_PADDING))  :  1.5/((float)(maxWorldY - minWorldY + MARGIN_PADDING));
  //printf ("minX = %d, minY = %d \t maxX = %d, maxY = %d", minWorldX, minWorldY, maxWorldX, maxWorldY);
  //Calculating scale ends here
  


  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
 
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();

  glScalef (2.0*scale,2.0*scale,1.0f);
  glTranslatef (-0.5/scale,-0.5/scale,0);
  
  for (int i = 0; i < simulation->intc; i++)
    draw(simulation->intersections[i]);
  
  for (int i = 0; i < simulation->roadc; i++) {
    draw(simulation->roads[i]);
    drawLights(simulation->roads[i]);

    for (int l = 0; l < simulation->roads[i]->numlanes; l++) {
      for (int j = -2; j < simulation->roads[i]->length; j++)
	{
	  if (simulation->roads[i]->cars[l][j])
	    draw(simulation->roads[i]->cars[l][j]);
	}
    }
  }

  glutSwapBuffers ();
}

void painter::output(GLfloat x, GLfloat y, char *text)
{
  //  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  //  glEnable(GL_BLEND);
  //  glEnable(GL_LINE_SMOOTH);
  //   glPushMatrix();
  glLineWidth(10.0);
  glColor3f(1.0f,0.0f,0.0f);


   glTranslatef(x,y,0);
   //   glScalef(100, 100, 100);
   char* p;
   for (p = text; *p; p++)
     glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
   // glPopMatrix();
}





void painter::draw (car* curr_car) {

  if (!curr_car)
    return;

  //  int init_x = curr_car->curr_road->init->x + curr_car->curr_road->init->x / NUM_SLOTS_IN_ROAD * 4;
  //int init_y = curr_car->curr_road->init->y + curr_car->curr_road->init->y / NUM_SLOTS_IN_ROAD * 4;
  int final_x = curr_car->curr_road->final->x + curr_car->curr_road->final->x / NUM_SLOTS_IN_ROAD * 4;
  int final_y = curr_car->curr_road->final->y + curr_car->curr_road->final->y / NUM_SLOTS_IN_ROAD * 4;


  // The first 0.5 is to include the offset of intersection to road
  // -1 coz position starts from 1 not 0. 
  // The last 0.5 as offset for between car padding
  //  float carOffset_len =  (INTERSECTION_SIZE/2 + 7.5*(float)curr_car->position/(float)(CAR_LENGTH+MIN_INTER_CAR_SPACE) + HCAR_LENGTH);//*(CAR_LENGTH+MIN_INTER_CAR_SPACE)
  float carOffset_len =  (INTERSECTION_SIZE/2 +  ROAD_SCALE*curr_car->position + HCAR_LENGTH);//*(CAR_LENGTH+MIN_INTER_CAR_SPACE)
  // offset of car along the length of the road from final intersection
  float carOffset_wid = LANE_WIDTH/2 + LANE_WIDTH * curr_car->currlaneIndex + HLEN_BW_LANES;

  glColor3f (curr_car->color.r,curr_car->color.g,curr_car->color.b);
  glBegin (GL_QUADS);

  switch (curr_car->curr_road->compass) {
      
  case NORTH:
    glVertex2f (((float)final_x*ROAD_SCALE - (carOffset_wid-HCAR_WIDTH))*scale, 
		((float)final_y*ROAD_SCALE - (carOffset_len + HCAR_LENGTH))*scale);
    glVertex2f (((float)final_x*ROAD_SCALE - (carOffset_wid-HCAR_WIDTH))*scale, 
		((float)final_y*ROAD_SCALE - (carOffset_len - HCAR_LENGTH))*scale);
    glVertex2f (((float)final_x*ROAD_SCALE - (carOffset_wid+HCAR_WIDTH))*scale, 
		((float)final_y*ROAD_SCALE - (carOffset_len - HCAR_LENGTH))*scale);
    glVertex2f (((float)final_x*ROAD_SCALE - (carOffset_wid+HCAR_WIDTH))*scale, 
		((float)final_y*ROAD_SCALE - (carOffset_len + HCAR_LENGTH))*scale);
    //displayText (((float)final_x*ROAD_SCALE - (carOffset_wid-HCAR_WIDTH))*scale, ((float)final_y*ROAD_SCALE - carOffset_len)*scale, "Car");
    break;
    
  case EAST:
    glVertex2f (((float)final_x*ROAD_SCALE - (carOffset_len + HCAR_LENGTH))*scale ,((float)final_y*ROAD_SCALE + (carOffset_wid-HCAR_WIDTH))*scale);
    glVertex2f (((float)final_x*ROAD_SCALE - (carOffset_len - HCAR_LENGTH))*scale ,((float)final_y*ROAD_SCALE + (carOffset_wid-HCAR_WIDTH))*scale);
    glVertex2f (((float)final_x*ROAD_SCALE - (carOffset_len - HCAR_LENGTH))*scale ,((float)final_y*ROAD_SCALE + (carOffset_wid+HCAR_WIDTH))*scale);
    glVertex2f (((float)final_x*ROAD_SCALE - (carOffset_len + HCAR_LENGTH))*scale ,((float)final_y*ROAD_SCALE + (carOffset_wid+HCAR_WIDTH))*scale);
    break;

  case SOUTH:
    glVertex2f (((float)final_x*ROAD_SCALE + (carOffset_wid-HCAR_WIDTH))*scale, ((float)final_y*ROAD_SCALE + (carOffset_len + HCAR_LENGTH))*scale);
    glVertex2f (((float)final_x*ROAD_SCALE + (carOffset_wid-HCAR_WIDTH))*scale, ((float)final_y*ROAD_SCALE + (carOffset_len - HCAR_LENGTH))*scale);
    glVertex2f (((float)final_x*ROAD_SCALE + (carOffset_wid+HCAR_WIDTH))*scale, ((float)final_y*ROAD_SCALE + (carOffset_len - HCAR_LENGTH))*scale);
    glVertex2f (((float)final_x*ROAD_SCALE + (carOffset_wid+HCAR_WIDTH))*scale, ((float)final_y*ROAD_SCALE + (carOffset_len + HCAR_LENGTH))*scale);
    break;

  case WEST:
    glVertex2f (((float)final_x*ROAD_SCALE + (carOffset_len + HCAR_LENGTH))*scale ,((float)final_y*ROAD_SCALE - (carOffset_wid-HCAR_WIDTH))*scale);
    glVertex2f (((float)final_x*ROAD_SCALE + (carOffset_len - HCAR_LENGTH))*scale ,((float)final_y*ROAD_SCALE - (carOffset_wid-HCAR_WIDTH))*scale);
    glVertex2f (((float)final_x*ROAD_SCALE + (carOffset_len - HCAR_LENGTH))*scale ,((float)final_y*ROAD_SCALE - (carOffset_wid+HCAR_WIDTH))*scale);
    glVertex2f (((float)final_x*ROAD_SCALE + (carOffset_len + HCAR_LENGTH))*scale ,((float)final_y*ROAD_SCALE - (carOffset_wid+HCAR_WIDTH))*scale);
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

  int x = curr_intersection->x / NUM_SLOTS_IN_ROAD * 4 + curr_intersection->x ; 
  int y = curr_intersection->y / NUM_SLOTS_IN_ROAD * 4 + curr_intersection->y ; 

   glColor3f (0.5f, 0.5f, 0.5f);
   glBegin (GL_QUADS);
   glVertex2f (((float) x*ROAD_SCALE - INTERSECTION_SIZE/2.0) * scale, ((float)y*ROAD_SCALE + INTERSECTION_SIZE/2.0)* scale);
   glVertex2f (((float) x*ROAD_SCALE + INTERSECTION_SIZE/2.0) * scale, ((float)y*ROAD_SCALE + INTERSECTION_SIZE/2.0)* scale);
   glVertex2f (((float) x*ROAD_SCALE + INTERSECTION_SIZE/2.0) * scale, ((float)y*ROAD_SCALE - INTERSECTION_SIZE/2.0)* scale);
   glVertex2f (((float) x*ROAD_SCALE - INTERSECTION_SIZE/2.0) * scale, ((float)y*ROAD_SCALE - INTERSECTION_SIZE/2.0)* scale);
   glEnd ();

}

void painter::draw (road* curr_road) {

  if (!curr_road)
    return;

  int init_x = curr_road->init->x + curr_road->init->x / NUM_SLOTS_IN_ROAD * 4;
  int init_y = curr_road->init->y + curr_road->init->y / NUM_SLOTS_IN_ROAD * 4;
  int final_x = curr_road->final->x + curr_road->final->x / NUM_SLOTS_IN_ROAD * 4;
  int final_y = curr_road->final->y + curr_road->final->y / NUM_SLOTS_IN_ROAD * 4;

  glColor3f (0.5f, 0.5f, 0.5f);
  glBegin (GL_QUADS);

  switch (curr_road->compass) {

  case NORTH:
    glVertex2f (((float)init_x*ROAD_SCALE - HLEN_BW_LANES)*scale, ((float)init_y*ROAD_SCALE + INTERSECTION_SIZE/2.0)*scale);
    glVertex2f (((float)init_x*ROAD_SCALE - HLEN_BW_LANES)*scale, ((float)final_y*ROAD_SCALE - INTERSECTION_SIZE/2.0)*scale);
    glVertex2f (((float)init_x*ROAD_SCALE - (ROAD_WIDTH+HLEN_BW_LANES))*scale, ((float)final_y*ROAD_SCALE - INTERSECTION_SIZE/2.0)*scale);
    glVertex2f (((float)init_x*ROAD_SCALE - (ROAD_WIDTH+HLEN_BW_LANES))*scale, ((float)init_y*ROAD_SCALE + INTERSECTION_SIZE/2.0)*scale);
    break;

  case EAST:
    glVertex2f (((float)init_x*ROAD_SCALE + INTERSECTION_SIZE/2.0)*scale,((float)init_y*ROAD_SCALE + (ROAD_WIDTH+HLEN_BW_LANES))*scale);
    glVertex2f (((float)final_x*ROAD_SCALE - INTERSECTION_SIZE/2.0)*scale,((float)init_y*ROAD_SCALE + (ROAD_WIDTH+HLEN_BW_LANES))*scale);
    glVertex2f (((float)final_x*ROAD_SCALE - INTERSECTION_SIZE/2.0)*scale,((float)init_y*ROAD_SCALE + HLEN_BW_LANES)*scale);
    glVertex2f (((float)init_x*ROAD_SCALE + INTERSECTION_SIZE/2.0)*scale,((float)init_y*ROAD_SCALE + HLEN_BW_LANES)*scale);
    break;

  case SOUTH:
    glVertex2f (((float)init_x*ROAD_SCALE + (ROAD_WIDTH+HLEN_BW_LANES))*scale, ((float)init_y*ROAD_SCALE - INTERSECTION_SIZE/2.0)*scale);
    glVertex2f (((float)init_x*ROAD_SCALE + (ROAD_WIDTH+HLEN_BW_LANES))*scale, ((float)final_y*ROAD_SCALE + INTERSECTION_SIZE/2.0)*scale);
    glVertex2f (((float)init_x*ROAD_SCALE + HLEN_BW_LANES)*scale, ((float)final_y*ROAD_SCALE + INTERSECTION_SIZE/2.0)*scale);
    glVertex2f (((float)init_x*ROAD_SCALE + HLEN_BW_LANES)*scale, ((float)init_y*ROAD_SCALE - INTERSECTION_SIZE/2.0)*scale);
    break;

  case WEST:
    glVertex2f (((float)init_x*ROAD_SCALE - INTERSECTION_SIZE/2.0)*scale,((float)init_y*ROAD_SCALE - HLEN_BW_LANES)*scale);
    glVertex2f (((float)final_x*ROAD_SCALE + INTERSECTION_SIZE/2.0)*scale,((float)init_y*ROAD_SCALE - HLEN_BW_LANES)*scale);
    glVertex2f (((float)final_x*ROAD_SCALE + INTERSECTION_SIZE/2.0)*scale,((float)init_y*ROAD_SCALE - (ROAD_WIDTH+HLEN_BW_LANES))*scale);
    glVertex2f (((float)init_x*ROAD_SCALE - INTERSECTION_SIZE/2.0)*scale,((float)init_y*ROAD_SCALE - (ROAD_WIDTH+HLEN_BW_LANES))*scale);
    break;

  default:
    printf ("Error in Road Endpoints! They are invalid\n");
    exit (-1);
  }

  glEnd ();
}

void painter::drawLights (road* curr_road) {

  if (!curr_road)
    return;

  int init_x = curr_road->init->x + curr_road->init->x / NUM_SLOTS_IN_ROAD * 4;
  int init_y = curr_road->init->y + curr_road->init->y / NUM_SLOTS_IN_ROAD * 4;
  int final_x = curr_road->final->x + curr_road->final->x / NUM_SLOTS_IN_ROAD * 4;
  int final_y = curr_road->final->y + curr_road->final->y / NUM_SLOTS_IN_ROAD * 4;

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
	glVertex2f (((float)init_x*ROAD_SCALE - ((ROAD_WIDTH+HLEN_BW_LANES) + light_right_offset))*scale, 
		    ((float)final_y*ROAD_SCALE - (INTERSECTION_SIZE/2.0 + (j+1)*TL_LIGHT_SIZE))*scale);
	glVertex2f (((float)init_x*ROAD_SCALE - ((ROAD_WIDTH+HLEN_BW_LANES) + light_right_offset))*scale, 
		    ((float)final_y*ROAD_SCALE - (INTERSECTION_SIZE/2.0 + j * TL_LIGHT_SIZE))*scale);
	glVertex2f (((float)init_x*ROAD_SCALE - ((ROAD_WIDTH+HLEN_BW_LANES) + light_left_offset - HLEN_BW_LIGHTS))*scale, 
		    ((float)final_y*ROAD_SCALE - (INTERSECTION_SIZE/2.0 + j * TL_LIGHT_SIZE))*scale);
	glVertex2f (((float)init_x*ROAD_SCALE - ((ROAD_WIDTH+HLEN_BW_LANES) + light_left_offset - HLEN_BW_LIGHTS))*scale, 
		    ((float)final_y*ROAD_SCALE - (INTERSECTION_SIZE/2.0 + (j+1)*TL_LIGHT_SIZE))*scale);
	break;

      case EAST:
	glVertex2f (((float)final_x*ROAD_SCALE - (INTERSECTION_SIZE/2.0 + (j+1)*TL_LIGHT_SIZE))*scale,
		    ((float)init_y*ROAD_SCALE + ((ROAD_WIDTH+HLEN_BW_LANES) + light_right_offset))*scale);
	glVertex2f (((float)final_x*ROAD_SCALE - (INTERSECTION_SIZE/2.0 + j * TL_LIGHT_SIZE))*scale,
		    ((float)init_y*ROAD_SCALE + ((ROAD_WIDTH+HLEN_BW_LANES) + light_right_offset))*scale);
	glVertex2f (((float)final_x*ROAD_SCALE - (INTERSECTION_SIZE/2.0 + j * TL_LIGHT_SIZE))*scale,
		    ((float)init_y*ROAD_SCALE + ((ROAD_WIDTH+HLEN_BW_LANES) + light_left_offset - HLEN_BW_LIGHTS))*scale);
	glVertex2f (((float)final_x*ROAD_SCALE - (INTERSECTION_SIZE/2.0 + (j+1) * TL_LIGHT_SIZE))*scale,
		    ((float)init_y*ROAD_SCALE + ((ROAD_WIDTH+HLEN_BW_LANES) + light_left_offset - HLEN_BW_LIGHTS))*scale);
	break;

      case SOUTH:
	glVertex2f (((float)init_x*ROAD_SCALE + ((ROAD_WIDTH+HLEN_BW_LANES) + light_right_offset))*scale, 
		    ((float)final_y*ROAD_SCALE + (INTERSECTION_SIZE/2.0 + (j+1)*TL_LIGHT_SIZE))*scale);
	glVertex2f (((float)init_x*ROAD_SCALE + ((ROAD_WIDTH+HLEN_BW_LANES) + light_right_offset))*scale, 
		    ((float)final_y*ROAD_SCALE + (INTERSECTION_SIZE/2.0 + j * TL_LIGHT_SIZE))*scale);
	glVertex2f (((float)init_x*ROAD_SCALE + ((ROAD_WIDTH+HLEN_BW_LANES) + light_left_offset - HLEN_BW_LIGHTS))*scale, 
		    ((float)final_y*ROAD_SCALE + (INTERSECTION_SIZE/2.0 + j * TL_LIGHT_SIZE))*scale);
	glVertex2f (((float)init_x*ROAD_SCALE + ((ROAD_WIDTH+HLEN_BW_LANES) + light_left_offset - HLEN_BW_LIGHTS))*scale, 
		    ((float)final_y*ROAD_SCALE + (INTERSECTION_SIZE/2.0 + (j+1)*TL_LIGHT_SIZE))*scale);
	break;

      case WEST:
	glVertex2f (((float)final_x*ROAD_SCALE + (INTERSECTION_SIZE/2.0 + (j+1)*TL_LIGHT_SIZE))*scale,
		    ((float)init_y*ROAD_SCALE - ((ROAD_WIDTH+HLEN_BW_LANES) + light_right_offset))*scale);
	glVertex2f (((float)final_x*ROAD_SCALE + (INTERSECTION_SIZE/2.0 + j * TL_LIGHT_SIZE))*scale,
		    ((float)init_y*ROAD_SCALE - ((ROAD_WIDTH+HLEN_BW_LANES) + light_right_offset))*scale);
	glVertex2f (((float)final_x*ROAD_SCALE + (INTERSECTION_SIZE/2.0 + j * TL_LIGHT_SIZE))*scale,
		    ((float)init_y*ROAD_SCALE - ((ROAD_WIDTH+HLEN_BW_LANES) + light_left_offset - HLEN_BW_LIGHTS))*scale);
	glVertex2f (((float)final_x*ROAD_SCALE + (INTERSECTION_SIZE/2.0 + (j+1) * TL_LIGHT_SIZE))*scale,
		    ((float)init_y*ROAD_SCALE - ((ROAD_WIDTH+HLEN_BW_LANES) + light_left_offset - HLEN_BW_LIGHTS))*scale);
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
	glVertex2f (((float)init_x*ROAD_SCALE - ((ROAD_WIDTH+HLEN_BW_LANES) + light_right_offset))*scale, 
		    ((float)final_y*ROAD_SCALE - (INTERSECTION_SIZE/2.0 + (j+1)*TL_LIGHT_SIZE))*scale);
	glVertex2f (((float)init_x*ROAD_SCALE - ((ROAD_WIDTH+HLEN_BW_LANES) + light_right_offset))*scale, 
		    ((float)final_y*ROAD_SCALE - (INTERSECTION_SIZE/2.0 + j * TL_LIGHT_SIZE))*scale);
	glVertex2f (((float)init_x*ROAD_SCALE - ((ROAD_WIDTH+HLEN_BW_LANES) + light_left_offset - HLEN_BW_LIGHTS))*scale, 
		    ((float)final_y*ROAD_SCALE - (INTERSECTION_SIZE/2.0 + j * TL_LIGHT_SIZE))*scale);
	glVertex2f (((float)init_x*ROAD_SCALE - ((ROAD_WIDTH+HLEN_BW_LANES) + light_left_offset - HLEN_BW_LIGHTS))*scale, 
		    ((float)final_y*ROAD_SCALE - (INTERSECTION_SIZE/2.0 + (j+1)*TL_LIGHT_SIZE))*scale);
	break;

      case EAST:
	glVertex2f (((float)final_x*ROAD_SCALE - (INTERSECTION_SIZE/2.0 + (j+1)*TL_LIGHT_SIZE))*scale,
		    ((float)init_y*ROAD_SCALE + ((ROAD_WIDTH+HLEN_BW_LANES) + light_right_offset))*scale);
	glVertex2f (((float)final_x*ROAD_SCALE - (INTERSECTION_SIZE/2.0 + j * TL_LIGHT_SIZE))*scale,
		    ((float)init_y*ROAD_SCALE + ((ROAD_WIDTH+HLEN_BW_LANES) + light_right_offset))*scale);
	glVertex2f (((float)final_x*ROAD_SCALE - (INTERSECTION_SIZE/2.0 + j * TL_LIGHT_SIZE))*scale,
		    ((float)init_y*ROAD_SCALE + ((ROAD_WIDTH+HLEN_BW_LANES) + light_left_offset - HLEN_BW_LIGHTS))*scale);
	glVertex2f (((float)final_x*ROAD_SCALE - (INTERSECTION_SIZE/2.0 + (j+1) * TL_LIGHT_SIZE))*scale,
		    ((float)init_y*ROAD_SCALE + ((ROAD_WIDTH+HLEN_BW_LANES) + light_left_offset - HLEN_BW_LIGHTS))*scale);
	break;

      case SOUTH:
	glVertex2f (((float)init_x*ROAD_SCALE + ((ROAD_WIDTH+HLEN_BW_LANES) + light_right_offset))*scale, 
		    ((float)final_y*ROAD_SCALE + (INTERSECTION_SIZE/2.0 + (j+1)*TL_LIGHT_SIZE))*scale);
	glVertex2f (((float)init_x*ROAD_SCALE + ((ROAD_WIDTH+HLEN_BW_LANES) + light_right_offset))*scale, 
		    ((float)final_y*ROAD_SCALE + (INTERSECTION_SIZE/2.0 + j * TL_LIGHT_SIZE))*scale);
	glVertex2f (((float)init_x*ROAD_SCALE + ((ROAD_WIDTH+HLEN_BW_LANES) + light_left_offset - HLEN_BW_LIGHTS))*scale, 
		    ((float)final_y*ROAD_SCALE + (INTERSECTION_SIZE/2.0 + j * TL_LIGHT_SIZE))*scale);
	glVertex2f (((float)init_x*ROAD_SCALE + ((ROAD_WIDTH+HLEN_BW_LANES) + light_left_offset - HLEN_BW_LIGHTS))*scale, 
		    ((float)final_y*ROAD_SCALE + (INTERSECTION_SIZE/2.0 + (j+1)*TL_LIGHT_SIZE))*scale);
	break;

      case WEST:
	glVertex2f (((float)final_x*ROAD_SCALE + (INTERSECTION_SIZE/2.0 + (j+1)*TL_LIGHT_SIZE))*scale,
		    ((float)init_y*ROAD_SCALE - ((ROAD_WIDTH+HLEN_BW_LANES) + light_right_offset))*scale);
	glVertex2f (((float)final_x*ROAD_SCALE + (INTERSECTION_SIZE/2.0 + j * TL_LIGHT_SIZE))*scale,
		    ((float)init_y*ROAD_SCALE - ((ROAD_WIDTH+HLEN_BW_LANES) + light_right_offset))*scale);
	glVertex2f (((float)final_x*ROAD_SCALE + (INTERSECTION_SIZE/2.0 + j * TL_LIGHT_SIZE))*scale,
		    ((float)init_y*ROAD_SCALE - ((ROAD_WIDTH+HLEN_BW_LANES) + light_left_offset - HLEN_BW_LIGHTS))*scale);
	glVertex2f (((float)final_x*ROAD_SCALE + (INTERSECTION_SIZE/2.0 + (j+1) * TL_LIGHT_SIZE))*scale,
		    ((float)init_y*ROAD_SCALE - ((ROAD_WIDTH+HLEN_BW_LANES) + light_left_offset - HLEN_BW_LIGHTS))*scale);
	break;

      default:
	printf ("Error in Road Endpoints! They are invalid\n");
	exit (-1);
      }

      glEnd ();
  }

}

#endif
