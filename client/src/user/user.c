#include "client.h"

void render_user(void) {
    t_response *response = mx_sdk_user_get_me();

    if (mx_is_response_error(response)) {
        g_print("Error: %s\n", mx_sdk_exception_get_message(response));
    }

    t_user *user = response->data;

    GtkWidget *user_name =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "user_name"));
    gtk_label_set_text(GTK_LABEL(user_name), user->name);

    GtkWidget *user_avatar =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "user_avatar"));

    t_response *file_resp = mx_sdk_file_find(user->photo_id);

    t_file *file = file_resp->data;
    if (file != NULL) {
        GdkPixbuf *pixbuf = load_pixbuf_from_url(file->url, 40, 40);
        GdkPixbuf *rounded_pixbuf = create_circled_image(pixbuf, 40);
        g_object_unref(pixbuf);

        if (pixbuf != NULL) {
            gtk_image_set_pixel_size(GTK_IMAGE(user_avatar), 20);
            gtk_image_set_from_pixbuf(GTK_IMAGE(user_avatar), rounded_pixbuf);
            g_object_unref(rounded_pixbuf);
        }
    }

    mx_sdk_response_free(file_resp, (t_func_free)mx_file_free);
    mx_sdk_response_free(response, (t_func_free)mx_user_free);
}
