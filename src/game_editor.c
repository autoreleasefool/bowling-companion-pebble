#include <pebble.h>
#include "game_editor.h"
#include "utils.h"
  
static uint8_t s_current_pin = 0;
static bool s_pin_knocked[5] = {false, false, false, false, false};
static PropertyAnimation *s_property_animation;
  
static GBitmap *s_indicator_bitmap;
static GBitmap *s_background_bitmap;
#ifdef PBL_PLATFORM_APLITE
static GBitmap *s_pin_enabled_bitmap_black, *s_pin_enabled_bitmap_white;
static GBitmap *s_pin_disabled_bitmap_black, *s_pin_disabled_bitmap_white;
#elif PBL_PLATFORM_BASALT
static GBitmap *s_pin_enabled, *s_pin_disabled;
#endif

// game values
static char* bowler_name;
static char* league_name;
static char* series_name;
static char* game_title;
static uint8_t currentGame = 0;

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GFont s_res_gothic_18_bold;
static GFont s_res_gothic_14;
static TextLayer *s_textlayer_gamenum;
static TextLayer *s_textlayer_league;
static Layer *s_layer_frame_bg;
static TextLayer *s_textlayer_f1_score;
static TextLayer *s_textlayer_f2_score;
static TextLayer *s_textlayer_f3_score;
static TextLayer *s_textlayer_f1_b1;
static TextLayer *s_textlayer_f1_b2;
static TextLayer *s_textlayer_f1_b3;
static TextLayer *s_textlayer_f2_b1;
static TextLayer *s_textlayer_f2_b2;
static TextLayer *s_textlayer_f2_b3;
static TextLayer *s_textlayer_f3_b1;
static TextLayer *s_textlayer_f3_b2;
static TextLayer *s_textlayer_f3_b3;
static BitmapLayer *s_bitmap_pin_0;
static BitmapLayer *s_bitmap_pin_1;
static BitmapLayer *s_bitmap_pin_2;
static BitmapLayer *s_bitmap_pin_3;
static BitmapLayer *s_bitmap_pin_4;
static BitmapLayer *s_bitmap_indicator;
static BitmapLayer *s_bitmap_background;

