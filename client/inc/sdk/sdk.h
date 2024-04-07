#ifndef MX_SDK_H
#define MX_SDK_H

#include "sdk/base.h"
#include "sdk/utils.h"
#include "sdk/http.h"
#include "sdk/structs.h"
#include "sdk/dto.h"
#include "sdk/entity.h"
#include "sdk/json.h"

/*H**********************************************************************
 * IMPORTANT :
 *       - The functions that starts from mx_sdk
 *         are the main functions of sdk
 *
 *       - Also SDK provides functions for creating/deleting
 *         different useful entities (CHECK structs.h and entity.h)
 *
 *       - You need to call function mx_sdk_init to initialize SDK
 *         and mx_sdk_free to free SDK
 *
 *       - In some functions you need to pass dto struct
 *         (you can find them in structs.h)
 *         in this case use fucntions from dto.h file(to init this structures)
 *         you dont need to worry about freeing those structures
 *         (after you pass them to functions)
 *
 *       - Most of the functions has comments that describe how
 *         to free t_response and what is the type of data(t_response.data)
 *
 *       - The t_response has void *data and t_server_exception *exception.
 *         Be careful they are nullable.
 *         functions will parse server response for you
 *         and will put result in data if request was successful
 *         and in exception otherwise
 *
 *       - You can find documentation for api in server/docs folder
 *H*/

/* Inits everythig that is necessary for sdk */
void mx_sdk_init(const char *backend_url);
/* Frees all resources in sdk */
void mx_sdk_free(void);

/* Creates sdk responce struct(t_responce) in dynamic memory */
t_response *mx_sdk_responce_init(void);
/* Prints sdk responce struct(t_responce) */
void mx_sdk_response_print(t_response *resp);
/* Returns exception message from sdk responce struct(t_responce) */
t_string mx_sdk_exception_get_message(t_response *resp);
/* Frees sdk responce struct(t_responce) in dynamic memory */
void mx_sdk_response_free(t_response *resp, t_func_free data_free);

/* Function to fetch HTTP request
    @param url URL to fetch
    @param method HTTP method
    @param headers HTTP:
     1. format: t_list of t_header
     2. nullable
     3. will be cleared automatically
    @param body HTTP body
     1. nullable
     2. will be cleared automatically
    @returns not parsed(data and exception equal NULL) t_response *
*/
t_response *mx_fetch(const char *url, const char *method, t_list *headers,
                     char *body);
t_response *mx_fetch_multipart(const char *url, const char *method,
                               t_list *headers, curl_mime *form);
/*
    to free response:
    mx_sdk_response_free(response, free);

    @return t_responce *(data == t_string(jwt_token))
 */
t_response *mx_sdk_login(const char *name, const char *password);
/*
     deletes credentials from memory

     @return bool true if successful
  */
bool mx_sdk_logout(void);
/*
    to free response:
    mx_sdk_response_free(response, free);

    @return t_responce *(data == t_string(jwt_token))
 */
t_response *mx_sdk_refresh_token(void);
/*
    to free response:
    mx_sdk_response_free(resp, (t_func_free)mx_user_free)

    @return t_responce *(data == t_user *)
 */
t_response *mx_sdk_change_password(const char *old_password,
                                   const char *new_password);
/*
    to free response::
    mx_sdk_response_free(resp, (t_func_free)mx_user_free)

    passed dto will be freed automatically
    @return t_responce *(data == t_user *)
 */
t_response *mx_sdk_register(t_user_create_dto *dto);
/*
    to free response::
    mx_sdk_response_free(resp, (t_func_free)mx_file_free)

    @return t_responce *(data == t_file *)
 */
t_response *mx_sdk_file_upload(const char *filepath, const char *filename);
/*
    to free response::
    mx_sdk_response_free(resp, (t_func_free)mx_file_free)

    @return t_responce *(data == t_file *)
 */
t_response *mx_sdk_file_find(int file_id);
/*
    to free response:
    mx_sdk_response_free(resp, (t_func_free)mx_message_free);

    @return t_responce *(data == t_message *)
 */

