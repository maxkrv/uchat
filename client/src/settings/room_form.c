#include "client.h"

static char *new_room_name = NULL;
static char *new_room_description = NULL;
int new_room_photo_id = -1;

static void clean_up(void) {
    GtkWidget *edit_room_name = GTK_WIDGET(gtk_builder_get_object(
        global_builder, "room_settings_entry_room_name"));
    GtkWidget *edit_room_description = GTK_WIDGET(gtk_builder_get_object(
        global_builder, "room_settings_entry_description"));
    GtkWidget *edit_room_avatar = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "room_avatar_button"));

    gtk_entry_set_text(GTK_ENTRY(edit_room_name), "");
    gtk_entry_set_text(GTK_ENTRY(edit_room_description), "");

    gtk_file_chooser_unselect_all(GTK_FILE_CHOOSER(edit_room_avatar));
}

static void on_edit_room_name_changed(GtkEntry *entry, gpointer user_data) {
    new_room_name = (char *)gtk_entry_get_text(entry);
    (void)user_data;
}

static void on_edit_room_description_changed(GtkEntry *entry,
                                             gpointer user_data) {
    new_room_description = (char *)gtk_entry_get_text(entry);
    (void)user_data;
}

static void on_edit_room_avatar_clicked(GtkFileChooserButton *button) {
    char *new_room_avatar_path =
        gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(button));
    if (new_room_avatar_path != NULL) {
        char filename[MX_BUFFER_SIZE];
        const char *extension = get_file_extension(new_room_avatar_path);
        snprintf(filename, sizeof(filename), "room_avatar.%s", extension);
        t_response *resp = mx_sdk_file_upload(new_room_avatar_path, filename);
        if (mx_is_response_error(resp)) {
            mx_sdk_response_free(resp, (t_func_free)mx_file_free);
            return;
        } else {
            t_file *file = (t_file *)resp->data;
            if (file != NULL)
                new_room_photo_id = file->id;
            mx_sdk_response_free(resp, (t_func_free)mx_file_free);
        }
    }
}

static void on_edit_room_submit(GtkButton *button, t_room *room) {
    if (new_room_description == NULL && new_room_name == NULL &&
        new_room_photo_id == -1) {
        return;
    }
    GtkWidget *dialog = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "room_settings_dialog"));
    t_room_create_dto *dto = mx_room_create_dto_init();

    dto->name =
        new_room_name != NULL ? g_strdup(new_room_name) : g_strdup(room->name);

    dto->description = new_room_description != NULL
                           ? g_strdup(new_room_description)
                           : g_strdup(room->description);

    dto->photo_id =
        new_room_photo_id != -1 ? new_room_photo_id : room->photo_id;

    t_response *response = mx_sdk_room_put(room->id, dto);

    t_room *updated_room = response->data;

    GtkWidget *room_user_name = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "chat_room_username"));
    GtkWidget *room_description =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "room_description"));

    set_room_name(updated_room, room_user_name);
    gtk_label_set_text(GTK_LABEL(room_description), updated_room->description);

    gtk_widget_hide(dialog);

    new_room_name = NULL;
    new_room_description = NULL;
    new_room_photo_id = -1;
    clean_up();
    mx_sdk_response_free(response, (t_func_free)mx_room_free);

    (void)button;
}

void init_edit_room_form(t_room *room) {
    GtkWidget *edit_room_name = GTK_WIDGET(gtk_builder_get_object(
        global_builder, "room_settings_entry_room_name"));
    GtkWidget *edit_room_description = GTK_WIDGET(gtk_builder_get_object(
        global_builder, "room_settings_entry_description"));
    GtkWidget *edit_room_avatar = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "room_avatar_button"));
    GtkWidget *edit_room_submit = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "room_settings_submit"));

    g_signal_connect(edit_room_name, "changed",
                     G_CALLBACK(on_edit_room_name_changed), room);
    g_signal_connect(edit_room_description, "changed",
                     G_CALLBACK(on_edit_room_description_changed), room);
    g_signal_connect(edit_room_avatar, "file-set",
                     G_CALLBACK(on_edit_room_avatar_clicked), NULL);

    g_signal_connect(edit_room_submit, "clicked",
                     G_CALLBACK(on_edit_room_submit), room);
}
