#include "../../inc/client.h"

// css_path should absolute path to css file(e.g. client/static/style.css)
void load_css(GtkWidget *window, const gchar *css_path) {
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider, css_path, NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                              GTK_STYLE_PROVIDER(provider),
                                              GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_widget_reset_style(window);
}
