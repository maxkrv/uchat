#include "client.h"

static void on_room_button_clicked(GtkWidget *widget, t_room *room) {
    show_selected_room(room);
    (void)widget;
}

void append_room_to_list(t_room *room) {
    GtkWidget *rooms_list =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "rooms_list"));
    GtkWidget *room_button = gtk_button_new();
    bool should_load_image = room->photo_id != 0;

    GtkWidget *room_name = gtk_label_new("");
    GtkWidget *image = gtk_image_new();

    set_room_name(room, room_name);

    if (should_load_image) {
        GdkPixbuf *pixbuf = load_pixbuf_from_url(room->photo->url);
        gtk_image_set_from_pixbuf(GTK_IMAGE(image), pixbuf);
        g_object_unref(pixbuf);
    } else {
        gtk_image_set_from_file(GTK_IMAGE(image),
                                "client/static/images/avatar.png");
    }

    GtkWidget *room_info = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);

    gtk_box_pack_start(GTK_BOX(room_info), image, FALSE, FALSE, 2);
    gtk_box_pack_start(GTK_BOX(room_info), room_name, FALSE, FALSE, 2);
    gtk_widget_set_halign(room_name, GTK_ALIGN_START);
    gtk_container_add(GTK_CONTAINER(room_button), room_info);

    gtk_box_pack_start(GTK_BOX(rooms_list), room_button, FALSE, FALSE, 2);
    gtk_widget_show_all(rooms_list);

    g_signal_connect(room_button, "clicked",
                     G_CALLBACK(on_room_button_clicked), room);
}

void render_rooms() {
    t_response *rooms_respone = mx_sdk_rooms_get();

    if (mx_is_response_error(rooms_respone)) {
        g_print("Error: %s\n", mx_sdk_exception_get_message(rooms_respone));
        mx_list_free((t_list **)&rooms_respone->data,
                     (t_func_free)mx_room_free);
        mx_sdk_response_free(rooms_respone, NULL);
        return;
    }

    GtkWidget *rooms_list =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "rooms_list"));

    GList *children, *iter;
    children = gtk_container_get_children(GTK_CONTAINER(rooms_list));

    for (iter = children; iter != NULL; iter = g_list_next(iter)) {
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    }

    for (t_list *current = rooms_respone->data; current;
         current = current->next) {
        append_room_to_list(current->data);
    }

    // mx_list_free((t_list **)&rooms_respone->data, (t_func_free)mx_room_free);
    // mx_sdk_response_free(rooms_respone, NULL);
}
