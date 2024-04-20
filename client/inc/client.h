#pragma once

#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include <curl/curl.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gtk/gtk.h>
#include "sdk/sdk.h"
#include "ws.h"
#include <time.h>

#ifdef DEFINE_GLOBALS
GtkBuilder *global_builder;
GtkWidget *global_window;
#else
extern GtkBuilder *global_builder;
extern GtkWidget *global_window;
#endif

// auth
void show_auth_container();
void hide_auth_container();

// user
void render_user();

// chat
void show_chat_container(bool should_load_rooms);
void show_selected_room(t_room *room, bool should_render_messages);

// user settings
void show_settings_dialog(GtkButton *button, gpointer user_data);
void hide_settings_dialog();
void init_update_user_form_field();
void init_update_password_form();

// room settings
void show_room_settings_dialog(GtkButton *button, gpointer user_data);
void clean_up_settings();

// room
void init_create_room(void);
void append_room_to_list(t_room *room, bool is_favorite);
void set_room_name(t_room *room, GtkWidget *room_user_name);
void set_room_photo(t_room *room, GtkWidget *image);
void render_rooms();
void clean_up_room_creation();
void init_edit_room_form(t_room *room);
void init_members_form(int room_id);

// message
void render_messages(int room_id);
void init_message_form(int chat_id);
void handle_edit_message(GtkButton *button, t_message *message);
void handle_reply_message(GtkButton *button, t_message *message);
void show_pin_message(int room_id);
void hide_pin_message();
void handle_pin_message(int room_id, int message_id);

// files
void show_file_chooser_dialog(GtkWidget *button, gpointer user_data);

// utils
GdkPixbuf *load_pixbuf_from_url(const char *url, int width, int height);
GdkPixbuf *create_circled_image(GdkPixbuf *src_pixbuf, int size);
void load_css(GtkWidget *window, const gchar *css_path);
void init_theme_switcher(GtkBuilder *builder, GtkWidget *window,
                         char *theme_switcher_widget_name);
gboolean is_empty_field(GtkEntry *entry);
void unixTimeToHoursMinutes(time_t unixTime, int *hours, int *minutes);
const char *get_file_extension(const char *file_path);
gboolean call_scrollbar_once(gpointer data);
char *find_format(const char *s, int c);

// members
void render_room_members(int room_id);

gboolean websocket_server_push_events(gpointer user_data);
