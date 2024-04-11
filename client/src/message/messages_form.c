#include "client.h"

char *message = NULL;
int global_chat_id = -1;

static void on_message_entry_changed(GtkEntry *entry) {
    message = (char *)gtk_entry_get_text(entry);
}

static void send_message(GtkButton *button) {
    if (!message || strlen(message) == 0 || message == NULL) {
        return;
    }
    t_message_create_dto *dto = mx_message_create_dto_init();
    dto->text = g_strdup(message);
    dto->room_id = global_chat_id;
    dto->reply_id = 0;
    dto->file_ids = NULL;
    t_response *response = mx_sdk_message_post(dto);
    if (mx_is_response_error(response)) {
        g_print("Error: %s\n", mx_sdk_exception_get_message(response));
        mx_sdk_response_free(response, (t_func_free)mx_message_free);
        mx_message_create_dto_free(dto);
        return;
    }

    mx_sdk_response_free(response, (t_func_free)mx_message_free);
    message = NULL;
    GtkWidget *message_entry =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "message_entry"));
    gtk_entry_set_text(GTK_ENTRY(message_entry), "");
    (void)button;
}

void init_message_form(int chat_id) {
    global_chat_id = chat_id;

    GtkWidget *message_entry =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "message_entry"));
    GtkWidget *send_button = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "send_message_button"));

    g_signal_connect(G_OBJECT(message_entry), "changed",
                     G_CALLBACK(on_message_entry_changed), NULL);
    g_signal_connect(G_OBJECT(send_button), "clicked",
                     G_CALLBACK(send_message), NULL);
}
