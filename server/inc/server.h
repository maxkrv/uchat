#ifndef UCHAT_H
#define UCHAT_H

#include "base.h"
#include "user.h"
#include "room.h"
#include "message.h"
#include "http.h"
#include "db.h"
#include "utils.h"
#include "shared.h"

int mx_run_server(t_env_params *env, char *addr);

#endif
