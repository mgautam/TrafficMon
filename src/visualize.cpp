#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "common.h"
#include "visualize.h"

bool stopAnime = false;
int simulation_interval = 256;

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
    //    stopAnime = ~stopAnime;
    //    printf ("stopAnime = %d\n",stopAnime);
    break;

   case 27: // Escape Key
     printf ("Escape key pressed\n");
     exit (0);
     break;
     

  case 119: // w
    glMatrixMode (GL_PROJECTION);
    glOrtho (-1,1,-0.9,1.1,-1,1);
    break;

  case 97: // a
    glMatrixMode (GL_PROJECTION);
    glOrtho (-1.1,0.9,-1,1,-1,1);
    //zoom in
    break;

  case 100: // d
    glMatrixMode (GL_PROJECTION);
    glOrtho (-0.9,1.1,-1,1,-1,1);
    //zoom in
    break;

  case 115: // s
    glMatrixMode (GL_PROJECTION);
    glOrtho (-1,1,-1.1,0.9,-1,1);
    //zoom in
    break;

  case 120: // x
    glMatrixMode (GL_PROJECTION);
    glOrtho (-2,2,-2,2,-1,1);
    //zoom out
    break;

  case 122: //z
    glMatrixMode (GL_PROJECTION);
    glOrtho (-0.5,0.5,-0.5,0.5,-1,1);
    //zoom in
    
    break;

  case 110: //n- NITRO
    printf ("NITROX %d powered by SEGA\n", simulation_interval);
    simulation_interval /= 2;
    break;

  case 112: //p-POWER
    printf ("Damper %d\n", simulation_interval);
    simulation_interval *= 2;
    break;

  }
}


void handleResize (int w, int h) {
  glViewport (0,0,w,h);
  glMatrixMode (GL_PROJECTION);

  glLoadIdentity ();
  gluPerspective (45.0, (double) w / (double) h, 0.0, 0.0);
}
