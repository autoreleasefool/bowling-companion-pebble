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
#include "league_list.h"
#include "series_list.h"

#define NUM_MENU_SECTIONS 2
#define NUM_FIRST_MENU_ITEMS 1
#define NUM_SECOND_MENU_ITEMS 1

static Window *s_main_window;
static MenuLayer *s_menulayer_leagues;

static void initialise_ui(void) {
  s_main_window = window_create();
}

static void destroy_ui(void) {
  window_destroy(s_main_window);
  menu_layer_destroy(s_menulayer_leagues);
}

static int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section, void *data) {
  return MENU_CELL_BASIC_HEADER_HEIGHT;
}

static uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
  return NUM_MENU_SECTIONS;
}

static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section, void *data) {
  switch (section) {
    case 0: return NUM_FIRST_MENU_ITEMS;
    case 1: return NUM_SECOND_MENU_ITEMS;
    default: return 0;
  }
}

static char* get_header_text(uint16_t section_index) {
  switch (section_index) {
    case 0: return "Leagues/Events";
    case 1: return "Create new event";
    default: return "";
  }
}

static char* get_row_text(uint16_t section, uint16_t row) {
  switch (section) {
    case 0:
      switch (row) {
        case 0: return "No leagues/events";
        default: return "";
      }
    case 1:
      switch (row) {
        case 0: return "New event";
        default: return "";
      }
    default:
      return "";
  }
}

static char* get_row_subtitle(uint16_t section, uint16_t row) {
  // TODO: return league/event average
  return "";
}

static void menu_draw_header_callback(GContext *ctx, const Layer *cell_layer, uint16_t section, void *data) {
  menu_cell_basic_header_draw(ctx, cell_layer, get_header_text(section));
}

static void menu_draw_row_callback(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
  uint16_t section = cell_index->section;
  uint16_t row = cell_index->row;
  menu_cell_basic_draw(ctx, cell_layer, get_row_text(section, row), get_row_subtitle(section, row), NULL);
}

static void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
  // TODO: show league series
  // TODO: open event games
  show_series_list();
}

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);

  s_menulayer_leagues = menu_layer_create(bounds);
  menu_layer_set_callbacks(s_menulayer_leagues, NULL, (MenuLayerCallbacks) {
    .get_num_sections = menu_get_num_sections_callback,
    .get_num_rows = menu_get_num_rows_callback,
    .get_header_height = menu_get_header_height_callback,
    .draw_row = menu_draw_row_callback,
    .draw_header = menu_draw_header_callback,
    .select_click = menu_select_callback,
  });

  menu_layer_set_click_config_onto_window(s_menulayer_leagues, window);
  layer_add_child(window_layer, menu_layer_get_layer(s_menulayer_leagues));
}

static void main_window_unload(Window *window) {
  destroy_ui();
}

void show_league_list(void) {
  initialise_ui();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  window_stack_push(s_main_window, true);
}

void hide_league_list(void) {
  window_stack_remove(s_main_window, true);
}
