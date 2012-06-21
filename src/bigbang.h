#ifndef BIGBANG_H
#define BIGBANG_H

#include "world.h"

class bigbang
{
public:
  static void create_world (world** sim);
  static void destroy_world (world** sim);
  static void save_world (world** sim);
  static void recreate_world (world** sim);
};

#endif
