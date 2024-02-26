#ifndef UCHAT_MESSAGE
#define UCHAT_MESSAGE

#include "base.h"
#include "utils.h"
#include "shared.h"


cJSON *mx_message_to_cjson(t_message *message);

cJSON *mx_messages_to_cjson(t_list *messages);

t_string mx_message_stringify(t_message *message);


cJSON *mx_read_message_to_cjson(t_read_message *message);

cJSON *mx_read_messages_to_cjson(t_list *message);

t_string mx_read_message_stringify(t_read_message *message);

#endif
