#ifndef IPFS_H
#define IPFS_H

#include "c_api.h"

#define BUFF_SIZE 100
#define DEFAULT_CHECK_INDEX 1
#define CHECK_PASS 0


// Do command "ipfs add command" and save the hash value.
void ipfs_add_command_save_result(char* file_name, unsigned char* hash_value);

// Encrypt the file with sessionkey and upload the file in IPFS environment.
// @param SST_session_ctx_t session_ctx to encrypt the file
void file_encrypt_upload(SST_session_ctx_t *session_ctx, SST_ctx_t *ctx, char *my_file_path, unsigned char *hash_value);

// Download the file in IPFS environment and decrypt the file with sessionkey.
// @param SST_session_ctx_t session_ctx to decrypt the file
void file_download_decrypt(SST_session_ctx_t *session_ctx);

// Request the data to datacenter
// @param SST_session_ctx_t session_ctx SST_ctx_t ctx to upload the data to datacenter.
void upload_to_datamanagement(SST_session_ctx_t *session_ctx, SST_ctx_t *ctx, unsigned char *hash_value);

// Receive the data from datacenter
// @param SST_session_ctx_t session_ctx SST_ctx_t ctx to download the data from datacenter.
void download_from_datamanagement(SST_session_ctx_t *session_ctx, SST_ctx_t *ctx);

#endif