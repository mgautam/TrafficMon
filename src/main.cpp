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


void display (void) {
   /* Do timer processing */
   /* maybe glutPostRedisplay(), if necessary */
  if (!stopDisplay)
    ppainter->draw ();


}

extern int learnTime;
extern int complyTime;

void timerCallback (int value)
{
  if (simulation->timestamp == 100000)
    {
      exit(0);
    }
  

  if (!stopAnime) {

    if (curr_mode == 0) {
      traffic_learner->naiveControl (simulation);
      if ( !(simulation->timestamp % PERFORMANCE_CALC_INTERVAL) )
      	traffic_learner->displayPerformance (PERFORMANCE_CALC_INTERVAL);
    }
    else if (curr_mode == 1)
      traffic_learner->learn ();
    else if (curr_mode == 2) {
      traffic_learner->comply ();
      if ( !(simulation->timestamp % PERFORMANCE_CALC_INTERVAL) )
	traffic_learner->displayPerformance (PERFORMANCE_CALC_INTERVAL);
    }
    else
    {

	if (learnTime > 0) 
	  {
	    traffic_learner->learn ();
	    learnTime--;
	    //printf ("learnTime:%d\n",learnTime);
	  }
	if (learnTime <= 0 && complyTime <=0)
	  complyTime = COMPLY_TIME;


	if (complyTime > 0)
	  {
	    traffic_learner->comply ();
	    complyTime--;
	    //printf ("complyTime:%d\n",complyTime);
	  }
	if (complyTime <= 0 && learnTime <=0) {
	  learnTime = LEARN_TIME;      
	  traffic_learner->displayPerformance (COMPLY_TIME);
	}     

    }
      
    //simulation->updateWorld();
    if (!stopDisplay) {
      ppainter->draw();
    }
  }
   /* call back again after simulation_interval has passed */
  glutTimerFunc ( simulation_interval, timerCallback, 0);
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
