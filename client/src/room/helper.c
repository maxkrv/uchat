#include "client.h"

void clean_up_room_creation(void) {
    GtkWidget *create_room_name =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "create_room_name"));
    GtkWidget *entry_description = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "create_room_description"));
    GtkWidget *create_contact_name = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "create_contact_name"));

    gtk_entry_set_text(GTK_ENTRY(create_room_name), "");
    gtk_entry_set_text(GTK_ENTRY(entry_description), "");
    gtk_entry_set_text(GTK_ENTRY(create_contact_name), "");    
}
