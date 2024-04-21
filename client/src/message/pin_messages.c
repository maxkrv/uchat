#include "client.h"

static t_pined_message *get_pin_message(int room_id) {
    t_response *pin_messages = mx_sdk_room_pined_get(room_id);
    t_list *data = pin_messages->data;
    t_pined_message *result = NULL;
    for (t_list *current = data; current; current = current->next) {
        if (current->next != NULL) {
            continue;
        }
        result = current->data;
    }
    mx_sdk_response_free(pin_messages, NULL);
    return result;
}

void show_pin_message(int room_id) {
    GtkWidget *pin_message_preview = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "pin_message_preview"));
    GtkWidget *pin_username =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "pin_username"));
    GtkWidget *pin_message =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "pin_message"));
    gtk_widget_hide(pin_message_preview);
    t_pined_message *message = get_pin_message(room_id);
    if (message == NULL) {
        return;
    }
    gtk_label_set_text(GTK_LABEL(pin_username),
                       message->message->author->name);
    gtk_label_set_text(GTK_LABEL(pin_message), message->message->text);
    gtk_widget_show(pin_message_preview);
    
}

void hide_pin_message() {
    GtkWidget *pin_message_preview = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "pin_message_preview"));

    gtk_widget_hide(pin_message_preview);
}

void handle_pin_message(int room_id, int message_id) {
    if (!room_id || !message_id) {
        return;
    }

    t_response *response = mx_sdk_room_pined_post(room_id, message_id);

    if (mx_is_response_ok(response)) {
        mx_sdk_response_free(response, (t_func_free)mx_room_pined_free);
    }
}
