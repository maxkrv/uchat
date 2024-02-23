#include "user.h"

void mx_user_ctrl_get(t_connection *c, t_http_message *m) {
  // parse into params
  // validate params
  t_user *user = mx_user_get(1);

  // user struct to json
  // send response
  // send events to clients if necessary(for example "client new message created
  // please update your data")
}

void mx_user_ctrl_post(t_connection *c, t_http_message *m) {}

void mx_user_ctrl_put(t_connection *c, t_http_message *m) {}

void mx_user_ctrl_delete(t_connection *c, t_http_message *m) {}