static void initialise_ui(void) {
  s_window = window_create();
  #ifndef PBL_SDK_3
    window_set_fullscreen(s_window, true);
  #endif
    
  s_indicator_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_INDICATOR);
  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);
  #ifdef PBL_PLATFORM_APLITE
    s_pin_enabled_bitmap_black = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_PIN_ENABLED);
    s_pin_enabled_bitmap_white = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_PIN_ENABLED);
    s_pin_disabled_bitmap_black = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_PIN_DISABLED);
    s_pin_disabled_bitmap_white = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_PIN_DISABLED);
  #elif PBL_PLATFORM_BASALT
    s_pin_enabled = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_PIN_ENABLED);
    s_pin_disabled = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_PIN_DISABLED);
  #endif
    
  s_bitmap_background = bitmap_layer_create(GRect(0, 0, 144, 168));
  bitmap_layer_set_bitmap(s_bitmap_background, s_background_bitmap);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_bitmap_background);
  
  s_res_gothic_18_bold = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
  s_res_gothic_14 = fonts_get_system_font(FONT_KEY_GOTHIC_14);
  // s_textlayer_gamenum
  s_textlayer_gamenum = text_layer_create(GRect(5, 5, 100, 20));
  text_layer_set_text(s_textlayer_gamenum, "Game #");
  text_layer_set_font(s_textlayer_gamenum, s_res_gothic_18_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_gamenum);
  
  // s_textlayer_league
  s_textlayer_league = text_layer_create(GRect(5, 25, 100, 20));
  text_layer_set_text(s_textlayer_league, league_name);
  text_layer_set_font(s_textlayer_league, s_res_gothic_14);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_league);
  
  // s_layer_frame_bg
  s_layer_frame_bg = layer_create(GRect(0, 45, 144, 36));
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_layer_frame_bg);
  
  // s_textlayer_f1_score
  s_textlayer_f1_score = text_layer_create(GRect(0, 63, 47, 15));
  text_layer_set_text(s_textlayer_f1_score, "999");
  text_layer_set_text_alignment(s_textlayer_f1_score, GTextAlignmentCenter);
  text_layer_set_font(s_textlayer_f1_score, s_res_gothic_14);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_f1_score);
  
  // s_textlayer_f2_score
  s_textlayer_f2_score = text_layer_create(GRect(48, 63, 48, 15));
  text_layer_set_text(s_textlayer_f2_score, "999");
  text_layer_set_text_alignment(s_textlayer_f2_score, GTextAlignmentCenter);
  text_layer_set_font(s_textlayer_f2_score, s_res_gothic_14);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_f2_score);
  
  // s_textlayer_f3_score
  s_textlayer_f3_score = text_layer_create(GRect(97, 63, 47, 15));
  text_layer_set_text(s_textlayer_f3_score, "999");
  text_layer_set_text_alignment(s_textlayer_f3_score, GTextAlignmentCenter);
  text_layer_set_font(s_textlayer_f3_score, s_res_gothic_14);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_f3_score);
  
  // s_textlayer_f1_b1
  s_textlayer_f1_b1 = text_layer_create(GRect(0, 45, 15, 15));
  text_layer_set_text(s_textlayer_f1_b1, "Hp");
  text_layer_set_text_alignment(s_textlayer_f1_b1, GTextAlignmentCenter);
  text_layer_set_font(s_textlayer_f1_b1, s_res_gothic_14);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_f1_b1);
  
  // s_textlayer_f1_b2
  s_textlayer_f1_b2 = text_layer_create(GRect(16, 45, 15, 15));
  text_layer_set_text(s_textlayer_f1_b2, "Hp");
  text_layer_set_text_alignment(s_textlayer_f1_b2, GTextAlignmentCenter);
  text_layer_set_font(s_textlayer_f1_b2, s_res_gothic_14);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_f1_b2);
  
  // s_textlayer_f1_b3
  s_textlayer_f1_b3 = text_layer_create(GRect(32, 45, 15, 15));
  text_layer_set_text(s_textlayer_f1_b3, "Hp");
  text_layer_set_text_alignment(s_textlayer_f1_b3, GTextAlignmentCenter);
  text_layer_set_font(s_textlayer_f1_b3, s_res_gothic_14);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_f1_b3);
  
  // s_textlayer_f2_b1
  s_textlayer_f2_b1 = text_layer_create(GRect(48, 45, 15, 15));
  text_layer_set_text(s_textlayer_f2_b1, "Hp");
  text_layer_set_text_alignment(s_textlayer_f2_b1, GTextAlignmentCenter);
  text_layer_set_font(s_textlayer_f2_b1, s_res_gothic_14);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_f2_b1);
  
  // s_textlayer_f2_b2
  s_textlayer_f2_b2 = text_layer_create(GRect(64, 45, 15, 15));
  text_layer_set_text(s_textlayer_f2_b2, "Hp");
  text_layer_set_font(s_textlayer_f2_b2, s_res_gothic_14);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_f2_b2);
  
  // s_textlayer_f2_b3
  s_textlayer_f2_b3 = text_layer_create(GRect(80, 45, 15, 15));
  text_layer_set_text(s_textlayer_f2_b3, "Hp");
  text_layer_set_font(s_textlayer_f2_b3, s_res_gothic_14);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_f2_b3);
  
  // s_textlayer_f3_b1
  s_textlayer_f3_b1 = text_layer_create(GRect(96, 45, 15, 15));
  text_layer_set_text(s_textlayer_f3_b1, "Hp");
  text_layer_set_font(s_textlayer_f3_b1, s_res_gothic_14);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_f3_b1);
  
  // s_textlayer_f3_b2
  s_textlayer_f3_b2 = text_layer_create(GRect(112, 45, 15, 15));
  text_layer_set_text(s_textlayer_f3_b2, "Hp");
  text_layer_set_font(s_textlayer_f3_b2, s_res_gothic_14);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_f3_b2);
  
  // s_textlayer_f3_b3
  s_textlayer_f3_b3 = text_layer_create(GRect(128, 45, 15, 15));
  text_layer_set_text(s_textlayer_f3_b3, "Hp");
  text_layer_set_font(s_textlayer_f3_b3, s_res_gothic_14);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_f3_b3);
  
  // s_bitmap_pin_0
  s_bitmap_pin_0 = bitmap_layer_create(GRect(0, 92, 28, 60));
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_bitmap_pin_0);
  
  // s_bitmap_pin_1
  s_bitmap_pin_1 = bitmap_layer_create(GRect(29, 92, 28, 60));
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_bitmap_pin_1);
  
  // s_bitmap_pin_2
  s_bitmap_pin_2 = bitmap_layer_create(GRect(58, 92, 28, 60));
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_bitmap_pin_2);
  
  // s_bitmap_pin_3
  s_bitmap_pin_3 = bitmap_layer_create(GRect(87, 92, 28, 60));
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_bitmap_pin_3);
  
  // s_bitmap_pin_4
  s_bitmap_pin_4 = bitmap_layer_create(GRect(116, 92, 28, 60));
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_bitmap_pin_4);
  
  // s_bitmap_indicator
  s_bitmap_indicator = bitmap_layer_create(GRect(0, 153, 28, 15));
  bitmap_layer_set_bitmap(s_bitmap_indicator, s_indicator_bitmap);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_bitmap_indicator);
  
  #ifdef PBL_PLATFORM_APLITE
    bitmap_layer_set_bitmap(s_bitmap_pin_0, s_pin_enabled_bitmap_white);
    bitmap_layer_set_bitmap(s_bitmap_pin_1, s_pin_enabled_bitmap_white);
    bitmap_layer_set_bitmap(s_bitmap_pin_2, s_pin_enabled_bitmap_white);
    bitmap_layer_set_bitmap(s_bitmap_pin_3, s_pin_enabled_bitmap_white);
    bitmap_layer_set_bitmap(s_bitmap_pin_4, s_pin_enabled_bitmap_white);
    bitmap_layer_set_compositing_mode(s_bitmap_pin_0, GCompOpAnd);
    bitmap_layer_set_compositing_mode(s_bitmap_pin_1, GCompOpAnd);
    bitmap_layer_set_compositing_mode(s_bitmap_pin_2, GCompOpAnd);
    bitmap_layer_set_compositing_mode(s_bitmap_pin_3, GCompOpAnd);
    bitmap_layer_set_compositing_mode(s_bitmap_pin_4, GCompOpAnd);
    bitmap_layer_set_compositing_mode(s_bitmap_indicator, GCompOpAnd);
  #elif PBL_PLATFORM_BASALT
    bitmap_layer_set_bitmap(s_bitmap_pin_0, s_pin_enabled);
    bitmap_layer_set_bitmap(s_bitmap_pin_1, s_pin_enabled);
    bitmap_layer_set_bitmap(s_bitmap_pin_2, s_pin_enabled);
    bitmap_layer_set_bitmap(s_bitmap_pin_3, s_pin_enabled);
    bitmap_layer_set_bitmap(s_bitmap_pin_4, s_pin_enabled);
    bitmap_layer_set_compositing_mode(s_bitmap_pin_0, GCompOpSet);
    bitmap_layer_set_compositing_mode(s_bitmap_pin_1, GCompOpSet);
    bitmap_layer_set_compositing_mode(s_bitmap_pin_2, GCompOpSet);
    bitmap_layer_set_compositing_mode(s_bitmap_pin_3, GCompOpSet);
    bitmap_layer_set_compositing_mode(s_bitmap_pin_4, GCompOpSet);
    bitmap_layer_set_compositing_mode(s_bitmap_indicator, GCompOpSet);
  #endif
}

