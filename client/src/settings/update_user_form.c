#include "../../inc/client.h"

char *new_username = NULL;
char *new_tag = NULL;
char *new_description = NULL;

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

static void on_update_user_button_clicked() {
    

    t_user_update_dto *dto = mx_user_update_dto_init();

    if (new_username != NULL) {
        dto->name = mx_strdup(new_username);
    }
    if (new_tag != NULL) {
        dto->tag = mx_strdup(new_tag);
    }
    if (new_description != NULL) {
        dto->description = mx_strdup(new_description);
    }

    g_print("Updating user with:\n");
    g_print("Name: %s\n", dto->name);
    g_print("Tag: %s\n", dto->tag);
    g_print("Description: %s\n", dto->description);


    t_response *response = mx_sdk_user_put_me(dto);

    if (mx_is_response_error(response)) {
        g_print("Error: %s\n", response->exception->message);

        mx_sdk_response_free(response, free);
        return;
    }

    mx_sdk_response_print(response);
    mx_sdk_response_free(response, free);
}


void init_update_user_form_field() {
    GtkWidget *username_entry = GTK_WIDGET(gtk_builder_get_object(global_builder, "username_entry"));
    GtkWidget *tag_entry = GTK_WIDGET(gtk_builder_get_object(global_builder, "tag_entry"));
    GtkWidget *description_entry = GTK_WIDGET(gtk_builder_get_object(global_builder, "description_entry"));
    GtkWidget *submit_button = GTK_WIDGET(gtk_builder_get_object(global_builder, "update_user_button"));

    g_signal_connect(username_entry, "changed", G_CALLBACK(on_username_entry_changed), NULL);
    g_signal_connect(tag_entry, "changed", G_CALLBACK(on_tag_entry_changed), NULL);
    g_signal_connect(description_entry, "changed", G_CALLBACK(on_description_entry_changed), NULL);
    g_signal_connect(submit_button, "clicked", G_CALLBACK(on_update_user_button_clicked), NULL);
}

