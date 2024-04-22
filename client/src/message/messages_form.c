#include "client.h"

char *message = NULL;
int global_chat_id = -1;
int edit_message_id = -1;
int reply_message_id = -1;
int new_file_id = -1;
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

static void on_close_image_preview_button_clicked(GtkButton *button,
                                                  gpointer user_data) {
    GtkWidget *image_preview_box = GTK_WIDGET(user_data);
    gtk_widget_hide(image_preview_box);
    (void)button;
}

static void on_cancel_file_chooser(GtkButton *button, gpointer user_data) {
    GtkWidget *dialog = GTK_WIDGET(user_data);
    gtk_widget_hide(dialog);
    (void)button;
}

static void on_apply_button_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *dialog = GTK_WIDGET(user_data);
    char *new_file_path =
        gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));

    if (new_file_path != NULL) {
        char filename[MX_BUFFER_SIZE];
        const char *extension = get_file_extension(new_file_path);
        snprintf(filename, sizeof(filename), "file.%s", extension);
        t_response *resp = mx_sdk_file_upload(new_file_path, filename);

        if (mx_is_response_error(resp)) {
            mx_sdk_response_free(resp, free);
            return;
        }

        t_file *file = (t_file *)resp->data;
        if (file != NULL)
            new_file_id = file->id;
        mx_sdk_response_free(resp, (t_func_free)mx_file_free);

        GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(new_file_path, NULL);
        if (pixbuf != NULL) {
            GdkPixbuf *scaled_pixbuf =
                gdk_pixbuf_scale_simple(pixbuf, 150, 150, GDK_INTERP_BILINEAR);
            g_object_unref(pixbuf);

            GtkWidget *image_preview = GTK_WIDGET(
                gtk_builder_get_object(global_builder, "image_preview"));

            gtk_image_set_from_pixbuf(GTK_IMAGE(image_preview), scaled_pixbuf);
            g_object_unref(scaled_pixbuf);

            GtkWidget *image_preview_box = GTK_WIDGET(
                gtk_builder_get_object(global_builder, "image_preview_box"));

            gtk_widget_show(image_preview_box);
        }
    }
    gtk_widget_hide(dialog);
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
    if ((!message || strlen(message) == 0 || message == NULL) &&
        new_file_id == -1) {
        return;
    }

    bool is_edit = edit_message_id != -1;

    t_message_create_dto *dto = mx_message_create_dto_init();

    if (is_edit) {
        dto->text = g_strdup(message);
        dto->reply_id = global_message->reply_id;
        dto->room_id = global_message->room_id;
        mx_push_back(&dto->file_ids, mx_itoa(new_file_id));
    } else {
        dto->text = g_strdup(message);
        dto->room_id = global_chat_id;
        dto->reply_id = reply_message_id != -1 ? reply_message_id : 0;
        mx_push_back(&dto->file_ids, mx_itoa(new_file_id));
    }

    GtkWidget *edit_message_box =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "edit_message_box"));

    if (is_edit) {
        edit_message(dto);
    } else {
        send_message(dto);
    }

    message = NULL;
    new_file_id = -1;
    GtkWidget *message_entry =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "message_entry"));
    GtkWidget *reply_box =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "reply_box"));
    GtkWidget *image_preview_box = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "image_preview_box"));
    gtk_widget_hide(reply_box);
    gtk_entry_set_text(GTK_ENTRY(message_entry), "");
    gtk_widget_hide(edit_message_box);
    gtk_widget_hide(image_preview_box);
    message = NULL;
    edit_message_id = -1;
    reply_message_id = -1;

    GtkWidget *scrolled_window = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "scrolled_chat_window"));

    g_timeout_add_full(G_PRIORITY_DEFAULT, 1000, call_scrollbar_once,
                       scrolled_window, NULL);
    gtk_widget_hide(image_preview_box);

    (void)button;
}

static void on_sticker_clicked(GtkWidget *widget) {
    char *file_path = (char *)g_object_get_data(G_OBJECT(widget), "file_path");
    if (file_path != NULL) {
        char filename[MX_BUFFER_SIZE];
        const char *extension = get_file_extension(file_path);
        snprintf(filename, sizeof(filename), "file.%s", extension);
        t_response *resp = mx_sdk_file_upload(file_path, filename);

        if (mx_is_response_error(resp)) {
            mx_sdk_response_free(resp, free);
            return;
        }

        t_file *file = (t_file *)resp->data;
        if (file != NULL)
            new_file_id = file->id;
        mx_sdk_response_free(resp, (t_func_free)mx_file_free);
    }

    submit(NULL);
    new_file_id = -1;

    GtkWidget *stickers_popover =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "stickers_popover"));

    gtk_popover_popdown(GTK_POPOVER(stickers_popover));
}

