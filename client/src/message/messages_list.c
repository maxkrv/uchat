#include "client.h"

void render_messages(int room_id) {
    g_print("Room id: %d\n", room_id);
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
        gtk_widget_set_halign(message_widget, message->author_id == user->id
                                                  ? GTK_ALIGN_END
                                                  : GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(message_list_box), message_widget, FALSE,
                         FALSE, 2);

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
    mx_sdk_response_free(response_user, (t_func_free)mx_user_free);

    gtk_widget_show(message_list_box);
}
