#include "../../inc/client.h"

gboolean is_empty_field(GtkEntry *entry) {
    const gchar *text = gtk_entry_get_text(entry);
    return (g_strcmp0(text, "") == 0);
}
