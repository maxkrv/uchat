#include "client.h"

char *new_room_name = NULL;
char *new_room_description = NULL;
char *new_contact_name = NULL;

static void show_error_message(GtkLabel *err_label, const gchar *message) {
    gtk_label_set_text(err_label, message);
    gtk_widget_show(GTK_WIDGET(err_label));
}

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
    GtkLabel *err_label_room = 
        GTK_LABEL(gtk_builder_get_object(global_builder, "error_message_create_group_chat"));

    if (is_empty_field(GTK_ENTRY(create_room_name))) {
        show_error_message(err_label_room, "Room name is empty\n");
        return;
    }

    t_room_create_dto *dto = mx_room_create_dto_init();
    dto->name = g_strdup(new_room_name);
    dto->description = g_strdup(new_room_description);

    t_response *response = mx_sdk_room_post(dto);

    if (mx_is_response_error(response)) {
        show_error_message(err_label_room, mx_sdk_exception_get_message(response));
        mx_sdk_response_free(response, (t_func_free)mx_room_free);
        return;
    }

    gtk_label_set_text(err_label_room, "");

    render_rooms();

    mx_sdk_response_free(response, (t_func_free)mx_room_free);
    gtk_widget_hide(dialog);
    clean_up_room_creation();
    new_room_name = NULL;
    new_room_description = NULL;
    new_contact_name = NULL;
    (void)button;
}

void on_submit_create_contact(GtkButton *button, gpointer user_data) {
    GtkWidget *dialog = GTK_WIDGET(user_data);
    GtkWidget *create_contact_name = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "create_contact_name"));
    GtkLabel *err_label_contact = 
        GTK_LABEL(gtk_builder_get_object(global_builder, "error_message_add_contact"));

    if (is_empty_field(GTK_ENTRY(create_contact_name))) {
        show_error_message(err_label_contact, "Contact name is empty\n");
        return;
    }

    char *room_name = "Direct";

    t_room_create_dto *dto = mx_room_create_dto_init();

    dto->name = g_strdup(room_name);
    dto->description = g_strdup("Private chat");

    t_response *response = mx_sdk_room_direct_post(dto, new_contact_name);

    if (mx_is_response_error(response)) {
        show_error_message(err_label_contact, mx_sdk_exception_get_message(response));
        mx_sdk_response_free(response, (t_func_free)mx_room_free);
        return;
    }

    gtk_label_set_text(err_label_contact, "");

    mx_sdk_response_free(response, (t_func_free)mx_room_free);
    gtk_widget_hide(dialog);
    clean_up_room_creation();
    new_room_name = NULL;
    new_room_description = NULL;
    new_contact_name = NULL;
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
    clean_up_room_creation();
    new_room_name = NULL;
    new_room_description = NULL;
    new_contact_name = NULL;
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
