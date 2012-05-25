#include <stdio.h>
#include <GL/glut.h>

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

void drawScene (void) {
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();

  glColor3f (1.0f,1.0f,0.0f);

  glBegin (GL_QUADS);
  glVertex3f (0.0f, 0.0f, 0.0f);
  glVertex3f (0.5f, 1.0f, 0.0f);
  glVertex3f (0.5f, 0.0f, 0.0f);
  glVertex3f (0.0f, 1.0f, 0.0f);
  glEnd ();

  glutSwapBuffers ();
}

int viewWorld (int argc, char **argv) {

    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (1024,768);

    glutCreateWindow ("World");
    glEnable (GL_DEPTH_TEST);
    glutDisplayFunc (drawScene);
    glutKeyboardFunc (handleKeyPress);
    glutReshapeFunc (handleResize);

    glutMainLoop ();

  return 0;
}
