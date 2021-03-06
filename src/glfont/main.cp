#include <GLUT/glut.h>
#include "glfont.h"

int main(int argc, char* argv[])
{
  glutInit (&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize (WINDOW_WIDTH,WINDOW_HEIGHT);

  glutCreateWindow ("World View");
  glDisable (GL_DEPTH_TEST);
  glutDisplayFunc (simulate);
  glutKeyboardFunc (handleKeyPress);
  glutReshapeFunc (handleResize);

  //  glClearColor (1.0f,1.0f,1.0f,0.0f);                                                                                                                                          

  glMatrixMode (GL_PROJECTION);
  glPushMatrix ();

  glutTimerFunc (simulation_interval, timerCallback, 0);

	//Initialize OpenGL
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//Initialize the viewport
	glViewport(0, 0, 640, 480);

	//Initialize OpenGL projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, 640.0f, 480.0f, 0.0f, -2.0f, 2.0f);

	//Clear back buffer
	glClear(GL_COLOR_BUFFER_BIT);

	//Create font
	GLuint textureName;
	glGenTextures(1, &textureName);
	PixelPerfectGLFont font;
	try {
		font.Create("fonts/quad20.glf", textureName);
	}
	catch(GLFontError::InvalidFile) {
		cerr << "Cannot load font\n";
		abort();
	}

	//Draw some stuff
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity( );
	glEnable(GL_TEXTURE_2D);
	glColor3f(1.0f, 0.0f, 0.0f);

	try {
		font.Begin();
		font.TextOut("hello world", 50, 50, 0);;
	}
	catch(GLFontError::InvalidFont) {
		cerr << "Trying to draw with an uninitialized font\n";
		abort();
	}
	glDisable(GL_TEXTURE_2D);
	glLoadIdentity();

}