static void destroy_ui(void) {
  window_destroy(s_window);
  text_layer_destroy(s_textlayer_gamenum);
  text_layer_destroy(s_textlayer_league);
  layer_destroy(s_layer_frame_bg);
  text_layer_destroy(s_textlayer_f1_score);
  text_layer_destroy(s_textlayer_f2_score);
  text_layer_destroy(s_textlayer_f3_score);
  text_layer_destroy(s_textlayer_f1_b1);
  text_layer_destroy(s_textlayer_f1_b2);
  text_layer_destroy(s_textlayer_f1_b3);
  text_layer_destroy(s_textlayer_f2_b1);
  text_layer_destroy(s_textlayer_f2_b2);
  text_layer_destroy(s_textlayer_f2_b3);
  text_layer_destroy(s_textlayer_f3_b1);
  text_layer_destroy(s_textlayer_f3_b2);
  text_layer_destroy(s_textlayer_f3_b3);
  bitmap_layer_destroy(s_bitmap_pin_0);
  bitmap_layer_destroy(s_bitmap_pin_1);
  bitmap_layer_destroy(s_bitmap_pin_2);
  bitmap_layer_destroy(s_bitmap_pin_3);
  bitmap_layer_destroy(s_bitmap_pin_4);
  bitmap_layer_destroy(s_bitmap_indicator);
  bitmap_layer_destroy(s_bitmap_background);
  
  gbitmap_destroy(s_indicator_bitmap);
  gbitmap_destroy(s_background_bitmap);
  #ifdef PBL_PLATFORM_APLITE
    gbitmap_destroy(s_pin_enabled_bitmap_black);
    gbitmap_destroy(s_pin_enabled_bitmap_white);
    gbitmap_destroy(s_pin_disabled_bitmap_black);
    gbitmap_destroy(s_pin_disabled_bitmap_white);
  #elif PBL_PLATFORM_BASALT
    gbitmap_destroy(s_pin_enabled);
    gbitmap_destroy(s_pin_disabled);
  #endif
}
// END AUTO-GENERATED UI CODE

