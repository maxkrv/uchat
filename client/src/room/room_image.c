#include "client.h"

void set_room_photo(t_room *room, GtkWidget *image) {
    t_response *response_user = mx_sdk_user_get_me();
    
    if (mx_is_response_error(response_user)) {
        g_print("Error: %s\n", mx_sdk_exception_get_message(response_user));
        mx_sdk_response_free(response_user, (t_func_free)mx_user_free);
        return;
    }

    t_response *response_members = mx_sdk_room_members_get(room->id);

    if (mx_is_response_error(response_members)) {
        g_print("Error: %s\n", mx_sdk_exception_get_message(response_members));
        mx_list_free((t_list **)&response_members->data,
                     (t_func_free)mx_room_member_free);
        mx_sdk_response_free(response_members, NULL);
        return;
    }

    t_user *user = response_user->data;
    
    if (g_strcmp0(room->type, "direct") == 0) {
        t_user *other_user = NULL;

        for (t_list *iter = response_members->data; iter != NULL;
             iter = iter->next) {
            t_room_member *member = (t_room_member *)iter->data;

            if (member->user->id != user->id) {
                other_user = member->user;
                break;
            }
        }

        if (other_user == NULL) {
            g_print("Error: Other user not found.\n");
            return;
        }

        GdkPixbuf *pixbuf = load_pixbuf_from_url(other_user->photo->url, 40, 40);
        if (pixbuf == NULL) {
            gtk_image_set_from_file(GTK_IMAGE(image),
                                "client/static/images/avatar.png");
            return;
        }
        GdkPixbuf *rounded_pixbuf = create_circled_image(pixbuf, 40);
        g_object_unref(pixbuf);
        gtk_image_set_from_pixbuf(GTK_IMAGE(image), rounded_pixbuf);
        g_object_unref(rounded_pixbuf);

    } else if (g_strcmp0(room->type, "notes") == 0) {
        GdkPixbuf *pixbuf =
            gdk_pixbuf_new_from_file("client/static/images/notes.png", NULL);
        GdkPixbuf *scaled_pixbuf =
            gdk_pixbuf_scale_simple(pixbuf, 35, 35, GDK_INTERP_BILINEAR);

        gtk_image_set_from_pixbuf(GTK_IMAGE(image), scaled_pixbuf);
        g_object_unref(pixbuf);
        g_object_unref(scaled_pixbuf);
    } else {
        GdkPixbuf *pixbuf = load_pixbuf_from_url(room->photo->url, 40, 40);
        if (pixbuf == NULL) {
            gtk_image_set_from_file(GTK_IMAGE(image),
                                "client/static/images/avatar.png");
            return;
        }
        
        GdkPixbuf *rounded_pixbuf = create_circled_image(pixbuf, 40);
        g_object_unref(pixbuf);
        gtk_image_set_from_pixbuf(GTK_IMAGE(image), rounded_pixbuf);
        g_object_unref(rounded_pixbuf);
    }

    mx_sdk_response_free(response_user, (t_func_free)mx_user_free);
    mx_list_free((t_list **)&response_members->data,
                 (t_func_free)mx_room_member_free);
    mx_sdk_response_free(response_members, NULL);
}
