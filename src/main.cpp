#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//#ifdef OPENGL_MODE

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "painter.h"
#include "input_device_handler.h"

//#endif

#include "config.h"
#include "world.h"

#include "factory.h"
#include "learner.h"


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

  glutPostRedisplay ();
}

extern int learnTime;
extern int complyTime;

void *coreEngine (void* ptr) {
  sleep (1); // Without this sleep I get seg fault! I don't know why

  while (true) {
    usleep (simulation_interval*1000);
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
	    //	    traffic_learner->displayPerformance (COMPLY_TIME);
	  }     
	  
	}
    }
  }

}

int main (int argc, char* argv[])
{
  pthread_t engineThread;
  int value;
  int iret = pthread_create ( &engineThread, NULL, coreEngine, (void*) value);

  factory::create_world(&simulation);

  traffic_learner = new learner(simulation);//, ppainter);

  ppainter = new painter(simulation, display, argc, argv);
  //ppainter->draw(); // Draw initial state
  ppainter->animate();


  
  return 0;
}
