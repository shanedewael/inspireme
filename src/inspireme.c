#include <pebble.h>

#define NUM_MENU_ITEMS 5
  
static Window *window;
static Window *inspiration;
static TextLayer *times;
static TextLayer *dates;
static TextLayer *caltext;
static TextLayer *inspire;

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (window_is_loaded(window)) {
    window_stack_push(inspiration, true);
  }
}
      
static void click_config_provider(void *context) {
  // Register the ClickHandlers
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
}
static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
  static char buffer[] = "00:00";


  strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);

  // Display this time on the TextLayer
  text_layer_set_text(times, buffer);

}

static void update_date() {
  time_t temp = time(NULL); 
  static char buffer[] = "00:00:00";
  struct tm *tick_time = localtime(&temp);
  strftime(buffer, sizeof(buffer), "%a. %d", tick_time);
  text_layer_set_text(dates, buffer);
}




static void main_window_load(Window *window) {
  // Create time TextLayer
  times = text_layer_create(GRect(-25, 60, 144, 100));
  text_layer_set_background_color(times, GColorClear);
  text_layer_set_text_color(times, GColorWhite);
  text_layer_set_text(times, "00:00");
  
  // Create date TextLayer
  dates = text_layer_create(GRect(-30, 100, 144, 50));
  text_layer_set_background_color(dates, GColorClear);
  text_layer_set_text_color(dates, GColorWhite);
  text_layer_set_text(dates, "00:00");

  // Improve the layout to be more like a watchface
  text_layer_set_font(times, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(times, GTextAlignmentRight);

  text_layer_set_font(dates, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  text_layer_set_text_alignment(dates, GTextAlignmentRight);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(times));
  
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(dates));
  
  // Add click handler
  window_set_click_config_provider(window, click_config_provider);

  
  // Make sure the time is displayed from the start
  update_time();
  
  //Update date
  update_date();
}

static void main_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(times);
  text_layer_destroy(dates);
}

static void insp_window_load(Window *inspiration) {
  //The static time layer
  inspire = text_layer_create(GRect(0, 0, 144, 168));
  text_layer_set_background_color(inspire, GColorBlack);
  text_layer_set_text_color(inspire, GColorWhite);
  text_layer_set_text_alignment(inspire, GTextAlignmentCenter);
  text_layer_set_font(inspire, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  text_layer_set_text(inspire, "Inspire me");
  


  
  layer_add_child(window_get_root_layer(inspiration), text_layer_get_layer(inspire));
  

  
  //Set the date

} 

static void insp_window_unload(Window *inspiration) {
  text_layer_destroy(inspire);
}


static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}
  
static void init() {
  // Create main Window element and assign to pointer
  window = window_create();
  window_set_background_color(window, GColorBlack);

  inspiration = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  window_set_window_handlers(inspiration, (WindowHandlers) {
    .load = insp_window_load,
    .unload = insp_window_unload
  });


  window_set_fullscreen(window, true);
  window_set_fullscreen(inspiration, true);

  // Show the Window on the watch, with animated=true
  window_stack_push(inspiration, true);
  window_stack_push(window, true);
  
  
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit() {
  // Destroy Window
  window_destroy(window);
  window_destroy(inspiration);
}

int main(void) {
  init();

  app_event_loop();
  deinit();

}