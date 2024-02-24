
char *mx_strncpy(char *dst, const char *src, int len) {
    char *d = dst;
    const char *s = src;

    for (int i = 0; i < len; i++) {
        if ((*d++ = *s++) == '\0') {
            while (++i < len) {
                *d++ = '\0';
            }
            break;
        }
    }

    return dst;
}