t_response *mx_sdk_message_find(int message_id);
/*
    to free response:
    mx_list_free((t_list **)&resp->data, (t_func_free)mx_message_free);
    mx_sdk_response_free(resp, NULL);

    @return t_responce *(data == t_list *(list of t_message))
 */
t_response *mx_sdk_messages_get(int room_id);
/*
    to free response:
    mx_sdk_response_free(resp, (t_func_free)mx_message_free);

    dto will be freed
    @return t_responce *(data == t_message *)
 */
t_response *mx_sdk_message_post(t_message_create_dto *message);
/*
    to free response:
    mx_sdk_response_free(resp, (t_func_free)mx_message_free);

    dto will be freed
    @return t_responce *(data == t_message *)
 */
t_response *mx_sdk_message_put(int message_id, t_message_create_dto *message);
/*
    to free response:
    mx_sdk_response_free(resp, (t_func_free)mx_message_free);

    @return t_responce *(data == t_message *)
 */
t_response *mx_sdk_message_delete(int message_id);
/*
    to free response:
    mx_list_free((t_list **)&resp->data, (t_func_free)mx_read_message_free);
    mx_sdk_response_free(resp, NULL);

    @return t_responce *(data == t_list *(list of t_read_message))
 */
t_response *mx_sdk_readers_get(int message_id);
/*
    to free response:
    mx_sdk_response_free(resp, (t_func_free)mx_read_message_free);

    @return t_responce *(data == t_read_message*)
 */
t_response *mx_sdk_readers_post(int message_id);
/*
    to free response:
    mx_sdk_response_free(resp, (t_func_free)mx_read_message_free);

    @return t_responce *(data == t_read_message*)
 */
t_response *mx_sdk_reader_find(int read_id);
/*
    to free response:
    mx_sdk_response_free(resp, (t_func_free)mx_room_free);

    @return t_responce *(data == t_room *)
 */
t_response *mx_sdk_room_find(int room_id);
/*
    to free response:
    mx_list_free((t_list **)&resp->data, (t_func_free)mx_room_free);
    mx_sdk_response_free(resp, NULL);

    @return t_responce *(data == t_list *(list of t_room))
 */
t_response *mx_sdk_rooms_get(void);
/*
    to free response:
    mx_sdk_response_free(resp, (t_func_free)mx_room_free);

    dto will be freed
    @return t_responce *(data == t_room *)
 */
t_response *mx_sdk_room_post(t_room_create_dto *room);
/*
    to free response:
    mx_sdk_response_free(resp, (t_func_free)mx_room_free);

    dto will be freed
    @return t_responce *(data == t_room *)
 */
t_response *mx_sdk_room_direct_post(t_room_create_dto *room,
                                    t_string username);
/*
    to free response:
    mx_sdk_response_free(resp, (t_func_free)mx_room_free);

    dto will be freed
    @return t_responce *(data == t_room *)
 */
t_response *mx_sdk_room_put(int room_id, t_room_create_dto *room);
/*
    to free response:
    mx_sdk_response_free(resp, (t_func_free)mx_room_free);

    @return t_responce *(data == t_room *)
 */
t_response *mx_sdk_room_delete(int room_id);
/*
    to free response:
    mx_list_free((t_list **)&resp->data, (t_func_free)mx_room_pined_free);
    mx_sdk_response_free(resp, NULL);

    @return t_responce *(data == t_list *(list of t_pined_message))
 */
t_response *mx_sdk_room_pined_get(int room_id);
/*
    to free response:
    mx_sdk_response_free(resp, (t_func_free)mx_room_pined_free);

    @return t_responce * (data == t_pined_message *)
 */
t_response *mx_sdk_room_pined_post(int room_id, int message_id);
/*
    to free response:
    mx_sdk_response_free(resp, (t_func_free)mx_room_pined_free);

    @return t_responce * (data == t_pined_message *)
 */
