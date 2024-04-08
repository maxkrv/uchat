#include "client.h"

void show_selected_room(t_room *room) {
    g_print("Selected room: %s\n", room->name);
    g_print("Selected room id: %d\n", room->id);

    GtkWidget *chat_container =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "chat_container"));
    GtkWidget *chat_stack =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "chat_stack"));
    GtkWidget *empty_state_widget =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "chat_empty"));
    GtkWidget *message_list_box = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "messages_list_box"));
    gtk_widget_hide(empty_state_widget);
    gtk_widget_hide(chat_stack);
    global_messages = NULL;

    gtk_box_pack_start(GTK_BOX(chat_container), chat_stack, TRUE, TRUE, 0);
    gtk_widget_show(chat_stack);

    GtkWidget *room_user_name = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "chat_room_username"));
    GtkWidget *room_description =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "room_description"));

    gtk_label_set_text(GTK_LABEL(room_user_name), room->name);
    gtk_label_set_text(GTK_LABEL(room_description), room->description);

    t_response *response = mx_sdk_messages_get(room->id);
    global_messages = response->data;

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

    for (t_list *current = global_messages; current; current = current->next) {
        t_message *message = current->data;

        GtkWidget *message_widget = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
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
        gtk_box_pack_start(GTK_BOX(message_widget), message_text, FALSE, FALSE,
                           2);
        gtk_box_pack_start(GTK_BOX(message_widget), message_time, FALSE, FALSE,
                           2);
        gtk_widget_set_halign(message_widget,
                              message->author_id == user->id
                                  ? GTK_ALIGN_END
                                  : GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(message_list_box), message_widget, FALSE,
                         FALSE, 2);

        //Add widgets styles
        GtkStyleContext *context;

        context = gtk_widget_get_style_context(message_widget);
        gtk_style_context_add_class(context, "message_box");

        context = gtk_widget_get_style_context(message_user_name);
        gtk_style_context_add_class(context, "message_username");

        context = gtk_widget_get_style_context(message_text);
        gtk_style_context_add_class(context, "message_text");

        context = gtk_widget_get_style_context(message_time);
        gtk_style_context_add_class(context, "message_time");

        gtk_widget_show_all(message_widget);
    }

    mx_list_free((t_list **)&response->data, (t_func_free)mx_message_free);
    mx_sdk_response_free(response, NULL);

    gtk_widget_show(message_list_box);
}
