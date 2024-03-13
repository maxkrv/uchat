#include "server.h"

t_file *mx_file_repo_get(int id) {
    return mx_init_file();
}
int mx_file_repo_create(char *filename, char *url) {
    return 1;
}

bool mx_file_repo_delete(int file_id) {
    return true;
}