t_response *mx_sdk_room_pined_delete(int pined_id);
/*
    to free response:
    mx_sdk_response_free(resp, (t_func_free)mx_room_member_free);

    @return t_responce *(data == t_room_member *)
 */
t_response *mx_sdk_room_members_find(int member_id);
/*
    to free response:
    mx_list_free((t_list **)&resp->data, (t_func_free)mx_room_member_free);
    mx_sdk_response_free(resp, NULL);

    @return t_responce *(data == t_list(list of t_room_member))
 */
t_response *mx_sdk_room_members_get(int room_id);
/*
    to free response:
    mx_sdk_response_free(resp, (t_func_free)mx_room_member_free);

    @return t_responce *(data == t_room_member *)
 */
t_response *mx_sdk_room_member_post(int room_id, int user_id, bool is_admin);
/*
    to free response:
    mx_sdk_response_free(resp, (t_func_free)mx_room_member_free);

    @return t_responce *(data == t_room_member *)
 */
t_response *mx_sdk_room_member_put(int member_id, bool is_admin);
/*
    to free response:
    mx_sdk_response_free(resp, (t_func_free)mx_room_member_free);

    @return t_responce *(data == t_room_member *)
 */
t_response *mx_sdk_room_member_delete(int member_id);
/*
    to free response:
    mx_sdk_response_free(resp, (t_func_free)mx_user_free);

    @return t_responce *(data == t_user *)
 */
t_response *mx_sdk_user_find_by_id(int user_id);
/*
    to free response:
    mx_sdk_response_free(resp, (t_func_free)mx_user_free);

    @return t_responce *(data == t_user *)
 */
t_response *mx_sdk_user_find_by_tag(t_string tag);
/*
    to free response:
    mx_sdk_response_free(resp, (t_func_free)mx_user_free);

    @return t_responce *(data == t_user *)
 */
t_response *mx_sdk_user_find_by_name(t_string name);
/*
     to free response:
     mx_sdk_response_free(resp, (t_func_free)mx_user_free);

     @return t_responce *(data == t_user *)
*/
t_response *mx_sdk_user_get_me(void);
/*
    to free response:
    mx_sdk_response_free(resp, (t_func_free)mx_user_free);

    dto will be freed
    @return t_responce *(data == t_user *)
 */
t_response *mx_sdk_user_put_me(t_user_update_dto *dto);
/*
    to free response:
    mx_sdk_response_free(resp, (t_func_free)mx_user_free);

    @return t_responce *(data == t_user *)
 */
t_response *mx_sdk_user_delete_me(void);
/*
    to free response:
    mx_list_free((t_list **)&resp->data, (t_func_free)mx_favorite_room_free);
    mx_sdk_response_free(resp, NULL);

    @return t_responce *(data == t_list *(list of t_favorite_room))
 */
t_response *mx_sdk_user_favorites_get(void);
/*
    to free response:
    mx_sdk_response_free(resp, (t_func_free)mx_favorite_room_free);

    @return t_responce *(data == t_favorite_room *)
 */
t_response *mx_sdk_user_favorites_post(int room_id);
/*
    to free response:
    mx_sdk_response_free(resp, (t_func_free)mx_favorite_room_free);

    @return t_responce *(data == t_favorite_room *)
 */
t_response *mx_sdk_user_favorites_delete(int favorite_id);
/*
    to free response:
    mx_list_free((t_list **)&resp->data, (t_func_free)mx_file_free);
    mx_sdk_response_free(resp, NULL);

    @return t_responce *(data == t_list *(list of t_file))
 */
t_response *mx_sdk_user_stickers_get(void);
/*
    to free response:
    mx_sdk_response_free(resp, (t_func_free)mx_file_free);

    @return t_responce *(data == t_file *)
 */
t_response *mx_sdk_user_stickers_post(int file_id);
/*
    to free response:
    mx_sdk_response_free(resp, (t_func_free)mx_file_free);

    @return t_responce *(data == t_file *)
 */
t_response *mx_sdk_user_stickers_delete(int favorite_id);
#endif
