#ifndef CONFIG_H
#define CONFIG_H

// CONTROL
#define BATCH_SIZE 50

#define MIN_TL_SWITCH_INTERVAL 10

//#define STEP_SIZE 0.1

#define NUM_SLOTS_IN_ROAD 10
#define NUM_TRAFFIC_PATTERNS 4
#define SQUARE_SIZE 1



// VIEW Parameters
#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500
#define MARGIN_PADDING 1


// Road
#define ROAD_WIDTH 11
//50
#define HLEN_BW_LANES 0
// 0.05*ROAD_WIDTH

// Traffic Lights
#define HLEN_BW_LIGHTS 0.05*ROAD_WIDTH
#define TL_LIGHT_SIZE ROAD_WIDTH

// Intersection
#define INTERSECTION_SIZE 2*(ROAD_WIDTH+HLEN_BW_LANES)

// Car
// If you change car length it changes road length in visualization
#define CAR_LENGTH  10
#define CAR_WIDTH 11
//30
#define HCAR_LENGTH CAR_LENGTH/2
#define HCAR_WIDTH CAR_WIDTH/2

#define MIN_INTER_CAR_SPACE 1.0f
// (3*halfCarLen)

#define ROAD_SCALE (CAR_LENGTH+MIN_INTER_CAR_SPACE)



// MODEL Parameters
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
#define NORTHSOUTH_AL_AMBER 4
#define EASTWEST_AHEADLEFT 1
#define EASTWEST_AL_AMBER 5
#define NORTHSOUTH_RIGHT 2
#define NORTHSOUTH_R_AMBER 6
#define EASTWEST_RIGHT 3
#define EASTWEST_R_AMBER 7

#endif
