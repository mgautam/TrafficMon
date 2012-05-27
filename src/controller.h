#ifndef CONTROLLER_H
#define CONTROLLER_H 1



#include "common.h"
#include "intersection.h"
#include "road.h"
#include <iostream>

class controller
{
public:
  controller(intersection* node)
  {
    this->node = node;
    

    east = west = south = north = 0;
    for (int i = 0; i < node->in_count; i++)
      {
	if (this->node->in[i]->init->x > node->x)
	  this->east = this->node->in[i];
	if (this->node->in[i]->init->x < node->x)
	  this->west = this->node->in[i];
	if (this->node->in[i]->init->y > node->y)
	  this->south = this->node->in[i];
	if (this->node->in[i]->init->y < node->y)
	  this->north = this->node->in[i];
      }
  }

  void switch_mode(int mode)
  {

  }
  

  
  intersection* node;

  road* north;
  road* south;
  road* east;
  road* west;

};

#endif
