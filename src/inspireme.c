#include <pebble.h>
#include <stdlib.h>

static Window *window;
static Window *inspiration;
static Window *juststop;
static TextLayer *times;
static TextLayer *dates;
static TextLayer *inspire;
static TextLayer *despire;
static AppSync sync;
static uint8_t sync_buffer[150];

enum InspirationKeys {
  QUOTE_TEMP_KEY = 0x0
};

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (window_is_loaded(window)) {
    window_stack_push(inspiration, true);
  }
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (window_is_loaded(window)) {
    window_stack_push(juststop, true);
  }
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void sync_error(DictionaryResult dict_error, 
 AppMessageResult app_message_error, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Sync Error: %d", app_message_error);
}

static void sync_success(const uint32_t key, const Tuple* new_tuple, 
 const Tuple* old_tuple, void* context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "App Sync Success %s", new_tuple->value->cstring);
  text_layer_set_text(inspire, new_tuple->value->cstring);
}

static void update_time() {
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);
  static char buffer[] = "00:00";
  strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
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
  times = text_layer_create(GRect(-25, 60, 144, 100));
  text_layer_set_background_color(times, GColorClear);
  text_layer_set_text_color(times, GColorWhite);
  text_layer_set_text(times, "00:00");
  dates = text_layer_create(GRect(-30, 100, 144, 50));
  text_layer_set_background_color(dates, GColorClear);
  text_layer_set_text_color(dates, GColorWhite);
  text_layer_set_text(dates, "00:00");
  text_layer_set_font(times, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(times, GTextAlignmentRight);
  text_layer_set_font(dates, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  text_layer_set_text_alignment(dates, GTextAlignmentRight);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(times));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(dates));
  window_set_click_config_provider(window, click_config_provider);
  update_time();
  update_date();
}

static void main_window_unload(Window *window) {
  text_layer_destroy(times);
  text_layer_destroy(dates);
}

static void insp_window_load(Window *inspiration) {
  inspire = text_layer_create(GRect(0, 0, 134, 158));
  text_layer_set_background_color(inspire, GColorBlack);
  text_layer_set_text_color(inspire, GColorWhite);
  text_layer_set_text_alignment(inspire, GTextAlignmentCenter);
  text_layer_set_font(inspire, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  
  Tuplet inital_values[] = {
    TupletCString(QUOTE_TEMP_KEY, "Loading...")
  };

  text_layer_set_text(inspire, QUOTE_TEMP_KEY);
  app_sync_init(&sync, sync_buffer, sizeof(sync_buffer), inital_values,
    ARRAY_LENGTH(inital_values), sync_success, sync_error, NULL);
  layer_add_child(window_get_root_layer(inspiration), text_layer_get_layer(inspire));
} 

static void insp_window_unload(Window *inspiration) {
  text_layer_destroy(inspire);
}

int random_num(int limit) {
  int divisor = RAND_MAX/(limit+1);
  int imback;
  do { 
    imback = rand() / divisor;
  } while (imback > limit);
  return imback;
}

const char * getbad() {
  int rando = (int) random_num(50);
  if (rando < 5) {
    return "You're going nowhere in life, you condescending piece of monkey smegma";
  } else if(rando < 10){
    return "Wanna hear a joke? Your life. HAHAHAHA";
  } else if(rando < 15) {
    return "Move bitch, get out the way.";
  } else if(rando < 20) {
    return "You don't even go here";
  } else if(rando < 25) {
    return "Uh.";
  } else if(rando < 30) {
    return "I can't say anything bad about you, because you're 100\% fucked up";
  } else if(rando < 35) {
    return "Can you just make me a new app please?";
  } else if(rando < 40) {
    return "You're as ugly as my mom's chesthair";
  } else if(rando < 45) {
    return "Ha! GAYYYYYYY! (in the non-homophobic way)";
  } else {
    return "Just get out. Go.";
  }
}

static void sto_window_load(Window *juststop) {
  despire = text_layer_create(GRect(0, 50, 134, 158));
  text_layer_set_background_color(despire, GColorBlack);
  text_layer_set_text_color(despire, GColorWhite);
  text_layer_set_text_alignment(despire, GTextAlignmentCenter);
  text_layer_set_font(despire, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  text_layer_set_text(despire, getbad());
  layer_add_child(window_get_root_layer(juststop), text_layer_get_layer(despire));
}



static void sto_window_unload(Window *juststop) {
  text_layer_destroy(despire);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void init() {
  window = window_create();
  window_set_background_color(window, GColorBlack);
  inspiration = window_create();
  window_set_background_color(inspiration, GColorBlack);
  juststop = window_create();
  window_set_background_color(juststop, GColorBlack);

  window_set_window_handlers(window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  window_set_window_handlers(inspiration, (WindowHandlers) {
    .load = insp_window_load,
    .unload = insp_window_unload
  });

  window_set_window_handlers(juststop, (WindowHandlers) {
    .load = sto_window_load,
    .unload = sto_window_unload
  });

  app_message_open(150, 150);
  window_set_fullscreen(window, true);
  window_set_fullscreen(inspiration, true);
  window_set_fullscreen(juststop, true);

  window_stack_push(inspiration, true);
  window_stack_push(juststop, true);
  window_stack_push(window, true);
  
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit() {
  window_destroy(window);
  window_destroy(inspiration);
  window_destroy(juststop);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}