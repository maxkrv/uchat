#include "client.h"

static void on_room_button_clicked(GtkWidget *widget, t_room *room) {
    show_selected_room(room, true);
    (void)widget;
}

static void delete_room(GtkWidget *widget, t_room *room) {
    GtkWidget *popover =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "message_popover"));

    t_response *response = mx_sdk_room_delete(room->id);

    if (mx_is_response_error(response)) {
        g_print("Error: %s\n", mx_sdk_exception_get_message(response));
    }
    gtk_popover_popdown(GTK_POPOVER(popover));
    mx_sdk_response_free(response, (t_func_free)mx_room_free);

    (void)widget;
}

static void on_pin_room_click(GtkWidget *widget, t_room *room) {
    GtkWidget *popover =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "message_popover"));

    t_response *response = mx_sdk_user_favorites_post(room->id);
    if (mx_is_response_error(response)) {
        g_print("Error: %s\n", mx_sdk_exception_get_message(response));
        mx_sdk_response_free(response, (t_func_free)mx_favorite_room_free);
        return;
    }
    gtk_popover_popdown(GTK_POPOVER(popover));
    mx_sdk_response_free(response, (t_func_free)mx_favorite_room_free);

    render_rooms();

    (void)widget;
}

static void on_unpin_room_click(GtkWidget *widget, t_room *room) {
    GtkWidget *popover =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "message_popover"));

    t_response *response = mx_sdk_user_favorites_delete(room->id);

    if (mx_is_response_error(response)) {
        g_print("Error: %s\n", mx_sdk_exception_get_message(response));
        mx_sdk_response_free(response, (t_func_free)mx_favorite_room_free);
        return;
    }
    gtk_popover_popdown(GTK_POPOVER(popover));
    mx_sdk_response_free(response, (t_func_free)mx_favorite_room_free);
    (void)widget;

    render_rooms();
}

static gboolean on_right_button_clicked(GtkWidget *widget,
                                        GdkEventButton *event, t_room *room) {
    if (event->button == 3) {
        GtkWidget *popover =
            GTK_WIDGET(gtk_builder_get_object(global_builder, "room_popover"));
        GtkWidget *delete_room_button = GTK_WIDGET(
            gtk_builder_get_object(global_builder, "delete_room_button"));
        GtkWidget *pin_room_button = GTK_WIDGET(
            gtk_builder_get_object(global_builder, "pin_room_button"));
        GtkWidget *unpin_room_button = GTK_WIDGET(
            gtk_builder_get_object(global_builder, "unpin_room_button"));

        gtk_widget_hide(pin_room_button);
        gtk_widget_hide(unpin_room_button);

        if (room->is_favorite) {
            gtk_widget_show(unpin_room_button);
        } else {
            gtk_widget_show(pin_room_button);
        }

        gtk_popover_set_relative_to(GTK_POPOVER(popover), widget);
        gtk_popover_set_position(GTK_POPOVER(popover), GTK_POS_BOTTOM);

        gtk_popover_popup(GTK_POPOVER(popover));

        g_signal_connect(delete_room_button, "clicked",
                         G_CALLBACK(delete_room), room);
        g_signal_connect(pin_room_button, "clicked",
                         G_CALLBACK(on_pin_room_click), room);
        g_signal_connect(unpin_room_button, "clicked",
                         G_CALLBACK(on_unpin_room_click), room);
    }

    return FALSE;
}

