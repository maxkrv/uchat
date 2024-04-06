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

static void login(const char *username, const char *password) {
    t_response *response = mx_sdk_login(username, password);

    if (mx_is_response_error(response)) {
        show_error_message(user_data->err_label,
                           mx_sdk_exception_get_message(response));

        mx_sdk_response_free(response, free);
        return;
    }

    hide_auth_container();
    show_chat_container(true);
    mx_sdk_response_free(response, free);
}

static void submit_login_clicked() {
    const gchar *username = user_data->username;
    const gchar *password = user_data->password;
    GtkLabel *label = user_data->err_label;

    if (username != NULL && password != NULL &&
        (gtk_label_get_text(label) == NULL ||
         g_strcmp0(gtk_label_get_text(label), "") == 0)) {
        login(username, password);
    } else {
        show_error_message(
            label, "Fields cannot be empty.\n Errors must be corrected");
    }
}

static void submit_register_clicked() {
    const gchar *username = user_data->username;
    const gchar *password = user_data->password;
    const gchar *confirm_p = user_data->confirm_password;
    GtkLabel *label = user_data->err_label;

    if (username != NULL && password != NULL && confirm_p != NULL &&
        (gtk_label_get_text(label) == NULL ||
         g_strcmp0(gtk_label_get_text(label), "") == 0)) {
        t_user_create_dto *user_create_dto = mx_user_create_dto_init();

        user_create_dto->name = mx_strdup(username);
        user_create_dto->password = mx_strdup(password);

        t_response *response = mx_sdk_register(user_create_dto);

        if (mx_is_response_error(response)) {
            show_error_message(label, mx_sdk_exception_get_message(response));

            mx_sdk_response_free(response, (t_func_free)mx_user_free);
            return;
        }
        mx_sdk_response_free(response, (t_func_free)mx_user_free);

        login(username, password);
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

void show_auth_container() {
    GtkWidget *auth_container =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "auth_container"));

    if (auth_container == NULL) {
        g_print("Error: %s\n", "Failed to load auth_container");
        return;
    }

    gtk_container_add(GTK_CONTAINER(global_window), auth_container);

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
        gtk_builder_get_object(global_builder, "error_message_login_label"));
    GtkLabel *err_label_reg = GTK_LABEL(gtk_builder_get_object(
        global_builder, "error_message_registration_label"));

    if (err_label_log == NULL || err_label_reg == NULL) {
        g_print("Error: Failed to obtain error labels\n");
        g_free(user_data);
        return;
    }

    GtkWidget *login_entry =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "login_entry"));
    GtkWidget *password_entry =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "password_entry"));

    GtkWidget *new_login_entry =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "new_login_entry"));
    GtkWidget *new_password_entry = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "new_password_entry"));
    GtkWidget *confirm_password_entry = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "confirm_password_entry"));

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
        GTK_BUTTON(gtk_builder_get_object(global_builder, "submit_login"));
    GtkButton *submit_register =
        GTK_BUTTON(gtk_builder_get_object(global_builder, "submit_register"));

    g_signal_connect(submit_login, "clicked", G_CALLBACK(submit_login_clicked),
                     NULL);
    g_signal_connect(submit_register, "clicked",
                     G_CALLBACK(submit_register_clicked), NULL);
}

void hide_auth_container() {
    GtkWidget *auth_container =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "auth_container"));

    if (auth_container == NULL) {
        g_print("Error: %s\n", "Failed to load auth_container");
        return;
    }

    gtk_container_remove(GTK_CONTAINER(global_window), auth_container);
    g_free(user_data->username);
    g_free(user_data->password);
    g_free(user_data->confirm_password);
    g_free(user_data);
    user_data = NULL;
}
