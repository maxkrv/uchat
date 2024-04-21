#ifndef BACKEND_AUTH_JWT_H
#define BACKEND_AUTH_JWT_H

#include "base.h"
#include "utils.h"
#include <time.h>
#include <openssl/sha.h>
#include <openssl/evp.h>

typedef struct {
    char *alg;
    char *typ;
    int exp;
    int iat;
} t_jwt_head;

// string head.payload.signature
typedef char *t_jwt_token;
// json payload string
typedef char *t_jwt_payload;
// user id(-1 in case of error)
typedef int t_user_id;

char *mx_base64_encode(unsigned char *input, int size);
char *mx_base64_decode(char *input);

char *mx_create_sha256_signature(char *input_string, char *secret_key);
bool mx_verify_sha256_signature(char *signature, char *input_string,
                                char *secret_key);

char *mx_verify_jwt(char *jwt, char *secret);
char *mx_create_jwt(char *payload_str, char *secret, int expiration_time);

char *mx_jwt_head_stringify(t_jwt_head *head);
t_jwt_head mx_jwt_head_parse(char *header_json);
void mx_jwt_head_free(t_jwt_head *head);

char *mx_hash_string(char *string);
bool mx_comp_hash(char *value, char *hash);

#endif
