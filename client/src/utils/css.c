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

static void on_theme_switch(GtkSwitch *G_GNUC_UNUSED theme_switch, gboolean state, GtkWidget *window) {
    gchar *css_path;
    if (state) {
        css_path = g_strdup("client/static/styles/dark.css");
    } else {
        css_path = g_strdup("client/static/styles/light.css");
    }
    load_css(window, css_path);
    g_free(css_path);
}

void init_theme_switcher(GtkBuilder *builder, GtkWidget *window, char *theme_switcher_widget_name) {
    GtkWidget *switch_widget = GTK_WIDGET(gtk_builder_get_object(builder, (gchar *) theme_switcher_widget_name));

    g_signal_connect(switch_widget, "state-set", G_CALLBACK(on_theme_switch), window);
}
