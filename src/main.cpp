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

#include "learner.h"

extern world* simulation;
world *simulation;

extern painter* ppainter;
painter* ppainter;

static learner *TrafficLearner;
static int performance = 0;

void runSimulation (void)
{
  TrafficLearner->naiveControl (simulation);
  simulation->updateWorld();
  //simulation->write_state(stdout,true);

  performance += TrafficLearner->evaluate (simulation->intersections,simulation->intc);
  if (simulation->timestamp % 1000 == 0) {
    printf ("Performance in last 1000 time steps is %d\n", performance);
    performance = 0;
  }
}

#ifdef OPENGL_MODE
void timerCallback (int value)
{
   /* Do timer processing */
   /* maybe glutPostRedisplay(), if necessary */
  if (!stopAnime) {
    runSimulation ();
    ppainter->draw();
  }

   /* call back again after simulation_interval has passed */
  glutTimerFunc ( simulation_interval, timerCallback, 0);
}
#endif

int main (int argc, char* argv[])
{

  factory::create_world(&simulation);

  TrafficLearner = new learner ();
 
  //simulation->write_state(stdout); // Write Initial State


#ifndef OPENGL_MODE
  while (true)
  {
    runSimulation ();
  }
#else
  ppainter = new painter(simulation, runSimulation, timerCallback, argc, argv);
  ppainter->draw(); // Draw initial state
  ppainter->animate();
#endif

  return 0;
}
