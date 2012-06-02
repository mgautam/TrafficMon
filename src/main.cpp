#include <stdio.h>

#include <GL/glut.h>

#include "config.h"
#include "world.h"
#include "painter.h"
#include "input_device_handler.h"
#include "factory.h"
#include "learner.h"

#define OPENGL_MODE 1

extern world* simulation;
world *simulation;

extern painter* ppainter;
painter* ppainter;

static learner *traffic_learner;
static int performance = 0;

void draw (void)
{
  traffic_learner->naiveControl (simulation);
  simulation->updateWorld();

  // performance += traffic_learner->evaluate (simulation->intersections,simulation->intc
  // 					    );
  // if (simulation->timestamp % 1000 == 0) {
  //   printf ("Performance in last 1000 time steps is %d\n", performance);
  //   performance = 0;
  // }

  ppainter->draw();


}

#ifdef OPENGL_MODE
void timerCallback (int value)
{
   /* Do timer processing */
   /* maybe glutPostRedisplay(), if necessary */
  if (!stopAnime) {
    draw ();
  }

   /* call back again after simulation_interval has passed */
  glutTimerFunc ( simulation_interval, timerCallback, 0);
}
#endif

int main (int argc, char* argv[])
{
  factory::create_world(&simulation);

#ifdef OPENGL_MODE
  ppainter = new painter(simulation, draw, timerCallback, argc, argv);
  // ppainter->draw(); // Draw initial state
  ppainter->animate();
#else
  ppainter = new painter(simulation, draw, NULL, argc, argv);
  traffic_learner = new learner(simulation, ppainter);
  traffic_learner->learn();
#endif

  return 0;
}
