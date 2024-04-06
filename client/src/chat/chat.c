#include "client.h"

static void show_settings_container_cb(void) {
    hide_chat_container();
    show_settings_container();
}

static void set_chat_scrollbar_to_bottom(GtkWidget *scrolled_chat_window) {
    GtkAdjustment *adjustment = gtk_scrolled_window_get_vadjustment(
        GTK_SCROLLED_WINDOW(scrolled_chat_window));
    double upper = gtk_adjustment_get_upper(adjustment);
    double page_size = gtk_adjustment_get_page_size(adjustment);
    gtk_adjustment_set_value(adjustment, upper - page_size);
}

// static void show_room_cb(

static void populate_chat_side_bar(t_user *user, t_list *rooms) {
    // GtkWidget *rooms_list =
    //     GTK_WIDGET(gtk_builder_get_object(global_builder, "rooms_list"));
    GtkWidget *user_name =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "user_name"));

    gtk_label_set_text(GTK_LABEL(user_name), user->name);
    (void)rooms;
}

void show_chat_container() {
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
    t_user *user = mx_entity_parse_string(response->body,
                                          (t_func_parser)mx_user_parse_cjson);

    t_response *rooms_response = mx_sdk_rooms_get();

    if (mx_is_response_error(rooms_response)) {
        g_print("Error: %s\n", mx_sdk_exception_get_message(response));
        mx_sdk_response_free(rooms_response, (t_func_free)mx_room_free);
        return;
    }

    mx_sdk_response_print(rooms_response);

    t_list *rooms = rooms_response->data;

    for (t_list *current = rooms; current; current = current->next) {
        t_room *room = current->data;

        g_print("Room id: %d\n", room->id);
        g_print("Room name: %s\n", room->name);
    }

    populate_chat_side_bar(user, rooms);
    mx_sdk_response_free(response, (t_func_free)mx_user_free);
    GtkWidget *scrolled_window = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "scrolled_chat_window"));
    GtkWidget *user_settings_button = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "user_settings_button"));

    g_signal_connect(user_settings_button, "clicked",
                     G_CALLBACK(show_settings_container_cb), global_builder);

    set_chat_scrollbar_to_bottom(scrolled_window);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                   GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    init_create_room();

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
