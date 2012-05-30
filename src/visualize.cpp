#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "common.h"
#include "visualize.h"
#include "world.h"
#include "painter.h"

bool stopAnime = false;

int simulation_interval = 256;

extern painter* ppainter;

void handleKeyPress  (unsigned char key, int x, int y) {
  switch (key) {
  case 32: // Space Bar 
    if (stopAnime) {
      stopAnime = false;
      printf ("Animation Resumed. Press SPACE on GL_WINDOW to pause");
    }
    else {
      stopAnime = true;
      printf ("Animation Paused. Press SPACE on GL_WINDOW to resume");
    }
    printf ("\n");
    break;

   case 27: // Quit //27
     printf ("Escape key pressed\n");
     exit (0);
     break;
     
  case 'k': // w -> Pan Above
    glMatrixMode (GL_PROJECTION);
    glOrtho (-1,1,-0.9,1.1,-1,1);
    break;

  case 'h': // a -> Pan Left
    glMatrixMode (GL_PROJECTION);
    glOrtho (-1.1,0.9,-1,1,-1,1);
    break;

  case 'l': // d -> Pan Right
    glMatrixMode (GL_PROJECTION);
    glOrtho (-0.9,1.1,-1,1,-1,1);
    break;

  case 'j': // s -> Pan Down
    glMatrixMode (GL_PROJECTION);
    glOrtho (-1,1,-1.1,0.9,-1,1);
    break;

  case 'x': // k -> Zoom out
    glMatrixMode (GL_PROJECTION);
    glOrtho (-2,2,-2,2,-1,1);
    break;

  case 'z': //j -> Zoom in
    glMatrixMode (GL_PROJECTION);
    glOrtho (-0.5,0.5,-0.5,0.5,-1,1);
    break;

  case 'r': //r -> reset View
    glMatrixMode (GL_PROJECTION);
    glPopMatrix ();
    glPushMatrix ();
    break;

  case '=': //n -> Increase Speed
    simulation_interval /= 2;
    printf ("NITROX %d powered by SEGA\n", simulation_interval);
    break;

  case '-': //m -> Decrease Speed
    simulation_interval *= 2;
    printf ("Damper %d\n", simulation_interval);
    break;

  case '.': // l -> lights
    printf ("L press\n");
    x = x + 1;
    break;

  case 's': // -> step to next unit in time dimension
    stopAnime = true;
    ppainter->display();
    //    ppainter->timerCallback(0);
    printf ("Key Pressed: Step to next unit in time dimension\n");
    break;

  }
}


void handleResize (int w, int h) {
  glViewport (0,0,w,h);
  glMatrixMode (GL_PROJECTION);

  glLoadIdentity ();
  gluPerspective (45.0, (double) w / (double) h, 0.0, 0.0);
}
