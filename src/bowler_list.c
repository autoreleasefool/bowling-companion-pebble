/*
 * Copyright 2015 Joseph Roque
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <pebble.h>
#include "bowler_list.h"
#include "league_list.h"

#define NUM_MENU_SECTIONS 2
#define NUM_FIRST_MENU_ITEMS 1
#define NUM_SECOND_MENU_ITEMS 1

// UI objects
static Window *s_main_window;
static MenuLayer *s_menulayer_bowlers;

// Graphics objects
static GBitmap *s_bitmap_bowler_new;
static GBitmap *s_bitmap_bowler_new_highlighted;
static GBitmap *s_bitmap_bowler;
static GBitmap *s_bitmap_bowler_highlighted;
static GBitmap *s_bitmap_x;
static GBitmap *s_bitmap_x_highlighted;

/*
 * Returns the height of the menu section header for the section represented by /section_index/.
 */
static int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  return MENU_CELL_BASIC_HEADER_HEIGHT;
}

/*
 * Returns the number of sections in the bowler menu
 */
static uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
  return NUM_MENU_SECTIONS;
}

/*
 * Returns the number of rows in the section of the menu represented by /section_index/.
 */
static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  switch (section_index) {
    case 0: return NUM_FIRST_MENU_ITEMS;
    // TODO: return actual number of bowlers
    case 1: return NUM_SECOND_MENU_ITEMS;
    default: return 0;
  }
}

/*
 * Returns the header text of the section of the menu represented by /section_index/.
 */
static char* get_header_text(uint16_t section_index) {
  switch (section_index) {
    case 0: return "Create new bowler";
    case 1: return "Existing bowlers";
    default: return "";
  }
}

/*
 * If /section_index/ is 0, returns "New bowler" if /row/ is 0, and an empty string otherwise. Else, if /section_index/
 * is 1, returns the name of the bowler at /row/ and "No bowlers" if the list of bowlers is empty.
 */
static char* get_row_title(uint16_t section_index, uint16_t row) {
  switch (section_index) {
    case 0:
      if (row == 0) {
        return "New bowler";
      } else {
        return "";
      }
    case 1:
      // TODO: return user's name
      if (row == 0) {
        return "No bowlers";
      } else {
        return "";
      }
    default:
      return "";
  }
}

/*
 * If /section_index/ is 0, returns NULL. Else, if /section_index/ is 1, returns the average of the bowler
 * at /row/ as a string, and NULL if the bowler does not exist.
 */
static char* get_row_subtitle(uint16_t section_index, uint16_t row) {
  // TODO: return user's average
  switch (section_index) {
    case 1:
      return "Avg: 200";
    default:
      return NULL;
  }
}

/*
 * If /section_index/ is 0, returns a bitmap representing a new bowler if /row/ is 0, NULL otherwise. Else, if
 * if /section_index/ is 1, returns a bitmap representing a bowler, NULL otherwise.
 */
static GBitmap* get_row_image(const Layer *cell_layer, uint16_t section_index, uint16_t row) {
  switch (section_index) {
    case 0:
      if (row == 0) {
        if (menu_cell_layer_is_highlighted(cell_layer))
          return s_bitmap_bowler_new_highlighted;
        else
          return s_bitmap_bowler_new;
      } else {
        return NULL;
      }
    case 1:
      // TODO: return bowler for bowlers, x for none
      if (menu_cell_layer_is_highlighted(cell_layer))
        return s_bitmap_x_highlighted;
      else
        return s_bitmap_x;
    default:
      return NULL;
  }
}

/*
 * Invokes the method /menu_cell_basic_header_draw/ with the text set as /get_header_text/.
 */
static void menu_draw_header_callback(GContext *ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
  menu_cell_basic_header_draw(ctx, cell_layer, get_header_text(section_index));
}

/*
 * Invokes the method /menu_cell_basic_draw/ with the title as /get_row_title/, the subtitle as /get_row_subtitle/,
 * and the image as /get_row_image/.
 */
static void menu_draw_row_callback(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
  uint16_t section_index = cell_index->section;
  uint16_t row = cell_index->row;
  menu_cell_basic_draw(ctx, cell_layer, get_row_title(section_index, row), get_row_subtitle(section_index, row), get_row_image(cell_layer, section_index, row));
}

/*
 * Loads the bowler's leagues represented by the menu item selected by the user, or creates a new bowler and a new
 * default "Open" league.
 */
static void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
  // TODO: show bowler's leagues
  show_league_list();
}

/*
 * Sets up interactions when the window has finished loading.
 */
static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);

  s_bitmap_bowler_new = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BOWLER_NEW);
  s_bitmap_bowler = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BOWLER);
  s_bitmap_x = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_CLOSE);

  #ifdef PBL_COLOR
    s_bitmap_bowler_new_highlighted = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BOWLER_NEW_HIGHLIGHT);
    s_bitmap_bowler_highlighted = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BOWLER_HIGHLIGHT);
    s_bitmap_x_highlighted = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_CLOSE_HIGHLIGHT);
  #else
    s_bitmap_bowler_new_highlighted = s_bitmap_bowler_new;
    s_bitmap_bowler_highlighted = s_bitmap_bowler;
    s_bitmap_x_highlighted = s_bitmap_x;
  #endif

  s_menulayer_bowlers = menu_layer_create(bounds);
  menu_layer_set_callbacks(s_menulayer_bowlers, NULL, (MenuLayerCallbacks) {
    .get_num_sections = menu_get_num_sections_callback,
    .get_num_rows = menu_get_num_rows_callback,
    .get_header_height = menu_get_header_height_callback,
    .draw_row = menu_draw_row_callback,
    .draw_header = menu_draw_header_callback,
    .select_click = menu_select_callback,
  });

  menu_layer_set_click_config_onto_window(s_menulayer_bowlers, window);
  layer_add_child(window_layer, menu_layer_get_layer(s_menulayer_bowlers));
}

/*
 * Releases references to UI objects.
 */
static void main_window_unload(Window *window) {
  window_destroy(s_main_window);
  menu_layer_destroy(s_menulayer_bowlers);

  gbitmap_destroy(s_bitmap_bowler_new);
  gbitmap_destroy(s_bitmap_bowler_new_highlighted);
  gbitmap_destroy(s_bitmap_bowler);
  gbitmap_destroy(s_bitmap_bowler_highlighted);
  gbitmap_destroy(s_bitmap_x);
  gbitmap_destroy(s_bitmap_x_highlighted);
}

/*
 * Sets up the window for the bowlers menu.
 */
void show_bowler_list(void) {
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  window_stack_push(s_main_window, true);
}

/*
 * Unloads this window.
 */
void hide_bowler_list(void) {
  window_stack_remove(s_main_window, true);
}
