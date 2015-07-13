#pragma once
#include <stdbool.h>
  
#define NEW_BOWLER "New bowler"
#define NEW_EVENT "New event"
#define NEW_SERIES "New series"
  
#define DEFAULT_BOWLER_NAME "Pebble bowler"
#define DEFAULT_EVENT_NAME "Pebble event"
#define DEFAULT_SERIES_NAME "Pebble series"

int get_value_of_frame(int frame, int ball, bool pin_state[]);
int get_value_of_frame_diff(int frame1, int ball1, int frame2, int ball2, bool pin_state[]);
bool is_frame_strike(int frame, int ball, bool pin_state[]);