#include "client.h"

static void on_room_button_clicked(GtkWidget *widget, t_room *room) {
    (void)widget;

    show_selected_room(room);
}

void append_room_to_list(t_room *room) {
    GtkWidget *rooms_list =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "rooms_list"));
    GtkWidget *room_button = gtk_button_new();
    bool should_load_image = room->photo_id != 0;

    GtkWidget *room_name = gtk_label_new(room->name);
    GtkWidget *image = gtk_image_new();

    if (should_load_image) {
        GdkPixbuf *pixbuf = load_pixbuf_from_url(room->photo->url);
        gtk_image_set_from_pixbuf(GTK_IMAGE(image), pixbuf);
        g_object_unref(pixbuf);
    } else {
        gtk_image_set_from_file(
            GTK_IMAGE(image),
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
