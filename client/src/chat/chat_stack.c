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

    render_messages(room->id);
    init_message_form(room->id);

    GtkWidget *scrolled_window = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "scrolled_chat_window"));

    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                   GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    g_timeout_add_full(G_PRIORITY_DEFAULT, 21,
                       call_scrollbar_once, scrolled_window, NULL);
}
