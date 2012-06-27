#ifndef CONFIG_H
#define CONFIG_H

// CONTROL
#define BATCH_SIZE 9

#define MIN_TL_AR_SWITCH_INTERVAL 3 /* minimum is 2, Cars will dissappear if this is below 2 */
#define MIN_TL_PATTERN_SWITCH_INTERVAL 5 /* minimum is above MIN_TL_AR_SWITCH_INTERVAL, haven't checked yet below it */


//#define STEP_SIZE 0.1

#define NUM_SLOTS_IN_ROAD 20
#define NUM_TRAFFIC_PATTERNS 4
#define SQUARE_SIZE 2
#define NUM_LANES_PER_ROAD 2

#define MAX_SLOTS_TO_CHECK 6
#define LEARNING_RATE 0.2

#define LEARN_TIME 9900
#define COMPLY_TIME 100
#define PERFORMANCE_CALC_INTERVAL 100



// VIEW Parameters
#define OPENGL_MODE 1
#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500
#define MARGIN_PADDING 1


// Road
#define ROAD_WIDTH 20
#define LANE_WIDTH ROAD_WIDTH/NUM_LANES_PER_ROAD
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
#define CAR_WIDTH LANE_WIDTH
//30
#define HCAR_LENGTH CAR_LENGTH/2
#define HCAR_WIDTH CAR_WIDTH/2

#define MIN_INTER_CAR_SPACE 1.0f
// (3*halfCarLen)

#define ROAD_SCALE (CAR_LENGTH+MIN_INTER_CAR_SPACE)

#define MAX_WAIT_AT_SIGNAL 18

// MODEL Parameters
#define RIGHT 0
#define LEFT  1
#define AHEAD 2
#define UTURN 3

#define RED 0
#define AMBER 1
#define GREEN 2

#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

#define NORTHSOUTH_AHEADLEFT 0
#define EASTWEST_AHEADLEFT 1
#define NORTHSOUTH_RIGHT 2
#define EASTWEST_RIGHT 3
#define NORTHSOUTH_AL_AMBER 4
#define EASTWEST_AL_AMBER 5
#define NORTHSOUTH_R_AMBER 6
#define EASTWEST_R_AMBER 7

#endif
