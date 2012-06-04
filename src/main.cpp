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

extern bool stopDisplay;

void nonDisplay (void);

void display (void) {
   /* Do timer processing */
   /* maybe glutPostRedisplay(), if necessary */
  if (!stopDisplay)
    ppainter->draw ();


}
void timerCallback (int value)
{
  if (!stopAnime) {

    if (curr_mode == 0)
      traffic_learner->naiveControl (simulation);
    else if (curr_mode == 1)
      traffic_learner->learn ();
    else if (curr_mode == 2)
      traffic_learner->comply ();
    else {
      traffic_learner->learn ();
      traffic_learner->comply ();
    }

    //simulation->updateWorld();
    if (!stopDisplay) {
      ppainter->draw();
    }
    // else {
    //   nonDisplay ();
    // }
  }
   /* call back again after simulation_interval has passed */
  glutTimerFunc ( simulation_interval, timerCallback, 0);
}

void nonDisplay (void)
{
  while (true) {
    if (!stopAnime) {
      
      if (curr_mode == 0)
	traffic_learner->naiveControl (simulation);
      else if (curr_mode == 1)
	traffic_learner->learn ();
      else if (curr_mode == 2)
	traffic_learner->comply ();
      else {
	traffic_learner->learn ();
	traffic_learner->comply ();
      }
	
    }
    if (!stopAnime)
      timerCallback (0);
  }
}



int main (int argc, char* argv[])
{
  factory::create_world(&simulation);

  traffic_learner = new learner(simulation, ppainter);

#ifdef OPENGL_MODE
  ppainter = new painter(simulation, display, timerCallback, argc, argv);
  //ppainter->draw(); // Draw initial state
  ppainter->animate();
#else
  traffic_learner->learn();
  ppainter = new painter(simulation, display, NULL, argc, argv);
#endif

  return 0;
}
