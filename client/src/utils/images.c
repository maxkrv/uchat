// #include "../../inc/client.h"

// static size_t write_callback(void *contents, size_t size, size_t nmemb,
//                              gpointer user_data) {
//     GByteArray *buffer = (GByteArray *)user_data;
//     g_byte_array_append(buffer, (const guint8 *)contents, size * nmemb);
//     return size * nmemb;


// // Function to load image from URL using libcurl
// // Usage:
// // ```c
// // GtkWidget *image = gtk_image_new();
// // const char *url = "https://i.etsystatic.com/26144034/r/il/4b2747/"
// //                   "2893024997/il_570xN.2893024997_hxfx.jpg";
// // GdkPixbuf *pixbuf = load_pixbuf_from_url(url);
// // if (pixbuf != NULL) {
// //     gtk_image_set_from_pixbuf(GTK_IMAGE(image), pixbuf);
// //     g_object_unref(pixbuf);
// // } else {
// //     g_printerr("Error loading image from URL\n");
// // }
// // ```
// GdkPixbuf *load_pixbuf_from_url(const char *url) {
//     CURL *curl = curl_easy_init();

//     if (curl) {
//         GByteArray *buffer = g_byte_array_new();
//         curl_easy_setopt(curl, CURLOPT_URL, url);
//         curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
//         curl_easy_setopt(curl, CURLOPT_WRITEDATA, buffer);

//         CURLcode res = curl_easy_perform(curl);
//         curl_easy_cleanup(curl);

//         if (res == CURLE_OK) {
//             GdkPixbufLoader *loader = gdk_pixbuf_loader_new();
//             gdk_pixbuf_loader_write(loader, (const guint8 *)buffer->data,
//                                     buffer->len, NULL);
//             gdk_pixbuf_loader_close(loader, NULL);
//             g_byte_array_unref(buffer);
//             return gdk_pixbuf_loader_get_pixbuf(loader);
//         } else {
//             g_byte_array_unref(buffer);
//             g_printerr("Error loading image from URL: %s\n",
//                        curl_easy_strerror(res));
//         }
//     }

//     return NULL;
// }