static void on_animation_stopped(Animation *anim, bool finished, void *context)
{
  //Free the memory used by the Animation
  property_animation_destroy((PropertyAnimation*) anim);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Animation destroyed");
}

static void set_game(uint8_t newGame) {
  currentGame = newGame + 1;
  const char* game_boiler = "Game #";
  const char* game_num = itoa(currentGame);

  if (game_title) {
    free(game_title);
    game_title = NULL;
  }
  game_title = (char*) malloc(strlen(game_boiler) + strlen(game_num) + 1);
  strcpy(game_title, game_boiler);
  strcat(game_title, game_num);
  text_layer_set_text(s_textlayer_gamenum, game_title);
  currentGame--;
}

static void update_pin_status(BitmapLayer *pin_bitmap, int8_t pin_to_update, bool knocked) {
  if (s_pin_knocked[pin_to_update] == knocked)
    return;
  
  s_pin_knocked[pin_to_update] = knocked;
  if (knocked) {
    #ifdef PBL_PLATFORM_APLITE
      bitmap_layer_set_bitmap(pin_bitmap, s_pin_disabled_bitmap_white);
    #elif PBL_PLATFORM_BASALT
      bitmap_layer_set_bitmap(pin_bitmap, s_pin_disabled);
    #endif
  } else {
    #ifdef PBL_PLATFORM_APLITE
      bitmap_layer_set_bitmap(pin_bitmap, s_pin_enabled_bitmap_white);
    #elif PBL_PLATFORM_BASALT
      bitmap_layer_set_bitmap(pin_bitmap, s_pin_enabled);
    #endif
  }
}

