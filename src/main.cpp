#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "config.h"
#include "world.h"

#include "bigbang.h"
#include "learner.h"



#ifdef OPENGL_MODE
   #ifdef __APPLE__
    #include <GLUT/glut.h>
   #else
    #include <GL/glut.h>
   #endif

   #include "painter.h"
   #include "input_device_handler.h"

#endif

extern world* simulation;
world *simulation;

extern learner *traffic_learner;
learner *traffic_learner;

extern int curr_mode;
int curr_mode = 1;

extern int simulation_interval;
int simulation_interval = 256;//msecs

extern bool stopAnime;
bool stopAnime = false;

extern bool fullSpeed;
bool fullSpeed = false;

extern bool displayMode;
bool displayMode = false;

extern int learnTime;
int learnTime = 0;

extern int complyTime;
int complyTime = COMPLY_TIME;

extern bool write_world;
extern bool load_world;

#ifdef OPENGL_MODE
   extern painter* ppainter;
   painter* ppainter;


 void display (void) {
     /* Do timer processing */
     /* maybe glutPostRedisplay(), if necessary */
    if (!fullSpeed)
      ppainter->draw ();
    else if (fullSpeed && displayMode)
      {
	usleep (simulation_interval*1000);
	ppainter->draw ();
      }

    glutPostRedisplay ();
  }

#endif


void *coreEngine (void *ptr) {

  while (true) {
    if (write_world) {
      bigbang::save_world (simulation);
      write_world = false;
    }
    else if (load_world) {
      bigbang::destroy_world (simulation);
      bigbang::recreate_world (simulation);
      load_world = false;
    }

    if (!fullSpeed)
      usleep (simulation_interval*1000);

    if (!stopAnime) {
      if (curr_mode == 0) {
	traffic_learner->naiveControl (simulation);
	if ( !(simulation->timestamp % PERFORMANCE_CALC_INTERVAL) )
	  traffic_learner->displayPerformance (PERFORMANCE_CALC_INTERVAL);
      }
      else if (curr_mode == 1) {
	traffic_learner->learn (fullSpeed);
      }
      else if (curr_mode == 2) {
	traffic_learner->comply ();
	if ( !(simulation->timestamp % PERFORMANCE_CALC_INTERVAL) )
	  traffic_learner->displayPerformance (PERFORMANCE_CALC_INTERVAL);
      }
      else
	{
	  
	  if (learnTime > 0) 
	    {
	      traffic_learner->learn (fullSpeed);
	      learnTime--;
	      //printf ("learnTime:%d\n",learnTime);
	    }
	  if (learnTime <= 0 && complyTime <=0) {
	    complyTime = COMPLY_TIME;
	    // write_world = true;//bigbang::save_world (simulation);
	  }
	  
	  
	  if (complyTime > 0)
	    {
	      traffic_learner->comply ();
	      complyTime--;
	      //printf ("complyTime:%d\n",complyTime);
	    }
	  if (complyTime <= 0 && learnTime <=0) {
	    learnTime = LEARN_TIME;
	    // load_world = true;   //bigbang::destroy_world (simulation);bigbang::load_world (simulation);
	    traffic_learner->displayPerformance (COMPLY_TIME);
	  }     
	  
	}
    }
    
  }
    
  return ptr;
}

int main (int argc, char* argv[])
{
  bigbang::create_world(&simulation);

  traffic_learner = new learner (simulation);

  #ifdef OPENGL_MODE
   pthread_t engineThread;
   int iret = pthread_create ( &engineThread, NULL, coreEngine, NULL);
   cout << "Thread Status:" << iret << endl;

   ppainter = new painter (simulation, display, argc, argv);
   //ppainter->draw(); // Draw initial state
   ppainter->animate ();

  #else
     coreEngine (NULL);
  #endif

  
  return 0;
}
