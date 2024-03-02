#include "../../inc/client.h"

static void show_error_message(GtkLabel *label, const gchar *message) {
    gtk_label_set_text(label, message);
    gtk_widget_show(GTK_WIDGET(label));
}

static void on_login_entry_changed(GtkEntry *entry, gpointer data) {
    GtkLabel *label = GTK_LABEL(data);

    const gchar *text = gtk_entry_get_text(entry);
    printf("Login: %s\n", text);
    if (!g_strcmp0(text,""))
        show_error_message(label, "Enter your login");
    else
        gtk_label_set_text(label, "");
}

static void on_password_entry_changed(GtkEntry *entry, gpointer data) {
    GtkLabel *label = GTK_LABEL(data);

    const gchar *password = gtk_entry_get_text(entry);
    gint len = g_utf8_strlen(password, -1);
    gboolean numb = FALSE, capital_letter = FALSE, small_letter = FALSE;
    for (gint i = 0; i < len; i++) {
        if (!numb && g_unichar_isdigit(password[i]))
           numb = TRUE;
        else if (!capital_letter && g_unichar_isupper(password[i]))
           capital_letter = TRUE;
        else if (!small_letter && g_unichar_islower(password[i]))
           small_letter = TRUE;
    }
    if (!g_strcmp0(password,""))
        show_error_message(label, "Enter your password");
    else if (!numb || !capital_letter || !small_letter)
	show_error_message(label, "The password must contain numbers,\n uppercase and lowercase letters");
    else if ((len > 16) || (len < 8))
        show_error_message(label, "The password must contain\n 8 - 16 characters");
    else
        gtk_label_set_text(label, "");
}

static void on_password_entry_visibility(GtkEntry *entry, GtkEntryIconPosition icon_position) {
    if (icon_position == GTK_ENTRY_ICON_SECONDARY) {
        gboolean current_visibility = gtk_entry_get_visibility(entry);
        gtk_entry_set_visibility(entry, !current_visibility);
    }
}

void show_auth_container(GtkBuilder *builder, GtkWidget *window) {
    GtkWidget *auth_container =
        GTK_WIDGET(gtk_builder_get_object(builder, "auth_container"));

    if (auth_container == NULL) {
        g_print("Error: %s\n", "Failed to load auth_container");
        return;
    }

    gtk_container_add(GTK_CONTAINER(window), auth_container);

    GtkLabel *err_label_log =
        GTK_LABEL(gtk_builder_get_object(builder, "error_message_login_label"));
    GtkLabel *err_label_reg =
        GTK_LABEL(gtk_builder_get_object(builder, "error_message_registration_label"));

    GtkWidget *login_entry =
        GTK_WIDGET(gtk_builder_get_object(builder, "login_entry"));
    GtkWidget *password_entry =
        GTK_WIDGET(gtk_builder_get_object(builder, "password_entry"));

    GtkWidget *new_login_entry =
        GTK_WIDGET(gtk_builder_get_object(builder, "new_login_entry"));
    GtkWidget *new_password_entry =
        GTK_WIDGET(gtk_builder_get_object(builder, "new_password_entry"));
    GtkWidget *confirm_password_entry =
        GTK_WIDGET(gtk_builder_get_object(builder, "confirm_password_entry"));

    g_signal_connect(login_entry, "changed",
                     G_CALLBACK(on_login_entry_changed), err_label_log);
    g_signal_connect(new_login_entry, "changed",
                     G_CALLBACK(on_login_entry_changed), err_label_reg);
    g_signal_connect(password_entry, "changed",
                     G_CALLBACK(on_password_entry_changed), err_label_log);
    g_signal_connect(new_password_entry, "changed",
                     G_CALLBACK(on_password_entry_changed), err_label_reg);
    g_signal_connect(confirm_password_entry, "changed",
                     G_CALLBACK(on_password_entry_changed), err_label_reg);
    g_signal_connect(password_entry, "icon-press",
                    G_CALLBACK(on_password_entry_visibility), NULL);
    g_signal_connect(new_password_entry, "icon-press",
                    G_CALLBACK(on_password_entry_visibility), NULL);
    g_signal_connect(confirm_password_entry, "icon-press",
                    G_CALLBACK(on_password_entry_visibility), NULL);
}
