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

// chat
void show_chat_container();
void hide_chat_container();

// user settings
void show_settings_container();
void hide_settings_container();
void init_update_user_form_field();
void init_update_password_form();

// utils
GdkPixbuf *load_pixbuf_from_url(const char *url);
void load_css(GtkWidget *window, const gchar *css_path);
void init_theme_switcher(GtkBuilder *builder, GtkWidget *window,
                         char *theme_switcher_widget_name);
gboolean is_empty_field(GtkEntry *entry);

gboolean websocket_server_push_events(gpointer user_data);
