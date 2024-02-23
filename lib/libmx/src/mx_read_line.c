#include "libmx.h"

int mx_read_line(char **lineptr, size_t buf_size, char delim, const int fd) {
    if (lineptr == NULL
        || buf_size == 0
        || fd < 0) {
        return -2;
    }

    char *res = mx_strnew(0);
    ssize_t bytes_read;
    char *buffer = mx_strnew(1);
    buf_size = 1;

    while ((bytes_read = read(fd, buffer, buf_size)) > 0) {
        if (buffer[0] == delim) {
            mx_strdel(&buffer);

            return mx_strlen(*lineptr = res);
        }

        char *tmp = mx_strjoin(res, buffer);
        mx_strdel(&res);
        res = tmp;
    }

    mx_strdel(&buffer);

    if (bytes_read == 0) {
        *lineptr = res;
        return  -1;
    } else if(bytes_read < 0) {
        mx_strdel(&res);
        return -2;
    }

    return mx_strlen(*lineptr = res);
}
