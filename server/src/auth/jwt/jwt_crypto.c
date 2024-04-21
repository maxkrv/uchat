#include "jwt.h"

char *mx_base64_encode(unsigned char *input, int len) {
    if (!input) {
        return NULL;
    }
    char buf[len * 2];

    int output_len = mg_base64_encode(input, len, buf, sizeof(buf));

    char *output = mx_strnew(output_len);
    mx_strncpy(output, buf, output_len);

    return output;
}

char *mx_base64_decode(char *input) {
    if (!input) {
        return NULL;
    }

    int len = strlen(input);
    char buf[len];

    int output_len = mg_base64_decode(input, len, buf, len);

    char *output = mx_strnew(output_len);
    mx_strncpy(output, buf, output_len);

    return output;
}

char *mx_create_sha256_signature(char *input_string, char *secret_key) {
    if (!input_string) {
        return NULL;
    }
    unsigned char buf_output[SHA256_DIGEST_LENGTH];
    char *to_hash = mx_strjoin(input_string, secret_key);
    const EVP_MD *md = EVP_sha256();
    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();

    EVP_DigestInit_ex(mdctx, md, NULL);
    EVP_DigestUpdate(mdctx, to_hash, strlen(to_hash));
    EVP_DigestFinal_ex(mdctx, buf_output, NULL);
    EVP_MD_CTX_free(mdctx);
    mx_strdel(&to_hash);

    return mx_base64_encode(buf_output, SHA256_DIGEST_LENGTH);
}

bool mx_verify_sha256_signature(char *signature, char *input_string,
                                char *secret_key) {
    if (!signature || !input_string) {
        return false;
    }

    char *actual_signature =
        mx_create_sha256_signature(input_string, secret_key);

    bool result = mx_strcmp(actual_signature, signature) == 0;

    mx_strdel(&actual_signature);
    return result;
}

char *mx_hash_string(char *string) {
    return mx_create_sha256_signature(string, NULL);
}

bool mx_comp_hash(char *value, char *hash) {
    return mx_verify_sha256_signature(hash, value, NULL);
}
