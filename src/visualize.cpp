#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "visualize.h"

void handleKeyPress  (unsigned char key, int x, int y) {
  switch (key) {
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
