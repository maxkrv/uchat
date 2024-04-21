#include "client.h"

static void set_chat_scrollbar_to_bottom(GtkWidget *scrolled_chat_window) {
    GtkAdjustment *adjustment = gtk_scrolled_window_get_vadjustment(
        GTK_SCROLLED_WINDOW(scrolled_chat_window));
    double upper = gtk_adjustment_get_upper(adjustment);
    double page_size = gtk_adjustment_get_page_size(adjustment);
    gtk_adjustment_set_value(adjustment, upper - page_size);
}

gboolean call_scrollbar_once(gpointer data) {
    GtkWidget *scrolled_chat_window = GTK_WIDGET(data);
    set_chat_scrollbar_to_bottom(scrolled_chat_window);
    return G_SOURCE_REMOVE;
}

void show_selected_room(t_room *room, bool should_render_messages) {
    GtkWidget *chat_container =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "chat_container"));
    GtkWidget *chat_stack =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "chat_stack"));
    GtkWidget *empty_state_widget =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "chat_empty"));
    GtkWidget *edit_message_box =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "edit_message_box"));
    GtkWidget *reply_box =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "reply_box"));
    GtkWidget *room_settings_button = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "room_settings_button"));
    GtkWidget *image_preview_box = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "image_preview_box"));
    GtkWidget *close_pin_message_button = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "close_pin_message_button"));

    gtk_widget_hide(empty_state_widget);
    gtk_widget_hide(edit_message_box);
    gtk_widget_hide(reply_box);
    gtk_widget_hide(room_settings_button);
    gtk_widget_hide(image_preview_box);
    gtk_widget_hide(chat_stack);

    show_pin_message(room->id);

    gtk_box_pack_start(GTK_BOX(chat_container), chat_stack, TRUE, TRUE, 0);

    if (g_strcmp0(room->type, "notes") != 0 &&
        g_strcmp0(room->type, "direct") != 0) {
        gtk_widget_show(room_settings_button);
    }

    gtk_widget_show(chat_stack);

    GtkWidget *room_user_name = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "chat_room_username"));
    GtkWidget *room_description =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "room_description"));
    GtkWidget *room_avatar =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "chat_avatar"));

    set_room_photo(room, room_avatar);
    gtk_widget_show(room_avatar);
    set_room_name(room, room_user_name);
    gtk_label_set_text(GTK_LABEL(room_description), room->description);

    if (should_render_messages) {
        render_messages(room->id);
    }

    init_message_form(room->id);

    g_signal_connect(room_settings_button, "clicked",
                     G_CALLBACK(show_room_settings_dialog), global_builder);
    g_signal_connect(close_pin_message_button, "clicked",
                     G_CALLBACK(hide_pin_message), global_builder);
    init_edit_room_form(room);
    init_members_form(room->id);
    render_room_members(room->id);

    GtkWidget *scrolled_window = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "scrolled_chat_window"));

    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                   GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);

    g_timeout_add_full(G_PRIORITY_DEFAULT, 21, call_scrollbar_once,
                       scrolled_window, NULL);
}
