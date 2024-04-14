#include "client.h"

char *message = NULL;
int global_chat_id = -1;
int edit_message_id = -1;
int reply_message_id = -1;
t_message *global_message = NULL;

static void on_close_edit_button_clicked(GtkButton *button) {
    GtkWidget *edit_message_box =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "edit_message_box"));
    gtk_widget_hide(edit_message_box);
    GtkWidget *edit_message_label = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "edit_message_label"));
    GtkWidget *message_entry =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "message_entry"));

    gtk_label_set_text(GTK_LABEL(edit_message_label), "");
    gtk_entry_set_text(GTK_ENTRY(message_entry), "");

    (void)button;
}

static void on_close_reply_button_clicked(GtkButton *button) {
    GtkWidget *reply_box =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "reply_box"));
    gtk_widget_hide(reply_box);
    GtkWidget *message_reply_label = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "message_reply_label"));
    GtkWidget *user_name_reply_lable = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "user_name_reply_lable"));

    gtk_label_set_text(GTK_LABEL(message_reply_label), "");
    gtk_label_set_text(GTK_LABEL(user_name_reply_lable), "");

    (void)button;
}

static void on_message_entry_changed(GtkEntry *entry) {
    message = (char *)gtk_entry_get_text(entry);
}

static void send_message(t_message_create_dto *dto) {
    t_response *response = mx_sdk_message_post(dto);
    if (mx_is_response_error(response)) {
        g_print("Error: %s\n", mx_sdk_exception_get_message(response));
    }

    mx_sdk_response_free(response, (t_func_free)mx_message_free);
}

static void edit_message(t_message_create_dto *dto) {
    if (!edit_message_id) {
        return;
    }

    t_response *response = mx_sdk_message_put(edit_message_id, dto);
    if (mx_is_response_error(response)) {
        g_print("Error: %s\n", mx_sdk_exception_get_message(response));
    }

    mx_sdk_response_free(response, (t_func_free)mx_message_free);
}

static void submit(GtkButton *button) {
    if (!message || strlen(message) == 0 || message == NULL) {
        return;
    }

    bool is_edit = edit_message_id != -1;

    t_message_create_dto *dto = mx_message_create_dto_init();

    if (is_edit) {
        dto->text = g_strdup(message);
        dto->reply_id = global_message->reply_id;
        dto->room_id = global_message->room_id;
        dto->file_ids = NULL;
    } else {
        dto->text = g_strdup(message);
        dto->room_id = global_chat_id;
        dto->reply_id = reply_message_id != -1 ? reply_message_id : 0;
    }

    GtkWidget *edit_message_box =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "edit_message_box"));

    if (is_edit) {
        edit_message(dto);
    } else {
        send_message(dto);
    }

    message = NULL;
    GtkWidget *message_entry =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "message_entry"));
    GtkWidget *reply_box =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "reply_box"));
    gtk_widget_hide(reply_box);
    gtk_entry_set_text(GTK_ENTRY(message_entry), "");
    gtk_widget_hide(edit_message_box);

    (void)button;
}

void init_message_form(int chat_id) {
    global_chat_id = chat_id;

    GtkWidget *message_entry =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "message_entry"));
    GtkWidget *send_button = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "send_message_button"));
    GtkWidget *close_edit_button = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "close_edit_button"));
    GtkWidget *close_reply_button = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "close_reply_button"));

    gtk_entry_set_text(GTK_ENTRY(message_entry), "");
    g_signal_connect(G_OBJECT(message_entry), "changed",
                     G_CALLBACK(on_message_entry_changed), NULL);
    g_signal_connect(G_OBJECT(send_button), "clicked", G_CALLBACK(submit),
                     NULL);
    g_signal_connect(G_OBJECT(close_edit_button), "clicked",
                     G_CALLBACK(on_close_edit_button_clicked), NULL);
    g_signal_connect(G_OBJECT(close_reply_button), "clicked",
                     G_CALLBACK(on_close_reply_button_clicked), NULL);
}

void handle_edit_message(GtkButton *button, t_message *message) {
    GtkWidget *popever =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "message_popover"));
    GtkWidget *edit_message_box =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "edit_message_box"));
    GtkWidget *edit_message_label = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "edit_message_label"));
    GtkWidget *message_entry =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "message_entry"));

    gtk_label_set_text(GTK_LABEL(edit_message_label), message->text);
    gtk_entry_set_text(GTK_ENTRY(message_entry), message->text);
    gtk_widget_show(edit_message_box);

    edit_message_id = message->id;

    gtk_popover_popdown(GTK_POPOVER(popever));

    global_message = message;

    (void)button;
}

void handle_reply_message(GtkButton *button, t_message *message) {
    GtkWidget *popever =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "message_popover"));
    GtkWidget *reply_box =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "reply_box"));
    GtkWidget *user_name_reply_lable = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "user_name_reply_lable"));
    GtkWidget *message_reply_label = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "message_reply_label"));

    gtk_label_set_text(GTK_LABEL(message_reply_label), message->text);
    gtk_label_set_text(GTK_LABEL(user_name_reply_lable),
                       message->author->name);
    gtk_widget_show(reply_box);

    reply_message_id = message->id;

    gtk_popover_popdown(GTK_POPOVER(popever));

    (void)button;
}
