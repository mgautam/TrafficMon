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
bool stopAnime = false;
int simulation_interval = 256;//msecs

static bool mirror = false;

extern int curr_mode;
int curr_mode = 1;

char modes[3][20]= {"Naive","Learning","Comply"};


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
      traffic_learner->learn ();
    else 
      traffic_learner->comply ();

    //simulation->updateWorld ();
    ppainter->draw ();
    printf (">> Key Pressed: Step to next unit in time dimension\n");
    break;

  case 'g':
    simulation->spawnCars (0*(SQUARE_SIZE+1)+0, BATCH_SIZE);
    // simulation->spawnCars (1*(SQUARE_SIZE+1)+0, BATCH_SIZE);
    printf (">> KeyPressed: %d more cars spawned\n", BATCH_SIZE);
    break;

  case 't':
    curr_mode = ((++curr_mode) % 3);
    printf (">> Toggle to %d:%s mode\n",curr_mode, modes[curr_mode]);
    break;

  case 'p':
    traffic_learner->print_to_file ();
    printf (">> Print Learned Table\n");
    break;
  }
}


void handleResize (int w, int h) {
  glViewport (0,0,w,h);
  glMatrixMode (GL_PROJECTION);

  glLoadIdentity ();
  gluPerspective (45.0, (double) w / (double) h, 0.0, 0.0);
}
