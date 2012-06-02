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

#define OPENGL_MODE 1

extern world* simulation;
world *simulation;

extern painter* ppainter;
painter* ppainter;

extern learner *traffic_learner;
learner *traffic_learner;
//static int performance = 0;

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
    traffic_learner->glLearn ();
    //traffic_learner->naiveControl (simulation);
    simulation->updateWorld();
    ppainter->draw ();
  }

   /* call back again after simulation_interval has passed */
  glutTimerFunc ( simulation_interval, timerCallback, 0);
}
#endif

int main (int argc, char* argv[])
{
  factory::create_world(&simulation);

  traffic_learner = new learner(simulation, ppainter);

#ifdef OPENGL_MODE
  ppainter = new painter(simulation, draw, timerCallback, argc, argv);
  // ppainter->draw(); // Draw initial state
  ppainter->animate();
#else
  traffic_learner->learn();
  ppainter = new painter(simulation, draw, NULL, argc, argv);
#endif

  return 0;
}
