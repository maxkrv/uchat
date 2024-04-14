#include "client.h"

static int user_id = -1;

static void handle_delete_message(GtkButton *button, t_message *message) {

    GtkWidget *popover =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "message_popover"));

    t_response *response = mx_sdk_message_delete(message->id);

    if (mx_is_response_error(response)) {
        g_print("Error: %s\n", mx_sdk_exception_get_message(response));
    }

    gtk_popover_popdown(GTK_POPOVER(popover));
    mx_sdk_response_free(response, (t_func_free)mx_message_free);

    (void)button;
}

void on_message_click(GtkWidget *widget, t_message *message) {
    bool is_mine = message->author_id == user_id;

    GtkWidget *popover =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "message_popover"));

    GtkWidget *reply_button = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "reply_message_button"));
    GtkWidget *delete_button = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "delete_message_button"));
    GtkWidget *edit_button = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "edit_message_button"));

    gtk_widget_hide(delete_button);
    gtk_widget_hide(edit_button);

    gtk_popover_set_relative_to(GTK_POPOVER(popover), widget);
    gtk_popover_set_position(GTK_POPOVER(popover), GTK_POS_TOP);

    gtk_popover_popup(GTK_POPOVER(popover));

    g_signal_connect(delete_button, "clicked",
                     G_CALLBACK(handle_delete_message), message);
    g_signal_connect(edit_button, "clicked", G_CALLBACK(handle_edit_message),
                     message);
    g_signal_connect(reply_button, "clicked", G_CALLBACK(handle_reply_message),
                     message);

    if (is_mine) {
        gtk_widget_show(delete_button);
        gtk_widget_show(edit_button);
    }
}

static void append_message(t_message *message, t_user *user,
                           GtkWidget *message_list_box) {
    bool is_mine = message->author_id == user->id;
    user_id = user->id;

    GtkWidget *message_button = gtk_button_new();
    GtkWidget *message_widget = gtk_button_box_new(GTK_ORIENTATION_VERTICAL);
    GtkWidget *message_user_name = gtk_label_new(message->author->name);
    GtkWidget *message_text = gtk_label_new(message->text);

    int hours, minutes;
    unixTimeToHoursMinutes(message->created_at, &hours, &minutes);
    char *time = g_strdup_printf("%02d:%02d", hours, minutes);
    GtkWidget *message_time = gtk_label_new(time);

    gtk_label_set_xalign(GTK_LABEL(message_user_name), 0);
    gtk_label_set_xalign(GTK_LABEL(message_text), 0);
    gtk_label_set_xalign(GTK_LABEL(message_time), 0);

    gtk_box_pack_start(GTK_BOX(message_widget), message_user_name, FALSE,
                       FALSE, 2);
    gtk_box_pack_start(GTK_BOX(message_widget), message_text, FALSE, FALSE, 2);
    gtk_box_pack_start(GTK_BOX(message_widget), message_time, FALSE, FALSE, 2);

    gtk_widget_set_halign(message_user_name, GTK_ALIGN_START);
    gtk_widget_set_halign(message_text, GTK_ALIGN_START);
    gtk_widget_set_halign(message_time, GTK_ALIGN_START);

    gtk_container_add(GTK_CONTAINER(message_button), message_widget);

    gtk_widget_set_halign(message_button,
                          is_mine ? GTK_ALIGN_END : GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(message_list_box), message_button, TRUE, TRUE,
                       2);

    GtkStyleContext *context;

    context = gtk_widget_get_style_context(message_widget);
    gtk_style_context_add_class(context, "message_box");

    context = gtk_widget_get_style_context(message_user_name);
    gtk_style_context_add_class(context, "message_username");

    context = gtk_widget_get_style_context(message_text);
    gtk_style_context_add_class(context, "message_text");

    context = gtk_widget_get_style_context(message_time);
    gtk_style_context_add_class(context, "message_time");

    context = gtk_widget_get_style_context(message_button);
    gtk_style_context_add_class(context, "message_bubble");

    g_signal_connect(message_button, "clicked", G_CALLBACK(on_message_click),
                     message);

    gtk_widget_show_all(message_button);
}

void render_messages(int room_id) {
    GtkWidget *message_list_box = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "messages_list_box"));

    t_response *response = mx_sdk_messages_get(room_id);
    t_response *response_user = mx_sdk_user_get_me();

    if (mx_is_response_error(response)) {
        g_print("Error: %s\n", mx_sdk_exception_get_message(response));
        mx_sdk_response_free(response, (t_func_free)mx_user_free);
        return;
    }
    t_user *user = response_user->data;

    GList *children, *iter;
    children = gtk_container_get_children(GTK_CONTAINER(message_list_box));

    for (iter = children; iter != NULL; iter = g_list_next(iter)) {
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    }

    for (t_list *current = response->data; current; current = current->next) {
        t_message *message = current->data;

        append_message(message, user, message_list_box);
    }

    mx_sdk_response_free(response, NULL);
    mx_sdk_response_free(response_user, (t_func_free)mx_user_free);

    gtk_widget_show(message_list_box);
}
