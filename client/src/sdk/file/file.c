#include "sdk/sdk.h"

t_response *mx_sdk_file_upload(const char *filepath, const char *filename) {
    FILE *fd = fopen(filepath, "r");

    if (!fd) {
        return mx_sdk_responce_init();
    }
    fclose(fd);

    t_sdk_env *env = mx_sdk_env_get();
    char *url = mg_mprintf("%s/api/v1/files/upload", env->backend_url);
    curl_mime *multipart = curl_mime_init(NULL);
    curl_mimepart *part = curl_mime_addpart(multipart);

    curl_mime_name(part, "file");
    curl_mime_filedata(part, filepath);
    curl_mime_filename(part, filename);

    t_response *response = mx_fetch_multipart(
        url, MX_HTTP_METHOD_POST, mx_headers_push_back_token(NULL), multipart);
    mx_parse_server_response(response, (t_func_parser)mx_file_parse_cjson);
    mx_strdel(&url);

    return response;
}

t_response *mx_sdk_file_find(int file_id) {
    t_sdk_env *env = mx_sdk_env_get();
    char *url = mg_mprintf("%s/api/v1/files/find?file_id=%d", env->backend_url,
                           file_id);
    t_response *response = mx_fetch(url, MX_HTTP_METHOD_GET, NULL, NULL);

    mx_parse_server_response(response, (t_func_parser)mx_file_parse_cjson);
    mx_strdel(&url);

    return response;
}
