#ifndef SDK_UTILS_H
#define SDK_UTILS_H

#include "sdk/base.h"
#include "sdk/structs.h"

void mx_list_free(t_list **list, t_func_free func);
void mx_delete_node(t_list **list, t_list *node, t_func_free clean_data);

char *mx_decode_uri_component(struct mg_str str);
t_string mx_encode_uri_component(struct mg_str str);
char *mx_gen_uuid(void);

t_server_exception *mx_server_exception_parse(t_string str);
void mx_server_exception_free(t_server_exception *e);

t_header *mx_sdk_header_create(char *key, char *value);
void mx_sdk_header_free(t_header *h);
t_list *mx_headers_push_back(t_list *headers, char *key, char *value);
t_list *mx_headers_push_back_token(t_list *headers);

void mx_parse_server_response(t_response *response, t_func_parser parser);
void mx_parse_array_server_response(t_response *response,
                                    t_func_parser parser);

bool mx_is_response_ok(t_response *response);
bool mx_is_response_error(t_response *response);
#endif
