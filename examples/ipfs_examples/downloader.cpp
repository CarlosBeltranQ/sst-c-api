// g++ -o downloader downloader.cpp -I/opt/homebrew/opt/openssl/include -L/opt/homebrew/opt/openssl/lib -L/usr/local/lib  -lssl -lcrypto -lsst-c-api
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <iostream>
#include <fstream>


extern "C" {
    #include "../../c_crypto.h"
    #include "../../ipfs.h"
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Enter config path" << std::endl;
        exit(1);
    }
    
    std::string config_path = argv[1];
    SST_ctx_t *ctx = init_SST(config_path.c_str());
    session_key_list_t *s_key_list = init_empty_session_key_list();
    ctx->config->purpose_index = 0;
    std::vector<char> file_name(BUFF_SIZE);
    std::vector<unsigned char> received_skey_id(SESSION_KEY_ID_SIZE);
    estimate_time_t estimate_time[5];

    receive_data_and_download_file(received_skey_id.data(), ctx, file_name.data(),
                                    &estimate_time[0]);
    
    session_key_t *session_key =
        get_session_key_by_ID(received_skey_id.data(), ctx, s_key_list);
    
    if (session_key == NULL) {
        std::cerr << "There is no session key." << std::endl;
        exit(1);
    } else {
        sleep(5);
        file_decrypt_save(*session_key, file_name.data());
    }
    sleep(3);

    // Step 1. Create Hash
    std::cout << "Creating hash of the file" << std::endl;

    // Open the file in binary read mode
    std::ifstream file("result.txt", std::ios::binary | std::ios::ate);
    if (!file) {
        std::cerr << "Error opening file for hash calculation" << std::endl;
        exit(1);
    }

    // Determine file size
    std::streamsize filesize = file.tellg();
    file.seekg(0, std::ios::beg);

    // Read the file data into the vector
    std::vector<unsigned char> file_data(filesize);
    if (!file.read(reinterpret_cast<char*>(file_data.data()), filesize)) {
        std::cerr << "Error reading file" << std::endl;
        exit(1);
    }
    file.close();

    // Compute the hash
    std::vector<unsigned char> hash_of_file(SHA256_DIGEST_LENGTH);
    unsigned int hash_length = 0;
    digest_message_SHA_256(reinterpret_cast<unsigned char*>(&file_data[0]), filesize, hash_of_file.data(), &hash_length);

    // Step 2. Send hash to uploader
    // Send the computed hash (raw bytes)
    SST_session_ctx_t *session_ctx = secure_connect_to_server(&s_key_list->s_key[0], ctx);
    send_secure_message(reinterpret_cast<char*>(hash_of_file.data()), hash_length, session_ctx);
    
    free_SST_ctx_t(ctx);
    return 0;
}
