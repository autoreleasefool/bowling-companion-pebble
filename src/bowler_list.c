#include <pebble.h>
#include "bowler_list.h"
#include "utils.h"
#include "colours.h"

#define NUM_MENU_SECTIONS 2
#define NUM_FIRST_MENU_ITEMS 1
#define NUM_SECOND_MENU_ITEMS 1
#define NO_BOWLERS "No bowlers"

static Window *s_window;
static MenuLayer *s_bowler_menu;
static GBitmap *s_plus_bitmap_highlighted, *s_plus_bitmap_not_highlighted;
static GBitmap *s_bowler_bitmap_highlighted, *s_bowler_bitmap_not_highlighted;

static uint16_t s_current_section = 0, s_current_row = 0;

// UI Functions

static void destroy_ui(void) {
  menu_layer_destroy(s_bowler_menu);
  gbitmap_destroy(s_plus_bitmap_not_highlighted);
  gbitmap_destroy(s_plus_bitmap_highlighted);
  gbitmap_destroy(s_bowler_bitmap_highlighted);
  gbitmap_destroy(s_bowler_bitmap_highlighted);
  window_destroy(s_window);
}

static void initialise_ui(void) {
  s_window = window_create();
  window_set_fullscreen(s_window, true);
  window_set_background_color(s_window, COLOUR_WINDOW);

  s_plus_bitmap_highlighted = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_PLUS_HIGHLIGHT);
  s_plus_bitmap_not_highlighted = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_PLUS_NO_HIGHLIGHT);
  s_bowler_bitmap_highlighted = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BOWLER_HIGHLIGHT);
  s_bowler_bitmap_not_highlighted = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BOWLER_NO_HIGHLIGHT);
}

static char* get_header_text(uint16_t section_index) {
  switch (section_index) {
    case 0:
      return "Create new bowler";
    case 1:
      return "Choose existing";
    default:
      return "";
  }
}

static char* get_row_text(uint16_t section, uint16_t row) {
  switch (section) {
    case 0:
      if (row == 0) {
        return NEW_BOWLER;
      } else {
        return "";
      }
    case 1:
      if (row == 0) {
        return NO_BOWLERS;
      } else {
        return "";
      }
    default:
      return "";
  }
}

static char* get_row_subtitle(uint16_t section, uint16_t row) {
  // TODO: return bowler's average
  return NULL;
}

static GBitmap* get_row_icon(uint16_t section, uint16_t row) {
  if (section == 0 && row == 0) {
    if (section == s_current_section && row == s_current_row)
      return s_plus_bitmap_highlighted;
    else
      return s_plus_bitmap_not_highlighted;
  } else {
    if (section == s_current_section && row == s_current_row)
      return s_bowler_bitmap_highlighted;
    else
      return s_bowler_bitmap_not_highlighted;
  }
}

static uint16_t menu-get_num_sections_callback(MenuLayer *menu_layer, void *data) {
  return NUM_MENU_SECTIONS;
}

static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section, void *data) {
  switch (section) {
    case 0:
      return NUM_FIRST_MENU_ITEMS;
    case 1:
      // TODO: return number of bowlers from companion app
      return NUM_SECOND_MENU_ITEMS;
    default:
      return 0;
  }
}

static int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section, void *data) {
  return MENU_CELL_BASIC_HEADER_HEIGHT;
}

static void menu_draw_header_callback(GContext *ctx, const Layer *cell_layer, uint16_t section, void *data) {
  menu_cell_basic_header_draw(ctx, cell_layer, get_header_text(section));
}

static void menu_draw_row_callback(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
  uint16_t section = cell_index->section;
  uint16_t row = cell_index->row;
  menu_cell_basic_draw(ctx, cell_layer, get_row_text(section, row), get_row_subtitle(section, row), get_row_icon(section, row));
}

static void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
  char* row_text = get_row_text(cell_index->section, cell_index->row);
  if (strcmp(row_text, NO_BOWLERS) != 0) {
    //s_league_list_created = true;
    //show_league_list(row_text);
  }
}

static void menu_selection_changed_callback(MenuLayer *menu_layer, MenuIndex new_index, MenuIndex old_index, void *data) {
  s_current_section = ew_index.section;
  s_current_row = new_index.row;
}

static void handle_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_Frame(window_layer);

  s_menu_layer = menu_layer_create(bounds);
  menu_layer_set_callbacks(s_menu_layer, NULL, (MenuLayerCallbacks) {
    .get_num_sections = menu_get_num_sections_callback,
    .get_num_rows = menu_get_num_rows_callback,
    .get_header_height = menu_get_header_height_callback,
    .draw_header = menu_draw_header_callback,
    .draw_row = menu_draw_row_callback,
    .select_click = menu_select_callback,
    .selection_changed = menu_selection_changed_callback,
  });

  menu_layer_set_click_config_onto_window(s_menu_layer, window);
  layer_add_child(window_layer, menu_layer_get_layer(s_menu_layer));
}

static void handle_window_unload(Window *window) {
  destroy_ui();
}

void show_bowler_list(void) {
  initialise_ui();

  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = handle_window_load,
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);
}

void hide_bowler_list(void) {
  window_stack_remove(s_window, true);
}
