#include "client.h"

char *new_room_name = NULL;
char *new_room_description = NULL;
char *new_contact_name = NULL;

static void on_room_name_entry_changed(GtkEntry *create_room_name) {
    const gchar *text = gtk_entry_get_text(create_room_name);
    new_room_name = g_strdup(text);
}

static void on_room_description_entry_changed(GtkEntry *create_room_name) {
    const gchar *text = gtk_entry_get_text(create_room_name);
    new_room_description = g_strdup(text);
}

static void on_contact_name_entry_changed(GtkEntry *create_contact_name) {
    const gchar *text = gtk_entry_get_text(create_contact_name);
    new_contact_name = g_strdup(text);
}

static void on_cancel_create_room(GtkButton *button, gpointer user_data) {
    GtkWidget *dialog = GTK_WIDGET(user_data);
    gtk_widget_hide(dialog);
    (void)button;
}

void on_submit_create_room(GtkButton *button, gpointer user_data) {
    GtkWidget *dialog = GTK_WIDGET(user_data);
    GtkWidget *create_room_name =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "create_room_name"));

    if (is_empty_field(GTK_ENTRY(create_room_name))) {
        g_print("Room name is empty\n");
        return;
    }

    t_room_create_dto *dto = mx_room_create_dto_init();
    dto->name = g_strdup(new_room_name);
    dto->description = g_strdup(new_room_description);

    t_response *response = mx_sdk_room_post(dto);

    if (mx_is_response_error(response)) {
        g_print("Error: %s\n", mx_sdk_exception_get_message(response));
        mx_sdk_response_free(response, (t_func_free)mx_room_free);
        return;
    }

    t_room *room = response->data;

    mx_push_front(&global_rooms, room);

    append_room_to_list(room);

    mx_sdk_response_free(response, (t_func_free)mx_room_free);
    gtk_widget_hide(dialog);
    (void)button;
}

void on_submit_create_contact(GtkButton *button, gpointer user_data) {
    GtkWidget *dialog = GTK_WIDGET(user_data);
    GtkWidget *create_contact_name = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "create_contact_name"));

    if (is_empty_field(GTK_ENTRY(create_contact_name))) {
        g_print("Contact name is empty\n");
        return;
    }

    char *room_name = "Direct";

    t_room_create_dto *dto = mx_room_create_dto_init();

    dto->name = g_strdup(room_name);
    dto->description = g_strdup("Private chat");

    t_response *response = mx_sdk_room_post(dto);

    if (mx_is_response_error(response)) {
        g_print("Error: %s\n", mx_sdk_exception_get_message(response));
        mx_sdk_response_free(response, (t_func_free)mx_room_free);
        return;
    }

    t_room *room = response->data;
    t_response *user_response = mx_sdk_user_find_by_name(new_contact_name);

    if (mx_is_response_error(user_response)) {
        g_print("Error: %s\n", mx_sdk_exception_get_message(response));
        mx_sdk_response_free(user_response, (t_func_free)mx_user_free);
        return;
    }

    t_user *user = user_response->data;

    t_response *add_member_response =
        mx_sdk_room_member_post(room->id, user->id, true);

    if (mx_is_response_error(add_member_response)) {
        g_print("Error: %s\n", mx_sdk_exception_get_message(response));
        mx_sdk_response_free(add_member_response,
                             (t_func_free)mx_room_member_free);
        return;
    }

    mx_push_front(&global_rooms, room);

    mx_sdk_response_free(response, (t_func_free)mx_room_free);
    mx_sdk_response_free(user_response, (t_func_free)mx_user_free);
    mx_sdk_response_free(add_member_response,
                         (t_func_free)mx_room_member_free);
    gtk_widget_hide(dialog);
    (void)button;
}

static void show_create_room_dialog(GtkButton *button, gpointer user_data) {
    GtkWidget *dialog = GTK_WIDGET(user_data);
    GtkWidget *cancel_button = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "create_room_cancel"));
    GtkWidget *accept_button = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "create_room_submit"));
    GtkWidget *create_contact_button = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "create_contact_button"));

    GtkWidget *create_room_name =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "create_room_name"));
    GtkWidget *entry_description = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "create_room_description"));
    GtkWidget *create_contact_name = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "create_contact_name"));

    gtk_window_set_transient_for(
        GTK_WINDOW(dialog),
        GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))));
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ON_PARENT);

    g_signal_connect(cancel_button, "clicked",
                     G_CALLBACK(on_cancel_create_room), dialog);
    g_signal_connect(accept_button, "clicked",
                     G_CALLBACK(on_submit_create_room), dialog);
    g_signal_connect(create_contact_button, "clicked",
                     G_CALLBACK(on_submit_create_contact), dialog);
    g_signal_connect(create_room_name, "changed",
                     G_CALLBACK(on_room_name_entry_changed), NULL);
    g_signal_connect(entry_description, "changed",
                     G_CALLBACK(on_room_description_entry_changed), NULL);
    g_signal_connect(create_contact_name, "changed",
                     G_CALLBACK(on_contact_name_entry_changed), NULL);
    gint response = gtk_dialog_run(GTK_DIALOG(dialog));

    if (response == GTK_RESPONSE_ACCEPT) {
        g_print("OK\n");
        // Perform actions for OK button
    } else if (response == GTK_RESPONSE_CANCEL) {
        g_print("Cancel\n");
        // Perform actions for Cancel button
    }

    gtk_widget_hide(dialog);
    (void)button;
}

void init_create_room(void) {
    GtkWidget *create_room_button = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "add_chat_modal_open"));
    GtkWidget *create_room_dialog = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "create_room_dialog"));

    g_signal_connect(create_room_button, "clicked",
                     G_CALLBACK(show_create_room_dialog), create_room_dialog);
}
