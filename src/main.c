#include <pebble.h>
#include "bowler_list.h"

void init(void) {
  show_bowler_list();
}

void deinit(void) {
  hide_bowler_list();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
    
  return 0;
}