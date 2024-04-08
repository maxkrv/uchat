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
t_list *global_rooms;
t_list *global_messages;
t_user *global_user;
#else
extern GtkBuilder *global_builder;
extern GtkWidget *global_window;
extern t_list *global_rooms;
extern t_list *global_messages;
extern t_user *global_user;
#endif

// auth
void show_auth_container();
void hide_auth_container();

// chat
void show_chat_container(bool should_load_rooms);
void hide_chat_container();
void show_selected_room(t_room *room);

// user settings
void show_settings_container();
void hide_settings_container();
void init_update_user_form_field();
void init_update_password_form();

// room settings
void show_room_settings_dialog(GtkButton *button, gpointer user_data);

// room
void init_create_room(void);
void append_room_to_list(t_room *room);

// utils
GdkPixbuf *load_pixbuf_from_url(const char *url);
void load_css(GtkWidget *window, const gchar *css_path);
void init_theme_switcher(GtkBuilder *builder, GtkWidget *window,
                         char *theme_switcher_widget_name);
gboolean is_empty_field(GtkEntry *entry);
void unixTimeToHoursMinutes(time_t unixTime, int *hours, int *minutes);

gboolean websocket_server_push_events(gpointer user_data);
