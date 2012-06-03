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

extern int curr_mode;

#ifdef OPENGL_MODE
void display (void) {
   /* Do timer processing */
   /* maybe glutPostRedisplay(), if necessary */
  if (!stopAnime) {

    if (curr_mode == 0)
      traffic_learner->naiveControl (simulation);
    else if (curr_mode == 1)
      traffic_learner->learn ();
    else 
      traffic_learner->comply ();

    //simulation->updateWorld();
    ppainter->draw ();
  }
}
void timerCallback (int value)
{
  display ();

   /* call back again after simulation_interval has passed */
  glutTimerFunc ( simulation_interval, timerCallback, 0);
}
#endif

int main (int argc, char* argv[])
{
  factory::create_world(&simulation);

  traffic_learner = new learner(simulation, ppainter);

#ifdef OPENGL_MODE
  ppainter = new painter(simulation, display, timerCallback, argc, argv);
  // ppainter->draw(); // Draw initial state
  ppainter->animate();
#else
  traffic_learner->learn();
  ppainter = new painter(simulation, display, NULL, argc, argv);
#endif

  return 0;
}
