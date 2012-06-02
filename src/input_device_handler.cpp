#include <stdio.h>
#include <stdlib.h>

#include <GL/glut.h>

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
    glOrtho (-2,2,-2,2,-1,1);
    printf (">> Zooming Out\n");
    break;

  case 'z':
    glMatrixMode (GL_PROJECTION);
    glOrtho (-0.5,0.5,-0.5,0.5,-1,1);
    printf (">> Zooming in");
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
    //traffic_learner->naiveControl (simulation);
    traffic_learner->glLearn ();
    simulation->updateWorld ();
    
    //    ppainter->simulate();
    ppainter->draw ();
    printf (">> Key Pressed: Step to next unit in time dimension\n");
    break;

  case 'g':
    simulation->spawnCars (0, BATCH_SIZE);
    simulation->spawnCars (1, BATCH_SIZE);
    printf (">> KeyPressed: %d Cars spawned\n", BATCH_SIZE);
    break;

  }
}


void handleResize (int w, int h) {
  glViewport (0,0,w,h);
  glMatrixMode (GL_PROJECTION);

  glLoadIdentity ();
  gluPerspective (45.0, (double) w / (double) h, 0.0, 0.0);
}