static void on_stickers_button_clicked(GtkWidget *widget, gpointer user_data) {
    GtkWidget *stickers_popover = GTK_WIDGET(user_data);

    for (int i = 1; i <= 9; i++) {
        GtkWidget *sticker_button = GTK_WIDGET(gtk_builder_get_object(
            global_builder, g_strdup_printf("sticker_button%d", i)));

        char *file_path = g_strdup_printf(
            "./client/static/images/stickers/sticker%d.png", i);
        GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(file_path, NULL);

        GdkPixbuf *scaled_pixbuf =
            gdk_pixbuf_scale_simple(pixbuf, 70, 70, GDK_INTERP_BILINEAR);

        GtkWidget *sticker = gtk_image_new_from_pixbuf(scaled_pixbuf);

        gtk_button_set_image(GTK_BUTTON(sticker_button), sticker);

        g_object_set_data_full(G_OBJECT(sticker_button), "file_path",
                               file_path, g_free);
        g_signal_connect(sticker_button, "clicked",
                         G_CALLBACK(on_sticker_clicked), NULL);
    }

    gtk_popover_set_relative_to(GTK_POPOVER(stickers_popover), widget);
    gtk_popover_set_position(GTK_POPOVER(stickers_popover), GTK_POS_TOP);

    gtk_popover_popup(GTK_POPOVER(stickers_popover));
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
    GtkWidget *file_chooser_button = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "file_chooser_button"));
    GtkWidget *stickers_button =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "stickers_button"));
    GtkWidget *stickers_popover =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "stickers_popover"));

    gtk_entry_set_text(GTK_ENTRY(message_entry), "");
    g_signal_connect(G_OBJECT(message_entry), "changed",
                     G_CALLBACK(on_message_entry_changed), NULL);
    g_signal_connect(G_OBJECT(send_button), "clicked", G_CALLBACK(submit),
                     NULL);
    g_signal_connect(G_OBJECT(close_edit_button), "clicked",
                     G_CALLBACK(on_close_edit_button_clicked), NULL);
    g_signal_connect(G_OBJECT(close_reply_button), "clicked",
                     G_CALLBACK(on_close_reply_button_clicked), NULL);
    g_signal_connect(file_chooser_button, "clicked",
                     G_CALLBACK(show_file_chooser_dialog), global_builder);
    g_signal_connect(G_OBJECT(stickers_button), "clicked",
                     G_CALLBACK(on_stickers_button_clicked), stickers_popover);
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

void show_file_chooser_dialog(GtkWidget *button, gpointer user_data) {
    GtkBuilder *builder = GTK_BUILDER(user_data);
    GtkWidget *dialog =
        GTK_WIDGET(gtk_builder_get_object(builder, "file_chooser_dialog"));
    GtkWidget *cancel_button =
        GTK_WIDGET(gtk_builder_get_object(builder, "file_chooser_cancel"));
    GtkWidget *apply_button =
        GTK_WIDGET(gtk_builder_get_object(builder, "file_chooser_apply"));
    GtkWidget *close_image_preview_button = GTK_WIDGET(
        gtk_builder_get_object(builder, "close_image_preview_button"));
    GtkWidget *image_preview_box =
        GTK_WIDGET(gtk_builder_get_object(builder, "image_preview_box"));

    gtk_window_set_transient_for(
        GTK_WINDOW(dialog),
        GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))));
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ON_PARENT);

    g_signal_connect(cancel_button, "clicked",
                     G_CALLBACK(on_cancel_file_chooser), dialog);
    g_signal_connect(apply_button, "clicked",
                     G_CALLBACK(on_apply_button_clicked), dialog);
    g_signal_connect(close_image_preview_button, "clicked",
                     G_CALLBACK(on_close_image_preview_button_clicked),
                     image_preview_box);

    gint response = gtk_dialog_run(GTK_DIALOG(dialog));

    if (response == GTK_RESPONSE_ACCEPT) {
        g_print("OK\n");
        // Perform actions for OK button
    } else if (response == GTK_RESPONSE_CANCEL) {
        g_print("Cancel\n");
        // Perform actions for Cancel button
    }
}
