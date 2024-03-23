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

// auth
void show_auth_container(GtkBuilder *builder, GtkWidget *window);

// utils
GdkPixbuf *load_pixbuf_from_url(const char *url);
void load_css(GtkWidget *window, const gchar *css_path);
void init_theme_switcher(GtkBuilder *builder, GtkWidget *window);
gboolean is_empty_field(GtkEntry *entry);

