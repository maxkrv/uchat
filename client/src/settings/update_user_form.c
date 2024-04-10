#include "../../inc/client.h"

char *new_username = NULL;
char *new_tag = NULL;
char *new_description = NULL;
int new_photo_id = -1;
t_user *api_user = NULL;

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

char *find_format(const char *s, int c) {
    while (*s != '\0') {
        if (*s == c)
            return (char *) s;
        s++;
    }
    return NULL;
}

static const char *get_file_extension(const char *file_path) {
    const char *with_dot = (find_format(file_path, '.'));

    if (with_dot != NULL && with_dot != file_path)
	return with_dot + 1;
    return "";
}

static void on_avatar_button_clicked(GtkFileChooserButton *button) {
    char *new_avatar_path =
        gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(button));
    if (new_avatar_path != NULL) {
        char filename[15];
        const char *extension = get_file_extension(new_avatar_path);
        snprintf(filename, sizeof(filename), "avatar.%s", extension);
	printf("Extension: %s\n", get_file_extension(new_avatar_path));
        printf("Path: %s\n", new_avatar_path);
        t_response *resp = mx_sdk_file_upload(new_avatar_path, filename);
        if (mx_is_response_error(resp)) {
            mx_sdk_response_free(resp, free);
	    return;
	}
	else {
	    t_file *file = (t_file *)resp->data;
            if (file != NULL)
                new_photo_id = file->id;
            mx_sdk_response_free(resp, (t_func_free)mx_file_free);
        }
     }
}

static void on_update_user_button_clicked(void) {
    MG_INFO((api_user->name));

    t_user_update_dto *dto = mx_user_update_dto_init();

    dto->name = new_username != NULL ? g_strdup(new_username)
                                     : g_strdup(api_user->name);
    dto->tag = new_tag != NULL ? g_strdup(new_tag) : g_strdup(api_user->tag);
    dto->description = new_description != NULL
                           ? g_strdup(new_description)
                           : g_strdup(api_user->description);
    dto->status = g_strdup(api_user->status);
    dto->photo_id = new_photo_id != -1 ? new_photo_id : api_user->photo_id;
    printf("Photo_id = %i", new_photo_id);

    t_response *response = mx_sdk_user_put_me(dto);

    if (mx_is_response_error(response)) {
        g_print("Error: %s\n", mx_sdk_exception_get_message(response));

        mx_sdk_response_free(response, (t_func_free)mx_user_free);
        return;
    }

    mx_sdk_response_free(response, (t_func_free)mx_user_free);
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

    t_response *response = mx_sdk_user_get_me();

    if (mx_is_response_error(response)) {
        g_print("Error: %s\n", mx_sdk_exception_get_message(response));
        return;
    }

    t_user *user = mx_entity_parse_string(response->body,
                                          (t_func_parser)mx_user_parse_cjson);

    api_user = user;
   
    g_signal_connect(username_entry, "changed",
                     G_CALLBACK(on_username_entry_changed), NULL);
    g_signal_connect(tag_entry, "changed", G_CALLBACK(on_tag_entry_changed),
                     NULL);
    g_signal_connect(description_entry, "changed",
                     G_CALLBACK(on_description_entry_changed), NULL);
    g_signal_connect(submit_button, "clicked",
                     G_CALLBACK(on_update_user_button_clicked), NULL);
    g_signal_connect(avatar_button, "file-set",
                     G_CALLBACK(on_avatar_button_clicked), NULL);

    mx_sdk_response_free(response, (t_func_free)mx_user_free);
}
