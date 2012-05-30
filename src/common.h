#ifndef COMMON_H
#define COMMON_H

#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500

#define RIGHT 0
#define LEFT  1
#define AHEAD 2

#define RED 0
#define AMBER 1
#define GREEN 2

#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

#define NORTHSOUTH_AHEADLEFT 0
#define NORTHSOUTH_AL_AMBER 1
#define EASTWEST_AHEADLEFT 2
#define EASTWEST_AL_AMBER 3
#define NORTHSOUTH_RIGHT 4
#define NORTHSOUTH_R_AMBER 5
#define EASTWEST_RIGHT 6
#define EASTWEST_R_AMBER 7

#define MIN_TL_SWITCH_INTERVAL 3

#define STEP_SIZE 0.1

// Intersection
#define  roadSideOffset 10
#define halfIntersectionLen roadSideOffset

// Road
#define lenBWlanes 0.05
#define lenBWlights 0.05
#define LightSize 0.5

// Car
#define CarLen  (roadSideOffset-lenBWlanes)
#define halfCarLen CarLen/2

// Don't change this until I come up with a fix
#define MIN_INTER_CAR_SPACE 0.0f;



// (3*halfCarLen)



#endif