static void update_indicator_position(void) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Animation broken");
  Layer *root_indicator_layer = bitmap_layer_get_layer(s_bitmap_indicator);
  GRect to_frame = GRect(0, 0, 0, 0);
  GRect from_frame = layer_get_frame(root_indicator_layer);
  switch (s_current_pin) {
    case 0:
      to_frame = GRect(0, 153, 28, 15);
      break;
    case 1:
      to_frame = GRect(29, 153, 28, 15);
      break;
    case 2:
      to_frame = GRect(58, 153, 28, 15);
      break;
    case 3:
      to_frame = GRect(87, 153, 28, 15);
      break;
    case 4:
      to_frame = GRect(116, 153, 28, 15);
      break;
  }
  
  AnimationHandlers handlers = {
    //The reference to the stopped handler is the only one in the array
    .stopped = (AnimationStoppedHandler) on_animation_stopped
  };
  
  s_property_animation = property_animation_create_layer_frame(root_indicator_layer, &from_frame, &to_frame);
  animation_set_duration((Animation*) s_property_animation, 400);
  animation_set_handlers((Animation*) s_property_animation, handlers, NULL);
  animation_schedule((Animation*) s_property_animation);
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (s_current_pin == 0)
    s_current_pin = 4;
  else
    s_current_pin--;
  update_indicator_position();
}

static void up_long_click_handler(ClickRecognizerRef recognizer, void *context) {
  // does nothing
}

static void up_long_click_release_handler(ClickRecognizerRef recognizer, void *context) {
  // TODO: prev frame
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  s_current_pin++;
  if (s_current_pin == 5)
    s_current_pin = 0;
  update_indicator_position();
}

static void down_long_click_handler(ClickRecognizerRef recognizer, void *context) {
  // does nothing
}

static void down_long_click_release_handler(ClickRecognizerRef recognizer, void *context) {
  // TODO: next frame
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  BitmapLayer *active_pin = NULL;
  
  switch (s_current_pin) {
    case 0:
      active_pin = s_bitmap_pin_0;
      break;
    case 1:
      active_pin = s_bitmap_pin_1;
      break;
    case 2:
      active_pin = s_bitmap_pin_2;
      break;
    case 3:
      active_pin = s_bitmap_pin_3;
      break;
    case 4:
      active_pin = s_bitmap_pin_4;
      break;
  }

  update_pin_status(active_pin, s_current_pin, !s_pin_knocked[s_current_pin]);
}

static void select_long_click_handler(ClickRecognizerRef recognizer, void *context) {
  // does nothing
}

static void select_long_click_release_handler(ClickRecognizerRef recognizer, void *context) {
  update_pin_status(s_bitmap_pin_0, 0x0, true);
  update_pin_status(s_bitmap_pin_1, 0x1, true);
  update_pin_status(s_bitmap_pin_2, 0x2, true);
  update_pin_status(s_bitmap_pin_3, 0x3, true);
  update_pin_status(s_bitmap_pin_4, 0x4, true);
  
  // TODO: go to next frame
}

static void click_config_provider(void *context) {
  // Register click handlers
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_long_click_subscribe(BUTTON_ID_UP, 700, up_long_click_handler, up_long_click_release_handler);
  window_long_click_subscribe(BUTTON_ID_DOWN, 700, down_long_click_handler, down_long_click_release_handler);
  window_long_click_subscribe(BUTTON_ID_SELECT, 700, select_long_click_handler, select_long_click_release_handler);
}

static void handle_window_load(Window* window) {
  set_game(0);
}

static void handle_window_unload(Window* window) {
  destroy_ui();
}

void show_game_editor(char* new_bowler_name, char* new_league_name, char* new_series_name) {
  if (strcmp(new_bowler_name, NEW_BOWLER) == 0)
    bowler_name = DEFAULT_BOWLER_NAME;
  else
    bowler_name = new_bowler_name;
  
  if (strcmp(new_league_name, NEW_EVENT) == 0)
    league_name = DEFAULT_EVENT_NAME;
  else
    league_name = new_league_name;
  
  if (strcmp(new_series_name, NEW_SERIES) == 0)
    series_name = DEFAULT_SERIES_NAME;
  else
    series_name = new_series_name;
  
  initialise_ui();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = handle_window_load,
    .unload = handle_window_unload,
  });
  update_indicator_position();
  window_set_click_config_provider(s_window, click_config_provider);
  window_stack_push(s_window, true);
}

void hide_game_editor(void) {
  window_stack_remove(s_window, true);
}
