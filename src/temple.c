#include "pebble.h"
  
Window *window;
TextLayer *text_date_layer;
TextLayer *text_time_layer;
TextLayer *text_label_layer;
TextLayer *text_temp_layer;
TextLayer *text_degree_layer;
Layer *line_layer;
Layer *circle_layer;

void line_layer_update_callback(Layer *layer, GContext* ctx) {
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_fill_rect(ctx, layer_get_bounds(layer), 0, GCornerNone);
}

void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
  static char time_text[] = "00:00";
  static char date_text[] = "Xxxxxxxxx 00";
  
  char *time_format;
  
  if(!tick_time) {
    time_t now = time(NULL);
    tick_time = localtime(&now);
  }
  
  strftime(date_text, sizeof(date_text), "%B %e", tick_time);
  text_layer_set_text(text_date_layer, date_text);
  
  if (clock_is_24h_style()) {
    time_format = "%R";
  } else {
    time_format = "%I:%M";
  }
  
  strftime(time_text, sizeof(time_text), time_format, tick_time);
  
  if (!clock_is_24h_style() && (time_text[0] == '0')) {
    memmove(time_text, &time_text[1], sizeof(time_text) - 1);
  }
  
  text_layer_set_text(text_time_layer, time_text);
}

void init(void) {
  window = window_create();
  window_stack_push(window, true); /*Animated*/
  window_set_background_color(window, GColorBlack);
  
  Layer *window_layer = window_get_root_layer(window);
  
  text_date_layer = text_layer_create(GRect(8,2, 144-8, 168-8));
  text_layer_set_text_color(text_date_layer, GColorWhite);
  text_layer_set_background_color(text_date_layer, GColorClear);
  text_layer_set_font(text_date_layer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  layer_add_child(window_layer, text_layer_get_layer(text_date_layer));
  
  text_time_layer = text_layer_create(GRect(7, 25, 144-7, 168-7));
  text_layer_set_text_color(text_time_layer, GColorWhite);
  text_layer_set_background_color(text_time_layer, GColorClear);
  text_layer_set_font(text_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  layer_add_child(window_layer, text_layer_get_layer(text_time_layer));
  
  GRect line_frame = GRect(8, 82, 139, 2);
  line_layer = layer_create(line_frame);
  layer_set_update_proc(line_layer, line_layer_update_callback);
  layer_add_child(window_layer, line_layer);
  
  text_label_layer = text_layer_create(GRect(8, 90, 144-8, 168-8));
  text_layer_set_text_color(text_label_layer, GColorWhite);
  text_layer_set_background_color(text_label_layer, GColorClear);
  text_layer_set_font(text_label_layer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  text_layer_set_text(text_label_layer, "Temple Temp");
  layer_add_child(window_layer, text_layer_get_layer(text_label_layer));
  
  text_temp_layer = text_layer_create(GRect(7, 112, 144-7, 168-7));
  text_layer_set_text_color(text_temp_layer, GColorWhite);
  text_layer_set_background_color(text_temp_layer, GColorClear);
  text_layer_set_font(text_temp_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text(text_temp_layer, "72 C");
  layer_add_child(window_layer, text_layer_get_layer(text_temp_layer));
  
  text_degree_layer = text_layer_create(GRect(57, 112, 66, 122));
  text_layer_set_text_color(text_degree_layer, GColorWhite);
  text_layer_set_background_color(text_degree_layer, GColorClear);
  text_layer_set_font(text_degree_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text(text_degree_layer, "o");
  layer_add_child(window_layer, text_layer_get_layer(text_degree_layer));
  
  tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);
  handle_minute_tick(NULL, MINUTE_UNIT);
}

void deinit(void) {
  tick_timer_service_unsubscribe();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}