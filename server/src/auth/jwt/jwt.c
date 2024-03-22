#include "jwt.h"

static char *create_jwt_header(int expiration_time) {
    time_t now = time(NULL);
    t_jwt_head head = {"HS256", "JWT", now + expiration_time, now};

    char *header_json = mx_jwt_head_stringify(&head);
    char *header =
        mx_base64_encode((unsigned char *)header_json, strlen(header_json));
    mx_strdel(&header_json);

    return header;
}

static bool verify_jwt_header(char *header) {
    time_t now = time(NULL);
    t_jwt_head head = mx_jwt_head_parse(header);

    if (mx_strcmp(head.alg, "HS256") != 0 || mx_strcmp(head.typ, "JWT") != 0 ||
        head.iat < 0 || head.exp < now) {
        mx_jwt_head_free(&head);
        return false;
    }
    mx_jwt_head_free(&head);

    return true;
}

static char *concat_with_dot_and_free(char *str1, char *str2) {
    char *s1_and_dot = mx_strjoin(str1, ".");
    char *s1_and_s1 = mx_strjoin(s1_and_dot, str2);
    mx_strdel(&str1);
    mx_strdel(&str2);
    mx_strdel(&s1_and_dot);

    return s1_and_s1;
}

static int get_str_arr_len(char **arr) {
    int i = 0;

    while (arr[i] != NULL) {
        i++;
    }

    return i;
}

t_jwt_token mx_create_jwt(char *payload_str, char *secret,
                          int expiration_time) {
    char *header = create_jwt_header(expiration_time);
    char *payload =
        mx_base64_encode((unsigned char *)payload_str, strlen(payload_str));
    char *header_and_payload = concat_with_dot_and_free(header, payload);
    char *signature = mx_create_sha256_signature(header_and_payload, secret);

    return concat_with_dot_and_free(header_and_payload, signature);
}

t_jwt_payload mx_verify_jwt(char *jwt, char *secret) {
    char **token = mx_strsplit(jwt, '.');

    if (get_str_arr_len(token) != 3) {
        mx_del_strarr(&token);

        return NULL;
    }

    char *payload = mx_base64_decode(token[1]);
    char *header = mx_base64_decode(token[0]);

    if (!verify_jwt_header(header)) {
        mx_del_strarr(&token);
        mx_strdel(&payload);
        return NULL;
    }

    char *header_and_payload =
        concat_with_dot_and_free(mx_strdup(token[0]), mx_strdup(token[1]));
    if (!mx_verify_sha256_signature(token[2], header_and_payload, secret)) {
        mx_del_strarr(&token);
        mx_strdel(&payload);
        mx_strdel(&header_and_payload);

        return NULL;
    };
    mx_del_strarr(&token);
    mx_strdel(&header_and_payload);

    return payload;
}
