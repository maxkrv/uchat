#include "../../inc/client.h"

static size_t write_callback(void *contents, size_t size, size_t nmemb,
                             gpointer user_data) {
    GByteArray *buffer = (GByteArray *)user_data;
    g_byte_array_append(buffer, (const guint8 *)contents, size * nmemb);
    return size * nmemb;
}

// Function to load image from URL using libcurl
// Usage:
// ```c
// GtkWidget *image = gtk_image_new();
// const char *url = "https://i.etsystatic.com/26144034/r/il/4b2747/"
//                   "2893024997/il_570xN.2893024997_hxfx.jpg";
// GdkPixbuf *pixbuf = load_pixbuf_from_url(url);
// if (pixbuf != NULL) {
//     gtk_image_set_from_pixbuf(GTK_IMAGE(image), pixbuf);
//     g_object_unref(pixbuf);
// } else {
//     g_printerr("Error loading image from URL\n");
// }
// ```
GdkPixbuf *load_pixbuf_from_url(const char *url, int width, int height) {
    CURL *curl = curl_easy_init();

    if (curl) {
        GByteArray *buffer = g_byte_array_new();
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, buffer);

        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res == CURLE_OK) {
            GdkPixbufLoader *loader = gdk_pixbuf_loader_new();
            gdk_pixbuf_loader_write(loader, (const guint8 *)buffer->data,
                                    buffer->len, NULL);
            gdk_pixbuf_loader_close(loader, NULL);
            g_byte_array_unref(buffer);

            GdkPixbuf *pixbuf = gdk_pixbuf_loader_get_pixbuf(loader);

            if (pixbuf != NULL) {
                GdkPixbuf *scaled_pixbuf = gdk_pixbuf_scale_simple(
                    pixbuf, width, height, GDK_INTERP_BILINEAR);
                g_object_unref(pixbuf);
                return scaled_pixbuf;
            } else {
                g_object_unref(loader);
                return NULL;
            }
        } else {
            g_byte_array_unref(buffer);
            g_printerr("Error loading image from URL: %s\n",
                       curl_easy_strerror(res));
        }
    }

    return NULL;
}

/* Creates image in circle
    @param src_pixbuf: source image
    @param size: size of the circle(width and height)

    @return: new image in circle
 */
GdkPixbuf *create_circled_image(GdkPixbuf *src_pixbuf, int size) {
    // Create a surface to draw on
    cairo_surface_t *surface =
        cairo_image_surface_create(CAIRO_FORMAT_ARGB32, size, size);
    cairo_t *cr = cairo_create(surface);

    // Create a circular path
    cairo_arc(cr, size / 2.0, size / 2.0, size / 2.0, 0, 2 * G_PI);
    cairo_clip(cr);

    // Scale and draw the image onto the circular path
    double scale_x = (double)size / gdk_pixbuf_get_width(src_pixbuf);
    double scale_y = (double)size / gdk_pixbuf_get_height(src_pixbuf);
    cairo_scale(cr, scale_x, scale_y);
    gdk_cairo_set_source_pixbuf(cr, src_pixbuf, 0, 0);
    cairo_paint(cr);

    // Cleanup
    cairo_destroy(cr);
    cairo_surface_flush(surface);

    // Convert the surface back to a pixbuf
    GdkPixbuf *result = gdk_pixbuf_get_from_surface(surface, 0, 0, size, size);

    // Free resources
    cairo_surface_destroy(surface);

    return result;
}
