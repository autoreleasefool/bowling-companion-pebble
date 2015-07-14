#include <pebble.h>

#include "bowler_list.h"

static void init(void);
static void deinit(void);

int main(void) {
  init();
  app_event_loop();
  deinit();

  return 0;
}

static void init(void) {
  show_bowler_list();
}

static void deinit(void) {
  hide_bowler_list();
}
