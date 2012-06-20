#ifdef OPENGL_MODE

#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "config.h"
#include "input_device_handler.h"
#include "world.h"
#include "painter.h"
#include "learner.h"

extern painter* ppainter;
extern world* simulation;
extern learner* traffic_learner;

extern bool stopAnime;
extern int simulation_interval;

extern bool fullSpeed;

extern int curr_mode;
extern int learnTime;
extern int complyTime;



static bool mirror = false;

char modes[4][20]= {"Naive","Learning","Comply", "Learn&Comply"};



void handleKeyPress  (unsigned char key, int x, int y) {
  switch (key) {
  case 32: // SPACE BAR
    if (stopAnime) {
      stopAnime = false;
      printf (">> Animation Resumed.");
    }
    else {
      stopAnime = true;
      printf (">> Animation Paused.");
    }
    printf (">> \n");
    break;

   case 27:
     printf (">> Escape key pressed.\n");
     exit (0);
     break;
     
  case 'k':
    glMatrixMode (GL_PROJECTION);
    if (mirror) glOrtho (-1,1,-0.9,1.1,-1,1);
    else glOrtho (-1,1,-1.1,0.9,-1,1);
    printf (">> Perspective View Window moved down\n");
    break;

  case 'j':
    glMatrixMode (GL_PROJECTION);
    if (mirror) glOrtho (-0.9,1.1,-1,1,-1,1);
    else glOrtho (-1.1,0.9,-1,1,-1,1);
    printf (">> Perspective View Window moved left\n");
    break;

  case 'l':
    glMatrixMode (GL_PROJECTION);
    if (mirror) glOrtho (-1.1,0.9,-1,1,-1,1);
    else glOrtho (-0.9,1.1,-1,1,-1,1);
    printf (">> Perspective View Window moved right\n");
    break;

  case 'i':
    glMatrixMode (GL_PROJECTION);
    if (mirror) glOrtho (-1,1,-1.1,0.9,-1,1);
    else glOrtho (-1,1,-0.9,1.1,-1,1);
    printf (">> Perspective View Window moved up\n");
    break;

  case 'm':
    mirror = !mirror;
    if (mirror) printf (">> Mirror Turned on\n");
    else printf (">> Mirror Turned off\n");
    break;

  case 'x':
    glMatrixMode (GL_PROJECTION);
    glOrtho (-1.2,1.2,-1.2,1.2,-1,1);
    printf (">> Zooming Out\n");
    break;

  case 'z':
    glMatrixMode (GL_PROJECTION);
    glOrtho (-5.0/6.0,5.0/6.0,-5.0/6.0,5.0/6.0,-1,1);
    printf (">> Zooming in\n");
    break;

  case 'r':
    glMatrixMode (GL_PROJECTION);
    glPopMatrix ();
    glPushMatrix ();
    printf (">> View reset");
    break;

  case '=':
    if (simulation_interval > 1)
      simulation_interval /= 2;
    printf (">> NITROX %d powered by SEGA\n", simulation_interval);
    break;

  case '-':
    if (simulation_interval < 1024*8)
    simulation_interval *= 2;
    printf (">> Damper %d\n", simulation_interval);
    break;

  case '.':
    printf (">> L press\n");
    x = x + 1;
    break;

  case 's':
    stopAnime = true;
    
    if (curr_mode == 0)
      traffic_learner->naiveControl (simulation);
    else if (curr_mode == 1)
      traffic_learner->learn (fullSpeed);
       else if (curr_mode == 2) {
      traffic_learner->comply ();
      if ( !(simulation->timestamp % PERFORMANCE_CALC_INTERVAL) )
	traffic_learner->displayPerformance (PERFORMANCE_CALC_INTERVAL);
    }
    else
    {

	if (learnTime > 0) 
	  {
	    traffic_learner->learn (fullSpeed);
	    learnTime--;
	    //printf ("learnTime:%d\n",learnTime);
	  }
	if (learnTime <= 0 && complyTime <=0)
	  complyTime = COMPLY_TIME;


	if (complyTime > 0)
	  {
	    traffic_learner->comply ();
	    complyTime--;
	    //printf ("complyTime:%d\n",complyTime);
	  }
	if (complyTime <= 0 && learnTime <=0) {
	  learnTime = LEARN_TIME;      
	  traffic_learner->displayPerformance (COMPLY_TIME);
	}     

    }
    //simulation->updateWorld ();
    ppainter->draw ();
    printf (">> Key Pressed: Step to next unit in time dimension\n");
    break;

  case 'g':
    simulation->spawnCars (0*(SQUARE_SIZE+1)+0, BATCH_SIZE);
    simulation->spawnCars (1*(SQUARE_SIZE+1)+2, BATCH_SIZE);
    printf (">> KeyPressed: %d more cars spawned\n", BATCH_SIZE);
    break;

  case 't':
    curr_mode = ((++curr_mode) % 4);
    if (curr_mode == 3) { //Learn&Comply MODE
      learnTime = 0;// This is the setting to intially know the performance
      complyTime = COMPLY_TIME; // and to compare with later performance
    }
    printf (">> Toggle to %d:%s mode\n",curr_mode, modes[curr_mode]);
    break;

  case 'p':
    traffic_learner->print_to_file ();
    printf (">> Print Learned Table\n");
    break;
  
  case 'f':
    fullSpeed = !fullSpeed;
    printf (">> Full Speed %c%cactivated\n", (!fullSpeed)*'d', (!fullSpeed)*'e');
    break;
  }
}


void handleResize (int w, int h) {
  glViewport (0,0,w,h);
  glMatrixMode (GL_PROJECTION);

  glLoadIdentity ();
  gluPerspective (45.0, (double) w / (double) h, 0.0, 0.0);
}

#endif
