#include "sdk/sdk.h"

t_response *mx_sdk_responce_init(void) {
    t_response *response = (t_response *)malloc(sizeof(t_response));
    response->status_code = 0;
    response->headers = NULL;
    response->body = NULL;
    response->data = NULL;
    response->exception = NULL;

    return response;
}

void mx_sdk_response_free(t_response *resp, t_func_free data_free) {
    if (!resp) {
        return;
    }

    mx_strdel(&resp->body);

    if (data_free) {
        data_free(resp->data);
    }
    mx_list_free(&resp->headers, (t_func_free)mx_sdk_header_free);
    mx_server_exception_free(resp->exception);
}
void mx_sdk_response_print(t_response *resp) {
    if (!resp) {
        return;
    }
    printf("==============================================\n");
    printf("Status code: %d\n", resp->status_code);
    if (resp->headers) {
        printf("\nHeaders: \n");

        for (t_list *list = resp->headers; list; list = list->next) {
            t_header *h = list->data;

            printf("%s: %s\n", h->key, h->value);
        }
    }
    if (resp->exception) {
        printf("\nException:\n");
        printf("%s %s %d\n", resp->exception->method, resp->exception->uri,
               resp->exception->timestamp);
        printf("Message: %s\n", resp->exception->message);
    } else {
        printf("\nBody: \n%s\n", resp->body);
    }
    printf("==============================================\n");
}

t_string mx_sdk_exception_get_message(t_response *resp) {
    if (!resp || !resp->exception) {
        return NULL;
    }

    return resp->exception->message;
}

// Callback function to write response body
static size_t write_body_cb(char *ptr, size_t size, size_t nmemb,
                            t_response *response) {
    size_t realsize = size * nmemb;

    response->body = realloc(response->body, realsize + 1);
    response->body[realsize] = '\0';

    mx_strncpy(response->body, ptr, realsize);

    return realsize;
}

static size_t write_header_cb(char *ptr, size_t size, size_t nmemb,
                              t_response *response) {
    size_t total_size = size * nmemb;
    char *line = mx_strndup(ptr, total_size);
    char *colon = strstr(line, ": ");
    char *end_of_line = strstr(line, "\r\n");

    if (!colon || !end_of_line || end_of_line - colon <= 2 ||
        colon - line <= 1) {
        mx_strdel(&line);
        return total_size;
    }
    char *key = mx_strndup(line, colon - line);
    char *value = mx_strndup(colon + 2, end_of_line - colon - 1);

    mx_strdel(&line);

    t_header *header = mx_sdk_header_create(key, value);

    mx_push_back(&response->headers, header);

    return total_size;
}

static struct curl_slist *headers_list_to_curl_list(t_list *headers) {
    struct curl_slist *curl_headers = NULL;

    for (t_list *l = headers; l; l = l->next) {
        t_header *header = l->data;

        char *h = mg_mprintf("%s: %s", header->key, header->value);

        curl_headers = curl_slist_append(curl_headers, h);
    }

    return curl_headers;
}

/* Function to fetch HTTP response
    @param url URL to fetch
    @param method HTTP method
    @param headers HTTP:
     1. format: t_list of t_header
     2. nullable
     3. will be cleared automatically
    @param body HTTP body
     1. nullable
     2. will be cleared automatically
    @returns t_response
*/
t_response *mx_fetch(const char *url, const char *method, t_list *headers,
                     char *body) {
    CURL *curl = curl_easy_init();
    struct curl_slist *head = headers_list_to_curl_list(headers);
    t_response *response = mx_sdk_responce_init();

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, head);

    if (body && mx_strcmp(method, MX_HTTP_METHOD_GET) != 0) {
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);
    }

    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_AUTOREFERER, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_body_cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, write_header_cb);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, response);

    CURLcode res = curl_easy_perform(curl);

    if (res == CURLE_OK) {
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE,
                          &(response->status_code));
    }
    // Clean up
    mx_strdel(&body);
    mx_list_free(&headers, (t_func_free)mx_sdk_header_free);
    mx_list_free((t_list **)&head, free);
    curl_easy_cleanup(curl);

    return response;
}

t_response *mx_fetch_multipart(const char *url, const char *method,
                               t_list *headers, curl_mime *form) {
    CURL *curl = curl_easy_init();
    struct curl_slist *head = headers_list_to_curl_list(headers);
    t_response *response = mx_sdk_responce_init();

    curl_slist_append(head, mx_strdup("Expect:"));
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, head);

    if (form && mx_strcmp(method, MX_HTTP_METHOD_GET) != 0) {
        curl_easy_setopt(curl, CURLOPT_MIMEPOST, form);
    }

    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_AUTOREFERER, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_body_cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, write_header_cb);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, response);

    CURLcode res = curl_easy_perform(curl);

    if (res == CURLE_OK) {
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE,
                          &(response->status_code));
    }
    // Clean up
    curl_mime_free(form);
    mx_list_free(&headers, (t_func_free)mx_sdk_header_free);
    mx_list_free((t_list **)&head, free);
    curl_easy_cleanup(curl);

    return response;
}
