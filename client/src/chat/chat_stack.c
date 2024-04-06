#include "client.h"

void show_selected_room(t_room *room) {
    GtkWidget *chat_container =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "chat_container"));
    GtkWidget *chat_stack =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "chat_stack"));
    GtkWidget *empty_state_widget =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "chat_empty"));

    gtk_widget_hide(empty_state_widget);
    gtk_widget_hide(chat_stack);

    gtk_box_pack_start(GTK_BOX(chat_container), chat_stack, TRUE, TRUE, 0);
    gtk_widget_show(chat_stack);

    GtkWidget *room_user_name = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "chat_room_username"));
    GtkWidget *room_description =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "room_description"));

    gtk_label_set_text(GTK_LABEL(room_user_name), room->name);
    gtk_label_set_text(GTK_LABEL(room_description), room->description);
}
