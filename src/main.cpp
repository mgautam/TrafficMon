#include <stdio.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "config.h"
#include "world.h"
#include "painter.h"
#include "input_device_handler.h"
#include "factory.h"

static world* simulation;
extern painter* ppainter;
painter* ppainter;

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
  glutTimerFunc ( simulation_interval, timerCallback, 0);
}

int main (int argc, char* argv[])
{

  factory::create_world(&simulation);

  simulation->write_state(stdout);
  ppainter = new painter(simulation, display, timerCallback, argc, argv);
  ppainter->draw();
  // ppainter->animate();


  while (true)
    {
      simulation->updateWorld();
      
    }
  
  return 0;
}
