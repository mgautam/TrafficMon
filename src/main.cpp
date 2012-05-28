#include "common.h"
#include "world.h"
#include "visualize.h"
#include "painter.h"
#include "factory.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h>


static world* simulation;
static painter* ppainter;


void display(void)
{
  simulation->updateWorld();
  ppainter->draw();
  simulation->write_state(stdout,true);
}

void timerCallback (int value)
{
   /* Do timer processing */
   /* maybe glutPostRedisplay(), if necessary */
  if (!stopAnime)
    display();
   /* call back again after elapsedUSecs have passed */
  glutTimerFunc (SIMULATION_INTERVAL, timerCallback, 0);
}

int main (int argc, char* argv[])
{
  factory::create_world(&simulation);

  //simulation->write_state(stdout);
  ppainter = new painter(simulation, display, timerCallback, argc, argv);
  ppainter->draw();
  ppainter->animate();

  return 0;
}
