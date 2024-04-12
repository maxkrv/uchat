#include "client.h"

static void show_settings_container_cb(void) {
    hide_chat_container();
    show_settings_container();
}


static void populate_chat_side_bar(t_user *user, bool should_load_rooms) {
    GtkWidget *rooms_list =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "rooms_list"));
    GtkWidget *user_name =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "user_name"));
    GtkWidget *chat_scroll = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "chat_scroll"));

    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(chat_scroll),
                                   GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);

    gtk_label_set_text(GTK_LABEL(user_name), user->name);

    for (t_list *current = global_rooms; current; current = current->next) {
        if (!should_load_rooms) {
            break;
        }

        t_room *room = current->data;

        append_room_to_list(room);
    }

    gtk_widget_show_all(rooms_list);

    GtkWidget *user_avatar =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "user_avatar"));

    t_response *file_resp = mx_sdk_file_find(user->photo_id);
    if (mx_is_response_error(file_resp)) {
        g_print("Error: %s\n", file_resp->exception->message);
        return;
    }
    t_file *file = (t_file *)file_resp->data;
    if (file != NULL) {
        GdkPixbuf *pixbuf = load_pixbuf_from_url(file->url);
        if (pixbuf != NULL) {
            gtk_image_set_pixel_size(GTK_IMAGE(user_avatar), 20);
            gtk_image_set_from_pixbuf(GTK_IMAGE(user_avatar), pixbuf);
            g_object_unref(pixbuf);
        }
    }

    printf("%s\n", file->url);
    mx_sdk_response_free(file_resp, (t_func_free)mx_file_free);
}

void show_chat_container(bool should_load_rooms) {
    GtkWidget *chat_container =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "chat_container"));

    if (chat_container == NULL) {
        g_print("Error: %s\n", "Failed to load chat_container");
        return;
    }

    t_response *response = mx_sdk_user_get_me();

    if (mx_is_response_error(response)) {
        g_print("Error: %s\n", mx_sdk_exception_get_message(response));
        mx_sdk_response_free(response, (t_func_free)mx_user_free);
        return;
    }
    t_user *user = response->data;
    global_user = user;
    t_response *rooms_response = mx_sdk_rooms_get();

    if (mx_is_response_error(rooms_response)) {
        g_print("Error: %s\n", mx_sdk_exception_get_message(response));
        mx_sdk_response_free(rooms_response, (t_func_free)mx_room_free);
        return;
    }

    t_list *rooms = rooms_response->data;
    global_rooms = rooms;

    populate_chat_side_bar(user, should_load_rooms);
    mx_sdk_response_free(response, (t_func_free)mx_user_free);

    GtkWidget *user_settings_button = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "user_settings_button"));

    g_signal_connect(user_settings_button, "clicked",
                     G_CALLBACK(show_settings_container_cb), global_builder);

    init_create_room();

    GtkWidget *room_settings_button = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "room_settings_button"));

    if (room_settings_button == NULL) {
        g_print("Error: %s\n", "Failed to load chat_settings_button");
    }

    g_signal_connect(room_settings_button, "clicked",
                     G_CALLBACK(show_room_settings_dialog), global_builder);

    gtk_container_add(GTK_CONTAINER(global_window), chat_container);
}

void hide_chat_container() {
    GtkWidget *chat_container =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "chat_container"));

    if (chat_container == NULL) {
        g_print("Error: %s\n", "Failed to load chat_container");
        return;
    }

    gtk_container_remove(GTK_CONTAINER(global_window), chat_container);
}
