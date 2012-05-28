#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


#include "visualize.h"

bool stopAnime = false;

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
  }
}

void handleResize (int w, int h) {
  glViewport (0,0,w,h);
  glMatrixMode (GL_PROJECTION);

  glLoadIdentity ();
  gluPerspective (45.0, (double) w / (double) h, 0.0, 0.0);
}
