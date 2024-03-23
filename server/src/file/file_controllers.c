#include "server.h"

void mx_file_ctrl_get(t_connection *c, t_http_message *m) {
    int file_id = mx_extract_id_from_query(m->query, "file_id");

    if (file_id <= 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid file id provided");
        return;
    }

    t_file *file = mx_file_service_get(file_id);

    if (!file) {
        mx_http_reply_exception(c, m, HTTP_STATUS_NOT_FOUND, "File not found");
        return;
    }

    t_string json_string = mx_file_stringify(file);

    mg_http_reply(c, HTTP_STATUS_OK, MX_HEADERS_JSON, json_string);
    mx_strdel(&json_string);
    mx_file_free(file);
}

void mx_file_ctrl_upload(t_connection *c, t_http_message *m) {
    struct mg_http_part part;
    t_user_id user_id = mx_auth(m);

    if (user_id <= 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNAUTHORIZED,
                                "Invalid token provided");
        return;
    }
    mg_http_next_multipart(m->body, 0, &part);

    if (!(mg_strcmp(part.name, mg_str("file")) == 0) || !part.filename.ptr ||
        c->recv.len > MX_MAX_FILE_SIZE) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid file provided");
        return;
    }

    t_string filename = mx_strndup(part.filename.ptr, part.filename.len);

    if (!filename) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid file provided");
        return;
    }
    t_file *file =
        mx_file_service_upload((char *)part.body.ptr, part.body.len, filename);

    mx_strdel(&filename);

    if (!file) {
        mx_http_reply_exception(c, m, HTTP_STATUS_INTERNAL_SERVER_ERROR,
                                "Cant upload file");
        return;
    }

    t_string json_string = mx_file_stringify(file);

    mg_http_reply(c, HTTP_STATUS_CREATED, MX_HEADERS_JSON, json_string);
    mx_strdel(&json_string);
    mx_file_free(file);
}
