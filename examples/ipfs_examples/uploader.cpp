
extern "C" {
    #include <sst-c-api/c_api.h>
    #include "../../c_crypto.h"
    #include "../../ipfs.h"
}
#include <unistd.h>

int main(int argc, char* argv[]) {
    char* config_path = argv[1];
    char* my_file_path = argv[2];
    char* add_reader_path = argv[3];
    SST_ctx_t* ctx = init_SST(config_path);

    FILE* add_reader_file = fopen(add_reader_path, "r");
    char addReader[64];
    if (add_reader_file == NULL) {
        fputs("Cannot open file.", stderr);
        fputc('\n', stderr);
        exit(1);
    }
    while (fgets(addReader, sizeof(addReader), add_reader_file) != NULL) {
        send_add_reader_req_via_TCP(ctx, addReader);
    }
    fclose(add_reader_file);
    // Set purpose to make session key request for file sharing.
    ctx->config->purpose_index = 1;
    estimate_time_t estimate_time[5];
    session_key_list_t* s_key_list_0 = get_session_key(ctx, NULL);
    if (s_key_list_0 == NULL) {
        printf("Failed to get session key. Returning NULL.\n");
        exit(1);
    }
    sleep(1);
    unsigned char hash_value[BUFF_SIZE];
    int hash_value_len;

    const char* filename = "Upload_result.csv";
    FILE* file;
    file = fopen(filename, "r");
    if (file) {
        fclose(file);
    } else {
        file = fopen(filename, "w");
        fprintf(
            file,
            "upload_time,keygenerate_time,enc_time,filemanager_time\n");  // columns
        fclose(file);
    }

    file = fopen(filename, "a");
    for (int i = 0; i < ctx->config->numkey; i++) {
        if (i != 0) {
            estimate_time[i].keygenerate_time = 0;
        }
        hash_value_len =
            file_encrypt_upload(&s_key_list_0->s_key[i], ctx, my_file_path,
                                &hash_value[0], &estimate_time[i]);
        sleep(1);
        upload_to_file_system_manager(&s_key_list_0->s_key[i], ctx,
                                      &hash_value[0], hash_value_len);
                                      
        sleep(5);
    }

    // Step 1. Create Hash
    unsigned char hash_of_file1[SHA256_DIGEST_LENGTH];
    unsigned int hash_length;
    digest_message_SHA_256(addReader, addReader, hash_of_file1, hash_length);

    // Step 2. Receive hash from downloader
    // Use Socket to receive hash from downloader
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(9090);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

    listen(serverSocket, 5);

    int clientSocket = accept(serverSocket, nullptr, nullptr);

    char buffer[1024] = {0};
    recv(clientSocket, buffer, sizeof(buffer), 0);

    close(serverSocket);

    // Step 3. Compare hash values
    if (memcmp(hash_of_file1, hash_of_file2, SHA256_DIGEST_LENGTH) == 0) {
        printf("Hash values are the same.\n");
    } else {
        printf("Hash values are different.\n");
    }                                                                       

    fclose(file);

    free_SST_ctx_t(ctx);
}