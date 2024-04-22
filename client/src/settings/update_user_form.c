#include "../../inc/client.h"

char *new_username = NULL;
char *new_tag = NULL;
char *new_description = NULL;
int new_photo_id = -1;

GtkLabel *err_label = NULL;

static void show_error_message(const gchar *message) {
    gtk_label_set_text(err_label, message);
    gtk_widget_show(GTK_WIDGET(err_label));
}

static void on_username_entry_changed(GtkEntry *entry) {
    const gchar *text = gtk_entry_get_text(entry);
    new_username = g_strdup(text);
}

static void on_tag_entry_changed(GtkEntry *entry) {
    const gchar *text = gtk_entry_get_text(entry);
    new_tag = g_strdup(text);
}

static void on_description_entry_changed(GtkEntry *entry) {
    const gchar *text = gtk_entry_get_text(entry);
    new_description = g_strdup(text);
}

const char *get_file_extension(const char *file_path) {
    const char *extension = strrchr(file_path, '.');
    if (extension != NULL) {
        return extension + 1;
    }

    return NULL;
}

static void on_avatar_button_clicked(GtkFileChooserButton *button) {
    char *new_avatar_path =
        gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(button));
    if (new_avatar_path != NULL) {
        char filename[MX_BUFFER_SIZE];
        const char *extension = get_file_extension(new_avatar_path);
        snprintf(filename, sizeof(filename), "avatar.%s", extension);
        t_response *resp = mx_sdk_file_upload(new_avatar_path, filename);
        if (mx_is_response_error(resp)) {
            mx_sdk_response_free(resp, free);
            return;
        } else {
            t_file *file = (t_file *)resp->data;
            if (file != NULL)
                new_photo_id = file->id;
            mx_sdk_response_free(resp, (t_func_free)mx_file_free);
        }
    }
}

static void on_update_user_button_clicked(void) {
    t_response *user_response = mx_sdk_user_get_me();
    t_user *api_user = user_response->data;

    t_user_update_dto *dto = mx_user_update_dto_init();
    dto->name = new_username != NULL ? g_strdup(new_username)
                                     : g_strdup(api_user->name);
    dto->tag = new_tag != NULL ? g_strdup(new_tag) : g_strdup(api_user->tag);
    dto->description = new_description != NULL
                           ? g_strdup(new_description)
                           : g_strdup(api_user->description);
    dto->status = g_strdup(api_user->status);
    dto->photo_id = new_photo_id != -1 ? new_photo_id : api_user->photo_id;

    t_response *response = mx_sdk_user_put_me(dto);

    if (mx_is_response_error(response)) {
        show_error_message(mx_sdk_exception_get_message(response));
        mx_sdk_response_free(response, (t_func_free)mx_user_free);
        return;
    }

    gtk_label_set_text(err_label, "");
    mx_sdk_response_free(user_response, (t_func_free)mx_user_free);

    clean_up_settings();

    new_username = NULL;
    new_tag = NULL;
    new_description = NULL;
    new_photo_id = -1;
}

void init_update_user_form_field(void) {
    GtkWidget *username_entry =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "username_entry"));
    GtkWidget *tag_entry =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "tag_entry"));
    GtkWidget *description_entry = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "description_entry"));
    GtkWidget *submit_button = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "update_user_button"));
    GtkWidget *avatar_button =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "avatar_button"));

    err_label = GTK_LABEL(
        gtk_builder_get_object(global_builder, "error_message_update_user"));

    t_response *response = mx_sdk_user_get_me();

    if (mx_is_response_error(response)) {
        show_error_message(mx_sdk_exception_get_message(response));
        return;
    }

    g_signal_connect(username_entry, "changed",
                     G_CALLBACK(on_username_entry_changed), NULL);
    g_signal_connect(tag_entry, "changed", G_CALLBACK(on_tag_entry_changed),
                     NULL);
    g_signal_connect(description_entry, "changed",
                     G_CALLBACK(on_description_entry_changed), NULL);
    g_signal_connect(avatar_button, "file-set",
                     G_CALLBACK(on_avatar_button_clicked), NULL);

    g_signal_connect(submit_button, "clicked",
                     G_CALLBACK(on_update_user_button_clicked), NULL);

    mx_sdk_response_free(response, (t_func_free)mx_user_free);
}
