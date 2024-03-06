#include "../../inc/client.h"

typedef struct {
    gchar *username;
    gchar *password;
    gchar *confirm_password;
    GtkLabel *err_label;
} UserData;

UserData *user_data = NULL;

static void show_error_message(GtkLabel *label, const gchar *message) {
    gtk_label_set_text(label, message);
    gtk_widget_show(GTK_WIDGET(label));
    user_data->err_label = label;
}

static void on_login_entry_changed(GtkEntry *entry, gpointer data) {
    GtkLabel *label = GTK_LABEL(data);

    const gchar *text = gtk_entry_get_text(entry);
    user_data->username = g_strdup(text);
    if (g_strcmp0(text, "") == 0)
        show_error_message(label, "Enter your login");
    else {
        gtk_label_set_text(label, "");
        user_data->err_label = label;
    }
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
    if (g_strcmp0(password, "") == 0)
        show_error_message(label, "Enter your password");
    else if (!numb || !capital_letter || !small_letter)
        show_error_message(label, "The password must contain numbers,\n "
                                  "uppercase and lowercase letters");
    else if ((len > 16) || (len < 8))
        show_error_message(label,
                           "The password must contain\n 8 - 16 characters");
    else {
        gtk_label_set_text(label, "");
        user_data->err_label = label;
    }
    user_data->password = g_strdup(password);
}

static void on_confirm_password_entry_changed(GtkEntry *entry, gpointer data) {
    GtkLabel *label = GTK_LABEL(data);
    const gchar *confirm_password = gtk_entry_get_text(GTK_ENTRY(entry));
    user_data->confirm_password = g_strdup(confirm_password);

    if (g_strcmp0(user_data->password, confirm_password) != 0)
        show_error_message(label, "Oops, these are different passwords");
    else {
        gtk_label_set_text(label, "");
        user_data->err_label = label;
    }
}

static void submit_log_clicked() {
    const gchar *login = user_data->username;
    const gchar *password = user_data->password;
    GtkLabel *label = user_data->err_label;

    if (login != NULL && password != NULL &&
        (gtk_label_get_text(label) == NULL ||
         g_strcmp0(gtk_label_get_text(label), "") == 0)) {
        printf("You have clicked submit\n");
    } else {
        show_error_message(
            label, "Fields cannot be empty.\n Errors must be corrected");
    }
}

static void submit_reg_clicked() {
    const gchar *login = user_data->username;
    const gchar *password = user_data->password;
    const gchar *confirm_p = user_data->confirm_password;
    GtkLabel *label = user_data->err_label;

    if (login != NULL && password != NULL && confirm_p != NULL &&
        (gtk_label_get_text(label) == NULL ||
         g_strcmp0(gtk_label_get_text(label), "") == 0)) {
        printf("You have clicked 'submit'");
    } else {
        show_error_message(
            label, "Fields cannot be empty.\n Errors must be corrected");
    }
}

static void on_password_entry_visibility(GtkEntry *entry,
                                         GtkEntryIconPosition icon_position) {
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

    user_data = g_new0(UserData, 1);
    if (user_data == NULL) {
        g_print("Error: Failed to allocate memory for user_data\n");
        return;
    }
    user_data->username = NULL;
    user_data->password = NULL;
    user_data->confirm_password = NULL;
    user_data->err_label = NULL;

    GtkLabel *err_label_log = GTK_LABEL(
        gtk_builder_get_object(builder, "error_message_login_label"));
    GtkLabel *err_label_reg = GTK_LABEL(
        gtk_builder_get_object(builder, "error_message_registration_label"));

    if (err_label_log == NULL || err_label_reg == NULL) {
        g_print("Error: Failed to obtain error labels\n");
        g_free(user_data);
        return;
    }

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

    if (is_empty_field(GTK_ENTRY(login_entry)) ||
        is_empty_field(GTK_ENTRY(new_login_entry)))
        show_error_message(err_label_log, "Please fill in the fields");

    g_signal_connect(login_entry, "changed",
                     G_CALLBACK(on_login_entry_changed), err_label_log);
    g_signal_connect(new_login_entry, "changed",
                     G_CALLBACK(on_login_entry_changed), err_label_reg);
    g_signal_connect(password_entry, "changed",
                     G_CALLBACK(on_password_entry_changed), err_label_log);

    g_signal_connect(new_password_entry, "changed",
                     G_CALLBACK(on_password_entry_changed), err_label_reg);
    g_signal_connect(confirm_password_entry, "changed",
                     G_CALLBACK(on_confirm_password_entry_changed),
                     err_label_reg);
    gtk_label_set_text(err_label_reg, "");
    g_signal_connect(password_entry, "icon-press",
                     G_CALLBACK(on_password_entry_visibility), NULL);
    g_signal_connect(new_password_entry, "icon-press",
                     G_CALLBACK(on_password_entry_visibility), NULL);
    g_signal_connect(confirm_password_entry, "icon-press",
                     G_CALLBACK(on_password_entry_visibility), NULL);

    GtkButton *submit_login =
        GTK_BUTTON(gtk_builder_get_object(builder, "submit_login"));
    GtkButton *submit_register =
        GTK_BUTTON(gtk_builder_get_object(builder, "submit_register"));

    g_signal_connect(submit_login, "clicked", G_CALLBACK(submit_log_clicked),
                     NULL);
    g_signal_connect(submit_register, "clicked",
                     G_CALLBACK(submit_reg_clicked), NULL);
}
