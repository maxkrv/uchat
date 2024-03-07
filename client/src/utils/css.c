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

static void on_theme_switch(GtkComboBox *combo_box, GtkWidget *window) {
    GtkTreeIter iter;
    GtkTreeModel *model = gtk_combo_box_get_model(combo_box);
    gchar *theme_name;

    if (gtk_combo_box_get_active_iter(combo_box, &iter)) {
        gtk_tree_model_get(model, &iter, 0, &theme_name, -1);
        gchar *css_path = g_strconcat("client/static/styles/", theme_name, ".css", NULL);
        load_css(window, css_path);
        g_free(css_path);
    }
}

void init_theme_switcher(GtkBuilder *builder, GtkWidget *window) {
    GtkWidget *combo_box = GTK_WIDGET(gtk_builder_get_object(builder, "theme_switcher"));

    g_signal_connect(combo_box, "changed", G_CALLBACK(on_theme_switch), window);
}