void append_room_to_list(t_room *room, bool is_favorite) {
    room->is_favorite = is_favorite;

    GtkWidget *rooms_list =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "rooms_list"));

    GtkWidget *room_button = gtk_button_new();

    GtkWidget *room_name = gtk_label_new("");
    GtkWidget *image = gtk_image_new();

    set_room_name(room, room_name);
    set_room_photo(room, image);
    GtkWidget *room_info = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);

    gtk_box_pack_start(GTK_BOX(room_info), image, FALSE, FALSE, 2);
    gtk_box_pack_start(GTK_BOX(room_info), room_name, FALSE, FALSE, 2);

    if (is_favorite) {
        GtkWidget *fav_icon = gtk_image_new();
        gtk_image_set_from_file(GTK_IMAGE(fav_icon),
                                "client/static/images/icons/pin-icon.png");
        gtk_box_pack_start(GTK_BOX(room_info), fav_icon, TRUE, TRUE, 2);

        gtk_widget_set_halign(fav_icon, GTK_ALIGN_END);
    }

    gtk_widget_set_halign(room_name, GTK_ALIGN_START);
    gtk_container_add(GTK_CONTAINER(room_button), room_info);

    gtk_box_pack_start(GTK_BOX(rooms_list), room_button, FALSE, FALSE, 2);
    gtk_widget_show_all(rooms_list);

    g_signal_connect(room_button, "clicked",
                     G_CALLBACK(on_room_button_clicked), room);

    if (g_strcmp0(room->type, "notes") != 0) {
        g_signal_connect(room_button, "button-press-event",
                         G_CALLBACK(on_right_button_clicked), room);
    }
}

static t_room *get_notes_room(t_list *rooms) {
    for (t_list *current = rooms; current; current = current->next) {
        t_room *room = current->data;

        if (room && mx_strcmp(room->type, "notes") == 0) {
            return room;
        }
    }
    return NULL;
}

static t_list *get_other_rooms(t_list *rooms, t_list *favorites) {
    t_list *other_rooms = NULL;

    for (t_list *current = rooms; current; current = current->next) {
        t_room *room = current->data;

        if (g_strcmp0(room->type, "notes") == 0) {
            continue;
        }
        bool is_favorite = false;

        for (t_list *current = favorites; current; current = current->next) {
            t_room *favorite = current->data;

            if (favorite->id == room->id) {
                is_favorite = true;
                break;
            }
        }
        if (!is_favorite) {
            mx_push_back(&other_rooms, room);
        }
    }

    return other_rooms;
}

static t_list *get_favorite_rooms(void) {
    t_response *favorites_response = mx_sdk_user_favorites_get();

    if (mx_is_response_error(favorites_response)) {
        g_print("Error: %s\n",
                mx_sdk_exception_get_message(favorites_response));
        mx_list_free((t_list **)&favorites_response->data,
                     (t_func_free)mx_favorite_room_free);
        mx_sdk_response_free(favorites_response, NULL);
        return NULL;
    }

    t_list *list = NULL;

    for (t_list *current = favorites_response->data; current;
         current = current->next) {
        t_favorite_room *favorite = current->data;

        mx_push_back(&list, favorite->room);
    }
    mx_list_free((t_list **)&favorites_response->data, free);
    mx_sdk_response_free(favorites_response, NULL);
    return list;
}

void render_rooms() {
    t_response *rooms_response = mx_sdk_rooms_get();

    if (mx_is_response_error(rooms_response)) {
        g_print("Error: %s\n", mx_sdk_exception_get_message(rooms_response));
        mx_list_free((t_list **)&rooms_response->data,
                     (t_func_free)mx_room_free);
        mx_sdk_response_free(rooms_response, NULL);
        return;
    }

    GtkWidget *rooms_list =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "rooms_list"));

    GList *children, *iter;
    children = gtk_container_get_children(GTK_CONTAINER(rooms_list));

    for (iter = children; iter != NULL; iter = g_list_next(iter)) {
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    }

    t_room *notes_room = get_notes_room(rooms_response->data);
    append_room_to_list(notes_room, false);

    t_list *favorite_rooms = get_favorite_rooms();

    for (t_list *current = favorite_rooms; current; current = current->next) {
        append_room_to_list(current->data, true);
    }
    t_list *other_rooms =
        get_other_rooms(rooms_response->data, favorite_rooms);

    for (t_list *current = other_rooms; current; current = current->next) {
        append_room_to_list(current->data, false);
    }
    mx_sdk_response_free(rooms_response, NULL);
}
